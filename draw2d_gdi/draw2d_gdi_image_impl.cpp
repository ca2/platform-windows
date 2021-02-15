﻿#include "framework.h"


inline byte byte_clip(i32 i)
{
   if(i >= 255)
      return 255;
   else if(i < 0)
      return 0;
   else
      return (byte) i;
}

namespace draw2d_gdi
{


   //   Creator : El Barto (ef00@luc.ac.be)
   //   Location : http://www.luc.ac.be/~ef00/ebgfx
   //   Date : 09-04-98
   //////////////////////////////////////////////////////////////////////


   image::image_impl(::layered * pobjectContext) :
      ::context_object(pobject),
      ::image_data(pobject),
      ::image_impl(pobject),
      m_pbitmap(e_create_new, pobject),
      m_spgraphics(e_create_new, pobject)
   {

      m_hbitmapOriginal    = nullptr;
      m_bMapped            = false;

   }


   ::draw2d::bitmap_pointer image::get_bitmap() const
   {

      return m_pbitmap;

   }


   ::draw2d::bitmap_pointer image::detach_bitmap()
   {

      return m_pbitmap.detach();

   }


   image::~image_impl ()
   {

      destroy ();

   }


   bool image::create(const ::size_i32 & size, ::eobject eobjectCreate, int iGoodStride)
   {

      synchronization_lock ml(&draw2d_gdi_mutex());

      if(m_pbitmap.is_set()
            && m_pbitmap->get_os_data() != nullptr
            && m_spgraphics.is_set()
            && m_spgraphics->get_os_data() != nullptr
            && size == this->size())
         return true;

      destroy();

      if (size.is_empty())
      {

         return false;

      }

      __zero(m_info);

      m_info.bmiHeader.biSize          = sizeof (BITMAPINFOHEADER);
      m_info.bmiHeader.biWidth         = size.cx;
      m_info.bmiHeader.biHeight        =- size.cy;
      m_info.bmiHeader.biPlanes        = 1;
      m_info.bmiHeader.biBitCount      = 32;
      m_info.bmiHeader.biCompression   = BI_RGB;
      m_info.bmiHeader.biSizeImage     = size.area() * 4;

      if(m_pbitmap.m_p == nullptr)
      {

         m_sizeRaw.cx = 0;

         m_sizeRaw.cy = 0;

         return false;

      }

      int iScan = size.cx * __sizeof(COLORREF);

      COLORREF * pcolorref = nullptr;

      //if(!m_pbitmap->CreateDIBSection(nullptr, &m_info, DIB_RGB_COLORS, (void **) &m_pcolorref, &m_iScan, nullptr, 0))
      if (!m_pbitmap->CreateDIBSection(nullptr, size, DIB_RGB_COLORS, (void **)&pcolorref, &iScan, nullptr, 0))
      {

         m_sizeRaw.cx = 0;

         m_sizeRaw.cy = 0;

         return false;

      }

      if(m_pbitmap->get_os_data() == nullptr)
      {

         destroy();

         return false;

      }

      m_spgraphics->CreateCompatibleDC(nullptr);

      m_spgraphics->m_pimage = this;

      m_hbitmapOriginal = (HBITMAP) m_spgraphics->get_os_data_ex(::draw2d_gdi::graphics::data_bitmap);

      ::draw2d::bitmap * pbitmap = m_spgraphics->SelectObject(m_pbitmap);

      if(pbitmap == nullptr || pbitmap->get_os_data() == nullptr)
      {

         destroy();

         return false;

      }

      init(size, pcolorref, iScan);

      m_uid = uidCreateImage;

      return true;

   }


   bool image::dc_select(bool bSelect)
   {
      if(bSelect)
      {
         return m_spgraphics->SelectObject(m_pbitmap) != nullptr;
      }
      else
      {
         return m_spgraphics.cast < graphics >()->SelectObject(m_hbitmapOriginal) != nullptr;
      }
   }


   bool image::create(::draw2d::graphics_pointer & pgraphics)
   {

      auto pbitmap = dynamic_cast<::draw2d_gdi::bitmap * >(pgraphics->get_current_bitmap());

      if (pbitmap == nullptr)
      {

         return false;

      }

      BITMAP bm;

      pbitmap->get_object(sizeof(bm), &bm);

      if (!create({ bm.bmWidth, bm.bmHeight }))
      {

         return false;

      }

      from(pgraphics);

      return true;

   }


   bool image::destroy ()
   {

      synchronization_lock ml(&draw2d_gdi_mutex());

      m_pbitmap->destroy();


      if(m_spgraphics.is_set() && m_spgraphics->is_set())
      {
         GDI_GRAPHICS(m_spgraphics.m_p)->SelectObject(m_hbitmapOriginal);
         m_spgraphics->DeleteDC();
      }

      return true;

   }


