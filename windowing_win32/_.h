#pragma once


#include "aura/_.h"
#include "aura/operating_system.h"
#undef new
#include <gdiplus.h>
#include "aura/user/_user.h"


#ifdef _WINDOWING_WIN32_STATIC
#define CLASS_DECL_WINDOWING_WIN32
#elif defined(_WINDOWING_WIN32_LIBRARY)
#define CLASS_DECL_WINDOWING_WIN32  CLASS_DECL_EXPORT
#else
#define CLASS_DECL_WINDOWING_WIN32  CLASS_DECL_IMPORT
#endif


// Derived from Microsoft KB318876!!
// on 2014-06-29!! During World Cup Games 2014!! Oitavas de Final, third game between Nederland and Mexico!!
// Thank you God!!
// Thank you Microsoft!!
// Thank you Pappi!! Pelo passeio no Mercado Municipal de Curitiba com a Mummi e pelo almoo com o-bento de sushi e onigiri delicioso!!
// Obrigado Mummi por tudo!! E por hoje tambm e por propiciar isso tudo!! Por falar to profundamente do Chico Xavier mesmo com muitas dvidas minhas!!
// Meu Deus me ajude!!

namespace windowing_win32
{

   
   class windowing;
   class hwnd_array;


   CLASS_DECL_WINDOWING_WIN32 void wingdi_enum_fonts(::draw2d::font_enum_item_array & itema, bool bRaster, bool bTrueType, bool bOther);
   CLASS_DECL_WINDOWING_WIN32 ::e_char_set wingdi_get_cs(int iCharSet);

   CLASS_DECL_WINDOWING_WIN32 HMONITOR get_primary_monitor_handle();
   CLASS_DECL_WINDOWING_WIN32 HMONITOR get_window_monitor_handle(HWND hwnd);


   CLASS_DECL_WINDOWING_WIN32 HBITMAP create_hbitmap(pixmap * ppixmap);
   CLASS_DECL_WINDOWING_WIN32 HBITMAP create_hbitmap_2(pixmap * ppixmap);


   CLASS_DECL_WINDOWING_WIN32 HBITMAP create_alpha_bitmap_v5(const ::image * pimage);
   CLASS_DECL_WINDOWING_WIN32 HICON create_alpha_icon(const ::image * pimage, bool bIcon = true, int xHotSpot = 0, int yHotSpot = 0);
   CLASS_DECL_WINDOWING_WIN32 HCURSOR create_alpha_cursor(const ::image * pimage, int xHotSpot, int yHotSpot);
   CLASS_DECL_WINDOWING_WIN32 HBITMAP create_windows_dib(const ::size_i32 & size, i32 * piScan = nullptr, color32_t ** ppdata = nullptr);;
   CLASS_DECL_WINDOWING_WIN32 HICON load_icon(::object * pobject, string_array & straMatter, string strIcon, int cx, int cy);


   ::e_status gdiplus_draw_text(::draw2d::graphics * pgraphics, ::draw2d::path * ppath, const string & str, rectangle_f64 & rectParam, const ::e_align & ealign, const ::e_draw_text & edrawtext, ::draw2d::font * pfont, double dFontWidth, ::draw2d::brush * pbrush = nullptr, bool bMeasure = false);


   inline auto gdiplus_color(const ::color & color)
   {

      return Gdiplus::Color(color.alpha, color.red, color.green, color.blue);

   }


   /// from top to bottom
   //CLASS_DECL_WINDOWING_WIN32 __pointer(hwnd_array) get_top_level_windows(bool bDesktop = true, bool bVisible = true);



} // namespace windowing_win32


#include "window_util.h"
#include "win32.h"


#include "buffer.h"
#include "display.h"
#include "cursor.h"
#include "icon.h"
#include "imm_client.h"
#include "imm_context.h"
#include "monitor.h"
#include "keyboard.h"


#include "top_level_enum.h"


#include "system_interaction.h"


#include "window.h"


#include "windowing.h"





