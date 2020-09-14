#include "framework.h"


namespace draw2d_gdi
{

   int get_os_fill_mode(::draw2d::e_fill_mode efillmode)
   {

      int nMode;
      switch(efillmode)
      {
      case ::draw2d::fill_mode_winding:
         nMode = WINDING;
         break;
      case ::draw2d::fill_mode_alternate:
         nMode = ALTERNATE;
         break;
      default:
         nMode = WINDING;
         break;
      }

      return nMode;

   }


   region::region(::layered * pobjectContext) :
      ::draw2d::region(pobject)
   {
      m_bUpdated = true;
   }


   region::~region()
   {

   }


   region::operator HRGN() const
   {

      return (HRGN)(this == nullptr ? nullptr : get_os_data());

   }


   bool region::create_rect(int x1, int y1, int x2, int y2)
   {

      return Attach(::CreateRectRgn(x1, y1, x2, y2));

   }


   bool region::create_rect(const ::rect & rect)
   {

      return Attach(::CreateRectRgnIndirect(rect));

   }


   bool region::create_oval(int x1, int y1, int x2, int y2)
   {

      return Attach(::CreateEllipticRgn(x1, y1, x2, y2));

   }


   bool region::create_oval(const ::rect & rect)
   {

      return Attach(::CreateEllipticRgnIndirect(rect));

   }


   bool region::create_polygon(LPPOINT lpPoints, i32 nCount, ::draw2d::e_fill_mode efillmode)
   {

      return Attach(::CreatePolygonRgn(lpPoints, nCount, get_os_fill_mode(efillmode)));

   }

   bool region::create_poly_polygon(LPPOINT lpPoints, LPINT lpPolyCounts, i32 nCount, ::draw2d::e_fill_mode efillmode)
   {

      return Attach(::CreatePolyPolygonRgn(lpPoints, lpPolyCounts, nCount, get_os_fill_mode(efillmode)));

   }


   bool region::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3)
   {

      return Attach(::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3));

   }


   bool region::CreateFromPath(::draw2d::graphics_pointer & pgraphics)
   {

      ASSERT(pgraphics != nullptr);

      return Attach(::PathToRegion((dynamic_cast<::draw2d_gdi::graphics * >(pgraphics))->get_handle1()));

   }


   bool region::CreateFromData(const XFORM* lpXForm, int nCount, const RGNDATA* pRgnData)
   {

      return Attach(::ExtCreateRegion(lpXForm, nCount, pRgnData));
   }
   int region::GetRegionData(LPRGNDATA lpRgnData, int nDataSize) const
   {

      ASSERT(get_os_data() != nullptr); return (int)::GetRegionData((HRGN)get_os_data(), nDataSize, lpRgnData);
   }
   void region::SetRectRgn(int x1, int y1, int x2, int y2)
   {

      ASSERT(get_os_data() != nullptr); ::SetRectRgn((HRGN)get_os_data(), x1, y1, x2, y2);
   }

   void region::SetRectRgn(const ::rect & rect)
   {

      ::SetRectRgn((HRGN)get_os_data(), rect.left, rect.top, rect.right, rect.bottom);

   }


   int region::CombineRgn(const ::draw2d::region* pRgn1, const ::draw2d::region* pRgn2, int nCombineMode)
   {

      ASSERT(get_os_data() != nullptr);

      return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgn1->get_os_data(), (HRGN)pRgn2->get_os_data(), nCombineMode);

   }


   int region::CopyRgn(const ::draw2d::region* pRgnSrc)
   {

      ASSERT(get_os_data() != nullptr);

      return ::CombineRgn((HRGN)get_os_data(), (HRGN)pRgnSrc->get_os_data(), nullptr, RGN_COPY);

   }


   bool region::EqualRgn(const ::draw2d::region* pRgn) const
   {

      ASSERT(get_os_data() != nullptr);

      return ::EqualRgn((HRGN)get_os_data(), (HRGN)pRgn->get_os_data()) != FALSE;

   }


   int region::OffsetRgn(int x, int y)
   {

      ASSERT(get_os_data() != nullptr);

      return ::OffsetRgn((HRGN)get_os_data(), x, y) != FALSE;


   }


   int region::OffsetRgn(const ::point & point)
   {

      ASSERT(get_os_data() != nullptr);

      return ::OffsetRgn((HRGN)get_os_data(), point.x, point.y);

   }


   int region::GetRgnBox(RECT * prect) const
   {

      ASSERT(get_os_data() != nullptr);

      return ::GetRgnBox((HRGN)get_os_data(), prect);

   }


   bool region::contains(int x, int y) const
   {

      ASSERT(get_os_data() != nullptr);

      return ::PtInRegion((HRGN)get_os_data(), x, y) != FALSE;

   }


   bool region::contains(const ::point & point) const
   {
      ASSERT(get_os_data() != nullptr);

      return ::PtInRegion((HRGN)get_os_data(), point.x, point.y) != FALSE;

   }


   bool region::RectInRegion(const ::rect & rect) const
   {

      ASSERT(get_os_data() != nullptr);

      return ::RectInRegion((HRGN)get_os_data(), rect) != FALSE;

   }


   bool region::create()
   {

      ::draw2d_gdi::object::create();

      //for(int i = 0; i <

      return true;

   }

   bool region::destroy()
   {

      ::draw2d::region::destroy();
      ::draw2d_gdi::object::destroy();

      return true;

   }

} // namespace draw2d_gdi