   bool image::to(::draw2d::graphics_pointer & pgraphics, const ::point_i32  & point, const ::size_i32 & size, const ::point_i32 & pointSrc)
   {

      return SetDIBitsToDevice(
             (dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1(),
             point.x, point.y,
             size.cx, size.cy,
             pointSrc.x, pointSrc.y, pointSrc.y, height() - pointSrc.y,
             colorref(), &m_info, 0)
             != false;

   }


   bool image::from(::draw2d::graphics_pointer & pgraphics)
   {

      ::draw2d::bitmap_pointer bitmap(get_context_application());

      bitmap->CreateCompatibleBitmap(pgraphics, 1, 1);

      ::draw2d_gdi::bitmap * pbitmap = dynamic_cast < ::draw2d_gdi::bitmap * > (pgraphics->SelectObject(bitmap));

      if(pbitmap == nullptr)
         return false;

      BITMAP bm;

      __memset(&bm, 0, sizeof(bm));

      if(!pbitmap->GetBitmap(&bm))
      {
         pgraphics->SelectObject(pbitmap);
         return false;
      }

      if (!create({ bm.bmWidth, bm.bmHeight }))
      {

         pgraphics->SelectObject(pbitmap);

         return false;

      }

      bool bOk = GetDIBits(GDI_HDC(pgraphics), (HBITMAP) pbitmap->get_os_data(), 0, this->height(), m_pcolorrefRaw, &(m_info), DIB_RGB_COLORS) != false;

      pgraphics->SelectObject(pbitmap);

      return bOk;

   }


   bool image::from(const ::point_i32 & pointDest, ::draw2d::graphics_pointer & pgraphics, const ::point_i32 & point, const ::size_i32 & sz)
   {

      return m_spgraphics->BitBlt(pointDest.x, pointDest.y, sz.cx, sz.cy, pgraphics, point.x, point.y, SRCCOPY) != false;

   }

   //void image::Fill ( int R, int G, int B )
   //{
   //   COLORREF color=rgb ( B, G, R );
   //   int size_i32=this->cx*this->cy;

   //   COLORREF * pcr;

   //   int iSize32 = size / 32;
   //   int i;
   //   for (i=0; i < iSize32; i+=32 )
   //   {
   //      pcr = &m_pcolorref[i];
   //      pcr[0] = color;
   //      pcr[1] = color;
   //      pcr[2] = color;
   //      pcr[3] = color;
   //      pcr[4] = color;
   //      pcr[5] = color;
   //      pcr[6] = color;
   //      pcr[7] = color;
   //      pcr[8] = color;
   //      pcr[9] = color;
   //      pcr[10] = color;
   //      pcr[11] = color;
   //      pcr[12] = color;
   //      pcr[13] = color;
   //      pcr[14] = color;
   //      pcr[15] = color;
   //      pcr[16] = color;
   //      pcr[17] = color;
   //      pcr[18] = color;
   //      pcr[19] = color;
   //      pcr[20] = color;
   //      pcr[21] = color;
   //      pcr[22] = color;
   //      pcr[23] = color;
   //      pcr[24] = color;
   //      pcr[25] = color;
   //      pcr[26] = color;
   //      pcr[27] = color;
   //      pcr[28] = color;
   //      pcr[29] = color;
   //      pcr[30] = color;
   //      pcr[31] = color;
   //   }

   //   for (i=0; i<size_i32; i++ )
   //   {
   //      m_pcolorref[i]=color;
   //   }
   //}

   //void image::set_rgb(int R, int G, int B)
   //{
   //   int size_i32=this->cx*this->cy;

   //   BYTE * pbyte = (BYTE *) m_pcolorref;

   //   int i;
   //   for (i=0; i<size_i32; i++ )
   //   {
   //      *pbyte++ = (BYTE) R;
   //      *pbyte++ = (BYTE) G;
   //      *pbyte++ = (BYTE) B;
   //      pbyte++;
   //   }
   //}

   //void image::ToAlpha(int i)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;

   //   while ( size-- )
   //   {
   //      dst[3] = dst[i];
   //      dst+=4;
   //   }
   //}

   //void image::from_alpha()
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   __int64 size = area();

   //   while ( size-- )
   //   {
   //      dst[0] = dst[3];
   //      dst[1] = dst[3];
   //      dst[2] = dst[3];
   //      dst+=4;
   //   }
   //}

   ////DIB = DIB * SRC_ALPHA

   //void image::mult_alpha(image_impl imageWork, bool bPreserveAlpha)
   //{
   //   return image::mult_alpha(nullptr, true);
   //   /*image_impl imageWork;

   //   if(imageWork == nullptr)
   //   {
   //      imageWork.create(this);
   //      imageWork = imageWork;
   //   }

   //   imageWork = create_image({width(),  height()});

   //   imageWork.Fill(0, 0, 0, 0);

   //   imageWork.channel_from(::color::e_channel_alpha, this);

   //   imageWork.channel_invert(::color::e_channel_alpha);


   //   BLENDFUNCTION bf;

   //   bf.BlendOp = AC_SRC_OVER;
   //   bf.BlendFlags = 0;
   //   bf.SourceConstantAlpha = 255;
   //   bf.AlphaFormat = AC_SRC_ALPHA;

   //   get_graphics()->alpha_blend(size(), imageWork.get_graphics(), bf);

   //   if(bPreserveAlpha)
   //   {

   //      imageWork.channel_invert(::color::e_channel_alpha);

   //      ::color::e_channel_from(::color::e_channel_alpha, imageWork);

   //   }
   //   */
   //}

   //void image::Map(int ToRgb, int FromRgb)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;

   //   while ( size-- )
   //   {
   //      *dst = (byte) (*dst == FromRgb ? ToRgb : *dst);
   //      dst+=4;
   //   }
   //}


   //void image::ToAlphaAndFill(int i, COLORREF cr)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;

   //   BYTE uchB = ::blue(cr);
   //   BYTE uchG = ::green(cr);
   //   BYTE uchR = ::red(cr);

   //   while ( size-- )
   //   {
   //      dst[3] = dst[i];
   //      dst[0] = uchB;
   //      dst[1] = uchG;
   //      dst[2] = uchR;
   //      dst+=4;
   //   }
   //}

   //void image::GrayToARGB(COLORREF cr)
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;

   //   u32 dwB = ::blue(cr);
   //   u32 dwG = ::green(cr);
   //   u32 dwR = ::red(cr);

   //   while (size-- > 0)
   //   {
   //      dst[3] = dst[0];
   //      dst[0] = (BYTE)(((dwB * dst[3]) / 256) & 0xff);
   //      dst[1] = (BYTE)(((dwG * dst[3]) / 256) & 0xff);
   //      dst[2] = (BYTE)(((dwR * dst[3]) / 256) & 0xff);
   //      dst+=4;
   //   }
   //}


   //void image::BitBlt(imagepimage, int op)
   //{
   //   if(op == 123) // zero dest rgb, invert alpha, and OR src rgb
   //   {
   //      int isize=this->cx*this->cy;
   //      LPDWORD lpbitsSrc= (LPDWORD) pimage->get_data();
   //      LPDWORD lpbitsDest= (LPDWORD) m_pcolorref;

   //      COLORREF _colorref = rgb ( 0, 0, 0 ) | (255 << 24);
   //      COLORREF colorrefa[2];
   //      colorrefa[0] = _colorref;
   //      colorrefa[1] = _colorref;

   //      COLORREF _colorrefN = rgb ( 255, 255, 255) | (0 << 24);
   //      COLORREF colorrefaN[2];
   //      colorrefaN[0] = _colorrefN;
   //      colorrefaN[1] = _colorrefN;
   //#ifdef _AMD64_

   //      //x64
   //#else
   //      _asm
   //      {
   //         emms
   //         mov      eax, isize
   //         mov      ebx, lpbitsDest
   //         mov      ecx, lpbitsSrc
   //         movq     mm0, colorrefa
   //         movq     mm7, colorrefaN
   //   fill_loop:
   //         cmp      eax, 1
   //         jle      fill_last
   //         movq     mm1, [ebx]
   //         movq     mm2, [ecx]
   //         pandn    mm1, mm0
   //         pand     mm2, mm7
   //         por      mm1, mm2
   //         movq     [ebx], mm1
   //
   //         sub      eax, 2
   //         add      ebx, 8
   //         add      ecx, 8

   //         jmp      fill_loop

   //   fill_last:
   //         emms
   //      }
   //#endif
   //   }

   //}


   //void image::Invert()
   //{
   //   int size_i32=this->cx*this->cy;
   //   byte * lpb = (byte *) m_pcolorref;
   //   for ( int i=0; i<size_i32; i++ )
   //   {
   //      lpb[0] = 255 - lpb[0];
   //      lpb[1] = 255 - lpb[1];
   //      lpb[2] = 255 - lpb[2];
   //      lpb += 4;
   //   }
   //}

   //void image::color::e_channel_invert(color::color::color::rgba::echannel echannel)
   //{
   //   __int64 size_i32=area();
   //   byte * lpb = (byte *) m_pcolorref;
   //   lpb += ((int)echannel) % 4;
   //   for ( int i=0; i<size_i32; i++ )
   //   {
   //      *lpb = 255 - *lpb;
   //      lpb += 4;
   //   }
   //}
   //void image::color::e_channel_multiply(color::color::color::rgba::echannel echannel, double dRate)
   //{
   //   if(dRate < 0)
   //      return;
   //   register __int64 size = area();
   //   byte * lpb = (byte *) get_data();
   //   lpb += ((int)echannel) % 4;
   //   register int iDiv = 256 * 256;
   //   register int iMul = (int) (dRate * ((double) iDiv));
   //   register int iRes;
   //   for(register __int64 i = 0; i < size; i++)
   //   {
   //      iRes = *lpb * iMul / iDiv;
   //      *lpb = (byte) (iRes > 255 ? 255 : iRes);
   //      lpb += 4;
   //   }
   //}

   //void image::FillGlass ( int R, int G, int B, int A )
   //{
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
   //      dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
   //      dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);
   //      dst+=4;
   //   }
   //}

   //void image::FillStippledGlass ( int R, int G, int B )
   //{
   //   COLORREF color=rgb ( B, G, R );
   //   int w=this->cx;
   //   int h=this->cy;

   //   for ( int j=0; j<w; j++ )
   //   {
   //      for ( int i=0; i<h; i++ )
   //      {
   //         m_pcolorref[j*w+i]=((i+j)&0x1) ? m_pcolorref[j*w+i] : color;
   //      }
   //   }
   //}

   //void image_impl__copy(image_impl image_impl)
   //{
   //   // If DibSize Wrong Re-create image_impl
   //   if ( (pimage->cx!=this->cx) || (pimage->cy!=this->cy) )
   //      pimage->create ( this->cx, this->cy );
   //   // do copy
   //   memcpy_dup ( pimage->get_data(), m_pcolorref, this->cx*this->cy*4 );
   //}


   //void image::Paste ( image_impl image_impl )
   //{
   //   // If DibSize Wrong Re-create image_impl
   //   if ( (this->cx!=pimage->cx) || (this->cy!=pimage->cy) )
   //      create ( pimage->cx, pimage->cy );
   //   // do Paste
   //   memcpy_dup ( m_pcolorref, pimage->get_data(), this->cx*this->cy*4 );
   //}

   //bool image::color_blend(COLORREF cr, BYTE bAlpha)
   //{

   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;

   //   u32 dwB = ::blue(cr);
   //   u32 dwG = ::green(cr);
   //   u32 dwR = ::red(cr);
   //
   //   u32 dwB_ = dwB << 8;
   //   u32 dwG_ = dwG << 8;
   //   u32 dwR_ = dwR << 8;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((dst[0]-dwB)*bAlpha+dwB_)>>8);
   //      dst[1]=(BYTE)(((dst[1]-dwG)*bAlpha+dwG_)>>8);
   //      dst[2]=(BYTE)(((dst[2]-dwG)*bAlpha+dwR_)>>8);
   //      dst+=4;
   //   }
   //   return true;
   //}


   //void image::Blend (image_impl image_impl, int A )
   //{
   //   if ( size()!=pimage->size() )
   //      return;

   //   BYTE *src=(BYTE*)pimage->get_data();
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
   //      dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
   //      dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //bool image::Blend(imagepimage, imagepimageA, int A)
   //{
   //   if(size() != pimage->size() ||
   //      size() != imageA.size())
   //      return false;

   //   BYTE *src=(BYTE*)pimage->get_data();
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   BYTE *alf=(BYTE*)imageA.m_pcolorref;
   //   int size_i32=this->cx*this->cy;

   //   A = 2 - A;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((src[0]-dst[0])*alf[A]+(dst[0]<<8))>>8);
   //      dst[1]=(BYTE)(((src[1]-dst[1])*alf[A]+(dst[1]<<8))>>8);
   //      dst[2]=(BYTE)(((src[2]-dst[2])*alf[A]+(dst[2]<<8))>>8);
   //      dst+=4;
   //      src+=4;
   //      alf+=4;
   //   }

   //   return true;
   //}

   //void image::Darken (image_impl image_impl )
   //{
   //   if ( size()!=pimage->size() )
   //      return;

   //   BYTE *src=(BYTE*)pimage->get_data();
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
   //      dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
   //      dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void image::Difference (image_impl image_impl )
   //{
   //   if ( size()!=pimage->size() )
   //      return;

   //   BYTE *src=(BYTE*)pimage->get_data();
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      int Difference;
   //      Difference=src[0]-dst[0];
   //      dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
   //      Difference=src[1]-dst[1];
   //      dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
   //      Difference=src[2]-dst[2];
   //      dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void image::Lighten (image_impl image_impl )
   //{
   //   if ( size()!=pimage->size() )
   //      return;

   //   BYTE *src=(BYTE*)pimage->get_data();
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
   //      dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
   //      dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
   //      dst+=4;
   //      src+=4;
   //   }
   //}


   //void image::Multiply (image_impl image_impl )
   //{
   //   if ( size()!=pimage->size() )
   //      return;

   //   BYTE *src=(BYTE*)pimage->get_data();
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
   //      dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
   //      dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //void image::Screen (image_impl image_impl )
   //{
   //   if ( size()!=pimage->size() )
   //      return;

   //   BYTE *src=(BYTE*)pimage->get_data();
   //   BYTE *dst=(BYTE*)m_pcolorref;
   //   int size_i32=this->cx*this->cy;
   //
   //   while ( size-- )
   //   {
   //      dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
   //      dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
   //      dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
   //      dst+=4;
   //      src+=4;
   //   }
   //}

   //////////////////////////////////////////////////////////////////////
   // Rectangle Functions
   //////////////////////////////////////////////////////////////////////

   //void image::copy (image_impl image_impl, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pimage->cx)<this->cx) ? pimage->cx : this->cx-x;
   //   int dy=((y+pimage->cy)<this->cy) ? pimage->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to copy return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;
   //   // If DibSize Wrong Re-create image_impl
   //   if ( (dx!=pimage->cx) || (dy!=pimage->cy) )
   //      pimage->create ( dx, dy );

   //   // Prepare buffer Addresses
   //   COLORREF *src=m_pcolorref+(py*this->cx)+px;
   //   COLORREF *dst=pimage->get_data();

   //   // Do copy
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //         dst[i]=src[i];
   //      src+=this->cx;
   //      dst+=pimage->cx;
   //   }
   //}

   //void image::PasteRect (image_impl image_impl, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pimage->cx)<this->cx) ? pimage->cx : this->cx-x;
   //   int dy=((y+pimage->cy)<this->cy) ? pimage->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Paste return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   COLORREF *src=pimage->get_data()+((py-y)*pimage->cx)+px-x;
   //   COLORREF *dst=m_pcolorref+(py*this->cx)+px;

   //   // Do Paste
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //         dst[i]=src[i];
   //      src+=pimage->cx;
   //      dst+=this->cx;
   //   }
   //}

   //void image::FillRect ( int x, int y, int w, int h, int R, int G, int B )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<this->cx) ? w : this->cx-x;
   //   int dy=((y+h)<this->cy) ? h : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Fill return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   COLORREF *dst=m_pcolorref+(py*this->cx)+px;
   //   COLORREF color=rgb ( B, G, R );

   //   // Do Fill
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[i]=color;
   //      }
   //      dst+=this->cx;
   //   }
   //}

   //void image::FillGlassRect ( int x, int y, int w, int h, int R, int G, int B, int A )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<this->cx) ? w : this->cx-x;
   //   int dy=((y+h)<this->cy) ? h : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to FillGlass return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do FillGlass
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((B-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((G-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((R-dst[2])*A+(dst[2]<<8))>>8);
   //         dst+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //   }
   //}

   //void image::FillStippledGlassRect ( int x, int y, int w, int h, int R, int G, int B )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+w)<this->cx) ? w : this->cx-x;
   //   int dy=((y+h)<this->cy) ? h : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to FillStippledGlass return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Address
   //   COLORREF *dst=m_pcolorref+(py*this->cx)+px;
   //   COLORREF color=rgb ( B, G, R );

   //   // Do FillStippledGlass
   //   for ( int j=0; j<dy; j++ )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[i]=((i+j)&0x1) ? dst[i] : color;
   //      }
   //      dst+=this->cx;
   //   }
   //}

   //void image::BlendRect (image_impl image_impl, int x, int y, int A )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pimage->cx)<this->cx) ? pimage->cx : this->cx-x;
   //   int dy=((y+pimage->cy)<this->cy) ? pimage->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Blend return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pimage->get_data()+(((py-y)*pimage->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Blend
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((src[0]-dst[0])*A+(dst[0]<<8))>>8);
   //         dst[1]=(BYTE)(((src[1]-dst[1])*A+(dst[1]<<8))>>8);
   //         dst[2]=(BYTE)(((src[2]-dst[2])*A+(dst[2]<<8))>>8);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pimage->cx-dx)<<2;
   //   }
   //}

   //void image::DarkenRect (image_impl image_impl, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pimage->cx)<this->cx) ? pimage->cx : this->cx-x;
   //   int dy=((y+pimage->cy)<this->cy) ? pimage->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Darken return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pimage->get_data()+(((py-y)*pimage->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Darken
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)((src[0]<dst[0]) ? src[0] : dst[0]);
   //         dst[1]=(BYTE)((src[1]<dst[1]) ? src[1] : dst[1]);
   //         dst[2]=(BYTE)((src[2]<dst[2]) ? src[2] : dst[2]);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pimage->cx-dx)<<2;
   //   }
   //}

   //void image::DifferenceRect (image_impl image_impl, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pimage->cx)<this->cx) ? pimage->cx : this->cx-x;
   //   int dy=((y+pimage->cy)<this->cy) ? pimage->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Difference return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pimage->get_data()+(((py-y)*pimage->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Difference
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         int Difference;
   //         Difference=src[0]-dst[0];
   //         dst[0]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         Difference=src[1]-dst[1];
   //         dst[1]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         Difference=src[2]-dst[2];
   //         dst[2]=(BYTE)((Difference<0) ? -Difference : Difference);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pimage->cx-dx)<<2;
   //   }
   //}

   //void image::LightenRect (image_impl image_impl, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pimage->cx)<this->cx) ? pimage->cx : this->cx-x;
   //   int dy=((y+pimage->cy)<this->cy) ? pimage->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Lighten return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pimage->get_data()+(((py-y)*pimage->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Lighten
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)((src[0]>dst[0]) ? src[0] : dst[0]);
   //         dst[1]=(BYTE)((src[1]>dst[1]) ? src[1] : dst[1]);
   //         dst[2]=(BYTE)((src[2]>dst[2]) ? src[2] : dst[2]);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pimage->cx-dx)<<2;
   //   }
   //}

   //void image::MultiplyRect (image_impl image_impl, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pimage->cx)<this->cx) ? pimage->cx : this->cx-x;
   //   int dy=((y+pimage->cy)<this->cy) ? pimage->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Multiply return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pimage->get_data()+(((py-y)*pimage->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Multiply
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(((src[0])*(dst[0]))>>8);
   //         dst[1]=(BYTE)(((src[1])*(dst[1]))>>8);
   //         dst[2]=(BYTE)(((src[2])*(dst[2]))>>8);
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pimage->cx-dx)<<2;
   //   }
   //}

   //void image::ScreenRect (image_impl image_impl, int x, int y )
   //{
   //   // Clip Rect
   //   int px=(x>=0) ? x : 0;
   //   int py=(y>=0) ? y : 0;
   //   int dx=((x+pimage->cx)<this->cx) ? pimage->cx : this->cx-x;
   //   int dy=((y+pimage->cy)<this->cy) ? pimage->cy : this->cy-y;
   //   dx=(x>=0) ? dx : dx + x;
   //   dy=(y>=0) ? dy : dy + y;

   //   // If Nothing to Screen return
   //   if ( (dx<=0) || (dy<=0) )
   //      return;

   //   // Prepare buffer Addresses
   //   BYTE *src=(BYTE *)pimage->get_data()+(((py-y)*pimage->cx)+px-x)*4;
   //   BYTE *dst=(BYTE *)m_pcolorref+((py*this->cx)+px)*4;

   //   // Do Screen
   //   while ( dy-- )
   //   {
   //      for ( int i=0; i<dx; i++ )
   //      {
   //         dst[0]=(BYTE)(255-(((255-src[0])*(255-dst[0]))>>8));
   //         dst[1]=(BYTE)(255-(((255-src[1])*(255-dst[1]))>>8));
   //         dst[2]=(BYTE)(255-(((255-src[2])*(255-dst[2]))>>8));
   //         dst+=4;
   //         src+=4;
   //      }
   //      dst+=(this->cx-dx)<<2;
   //      src+=(pimage->cx-dx)<<2;
   //   }
   //}

   //////////////////////////////////////////////////////////////////////
   // Line Functions
   //////////////////////////////////////////////////////////////////////

   /*void image::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
   {
      int dx, dy, k1, k2, d, x, y;
      COLORREF color=rgb ( B, G, R );

      dx=x2-x1;
      dy=y2-y1;
      d=(dy<<1)-dx;
      k1=dy<<1;
      k2=(dy-dx)<<1;
      x=x1;
      y=y1;

      m_pcolorref[y*this->cx+x]=color;
      while (x<dx)
      {
         if (d<=0)
         {
            d+=k1;
            x++;
         }
         else
         {
            d+=k2;
            x++;
            y++;
         }
         m_pcolorref[y*this->cx+x]=color;
      }
   }*/

//   void image::Line ( int x1, int y1, int x2, int y2, int R, int G, int B )
//   {
//      int d, x, y, ax, ay, sx, sy, dx, dy;
//      COLORREF color=rgb ( B, G, R );
//
//      dx=x2-x1;
//      ax=abs ( dx )<<1;
//      sx=(dx<0) ? -1 : 1;
//      dy=y2-y1;
//      ay=abs ( dy )<<1;
//      sy=(dy<0) ? -1 : 1;
//      x=x1;
//      y=y1;
//
//      if ( ax>ay )
//      {
//         d=ay-(ax>>1);
//         while ( x!=x2 )
//         {
//            m_pcolorref[y*this->cx+x]=color;
//            if ( d>=0 )
//            {
//               y+=sy;
//               d-=ax;
//            }
//            x+=sx;
//            d+=ay;
//         }
//      }
//      else
//      {
//         d=ax-(ay>>1);
//         while ( y!=y2 )
//         {
//            m_pcolorref[y*this->cx+x]=color;
//            if ( d>=0 )
//            {
//               x+=sx;
//               d-=ay;
//            }
//            y+=sy;
//            d+=ax;
//         }
//      }
//   }
//
//   void image::LineGlass ( int x1, int y1, int x2, int y2, int R, int G, int B, int A )
//   {
//      int d, x, y, ax, ay, sx, sy, dx, dy;
////      COLORREF color=rgb ( B, G, R );
//      BYTE *dst=(BYTE *)m_pcolorref;
//
//      dx=x2-x1;
//      ax=abs ( dx )<<1;
//      sx=(dx<0) ? -1 : 1;
//      dy=y2-y1;
//      ay=abs ( dy )<<1;
//      sy=(dy<0) ? -1 : 1;
//      x=x1;
//      y=y1;
//
//      if ( ax>ay )
//      {
//         d=ay-(ax>>1);
//         while ( x!=x2 )
//         {
//            dst[(y*this->cx+x)<<2]=(BYTE)(((B-dst[(y*this->cx+x)<<2])*A+(dst[(y*this->cx+x)<<2]<<8))>>8);
//            dst[((y*this->cx+x)<<2)+1]=(BYTE)(((G-dst[((y*this->cx+x)<<2)+1])*A+(dst[((y*this->cx+x)<<2)+1]<<8))>>8);
//            dst[((y*this->cx+x)<<2)+2]=(BYTE)(((R-dst[((y*this->cx+x)<<2)+2])*A+(dst[((y*this->cx+x)<<2)+2]<<8))>>8);
//            if ( d>=0 )
//            {
//               y+=sy;
//               d-=ax;
//            }
//            x+=sx;
//            d+=ay;
//         }
//      }
//      else
//      {
//         d=ax-(ay>>1);
//         while ( y!=y2 )
//         {
//            dst[(y*this->cx+x)<<2]=(BYTE)(((B-dst[(y*this->cx+x)<<2])*A+(dst[(y*this->cx+x)<<2]<<8))>>8);
//            dst[((y*this->cx+x)<<2)+1]=(BYTE)(((G-dst[((y*this->cx+x)<<2)+1])*A+(dst[((y*this->cx+x)<<2)+1]<<8))>>8);
//            dst[((y*this->cx+x)<<2)+2]=(BYTE)(((R-dst[((y*this->cx+x)<<2)+2])*A+(dst[((y*this->cx+x)<<2)+2]<<8))>>8);
//            if ( d>=0 )
//            {
//               x+=sx;
//               d-=ay;
//            }
//            y+=sy;
//            d+=ax;
//         }
//      }
//   }
//
//   void image::Mask(COLORREF crMask, COLORREF crInMask, COLORREF crOutMask)
//   {
//      COLORREF crFind = rgb(::blue(crMask), ::green(crMask), ::red(crMask));
//      COLORREF crSet = rgb(::blue(crInMask), ::green(crInMask), ::red(crInMask));
//      COLORREF crUnset  = rgb(::blue(crOutMask), ::green(crOutMask), ::red(crOutMask));
//
//      int size_i32=this->cx*this->cy;
//
//      for ( int i=0; i<size_i32; i++ )
//         if(m_pcolorref[i]== crFind)
//            m_pcolorref[i] = crSet;
//         else
//            m_pcolorref[i] = crUnset;
//
//   }
//
//   void image::transparent_color(::color::color color)
//   {
//      COLORREF crFind = color.get_rgb();
//      __int64 size = area();
//
//      for ( int i=0; i<size_i32; i++ )
//         if((m_pcolorref[i] & 0x00ffffff) == crFind)
//            ((byte *)&m_pcolorref[i])[3] = 255;
//         else
//            ((byte *)&m_pcolorref[i])[3] = 0;
//   }
//
//   void image::color::e_channel_mask(unsigned char uchFind, unsigned char uchSet, unsigned char uchUnset, color::color::color::rgba::echannel echannel)
//   {
//      int size = this->cx * this->cy;
//      unsigned char * puch = (unsigned char * ) m_pcolorref;
//      puch += ((int) echannel) % 4;
//
//      for(int i = 0; i < size; i++)
//      {
//         if(*puch == uchFind)
//            *puch = uchSet;
//         else
//            *puch = uchUnset;
//         puch += 4;
//      }
//   }

   //u32 image::GetPixel(int x, int y)
   //{
   //   u32 dw = *(m_pcolorref + x + (this->cy - y - 1) * this->cx);
   //   return rgb(::blue(dw), ::green(dw), ::red(dw));
   //}

   // too slow for animation on AMD XP Atlhon.
   // TOP SUGGESTION:
   // The gradient can't have more then 256 levels of the most bright color
   // (white). So creating a radial fill of radius 256 and then using fasting
   // stretching algorithms is much faster than calculating radial fill.
//   void image::RadialFill(BYTE alpha, BYTE red, BYTE green, BYTE blue, int xCenter, int yCenter, int iRadius)
//   {
//      if (iRadius == 0)
//         return;
//      /*if(version == 0)
//      {
//
//         int iR = iRadius - 1;
//
//         int xL = xCenter - iR;
//         int xU = xCenter + iR;
//         int yL = yCenter - iR;
//         int yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= m_Size.cx) xU = m_Size.cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= m_Size.cy) yU = m_Size.cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * m_Size.cx));
//         u32 dwAdd = ((m_Size.cx - 1 - xU) + xL) * 4;
//         int size_i32=m_Size.cx*m_Size.cy;
//         double iLevel;
//
//         int dx, dy;
//         int dx0, dy0;
//         int dx1, dy1;
//         int dx2, dy2;
//         int dx3, dy3;
//         int dx4, dy4;
//         int dx5, dy5;
//         int dx6, dy6;
//         int dx7, dy7;
//         int dx8, dy8;
//         int dx9, dy9;
//         int dxA, dyA;
//         int dxB, dyB;
//         int dxC, dyC;
//         int dxD, dyD;
//         int dxE, dyE;
//         int dxF, dyF;
//
//         unsigned long dr;
//         unsigned long dq;
//         unsigned long dr0, dq0;
//         unsigned long dr1, dq1;
//         unsigned long dr2, dq2;
//         unsigned long dr3, dq3;
//         unsigned long dr4, dq4;
//         unsigned long dr5, dq5;
//         unsigned long dr6, dq6;
//         unsigned long dr7, dq7;
//         unsigned long dr8, dq8;
//         unsigned long dr9, dq9;
//         unsigned long drA, dqA;
//         unsigned long drB, dqB;
//         unsigned long drC, dqC;
//         unsigned long drD, dqD;
//         unsigned long drE, dqE;
//         unsigned long drF, dqF;
//         int x, y;
//
//         {
//            for(y = yL; y <= yU; y++)
//            {
//               for(x = xL; x <= xU; x++)
//               {
//                  dx = abs(x - xCenter);
//                  dy = abs(y - yCenter);
//                  isqrt((dx * dx) + (dy * dy), &dr, &dq);
//                  if(dr < iRadius)
//                  {
//                     iLevel = 1.0 - dr * 1.0 / iRadius;
//                     dst[0] = blue  * iLevel;
//                     dst[1] = green * iLevel;
//                     dst[2] = red   * iLevel;
//                  }
//                  dst += 4;
//               }
//               dst += dwAdd;
//            }
//         }
//      }
//      else if(version == 1)*/
//      {
//
//         byte * lpbAlloc = (byte *) malloc((iRadius * iRadius) + 4);
//         byte * lpb = (byte *) (((int) lpbAlloc + 3) & ~3);
//
//
//         int x, y;
//         int b;
//
////         int rect2 = iRadius * iRadius;
//
//         for(y = 0; y < iRadius; y++)
//         {
//            for(x = y; x < iRadius; x++)
//            {
//               b = (int) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
//               if(b > 255)
//                  b = 0;
//               else
//                  b = 255 - b;
//
//
//               lpb[x + y * iRadius] = (byte) b;
//               lpb[y + x * iRadius] = (byte) b;
//            }
//         }
//
//
//         int iR = iRadius - 1;
//
//         int xL = xCenter - iR;
//         int xU = xCenter + iR;
//         int yL = yCenter - iR;
//         int yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= this->cx) xU = this->cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= this->cy) yU = this->cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * this->cx));
//         u32 dwAdd = ((this->cx - 1 - xU) + xL) * 4;
////         int size_i32=this->cx*this->cy;
//
//         int dx, dy;
//
//         // Top Left
//
//         for(y = yL; y <= yU; y++)
//         {
//            for(x = xL; x <= xU; x++)
//            {
//               dx = abs(x - xCenter);
//               dy = abs(y - yCenter);
//               b = lpb[dx + dy * iRadius];
//               dst[0] = (byte) (blue     * b / 255);
//               dst[1] = (byte) (green    * b / 255);
//               dst[2] = (byte) (red      * b / 255);
//               dst[3] = (byte) (alpha    * b / 255);
//               dst += 4;
//            }
//            dst += dwAdd;
//         }
//
//         free(lpbAlloc);
//      }
//   }
//
//
//   void image::RadialFill(
//      BYTE alpha1, BYTE red1, BYTE green1, BYTE blue1,
//      BYTE alpha2, BYTE red2, BYTE green2, BYTE blue2,
//      int xCenter, int yCenter, int iRadius)
//   {
//      if (iRadius == 0)
//         return;
//      /*if(version == 0)
//      {
//
//         int iR = iRadius - 1;
//
//         int xL = xCenter - iR;
//         int xU = xCenter + iR;
//         int yL = yCenter - iR;
//         int yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= m_Size.cx) xU = m_Size.cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= m_Size.cy) yU = m_Size.cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * m_Size.cx));
//         u32 dwAdd = ((m_Size.cx - 1 - xU) + xL) * 4;
//         int size_i32=m_Size.cx*m_Size.cy;
//         double iLevel;
//
//         int dx, dy;
//         int dx0, dy0;
//         int dx1, dy1;
//         int dx2, dy2;
//         int dx3, dy3;
//         int dx4, dy4;
//         int dx5, dy5;
//         int dx6, dy6;
//         int dx7, dy7;
//         int dx8, dy8;
//         int dx9, dy9;
//         int dxA, dyA;
//         int dxB, dyB;
//         int dxC, dyC;
//         int dxD, dyD;
//         int dxE, dyE;
//         int dxF, dyF;
//
//         unsigned long dr;
//         unsigned long dq;
//         unsigned long dr0, dq0;
//         unsigned long dr1, dq1;
//         unsigned long dr2, dq2;
//         unsigned long dr3, dq3;
//         unsigned long dr4, dq4;
//         unsigned long dr5, dq5;
//         unsigned long dr6, dq6;
//         unsigned long dr7, dq7;
//         unsigned long dr8, dq8;
//         unsigned long dr9, dq9;
//         unsigned long drA, dqA;
//         unsigned long drB, dqB;
//         unsigned long drC, dqC;
//         unsigned long drD, dqD;
//         unsigned long drE, dqE;
//         unsigned long drF, dqF;
//         int x, y;
//
//         {
//            for(y = yL; y <= yU; y++)
//            {
//               for(x = xL; x <= xU; x++)
//               {
//                  dx = abs(x - xCenter);
//                  dy = abs(y - yCenter);
//                  isqrt((dx * dx) + (dy * dy), &dr, &dq);
//                  if(dr < iRadius)
//                  {
//                     iLevel = 1.0 - dr * 1.0 / iRadius;
//                     dst[0] = blue  * iLevel;
//                     dst[1] = green * iLevel;
//                     dst[2] = red   * iLevel;
//                  }
//                  dst += 4;
//               }
//               dst += dwAdd;
//            }
//         }
//      }
//      else if(version == 1)*/
//      {
//
//         byte * lpbAlloc = (byte *) malloc((iRadius * iRadius) + 4);
//         byte * lpb = (byte *) (((int) lpbAlloc + 3) & ~3);
//
//
//         int x, y;
//         int b;
//
////         int rect2 = iRadius * iRadius;
//
//         for(y = 0; y < iRadius; y++)
//         {
//            for(x = y; x < iRadius; x++)
//            {
//               b = (int) (sqrt((double) (x * x) + (y * y)) * 255 / iRadius);
//               if(b > 255)
//                  b = 0;
//               else
//                  b = ~b;
//
//
//               lpb[x + y * iRadius] = (byte) b;
//               lpb[y + x * iRadius] = (byte) b;
//            }
//         }
//
//
//         int iR = iRadius - 1;
//
//         int xL = xCenter - iR;
//         int xU = xCenter + iR;
//         int yL = yCenter - iR;
//         int yU = yCenter + iR;
//
//
//         if(xL < 0) xL = 0;
//         if(xU >= this->cx) xU = this->cx - 1;
//         if(yL < 0) yL = 0;
//         if(yU >= this->cy) yU = this->cy - 1;
//
//
//         BYTE *dst = ((BYTE*)(m_pcolorref + xL + yL * this->cx));
//         u32 dwAdd = ((this->cx - 1 - xU) + xL) * 4;
////         int size_i32=this->cx*this->cy;
//
//         int dx, dy;
//
//         BYTE bComp;
//
//         // Top Left
//
//         for(y = yL; y <= yU; y++)
//         {
//            for(x = xL; x <= xU; x++)
//            {
//               dx = abs(x - xCenter);
//               dy = abs(y - yCenter);
//               b = lpb[dx + dy * iRadius];
//               bComp = (byte) ~b;
//               dst[0] = (byte) (((blue1  * b) + (blue2  * bComp)) / 255);
//               dst[1] = (byte) (((green1 * b) + (green2 * bComp)) / 255);
//               dst[2] = (byte) (((red1   * b) + (red2   * bComp)) / 255);
//               dst[3] = (byte) (((alpha1 * b) + (alpha2 * bComp)) / 255);
//               dst += 4;
//            }
//            dst += dwAdd;
//         }
//
//         free(lpbAlloc);
//      }
//   }
//


   bool image::SetIconMask(::draw2d::icon * picon, int cx, int cy)
   {

      if (!create({ cx, cy }))
      {

         return false;

      }

      if (cx <= 0 || cy <= 0)
      {

         return false;

      }

      // White blend image_impl
      ::image_pointer pimage1;

      pimage1 = create_image({cx,  cy});

      pimage1->set(255, 255, 255);

      pimage1->g()->DrawIcon(
      0, 0,
      picon,
      cx, cy,
      0,
      nullptr,
      DI_IMAGE | DI_MASK);

      // Black blend image_impl
      ::image_pointer pimage2;

      pimage2 = create_image({cx,  cy});

      pimage2->fill(0, 0, 0, 0);

      pimage2->g()->DrawIcon(
      0, 0,
      picon,
      cx, cy,
      0,
      nullptr,
      DI_IMAGE | DI_MASK);

      // Mask image_impl
      ::image_pointer pimageM;

      imageM = create_image({cx,  cy});

      imageM.g()->DrawIcon(
      0, 0,
      picon,
      cx, cy,
      0,
      nullptr,
      DI_MASK);

      BYTE * rect1=(BYTE*)pimage1->colorref();
      BYTE * rect2=(BYTE*)pimage2->colorref();
      BYTE * srcM=(BYTE*)imageM.colorref();
      BYTE * dest=(BYTE*)colorref();
      i64 iSize = area();

      BYTE b;
      BYTE bMax;
      while ( iSize-- > 0)
      {
         if(srcM[0] == 255)
         {
            bMax = 0;
         }
         else
         {
            bMax = 0;
            b =(BYTE)(rect1[0]  - rect2[0]);
            bMax = maximum(b, bMax);
            b =(BYTE)(rect1[1]  - rect2[1]);
            bMax = maximum(b, bMax);
            b =(BYTE)(rect1[2]  - rect2[2]);
            bMax = maximum(b, bMax);
            bMax = 255 - bMax;
         }
         dest[0]  =  bMax;
         dest[1]  =  bMax;
         dest[2]  =  bMax;
         dest     += 4;
         srcM     += 4;
         rect1       += 4;
         rect2       += 4;

      }

      return true;

   }


   //void image::rotate(image_impl image_impl, double dAngle, double dScale)
   //{
   //  // image_impl image(get_context_application());
   ////   pimage->Paste(this);

   //   int cx = this->cx;
   //   int cy = this->cy;

   //   int l = maximum(cx, cy);

   //
   //   int jmax = minimum(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = minimum(l, cx / 2);
   //   int imin = - imax;


   //   int joff = cy / 2;
   //   int ioff = cx / 2;

   //   //int iAngle = iStep % 360;
   //   //int iAngle = iStep;
   //   //int iAngle = 1;
   //   //int k = 0;

   ///*     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         x=int(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //         y=int(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pimage->m_pcolorref[abs(y%this->cy)*this->cx+abs(x%this->cx)];
   //         //k++;
   //      }
   //      (j+joff)*cx+(i+ioff)
   //   }*/

   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = this->cx - 1;
   //   int cy1 = this->cy - 1;
   //     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % this->cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % this->cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

   //         if((x / this->cx) % 2 == 0)
   //         {
   //            x %= this->cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % this->cx);
   //         }

   //         if((y / this->cy) % 2 == 0)
   //         {
   //            y %= this->cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % this->cy);
   //         }


   //
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pimage->get_data()[y * cx + x];
   //         k++;
   //      }
   //   }
   //}


   //void image::Rotate034(image_impl image_impl, double dAngle, double dScale)
   //{
   //
   //   int cx = this->cx;
   //   int cy = this->cy;

   //   int l = maximum(cx, cy);

   //
   //   int jmax = minimum(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = minimum(l, cx / 2);
   //   int imin = - imax;


   //   if((cy % 2) == 1)
   //      jmax++;

   //   if((cx % 2) == 1)
   //      imax++;
   //
   //   int joff = cy / 2;
   //   int ioff = cx / 2;

   //
   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = this->cx - 1;
   //   int cy1 = this->cy - 1;
   //     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % this->cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % this->cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

   //         if((x / this->cx) % 2 == 0)
   //         {
   //            x %= this->cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % this->cx);
   //         }

   //         if((y / this->cy) % 2 == 0)
   //         {
   //            y %= this->cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % this->cy);
   //         }


   //
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pimage->get_data()[y * cx + x];
   //         k++;
   //      }
   //   }
   //}

   //void image::rotate(
   //   image_impl image_impl,
   //   const ::rectangle_i32 & rectangle,
   //   double dAngle,
   //   double dScale)
   //{
   //  // image_impl image(get_context_application());
   ////   pimage->Paste(this);



   //   ::rectangle_i32 rectangle(rectangle);

   //   int cx = rectangle.width();
   //   int cy = rectangle.height();

   //   int l = maximum(cx, cy);
   //
   //   int jmax = minimum(l, cy / 2);
   //   int jmin = - jmax;
   //   int imax = minimum(l, cx / 2);
   //   int imin = - imax;


   //   int joff = cy / 2 + rectangle.left;
   //   int ioff = cx / 2 + rectangle.top;

   //   //int iAngle = iStep % 360;
   //   //int iAngle = iStep;
   //   //int iAngle = 1;
   //   //int k = 0;

   ///*     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         x=int(cos10(i, iAngle) - sin10(j, iAngle)) + ioff;
   //         y=int(sin10(i, iAngle) + cos10(j, iAngle)) + joff;
   //         m_pcolorref[(j+joff)*cx+(i+ioff)]=
   //            pimage->m_pcolorref[abs(y%this->cy)*this->cx+abs(x%this->cx)];
   //         //k++;
   //      }
   //      (j+joff)*cx+(i+ioff)
   //   }*/

   //   int k = 0;
   //   double dCos = ::cos(dAngle * dPi / 180.0) * dScale;
   //   double dSin = ::sin(dAngle * dPi / 180.0) * dScale;
   //   int cx1 = cx - 1;
   //   int cy1 = cy - 1;
   //     for ( int j=jmin; j<jmax; j++ )
   //   {
   //      for ( int i=imin; i<imax; i++ )
   //      {
   //         int x, y;

   //         // A Combination of a 2d Translation/rotation/Scale Matrix
   //         //x=abs((int(dCos * i - dSin * j) + ioff) % this->cx);
   //         //y=abs((int(dSin * i + dCos * j) + joff) % this->cy);

   //         x = (int) abs((dCos * i - dSin * j) + ioff);
   //         y = (int) abs((dSin * i + dCos * j) + joff);

   //         if((x / cx) % 2 == 0)
   //         {
   //            x %= cx;
   //         }
   //         else
   //         {
   //            x = cx1 - (x % cx);
   //         }

   //         if((y / cy) % 2 == 0)
   //         {
   //            y %= cy;
   //         }
   //         else
   //         {
   //            y = cy1 - (y % cy);
   //         }


   //
   //         m_pcolorref[(j+joff)*this->cx+(i+ioff)]=
   //            pimage->get_data()[y * this->cx + x];
   //         k++;
   //      }
   //   }
   //}

   ///*int image::cos(int i, int iAngle)
   //{
   //   return (int) (((_int64) i * CosN[iAngle]) >> 32);
   //}

   //int image::sin(int i, int iAngle)
   //{
   //   return (int) (((_int64) i * SinN[iAngle]) >> 32);
   //}*/




   //void image::Fill (int A, int R, int G, int B )
   //{
   //   COLORREF color = rgb ( B, G, R ) | (A << 24);
   //   int size_i32=this->cx*this->cy;

   //   COLORREF * pcr;

   //   int iSize32 = size / 32;
   //   int i;
   //   for (i=0; i < iSize32; i+=32 )
   //   {
   //      pcr = &m_pcolorref[i];
   //      pcr[0] = color;
   //      pcr[1] = color;
   //      pcr[2] = color;
   //      pcr[3] = color;
   //      pcr[4] = color;
   //      pcr[5] = color;
   //      pcr[6] = color;
   //      pcr[7] = color;
   //      pcr[8] = color;
   //      pcr[9] = color;
   //      pcr[10] = color;
   //      pcr[11] = color;
   //      pcr[12] = color;
   //      pcr[13] = color;
   //      pcr[14] = color;
   //      pcr[15] = color;
   //      pcr[16] = color;
   //      pcr[17] = color;
   //      pcr[18] = color;
   //      pcr[19] = color;
   //      pcr[20] = color;
   //      pcr[21] = color;
   //      pcr[22] = color;
   //      pcr[23] = color;
   //      pcr[24] = color;
   //      pcr[25] = color;
   //      pcr[26] = color;
   //      pcr[27] = color;
   //      pcr[28] = color;
   //      pcr[29] = color;
   //      pcr[30] = color;
   //      pcr[31] = color;
   //   }

   //   for (i=0; i<size_i32; i++ )
   //   {
   //      m_pcolorref[i]=color;
   //   }

   //}

   //COLORREF image::GetAverageColor()
   //{
   //   double dR = 0.0;
   //   double dG = 0.0;
   //   double dB = 0.0;
   //   int iRLine;
   //   int iGLine;
   //   int iBLine;
   //   double dDiv = this->cx * this->cy;
   //   if(dDiv > 0)
   //   {
   //      byte * lpb = (byte *) m_pcolorref;
   //      for (int y = 0; y < this->cy; y++)
   //      {
   //         iRLine = 0;
   //         iGLine = 0;
   //         iBLine = 0;
   //         for (int x = 0; x < this->cx; x++)
   //         {
   //            iRLine += lpb[2];
   //            iGLine += lpb[1];
   //            iBLine += lpb[0];
   //            lpb += 4;
   //         }
   //         dR += iRLine / dDiv;
   //         dG += iGLine / dDiv;
   //         dB += iBLine / dDiv;
   //      }
   //      int iR = (int) dR;
   //      int iG = (int) dG;
   //      int iB = (int) dB;
   //      return rgb(iR, iG, iB);
   //   }
   //   else
   //   {
   //      return 0;
   //   }
   //
   //}


   //void image::xor(image_impl image_impl)
   //{
   //   if(this->cx != pimage->cx
   //   || this->cy != pimage->cy)
   //   {
   //      return;
   //   }
   //   int iCount = this->cx * this->cy;
   //   LPDWORD lpd1 = (LPDWORD) m_pcolorref;
   //   LPDWORD lpd2 = (LPDWORD) pimage->get_data();
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      *lpd1 = *lpd1 ^ *lpd2;
   //      lpd1++;
   //      lpd2++;
   //   }
   //}

   //void image::create_frame(::size_i32 size, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   int iFrameWidth = size.cx / iSliceCount;
   //   int iFrameHeight = size.cy / iSliceCount;
   //   create(iFrameWidth, iFrameHeight);
   //}

   //void image::set_frame1(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = this->cx / iSliceCount;
   //   int iFrameHeight = this->cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * this->cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = 0; y < iFrameHeight; y++)
   //   {
   //      lpDestLine = &lpDest[y * this->cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine = *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void image::set_frame2(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   if(lpdata == nullptr)
   //      return;
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = this->cx / iSliceCount;
   //   int iFrameHeight = this->cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * this->cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * this->cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine = *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void image::xor_image_frame2(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   if(lpdata == nullptr)
   //      return;
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   if(iSliceCount == 0)
   //      iSliceCount = 1;
   //   int iFrameWidth = this->cx / iSliceCount;
   //   int iFrameHeight = this->cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpDest = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight * this->cx];
   //   COLORREF * lpSrc = (COLORREF *) lpdata;
   //   COLORREF * lpDestLine;
   //   for(int y = iFrameHeight - 1; y >= 0; y--)
   //   {
   //      lpDestLine = &lpDest[y * this->cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDestLine ^= *lpSrc;
   //          lpDestLine++;
   //          lpSrc++;
   //      }
   //   }
   //}

   //void image::get_frame(void * lpdata, int iFrame, int iFrameCount)
   //{
   //   int iSliceCount = (int) sqrt((double) iFrameCount);
   //   int iFrameWidth = this->cx / iSliceCount;
   //   int iFrameHeight = this->cy / iSliceCount;
   //   int iX = iFrame % iSliceCount;
   //   int iY = iFrame / iSliceCount;
   //   COLORREF * lpSrc = &m_pcolorref[iFrameWidth * iX + iY * iFrameHeight *  this->cx];
   //   COLORREF * lpDest = (COLORREF *) lpdata;
   //   COLORREF * lpSrcLine;
   //   for(int y = 0; y < iFrameHeight; y++)
   //   {
   //      lpSrcLine = &lpSrc[y * this->cx];
   //      for(int x = 0; x < iFrameWidth; x++)
   //      {
   //          *lpDest = *lpSrcLine;
   //          lpDest++;
   //          lpSrcLine++;
   //      }
   //   }
   //}

   //bool image::is_rgb_black()
   //{
   //   int iSize = this->cx * this->cy;
   //   COLORREF * lp = m_pcolorref;
   //   for(int i = 0; i < iSize; i++)
   //   {
   //      if((*lp & 0x00FFFFFF) != 0)
   //         return false;
   //      lp++;
   //   }
   //   return true;
   //}

   //void image::DivideRGB(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = this->cx * this->cy;
   //   byte * lp = ((byte *) m_pcolorref);
   //   int i = 0;
   //   int iCount1 = iCount - iCount % 8;
   //   for(; i < iCount1; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;

   //      lp[4] /= (byte) iDivide;
   //      lp[5] /= (byte) iDivide;
   //      lp[6] /= (byte) iDivide;

   //      lp[8] /= (byte) iDivide;
   //      lp[9] /= (byte) iDivide;
   //      lp[10] /= (byte) iDivide;

   //      lp[12] /= (byte) iDivide;
   //      lp[13] /= (byte) iDivide;
   //      lp[14] /= (byte) iDivide;

   //      lp[16] /= (byte) iDivide;
   //      lp[17] /= (byte) iDivide;
   //      lp[28] /= (byte) iDivide;

   //      lp[20] /= (byte) iDivide;
   //      lp[21] /= (byte) iDivide;
   //      lp[22] /= (byte) iDivide;

   //      lp[24] /= (byte) iDivide;
   //      lp[25] /= (byte) iDivide;
   //      lp[26] /= (byte) iDivide;

   //      lp[28] /= (byte) iDivide;
   //      lp[29] /= (byte) iDivide;
   //      lp[30] /= (byte) iDivide;

   //      lp += 4 * 8;
   //   }
   //   for(; i < iCount; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}

   //void image::DivideARGB(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = this->cx * this->cy;
   //   byte * lp = ((byte *) m_pcolorref);
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      lp[0] /= (byte) iDivide;
   //      lp[1] /= (byte) iDivide;
   //      lp[2] /= (byte) iDivide;
   //      lp[3] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}

   //void image::DivideA(int iDivide)
   //{
   //   if(iDivide == 0)
   //   {
   //      return;
   //   }
   //   int iCount = this->cx * this->cy;
   //   byte * lp = ((byte *) m_pcolorref);
   //   for(int i = 0; i < iCount; i++)
   //   {
   //      lp[3] /= (byte) iDivide;
   //      lp +=4;
   //   }
   //}


   bool image::stretch_image(::image * pimage)
   {

      return ::StretchDIBits(
             GDI_HDC(m_spgraphics.m_p),
             0, 0,
             this->width(), this->height(),
             0, 0,
             pimpl->width(), pimpl->height(),
             pimpl->get_data(),
             &dynamic_cast < image_impl * >(pimpl)->m_info,
             DIB_RGB_COLORS,
             SRCCOPY) != false;

   }


   ::draw2d::graphics * image::_get_graphics() const
   {
      return m_spgraphics;
   }

   //double image::pi()
   //{
   //   return dPi;
   //}

   // void image::fill_channel(int intensity, color::color::color::rgba::echannel echannel)
   // {
   //     int offset = ((int)echannel) % 4;
   //    int size_i32=this->cx*this->cy;

   //    BYTE * pb;

   //    int iSize32 = size / 32;
   //    int i;
   //    for (i=0; i < iSize32; i+=32 )
   //    {
   //       pb = ((BYTE * ) &m_pcolorref[i]) + offset;
   //       pb[0 * 4] = (byte) intensity;
   //       pb[1 * 4] = (byte) intensity;
   //       pb[2 * 4] = (byte) intensity;
   //       pb[3 * 4] = (byte) intensity;
   //       pb[4 * 4] = (byte) intensity;
   //       pb[5 * 4] = (byte) intensity;
   //       pb[6 * 4] = (byte) intensity;
   //       pb[7 * 4] = (byte) intensity;
   //       pb[8 * 4] = (byte) intensity;
   //       pb[9 * 4] = (byte) intensity;
   //       pb[10 * 4] = (byte) intensity;
   //       pb[11 * 4] = (byte) intensity;
   //       pb[12 * 4] = (byte) intensity;
   //       pb[13 * 4] = (byte) intensity;
   //       pb[14 * 4] = (byte) intensity;
   //       pb[15 * 4] = (byte) intensity;
   //       pb[16 * 4] = (byte) intensity;
   //       pb[17 * 4] = (byte) intensity;
   //       pb[18 * 4] = (byte) intensity;
   //       pb[19 * 4] = (byte) intensity;
   //       pb[20 * 4] = (byte) intensity;
   //       pb[21 * 4] = (byte) intensity;
   //       pb[22 * 4] = (byte) intensity;
   //       pb[23 * 4] = (byte) intensity;
   //       pb[24 * 4] = (byte) intensity;
   //       pb[25 * 4] = (byte) intensity;
   //       pb[26 * 4] = (byte) intensity;
   //       pb[27 * 4] = (byte) intensity;
   //       pb[28 * 4] = (byte) intensity;
   //       pb[29 * 4] = (byte) intensity;
   //       pb[30 * 4] = (byte) intensity;
   //       pb[31 * 4] = (byte) intensity;
   //    }

   //    for (i=0; i<size_i32; i++ )
   //    {
   //       *(((BYTE * ) &m_pcolorref[i]) + offset) = (byte) intensity;
   //    }
   //}


   // int image::cos(int i, int iAngle)
   // {
   //    return (int) (((_int64) i * CosN[iAngle]) >> 31);
   // }

   // int image::sin(int i, int iAngle)
   // {
   //    return (int) (((_int64) i * SinN[iAngle]) >> 31);
   // }

   // int image::cos10(int i, int iAngle)
   // {
   //    return (int) (((_int64) i * Cos10N[iAngle]) >> 34);
   // }

   // int image::sin10(int i, int iAngle)
   // {
   //    return (int) (((_int64) i * Sin10N[iAngle]) >> 34);
   // }

   // int image::width()
   // {
   //    return this->cx;
   // }

   // int image::height()
   // {
   //    return this->cy;
   // }


   //bool image::update_window(::aura::draw_interface * pwnd, ::message::message * pmessage, bool bTransferBuffer)
   //{

   //   rectangle_i64 rectWindow;

   //   pwnd->GetWindowRect(rectWindow);

   //   m_spgraphics->SetViewportOrg(0, 0);

   //   map();


   //   BYTE *dst=(BYTE*)get_data();
   //   i64 size = area();


   //   // >> 8 instead of / 255 subsequent alpha_blend operations say thanks on true_blend because (255) * (1/254) + (255) * (254/255) > 255

   //   /*
   //   while (size >= 8)
   //      {
   //         dst[0] = LOBYTE(((i32)dst[0] * (i32)dst[3])>> 8);
   //         dst[1] = LOBYTE(((i32)dst[1] * (i32)dst[3])>> 8);
   //         dst[2] = LOBYTE(((i32)dst[2] * (i32)dst[3])>> 8);

   //         dst[4+0] = LOBYTE(((i32)dst[4+0] * (i32)dst[4+3])>> 8);
   //         dst[4+1] = LOBYTE(((i32)dst[4+1] * (i32)dst[4+3])>> 8);
   //         dst[4+2] = LOBYTE(((i32)dst[4+2] * (i32)dst[4+3])>> 8);

   //         dst[8+0] = LOBYTE(((i32)dst[8+0] * (i32)dst[8+3])>> 8);
   //         dst[8+1] = LOBYTE(((i32)dst[8+1] * (i32)dst[8+3])>> 8);
   //         dst[8+2] = LOBYTE(((i32)dst[8+2] * (i32)dst[8+3])>> 8);

   //         dst[12+0] = LOBYTE(((i32)dst[12+0] * (i32)dst[12+3])>> 8);
   //         dst[12+1] = LOBYTE(((i32)dst[12+1] * (i32)dst[12+3])>> 8);
   //         dst[12+2] = LOBYTE(((i32)dst[12+2] * (i32)dst[12+3])>> 8);

   //         dst[16+0] = LOBYTE(((i32)dst[16+0] * (i32)dst[16+3])>> 8);
   //         dst[16+1] = LOBYTE(((i32)dst[16+1] * (i32)dst[16+3])>> 8);
   //         dst[16+2] = LOBYTE(((i32)dst[16+2] * (i32)dst[16+3])>> 8);

   //         dst[20+0] = LOBYTE(((i32)dst[20+0] * (i32)dst[20+3])>> 8);
   //         dst[20+1] = LOBYTE(((i32)dst[20+1] * (i32)dst[20+3])>> 8);
   //         dst[20+2] = LOBYTE(((i32)dst[20+2] * (i32)dst[20+3])>> 8);

   //         dst[24+0] = LOBYTE(((i32)dst[24+0] * (i32)dst[24+3])>> 8);
   //         dst[24+1] = LOBYTE(((i32)dst[24+1] * (i32)dst[24+3])>> 8);
   //         dst[24+2] = LOBYTE(((i32)dst[24+2] * (i32)dst[24+3])>> 8);

   //         dst[28+0] = LOBYTE(((i32)dst[28+0] * (i32)dst[28+3])>> 8);
   //         dst[28+1] = LOBYTE(((i32)dst[28+1] * (i32)dst[28+3])>> 8);
   //         dst[28+2] = LOBYTE(((i32)dst[28+2] * (i32)dst[28+3])>> 8);

   //         dst += 4 * 8;
   //         size -= 8;
   //      }
   //      while(size--)
   //      {
   //         dst[0] = LOBYTE(((i32)dst[0] * (i32)dst[3])>> 8);
   //         dst[1] = LOBYTE(((i32)dst[1] * (i32)dst[3])>> 8);
   //         dst[2] = LOBYTE(((i32)dst[2] * (i32)dst[3])>> 8);
   //         dst += 4;
   //      }
   //   */

   //   class ::rectangle_i32 rectangle(rectWindow);

   //   //Application.window_graphics_update_window(pwnd->get_window_graphics(),pwnd->get_handle(),m_pcolorref,rectangle,m_size.cx,m_size.cy,m_iScan, bTransferBuffer);

   //   return true;

   //}


   bool image::map(bool bApplyAlphaTransform)
   {

      if (m_bMapped)
      {

         return true;

      }

      if (m_pcolorrefRaw == nullptr)
      {

         return false;

      }

      GdiFlush();

      ((::image *) this)->m_bMapped = true;

      return true;

   }


   bool image::_unmap()
   {

      if (!m_bMapped)
      {

         return true;

      }

      if (m_pcolorrefRaw == nullptr)
      {

         return false;

      }

      i64 iArea = area();

      byte * p = (byte *)m_pcolorrefRaw;

      GdiFlush();

      /*
      for(int y = 0; y < cy; y++)
      {
        byte * p = &((byte *) m_pcolorref)[scan * y];
        for(int x = 0; x < cx; x++)
        {
           p[0] = (p[0] * p[3] / 255);
           p[1] = (p[1] * p[3] / 255);
           p[2] = (p[2] * p[3] / 255);
           p += 4;
        }
      }

      */

      ((::image *) this)->m_bMapped = false;

      return true;

   }


   //bool image::print_window(::aura::draw_interface * pwnd, ::message::message * pmessage)
   //{

   //   __pointer(::user::message) pusermessage(pmessage);

   //   if (pusermessage->m_wparam == 0)
   //   {

   //      return false;

   //   }

   //   m_spgraphics->attach((HDC) pusermessage->m_wparam);

   //   ::rectangle_i32 rectx;

   //   ::draw2d::bitmap * pbitmap = m_spgraphics->get_current_bitmap();

   //   ::GetCurrentObject((HDC) pusermessage->m_wparam, OBJ_BITMAP);

   //   //      u32 dw = ::GetLastError();
   //   ::size_i32 size = pbitmap->get_size();

   //   rectx.left = 0;
   //   rectx.top = 0;
   //   rectx.right = size.cx;
   //   rectx.bottom = size.cy;

   //   try
   //   {

   //      ::rectangle_i32 rectWindow;

   //      pwnd->GetWindowRect(rectWindow);

   //      ::image_pointer pimage = create_image(rectWindow.bottom_right());

   //      if (!pimage)
   //      {

   //         return false;

   //      }

   //      ::draw2d::graphics_pointer & pgraphics = pimage->g();

   //      if (pgraphics->get_os_data() == nullptr)
   //      {

   //         return false;

   //      }

   //      ::rectangle_i32 rectPaint;
   //      ::rectangle_i32 rectUpdate;
   //      rectUpdate = rectWindow;
   //      rectPaint = rectWindow;
   //      rectPaint.offset(-rectPaint.top_left());
   //      m_spgraphics->SelectClipRgn(nullptr);
   //      pwnd->_001OnDeferPaintLayeredWindowBackground(pimage->g());
   //      m_spgraphics->SelectClipRgn(nullptr);

   //      m_spgraphics-> SetViewportOrg(::point_i32());

   //      pwnd->_000OnDraw(pimage->g());

   //      m_spgraphics->SetViewportOrg(::point_i32());
   //      //(dynamic_cast<::win::graphics * >(pgraphics))->FillSolidRect(rectUpdate.left, rectUpdate.top, 100, 100, 255);
   //      m_spgraphics->SelectClipRgn(nullptr);
   //      m_spgraphics->SetViewportOrg(::point_i32());

   //      m_spgraphics->SelectClipRgn( nullptr);
   //      m_spgraphics->BitBlt(rectPaint.left, rectPaint.top,
   //                           rectPaint.width(), rectPaint.height(),
   //                           pgraphics, rectUpdate.left, rectUpdate.top,
   //                           SRCCOPY);

   //   }
   //   catch(...)
   //   {
   //   }

   //   m_spgraphics->fill_rectangle(rectx, rgb(255, 255, 255));

   //   pmessage->m_bRet = true;
   //   pusermessage->set_lresult(0);

   //   return true;

   //}

//
//procedure TAlphaBitmapWrapper.ProcessTransparency(const Alpha: Byte; TranspRect: TRect);
//var
//  LoopX : Integer;
//  PreMult : Single;
//  PQuad : PQuadColor;
//  LoopY: Integer;
//begin
//  GdiFlush; // Need to flush before any manipulation of bits
//  IntersectRect(TranspRect, TranspRect, Rect(0, 0, FWidth, FHeight)); // Clip to valid bounds
//
//  PreMult := Alpha / 255.0;
//  for LoopY := TranspRect.Top to TranspRect.Bottom - 1 do begin
//    PQuad := FQuads;
//    Inc(PQuad, LoopY);
//    for LoopX := TranspRect.Left to TranspRect.Right - 1 do begin
//      if PQuad.WrittenByGDI then begin
//        PQuad.SetAlpha(Alpha, PreMult);
//      end else begin
//        PQuad.Quad := 0;
//      end;
//      Inc(PQuad);
//    end;
//  end;
//end;


   bool image::process_blend(COLORREF clr, i32 x, i32 y, ::draw2d::e_alpha_mode ealphamode, ::image * pimageSrc)
   {

      ::GdiFlush();

      i32 a = colorref_get_a_value(clr);

      ::color::color::e_channel echannel;

      i32 bTune;

      if(colorref_get_g_value(clr) > colorref_get_b_value(clr))
      {
         if(colorref_get_b_value(clr) > colorref_get_r_value(clr))
         {
            echannel = ::color::e_channel_green;
            bTune = colorref_get_g_value(clr);
         }
         else if(colorref_get_g_value(clr) >  colorref_get_r_value(clr))
         {
            echannel = ::color::e_channel_green;
            bTune = colorref_get_g_value(clr);
         }
         else
         {
            echannel = ::color::e_channel_red;
            bTune = colorref_get_r_value(clr);
         }
      }
      else if(colorref_get_g_value(clr) > colorref_get_r_value(clr))
      {
         echannel = ::color::e_channel_blue;
         bTune = colorref_get_b_value(clr);
      }
      else if(colorref_get_r_value(clr) > colorref_get_b_value(clr))
      {
         echannel = ::color::e_channel_red;
         bTune = colorref_get_r_value(clr);
      }
      else
      {
         echannel = ::color::e_channel_blue;
         bTune = colorref_get_b_value(clr);
      }

      i64 size = area();

      byte * pb = (byte *)m_pcolorrefRaw;
      byte * pbTune = ((byte *)m_pcolorrefRaw) + ((i32) echannel);

      //bTune *= 255;

      bTune = 255  * 3;

      i32 aTune = a * 255;

      i32 rectangle = colorref_get_r_value(clr);
      i32 g = colorref_get_g_value(clr);
      i32 b = colorref_get_b_value(clr);

      if(bTune == 0)
      {

         rectangle_i32 *= a;
         g *= a;
         b *= a;

         while(size > 0)
         {
            if(pb[3] == 0)
            {
               pb[0] = rectangle_i32 * pb[0] / (255 * 255);
               pb[1] = g * pb[1] / (255 * 255);
               pb[2] = b * pb[2] / (255 * 255);
               pb[3] = a;
            }
            else
            {
               pb[3] = 0;
            }
            size--;
            pb+=4;
         }

      }
      else
      {
         if(ealphamode == ::draw2d::alpha_mode_blend)
         {
            while(size > 0)
            {
               //if(pb[3] == 0)
               {
                  pb[3] = byte_clip(a * (pb[0] + pb[1] + pb[2]) / bTune);
                  pb[0] = b * pb[3] / 255;
                  pb[1] = g * pb[3] / 255;
                  pb[2] = rectangle_i32 * pb[3] / 255;
               }
               //else
               {
                  // pb[3] = 0;
               }
               size--;
               pb+=4;
               pbTune+=4;
            }
         }
         else
         {
            byte * ps = (byte *) pimageSrc->m_pcolorrefRaw;
            ps += pimageSrc->m_iScan * y + x * sizeof(COLORREF);
            int s = pimageSrc->m_iScan - width() * sizeof(COLORREF);
            int c = width();

            ::rectangle_i32 rect1(x, y,  x  + width(), y + height());
            ::rectangle_i32 rect2(0, 0, pimageSrc->width(), pimageSrc->height());
            ::rectangle_i32 rect3;

            rect3.intersect(rect1, rect2);

            size = minimum(size, (i32) rect3.area());
            while(size > 0)
            {
               if(pb[3] == 0)
               {
                  ps[3] = byte_clip(a * (pb[0] + pb[1] + pb[2]) / bTune);
                  ps[0] = (byte) b;
                  ps[1] = (byte) g;
                  ps[2] = (byte) rectangle_i32;
               }
               else
               {
                  /*                  pb[3] = ps[3];
                                    pb[2] = ps[2];
                                    pb[1] = ps[1];
                                    pb[0] = ps[0];
                                    */
               }
               c--;
               size--;
               pb+=4;
               pbTune+=4;
               ps+=4;
               if(c <= 0)
               {
                  c = width();
                  ps += s;
               }
            }
         }
      }


      return true;

   }


   bool image::process_blend(::image * pimage, i32 x, i32 y, ::draw2d::e_alpha_mode ealphamode, ::image * pimageSrc)
   {

      if (::is_null(pimpl))
      {

         return false;

      }

      if (pimageSrc->size() != size())
      {

         return false;

      }

      ::GdiFlush();

      i32 size = (i32) area();

      byte * pdataDst = (byte *) pimageSrc->colorref();

      byte * pdataSrc = (byte *) pimpl->colorref();

      if(ealphamode == ::draw2d::alpha_mode_set)
      {
         byte * ps = (byte *) pimageSrc->colorref();
         ps += pimageSrc->m_iScan * y + x * sizeof(COLORREF);
         int s = pimageSrc->m_iScan - width() * sizeof(COLORREF);
         int c = width();

         while(size > 0)
         {

            if(pdataDst[3] == 0)
            {
               ps[3] = pdataSrc[3] * pdataDst[1] / 255;
               ps[0] = pdataSrc[0] * ps[3] / 255;
               ps[1] = pdataSrc[1] * ps[3] / 255;
               ps[2] = pdataSrc[2] * ps[3] / 255;
            }
            else
            {
               //pdataDst[3] = ps[3];
               //pdataDst[2] = ps[2];
               //pdataDst[1] = ps[1];
               //pdataDst[0] = ps[0];

            }
            c--;
            size--;

            pdataDst+=4;
            pdataSrc+=4;
            ps+=4;
            if(c <= 0)
            {
               c = width();
               ps+= s;
            }

         }

      }
      else
      {
         while(size > 0)
         {


            pdataDst[3] = pdataSrc[3] * pdataDst[1] / 255;
            pdataDst[0] = pdataSrc[0] * pdataDst[3] / 255;
            pdataDst[1] = pdataSrc[1] * pdataDst[3] / 255;
            pdataDst[2] = pdataSrc[2] * pdataDst[3] / 255;

            size--;
            pdataDst+=4;
            pdataSrc+=4;

         }

      }

      return true;

   }


   bool image::process_blend(::draw2d::brush * pbrush, i32 x, i32 y, ::draw2d::e_alpha_mode ealphamode, ::image * pimageSrc)
   {

      if(pbrush->m_etype == ::draw2d::brush::e_type_null)
      {

         return false;

      }

      if(pbrush->m_etype == ::draw2d::brush::type_solid)
      {

         process_blend(pbrush->m_color, x, y, ealphamode, pimageSrc);

         GDI_BRUSH(pbrush)->m_bProcess = false;

         return true;

      }

      if(pbrush->m_etype == ::draw2d::brush::type_linear_gradient_point_color)
      {

         ::image_pointer pimage1;

         pimage1 = create_image(get_size());

         point_i32 p1 = pbrush->m_point1;
         point_i32 p2 = pbrush->m_point2;
         p1.offset(-x, -y);
         p2.offset(-x, -y);
         pimage1->gradient_fill(pbrush->m_color1, pbrush->m_color2, p1, p2);
         process_blend(pimage1, x, y, ealphamode, pimageSrc);
         GDI_BRUSH(pbrush)->m_bProcess = false;
         return true;
      }

      return false;

   }


   bool image::process_initialize(::draw2d::brush * pbrush, bool bReset)
   {
      if(pbrush->m_etype == ::draw2d::brush::e_type_null)
      {
         return false;
      }
      if(pbrush->m_etype == ::draw2d::brush::type_solid)
      {
         GDI_BRUSH(pbrush)->m_bProcess = true;
         pbrush->m_bUpdated = false;
         if(bReset)
         {
            fill(255, 0, 0, 0);
         }
         return true;
      }
      if(pbrush->m_etype == ::draw2d::brush::type_linear_gradient_point_color)
      {
         GDI_BRUSH(pbrush)->m_bProcess = true;
         pbrush->m_bUpdated = false;
         if(bReset)
         {
            fill(255, 0, 0, 0);
         }
         return true;
      }
      return false;
   }


   bool image::process_blend(::draw2d::pen * ppen, i32 x, i32 y, ::draw2d::e_alpha_mode ealphamode, ::image * pimage)
   {

      if(ppen->m_etype == ::draw2d::pen::e_type_null)
      {

         return false;

      }

      if(ppen->m_etype == ::draw2d::e_pen_solid)
      {

         process_blend(ppen->m_color, x, y, ealphamode, pimpl);

         GDI_PEN(ppen)->m_bProcess = false;

         return true;

      }

      return false;

   }


   bool image::process_initialize(::draw2d::pen * ppen, bool bReset)
   {

      if(ppen->m_etype == ::draw2d::pen::e_type_null)
      {

         return false;

      }

      if(ppen->m_etype == ::draw2d::e_pen_solid)
      {

         GDI_PEN(ppen)->m_bProcess = true;

         ppen->m_bUpdated = false;

         if(bReset)
         {

            fill(255, 0, 0, 0);

         }

         return true;

      }

      return false;

   }


   /*   bool image::from(::draw2d::graphics_pointer & pgraphics, FIBITMAP *pfibitmap, bool bUnloadFI)
      {

         if(pfibitmap == nullptr)
              return false;

         BITMAPINFO * pbi = FreeImage_GetInfo(pfibitmap);
         void * pdata = FreeImage_GetBits(pfibitmap);

         if(!create(pbi->bmiHeader.biWidth, pbi->bmiHeader.biHeight))
            return false;


         COLORREF * pcolorref = nullptr;

         HBITMAP hbitmap = ::CreateDIBSection(nullptr, &m_info, DIB_RGB_COLORS, (void **) &pcolorref, nullptr, 0);

         if(hbitmap == nullptr)
         {
            Destroy();
            return false;
         }

         HDC hdc = ::CreateCompatibleDC(nullptr);

         if(pbi->bmiHeader.biHeight != SetDIBits(
            hdc,
            hbitmap,
            0,
            pbi->bmiHeader.biHeight,
            pdata,
            pbi,
            DIB_RGB_COLORS))
         {
            Destroy();
            if(bUnloadFI)
            {
               FreeImage_Unload(pfibitmap);
            }
            return false;
         }

         memcpy_dup(m_pcolorref, pcolorref, (size_t) (area() * sizeof(COLORREF)));


         RGBQUAD bkcolor;

         if(pbi->bmiHeader.biBitCount == 32)
         {
         }
         else if(pbi->bmiHeader.biBitCount <= 24 && FreeImage_GetTransparencyCount(pfibitmap) <= 0)
         {
            fill_channel(0xff, ::color::e_channel_alpha);
         }
         else if(FreeImage_GetBackgroundColor(pfibitmap, &bkcolor))
         {
            transparent_color(bkcolor);
         }

         if(bUnloadFI)
         {
            FreeImage_Unload(pfibitmap);
         }


         return true;
      }*/


   COLORREF image::make_colorref(i32 a, i32 rectangle, i32 g, i32 b)
   {

      return argb(a, b, g, rectangle);

   }


} // namespace draw2d_gdi





