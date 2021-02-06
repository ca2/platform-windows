// created by Camilo 2021-01-31 04:56 BRT <3CamiloSasukeThomasBorregaardSoerensen
#include "framework.h"
#include "acme/const/timer.h"
#include "window.h"
#include <dwmapi.h>

#undef ALOG_CONTEXT
#define ALOG_CONTEXT ::trace_object(::trace_category_windowing)
wstring CLASS_DECL_WINDOWING_WIN32 windows_get_user_interaction_window_class(::user::interaction * puserinteraction);

extern CLASS_DECL_ACME const unichar gen_Wnd[];           // simple child windows/controls
extern CLASS_DECL_ACME const unichar gen_WndControlBar[]; // controls with gray backgrounds
extern CLASS_DECL_ACME const unichar gen_WndMDIFrame[];
extern CLASS_DECL_ACME const unichar gen_WndFrameOrView[];
extern CLASS_DECL_ACME const unichar gen_WndOleControl[];



CLASS_DECL_WINDOWING_WIN32 WNDPROC windows_user_interaction_impl_get_window_procedure();


bool has_autohide_appbar(::u32 edge, const rectangle_i32 & mon);


CLASS_DECL_WINDOWING_WIN32 bool __is_combo_box_control(HWND hwnd, ::u32 nStyle);
CLASS_DECL_WINDOWING_WIN32 bool __check_center_dialog(const char * pszResource);

CLASS_DECL_WINDOWING_WIN32 bool __compare_class_name(HWND hwnd, const char * pszClassName);

CLASS_DECL_WINDOWING_WIN32 bool hook_window_create(::windowing_win32::window * pwindow);
CLASS_DECL_WINDOWING_WIN32 bool unhook_window_create();
CLASS_DECL_WINDOWING_WIN32 void reset_message_cache();


namespace windowing_win32
{


   window::window()
   {

      //set_layer(LAYERED_IMPL, this);
      //m_plongmap  = new iptr_to_iptr;

//      m_iXic = 0;
//
//      m_xic = nullptr;
//
//      for (auto &i : m_iaNetWmState)
//      {
//
//         i = -1;
//
//      }
//
//      m_hcursorLast = 0;
//
//      m_hthread = 0;
//
//      m_window = None;

      m_pimpl = nullptr;

      m_bMessageOnlyWindow = false;

      //      m_osdisplay = nullptr;
      //
      //      __zero(m_visual);
      //
      //      m_iDepth = -1;
      //
      //      m_iScreen = -1;
      //
      //      m_colormap = None;
      //
      //      m_iXic = 0;

   }


   window::~window()
   {

      //::acme::del(m_plongmap);

   }


   void window::assert_valid() const
   {

      //::windowing_win32::window::assert_valid();

      //if (((::windowing_win32::window *)this)->get_hwnd() == nullptr)
      //   return;     // null (unattached) windows are valid

      //// check for special wnd??? values
      //ASSERT(HWND_TOP == nullptr);       // same as desktop
      //if (((::windowing_win32::window *)this)->get_hwnd() == HWND_BOTTOM)
      //{
      //}
      //else if (((::windowing_win32::window *)this)->get_hwnd() == HWND_TOPMOST)
      //{
      //}
      //else if (((::windowing_win32::window *)this)->get_hwnd() == HWND_NOTOPMOST)
      //{
      //}
      //else
      //{
      //   // should be a normal window
      //   ASSERT(::IsWindow(((::windowing_win32::window *)this)->get_hwnd()));

      //}
   }

   
   void window::dump(dump_context & dumpcontext) const
   {

      //::windowing_win32::window::dump(dumpcontext);

      //dumpcontext << "\nm_oswindow_ = " << ((::windowing_win32::window *)this)->get_hwnd();

      //if (((::windowing_win32::window *)this)->get_hwnd() == nullptr || ((::windowing_win32::window *)this)->get_hwnd() == HWND_BOTTOM ||
      //   ((::windowing_win32::window *)this)->get_hwnd() == HWND_TOPMOST || ((::windowing_win32::window *)this)->get_hwnd() == HWND_NOTOPMOST)
      //{
      //   // not a normal window - nothing more to dump
      //   return;
      //}

      //if (!::IsWindow(((::windowing_win32::window *)this)->get_hwnd()))
      //{
      //   // not a valid window
      //   dumpcontext << " (illegal hwnd)";
      //   return; // don't do anything more
      //}

      ////auto puserinteraction = System.ui_from_handle(((::windowing_win32::window *)this)->get_hwnd());
      ////if (puserinteraction->m_pimpl != (::user::window *)this)
      ////   dumpcontext << " (Detached or temporary window)";
      ////else
      ////   dumpcontext << " (permanent window)";

      ////wchar_t szBuf[64];
      ////::GetClassNameW(((::windowing_win32::window *)this)->get_hwnd(), szBuf, _countof(szBuf));
      ////dumpcontext << "\nclass name = \"" << szBuf << "\"";

      ////::rectangle_i32 rectangle;
      ////((::windowing_win32::window *)this)->puserinteraction->layout().window().screen_rect(&rectangle);
      ////dumpcontext << "\nrect = " << rectangle_i32;
      ////dumpcontext << "\nparent ::user::window * = " << ::hex::lower_from((::iptr)((::windowing_win32::window *)this)->get_parent());

      ////dumpcontext << "\nstyle = " << (uptr)::GetWindowLong(((::windowing_win32::window *)this)->get_hwnd(), GWL_STYLE);
      ////if (::GetWindowLong(((::windowing_win32::window *)this)->get_hwnd(), GWL_STYLE) & WS_CHILD)
      ////   dumpcontext << "\nid = " << __get_dialog_control_id(((::windowing_win32::window *)this)->get_hwnd());

      ////dumpcontext << "\n";

   }


   //void window::install_system_interaction_message_handling(::user::interaction * puserinteraction)
   //{

   //   auto pchannel = puserinteraction->m_pimpl;

   //   MESSAGE_LINK(WM_SETTINGCHANGE, pchannel, this, &window::_001OnMessage);
   //   MESSAGE_LINK(WM_FONTCHANGE, pchannel, this, &window::_001OnMessage);

   //}


   ::e_status window::create_window(::user::interaction_impl * pimpl)
   {


      auto psession = Session;

      auto puser = psession->user();

      auto pwindowing = puser->windowing();

      m_pwindowing = pwindowing;


      //__refer(puserinteraction->m_pthreadUserInteraction, ::get_task() OBJ_REF_DBG_COMMA_THIS_FUNCTION_LINE);

      //puserinteraction->m_pthreadUserInteraction->uiptra().add(puserinteraction);

      //__refer(m_pthreadUserImpl, puserinteraction->m_pthreadUserInteraction OBJ_REF_DBG_COMMA_THIS_FUNCTION_LINE);

      //m_strDebug += ::str::demangle(puserinteraction->type_name()) + ";";

      //ASSERT(pusersystem->m_createstruct.lpszClass == nullptr || __is_valid_string(pusersystem->m_createstruct.lpszClass) || __is_valid_atom(pusersystem->m_createstruct.lpszClass));

      //ENSURE_ARG(pusersystem->m_createstruct.lpszName == nullptr || __is_valid_string(pusersystem->m_createstruct.lpszName));

      //wstring wstrClassName(pusersystem->m_createstruct.lpszClass);
      wstring wstrClassName;

      //if (wstrClassName.is_empty())
      //{

      auto puserinteraction = pimpl->m_puserinteraction;

      wstrClassName = windows_get_user_interaction_window_class(puserinteraction);

      //}

      //pusersystem->m_createstruct.lpszClass = wstrClassName;

      if (!puserinteraction->m_pusersystem)
      {

         puserinteraction->m_pusersystem = __new(::user::system);

      }

      if (!puserinteraction->pre_create_window(puserinteraction->m_pusersystem))
      {

         return false;

      }

      install_message_routing(puserinteraction);

      //if (!hook_window_create(this))
      //{

      //   PostNcDestroy();

      //   return false;

      //}

      // if window is not created, it may destroy this object, so keep the app as local ::payload

      //thread_property("wnd_init") = this;

      //::aura::application * papp = &Application;

      //wstring wstrWindowName(pusersystem->m_createstruct.lpszName);
      wstring wstrWindowName;

      //string char

      //wstring wchar_t

      //if (wstrWindowName.is_empty())
      //{

      //   wstrWindowName = L"window";

      //}

#if 0

      WCHAR szTitle[256];
      WCHAR szWindowClass[256];

      wcscpy(szTitle, L"123");
      wcscpy(szWindowClass, L"WindowsDestkop1");

      //HWND hwnd = CreateWindowExW(pusersystem->m_createstruct.dwExStyle, szWindowClass, wstrWindowName, pusersystem->m_createstruct.style,
        // pusersystem->m_createstruct.x, pusersystem->m_createstruct.y, pusersystem->m_createstruct.cx, pusersystem->m_createstruct.cy, pusersystem->m_createstruct.hwndParent, pusersystem->m_createstruct.hMenu, pusersystem->m_createstruct.hInstance, pusersystem->m_createstruct.lpCreateParams);
      HWND hwnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED,
         CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, pusersystem->m_createstruct.hInstance, nullptr);
      //if (!hwnd)
      //{
         //return false;
      //}

      ::ShowWindow(hwnd, SW_SHOWNORMAL);
      ::UpdateWindow(hwnd);

#if 0

      MSG msg;

      // Main message loop:
      while (mq_get_message(&msg, nullptr, 0, 0))
      {
         //if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
         {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
         }
      }

#else

      m_puserthread->run();

#endif

      //return true;

#else

      ::u32 dwExStyle = puserinteraction->GetExStyle();
      ::u32 dwStyle = puserinteraction->GetStyle();

      auto pusersystem = puserinteraction->m_pusersystem;

      auto & createstruct = pusersystem->m_createstruct;

      dwExStyle = createstruct.dwExStyle;
      dwStyle = createstruct.style;

      windowing()->__synthesizes_creates_styles(puserinteraction, dwExStyle, dwStyle);

      pusersystem->m_pwindow = this;

      puserinteraction->layout().sketch().set_modified();

      int x = puserinteraction->layout().sketch().origin().x;
      int y = puserinteraction->layout().sketch().origin().y;
      int cx = puserinteraction->layout().sketch().size().cx;
      int cy = puserinteraction->layout().sketch().size().cy;

      HWND hwndParent = nullptr;

      if (puserinteraction->m_bMessageWindow)
      {

         hwndParent = HWND_MESSAGE;

      }

      HMENU hmenu = nullptr;

      HINSTANCE hinstance = (HINSTANCE)GetModuleHandleW(L"windowing_win32.dll");

      void * lpCreateParams = nullptr;

      if (puserinteraction->m_pusersystem)
      {

         lpCreateParams = puserinteraction->m_pusersystem->m_createstruct.lpCreateParams;

      }

      m_pimpl = pimpl;

      HWND hwnd = ::CreateWindowExW(
         dwExStyle,
         wstrClassName,
         wstrWindowName,
         dwStyle,
         x,
         y,
         cx,
         cy,
         hwndParent,
         hmenu,
         hinstance,
         lpCreateParams);

#endif

      //HWND hwnd = CreateWindowW(wstrClassName, wstrWindowName, WS_OVERLAPPED,
      //   CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, pusersystem->m_createstruct.hInstance, nullptr);

      //::ShowWindow(hwnd,SW_SHOWNORMAL);
      //::UpdateWindow(hwnd);

      //bool bUnicode = IsWindowUnicode(hwnd);

      u32 dwLastError = ::GetLastError();

      //if (!unhook_window_create())
      //{

      //   PostNcDestroy();        // cleanup if CreateWindowEx fails too soon

      //}

      if (puserinteraction->m_bMessageWindow)
      {

         puserinteraction->m_ewindowflag -= e_window_flag_graphical;

      }

      if (hwnd == nullptr)
      {

         //if (papp == nullptr)
         //{

         //   return false;

         //}

         string strLastError = FormatMessageFromSystem(dwLastError);

         string strMessage;

         strMessage.Format("%s\n\nSystem Error Code: %d", strLastError, dwLastError);

         TRACE(trace_category_appmsg, e_trace_level_warning, "Warning: Window creation failed: get_last_error returned:\n");

         TRACE(trace_category_appmsg, e_trace_level_warning, "%s\n", strMessage);

         try
         {

            if (dwLastError == 0x0000057e)
            {

               TRACE("Cannot create a top-level child window.");

            }
            else
            {

               TRACE("%s", strMessage);

            }

         }
         catch (...)
         {

         }

      }

      if (hwnd == nullptr)
      {

         return false;

      }

      puserinteraction->m_ewindowflag += ::e_window_flag_is_window;

      //puserinteraction->layout().sketch() = ::point_i32(pusersystem->m_createstruct.x, pusersystem->m_createstruct.y);
      //puserinteraction->layout().sketch() = ::size_i32(pusersystem->m_createstruct.cx, pusersystem->m_createstruct.cy);

      //puserinteraction->layout().window() = ::point_i32(pusersystem->m_createstruct.x, pusersystem->m_createstruct.y);
      //puserinteraction->layout().window() = ::size_i32(pusersystem->m_createstruct.cx, pusersystem->m_createstruct.cy);

      bool bUnicode = ::IsWindowUnicode(hwnd) != false;

      if (bUnicode)
      {

         output_debug_string("window is unicode");

      }
      else
      {

         output_debug_string("window is ANSI");

      }

      //WNDCLASSEXW wndcls;

      //if (wstrClassName.get_length() > 0 && GetClassInfoExW(System.m_hinstance, wstrClassName, &wndcls) && wndcls.hIcon != nullptr)
      {

         //Application.set_icon(puserinteraction, __new(::draw2d::icon(get_context_application(), wndcls.hIcon)), false);

         //Application.set_icon(puserinteraction, __new(::draw2d::icon(get_context_application(), wndcls.hIcon)), true);

      }

      if (hwnd != get_hwnd())
      {

         set_hwnd(hwnd);
         //ASSERT(false); // should have been set in send msg hook

      }

      /// this Windows native window "holds" context_object to the
      /// wrapping object.
      puserinteraction->add_ref(OBJ_REF_DBG_THIS);

      puserinteraction->m_ewindowflag |= e_window_flag_window_created;

      return true;

   }




   bool window::has_capture() const
   {

      return ::GetCapture() == get_hwnd();

   }


   bool window::has_focus() const
   {

      return ::GetFocus() == get_hwnd();

   }


   void window::_001OnMessage(::message::message * pmessage)
   {

      __pointer(::message::base) pbase(pmessage);

      if (pbase != nullptr)
      {

         wparam wparam = pbase->m_wparam;

         lparam lparam = pbase->m_lparam;

         string strLparamString;

         if (pbase->m_id == WM_SETTINGCHANGE && wparam == 0)
         {

            strLparamString = (LPARAM(lparam));

         }

         if (pbase->m_id == WM_FONTCHANGE)
         {


            System.process_subject(id_os_font_change);

            //fork([this]()
              // {

                 // psession->call(e_routine_font_change);

               //});

         //}
         }
         else if (
            pbase->m_id == WM_SETTINGCHANGE &&
            strLparamString == "ImmersiveColorSet")
         {

            System.process_subject(id_os_dark_mode);

         }
         else if (pbase->m_id == e_message_display_change ||
            (pbase->m_id == WM_SETTINGCHANGE &&
               (pbase->m_wparam == SPI_SETWORKAREA)))
         {

            output_debug_string("test");

            //throw_todo();

            //System.enum_display_monitors();

            //__pointer(::user::interaction) puserinteraction;

            //while(System.get_frame(puserinteraction))
            //{

            //   try
            //   {

            //      puserinteraction->post_message(e_message_display_change);

            //   }
            //   catch(...)
            //   {
            //   }

            //}


         }

      }

   }



   //void window::set_wm_class(const char * psz)
   //{

   //   //      m_strWMClass = psz;
   //   //
   //   //      xdisplay d(display());
   //   //
   //   //      XClassHint classHint;
   //   //
   //   //      classHint.res_name = m_strWMClass;
   //   //
   //   //      classHint.res_class = m_strWMClass;
   //   //
   //   //      XSetClassHint(display(), window(), &classHint);

   //}


   //i32 window::map_window()
   //{

   //   //      int i = 0;
   //   //
   //   //      {
   //   //
   //   //         windowing_output_debug_string("\nwindow::map_window");
   //   //
   //   //         xdisplay d(display());
   //   //
   //   //         i = XMapWindow(display(), window());
   //   //
   //   //      }
   //   //
   //   //#ifndef RASPBIAN
   //   //
   //   //      if (g_psncontext != nullptr)
   //   //      {
   //   //
   //   //         sn_launchee_context_complete(g_psncontext);
   //   //
   //   //         g_psncontext = nullptr;
   //   //
   //   //      }
   //   //
   //   //#endif // RASPBIAN
   //   //
   //   //      windowing_output_debug_string("\nwindow::map_window END");

   //   return 0;

   //}


   //i32 window::unmap_window(bool bWithdraw)
   //{

   //   //windowing_output_debug_string("\nwindow::unmap_window");

   //   //xdisplay d(display());

   //   //int i;

   //   //if (bWithdraw)
   //   //{

   //   //   i = XWithdrawWindow(display(), window(), m_iScreen);

   //   //}
   //   //else
   //   //{

   //   //   i = XUnmapWindow(display(), window());

   //   //}

   //   //windowing_output_debug_string("\nwindow::unmap_window END");

   //   //return i;

   //}


   //oswindow_dataptra * window::s_pdataptra = nullptr;


   //::mutex * window::s_pmutex = nullptr;


   //i32 oswindow_find_message_only_window(::user::window * pimpl)
   //{

   //   if (pimpl == nullptr)
   //      return -1;

   //   single_lock slOsWindow(window::s_pmutex, true);

   //   for (i32 i = 0; i < ::window::s_pdataptra->get_count(); i++)
   //   {

   //      if (::window::s_pdataptra->element_at(i)->m_bMessageOnlyWindow
   //         && ::window::s_pdataptra->element_at(i)->m_pimpl == pimpl)
   //      {

   //         return i;

   //      }

   //   }

   //   return -1;

   //}

   //i32 oswindow_find(Display * pdisplay, Window window)
   //{

   //   single_lock slOsWindow(::window::s_pmutex, true);

   //   for (i32 i = 0; i < ::window::s_pdataptra->get_count(); i++)
   //   {
   //      if (!::window::s_pdataptra->element_at(i)->m_bMessageOnlyWindow
   //         && ::window::s_pdataptra->element_at(i)->m_osdisplay->display() == pdisplay
   //         && ::window::s_pdataptra->element_at(i)->m_window == window)
   //      {
   //         return i;
   //      }
   //   }

   //   return -1;

   //}

   //i32 oswindow_find(Window window)
   //{

   //   single_lock slOsWindow(::window::s_pmutex, true);

   //   for (i32 i = 0; i < ::window::s_pdataptra->get_count(); i++)
   //   {
   //      if (!::window::s_pdataptra->element_at(i)->m_bMessageOnlyWindow
   //         && ::window::s_pdataptra->element_at(i)->m_window == window)
   //      {
   //         return i;
   //      }
   //   }

   //   return -1;

   //}


   //window * oswindow_get_message_only_window(::user::window * puserinteraction)
   //{

   //   if (puserinteraction == nullptr)
   //   {

   //      return nullptr;

   //   }

   //   single_lock slOsWindow(::window::s_pmutex, true);

   //   iptr iFind = oswindow_find_message_only_window(puserinteraction);

   //   if (iFind >= 0)
   //   {

   //      return ::window::s_pdataptra->element_at(iFind);

   //   }

   //   ::window * pdata = new window;

   //   pdata->m_bMessageOnlyWindow = true;
   //   pdata->m_window = None;
   //   pdata->m_pimpl = puserinteraction;
   //   pdata->m_osdisplay = nullptr;
   //   pdata->m_parent = 0;
   //   pdata->m_pmq = puserinteraction->puserinteraction->m_pthreadUserInteraction->get_mq();

   //   ::window::s_pdataptra->add(pdata);

   //   return pdata;

   //}


   //window * oswindow_defer_get(Display * pdisplay, Window window)
   //{

   //   single_lock slOsWindow(::window::s_pmutex, true);

   //   iptr iFind = oswindow_find(pdisplay, window);

   //   if (iFind < 0)
   //   {

   //      return nullptr;

   //   }

   //   return ::window::s_pdataptra->element_at(iFind);

   //}


   //window *
   //   oswindow_get(Display * pdisplay, Window window, Visual * pvisual, int iDepth, int iScreen, Colormap colormap)
   //{

   //   single_lock slOsWindow(::window::s_pmutex, true);

   //   iptr iFind = oswindow_find(pdisplay, window);

   //   if (iFind >= 0)
   //   {

   //      return ::window::s_pdataptra->element_at(iFind);

   //   }

   //   ::window * pdata = new ::window;

   //   pdata->m_bMessageOnlyWindow = false;
   //   pdata->m_osdisplay = osdisplay_get(pdisplay);
   //   pdata->m_window = window;

   //   if (pvisual != nullptr)
   //   {

   //      pdata->m_visual = *pvisual;

   //   }

   //   pdata->m_iDepth = iDepth;
   //   pdata->m_iScreen = iScreen;
   //   pdata->m_colormap = colormap;
   //   pdata->m_parent = 0;

   //   ::window::s_pdataptra->add(pdata);

   //   return pdata;

   //}


   //void x11_on_start_session()
   //{

   //   Display * dpy = x11_get_display();
   //
   //   g_oswindowDesktop = oswindow_get(dpy, DefaultRootWindow(dpy));
   //
   //   g_oswindowDesktop->m_pimpl = nullptr;
   //
   //   XSelectInput(g_oswindowDesktop->display(), g_oswindowDesktop->window(), StructureNotifyMask);
   //
   //}


   /*window * oswindow_get(Window window)
   {

      single_lock slOsWindow(::window::s_pmutex, true);

      iptr iFind = oswindow_find(window);

      if (iFind < 0)
      {

         return nullptr;

      }

      return ::window::s_pdataptra->element_at(iFind);

   }*/


   //bool window::bamf_set_icon()
   //{

   //   ::linux::desktop_file file;

   //   file.bamf_set_icon(this);

   //   return true;

   //}


   //int window::x_change_property(Atom property, Atom type, int format, int mode, const unsigned char * data,
   //   int nelements)
   //{

   //   return XChangeProperty(display(), window(), property, type, format, mode, data, nelements);

   //}


   //Atom window::intern_atom(const char * pszAtomName, bool bCreate)
   //{

   //   return m_osdisplay->intern_atom(pszAtomName, bCreate);

   //}


   //Atom window::intern_atom(e_net_wm_state estate, bool bCreate)
   //{

   //   return m_osdisplay->intern_atom(estate, bCreate);

   //}

//
//   bool window::set_icon(::image * pimage)
//   {
//
//      // http://stackoverflow.com/questions/10699927/xlib-argb-window-icon
//      // http://stackoverflow.com/users/432509/ideasman42
//
//#if 0
//
//      unsigned int buffer[] =
//      {
//         16, 16,
//         4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 338034905, 3657433343, 0, 184483840, 234881279, 3053453567, 3221225727, 1879048447, 0, 0, 0, 0, 0, 0, 0, 1224737023, 3305111807, 3875537151,0, 0, 2063597823, 1291845887, 0, 67109119, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 50266112, 3422552319, 0, 0, 3070230783, 2063597823, 2986344703, 771752191, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3422552319, 0, 0, 3372220671, 1509949695, 704643327, 3355443455, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 3422552319, 0, 134152192, 3187671295, 251658495, 0, 3439329535, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3422552319, 0, 0, 2332033279, 1342177535, 167772415, 3338666239, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 3422552319, 0, 0, 436207871, 3322085628, 3456106751, 1375731967, 4278255360, 4026597120, 3758161664, 3489726208, 3204513536, 2952855296, 2684419840, 2399207168, 2130771712, 1845559040, 1593900800, 1308688128, 1040252672, 755040000, 486604544, 234946304, 4278255360, 4043374336, 3774938880, 3506503424, 3221290752, 2952855296, 2667642624, 2399207168, 2130771712, 1862336256, 1627453957, 1359017481, 1073805064, 788591627, 503379721, 218169088, 4278255360, 4043374336, 3758161664, 3506503424, 3221290752, 2952855296, 2684419840, 2415984384, 2130771712, 1862336256, 1577123584, 1308688128, 1040252672, 755040000, 486604544, 218169088, 4278190335, 4026532095, 3758096639, 3489661183, 3221225727, 2952790271, 2667577599, 2415919359, 2130706687, 1862271231, 1593835775, 1325400319, 1056964863, 771752191, 520093951, 234881279, 4278190335, 4026532095, 3758096639, 3489661183, 3221225727, 2952790271, 2667577599, 2415919359, 2130706687, 1862271231, 1593835775, 1325400319, 1056964863, 771752191, 503316735, 234881279, 4278190335, 4026532095, 3758096639, 3489661183, 3221225727, 2952790271, 2684354815, 2399142143, 2130706687, 1862271231, 1593835775, 1325400319, 1040187647, 771752191, 520093951, 234881279, 4294901760, 4043243520, 3774808064, 3506372608, 3221159936, 2952724480, 2684289024, 2399076352, 2147418112, 1862205440, 1593769984, 1308557312, 1040121856, 771686400, 503250944, 234815488, 4294901760, 4060020736, 3758030848, 3506372608, 3221159936, 2952724480, 2684289024, 2415853568, 2130640896, 1862205440, 1593769984, 1308557312, 1040121856, 771686400, 503250944, 234815488, 4294901760, 4043243520, 3774808064, 3489595392, 3237937152, 2952724480, 2684289024, 2415853568, 2147418112, 1862205440, 1593769984, 1325334528, 1056899072, 788463616, 503250944, 234815488,
//         32, 32,
//         4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 0, 0, 0, 0, 0, 0, 0, 268369920, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 1509949695, 3120562431, 4009754879, 4194304255, 3690987775, 2130706687, 83886335, 0, 50331903, 1694499071, 3170894079, 3992977663, 4211081471, 3657433343, 1879048447, 16777471, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3087007999, 2281701631, 1191182591, 1040187647, 2030043391, 4127195391, 2566914303, 0, 16777471, 3254780159, 2181038335, 1191182591, 973078783, 2030043391,4177527039, 2130706687, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 0, 0, 0, 2214592767, 4093640959, 0, 0, 0, 0, 0, 0, 0, 2298478847, 3909091583, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2214592767, 3607101695, 0, 0, 0, 0, 0, 0, 0, 1946157311, 4093640959, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 536871167, 1191182591, 2281701631,3019899135, 637534463, 0, 0, 0, 100597760, 251592704, 33488896, 0, 3321889023, 2919235839, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2550137087, 4278190335, 4278190335, 3405775103, 570425599, 0, 0, 0, 0, 0, 0, 2046820607, 4043309311, 620757247, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 33488896, 0, 0, 218104063, 1291845887, 3841982719, 3388997887, 0, 0, 0, 0, 0, 1996488959, 4093640959, 1073742079, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1761607935, 4278190335, 150995199, 0, 0, 67109119, 2550137087, 3909091583, 889192703, 0, 0, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 0, 0, 0, 2181038335, 3925868799, 0, 0, 218104063, 3070230783, 3623878911, 570425599, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 805306623, 3288334591, 1795162367, 1040187647, 1023410431, 2231369983, 4211081471, 1694499071, 0, 369099007, 3456106751, 3825205503, 1174405375, 872415487, 872415487, 872415487, 872415487, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4293984270, 2046951677, 3422552319, 4110418175, 4177527039, 3405775103, 1409286399, 0, 0, 1409286399, 4278190335, 4278190335, 4278190335, 4278190335, 4278190335, 4278190335, 4278190335, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760,4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 4294901760, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4278255360, 4144037632, 4009819904, 3875602176, 3741384448, 3607166720, 3472948992, 3338731264, 3204513536, 3053518592, 2936078080, 2801860352, 2650865408, 2516647680, 2382429952, 2264989440, 2113994496, 1996553984, 1862336256, 1728118528, 1577123584, 1459683072, 1325465344, 1191247616, 1040252672, 922812160, 771817216, 637599488, 503381760, 385941248, 234946304, 100728576, 4278255360, 4144037632, 4009819904, 3875602176, 3724607232, 3607166720, 3472948992, 3338731264, 3204513536, 3070295808, 2936078080, 2801860352, 2667642624, 2516647680, 2399207168, 2264989440, 2130771712, 1996553984, 1845559040, 1728118528, 1593900800, 1459683072, 1308688128, 1191247616, 1057029888, 922812160, 788594432, 637599488, 503381760, 369164032, 234946304, 117505792, 4278255360, 4144037632, 4009819904, 3875602176, 3741384448, 3607166720, 3472948992, 3338731264, 3204513536, 3053518592, 2919300864, 2801860352, 2650865408, 2533424896, 2399207168, 2264989440, 2113994496, 1996553984, 1862336256, 1728118528,1593900800, 1459683072, 1325465344, 1191247616, 1040252672, 906034944, 771817216, 654376704, 503381760, 369164032, 234946304, 117505792, 4278255360, 4144037632, 4009819904, 3858824960, 3741384448, 3607166720, 3472948992, 3338731264, 3204513536, 3070295808, 2936078080, 2801860352, 2667642624, 2533424896, 2382429952, 2264989440, 2130771712, 1979776768, 1862336256, 1728118528, 1577123584, 1442905856, 1325465344, 1191247616, 1040252672, 922812160, 771817216, 637599488, 503381760, 369164032, 234946304, 100728576, 4278255360, 4144037632, 4009819904, 3875602176, 3741384448, 3607166720, 3472948992, 3338731264, 3204513536, 3070295808, 2919300864, 2801860352, 2667642624, 2533424896, 2399207168, 2264989440, 2113994496, 1996553984, 1862336256, 1728118528, 1593900800, 1442905856, 1342241795, 1174470400, 1057029888, 906034944, 788594432, 654376704, 503381760, 385941248, 251723520, 100728576, 4278190335, 4160749823, 4026532095, 3892314367, 3741319423, 3623878911, 3472883967, 3338666239, 3221225727, 3070230783, 2952790271, 2818572543, 2667577599, 2533359871, 2399142143, 2264924415, 2147483903, 1996488959, 1862271231, 1728053503, 1593835775, 1459618047, 1325400319, 1191182591, 1056964863, 922747135, 788529407, 654311679, 520093951,385876223, 251658495, 117440767, 4278190335, 4160749823, 4026532095, 3892314367, 3741319423, 3623878911, 3489661183, 3355443455, 3221225727, 3087007999, 2936013055, 2801795327, 2667577599, 2533359871, 2399142143, 2281701631, 2130706687, 1996488959, 1862271231, 1728053503, 1593835775,1459618047, 1325400319, 1191182591, 1056964863, 922747135, 788529407, 654311679, 520093951, 385876223, 234881279, 100663551, 4278190335, 4160749823, 4026532095, 3892314367, 3758096639, 3623878911, 3489661183, 3355443455, 3221225727, 3087007999, 2936013055, 2801795327, 2667577599, 2550137087, 2415919359, 2264924415, 2130706687, 1996488959, 1862271231, 1728053503, 1593835775, 1459618047, 1325400319, 1191182591, 1056964863, 922747135, 788529407, 654311679, 503316735, 369099007, 251658495, 100663551, 4278190335, 4160749823, 4026532095, 3892314367, 3758096639, 3623878911, 3489661183, 3355443455, 3204448511, 3087007999, 2936013055, 2818572543, 2667577599, 2533359871, 2399142143, 2264924415, 2130706687, 1996488959, 1879048447, 1728053503, 1593835775, 1459618047, 1325400319, 1191182591, 1056964863, 922747135, 788529407, 654311679, 520093951, 385876223, 251658495, 117440767, 4278190335, 4160749823, 4026532095, 3892314367, 3758096639, 3623878911, 3489661183, 3355443455, 3221225727, 3087007999, 2952790271, 2818572543, 2667577599, 2533359871, 2399142143, 2264924415, 2147483903, 2013266175, 1862271231, 1744830719, 1610612991, 1476395263, 1342177535, 1191182591, 1056964863, 922747135, 788529407, 654311679, 520093951, 385876223, 251658495, 100663551, 4294901760, 4160684032, 4026466304, 3909025792, 3774808064, 3623813120, 3489595392, 3355377664, 3237937152, 3103719424, 2952724480, 2818506752, 2684289024, 2550071296, 2415853568, 2281635840, 2147418112, 2013200384, 1878982656, 1744764928, 1593769984, 1476329472,1325334528, 1207894016, 1056899072, 939458560, 788463616, 654245888, 520028160, 385810432, 251592704, 117374976, 4294901760, 4177461248, 4043243520, 3909025792, 3774808064, 3640590336, 3506372608, 3355377664, 3221159936, 3086942208, 2952724480, 2818506752, 2701066240, 2550071296, 2415853568, 2281635840, 2147418112, 2013200384, 1878982656, 1727987712, 1610547200, 1476329472, 1325334528, 1191116800, 1073676288, 922681344, 788463616, 654245888, 520028160, 385810432, 251592704, 100597760, 4294901760, 4177461248, 4043243520, 3909025792, 3774808064, 3640590336, 3489595392, 3372154880, 3237937152, 3103719424, 2952724480, 2818506752, 2700935170, 2550071296, 2415853568, 2281635840, 2147418112, 2013200384, 1878982656, 1744764928, 1610547200, 1459552256, 1342111744, 1191116800, 1056899072, 922681344, 788463616, 671023104, 520028160, 385810432, 251592704, 100597760, 4294901760, 4177461248, 4043243520, 3909025792, 3774808064, 3640590336, 3489595392, 3372154880, 3237937152, 3086942208, 2969501696, 2818506752, 2684289024, 2550071296, 2432630784, 2281635840, 2147418112, 2013200384, 1862205440, 1744764928, 1610547200, 1476329472, 1342111744, 1191116800, 1056899072, 922681344, 788463616, 654245888, 520028160, 385810432, 251592704, 117374976, 4294901760, 4177461248, 4043243520, 3909025792, 3774808064, 3623813120, 3506372608, 3372154880, 3237937152, 3103719424, 2952724480, 2835283968, 2684289024, 2550071296, 2432630784, 2281635840, 2147418112, 2046492676, 1862205440, 1744764928, 1610547200, 1476329472, 1342111744,1207894016, 1056899072, 939458560, 788463616, 654245888, 536281096, 385810432, 251592704, 134152192,
//      };
//
//      xdisplay d(display());
//
//      int iScreen = DefaultScreen(d);
//
//      Atom net_wm_icon = d.intern_atom("_NET_WM_ICON", False);
//
//      Atom cardinal = d.intern_atom("CARDINAL", False);
//
//      int length = 2 + 16 * 16 + 2 + 32 * 32;
//
//      int status = XChangeProperty(d, w->window(), net_wm_icon, cardinal, 32, PropModeReplace, (const unsigned char *)buffer, length);
//
//      if (status != 0)
//      {
//
//         return false;
//
//      }
//
//#elif 1
//
//      auto d1 = create_image({ 32, 32 });
//
//      if (!::is_ok(d1))
//      {
//
//         return false;
//
//      }
//
//      d1->g()->set_interpolation_mode(::draw2d::e_interpolation_mode_high_quality_bicubic);
//
//      d1->g()->stretch(d1->rectangle(), pimage->g(), pimage->rectangle());
//
//      memory m(puserinteraction->get_context_application());
//
//      int length = 2 + d1->area();
//
//      m.set_size(length * 4);
//
//      unsigned int * pcr = (unsigned int *)m.get_data();
//
//      pcr[0] = d1->width();
//
//      pcr[1] = d1->height();
//
//      int c = d1->area();
//
//      for (int i = 0; i < c; i++)
//      {
//
//         pcr[i + 2] = d1->colorref()[i];
//
//      }
//
//      windowing_output_debug_string("\nwindow::set_icon");
//
//      xdisplay d(display());
//
//      Atom net_wm_icon = intern_atom("_NET_WM_ICON", False);
//
//      Atom cardinal = intern_atom("CARDINAL", False);
//
//      int status = x_change_property(net_wm_icon, cardinal, 32, PropModeReplace, (const unsigned char *)pcr, length);
//
//      if (status == BadAlloc)
//      {
//
//
//      }
//      else if (status != 0)
//      {
//
//         return false;
//
//      }
//
//      windowing_output_debug_string("\nwindow::set_icon END");
//
//      fflush(stdout);
//
//#else
//
//      image d1(w->puserinteraction->create_new, this);
//
//      if (!d1->create(24, 24))
//      {
//
//         return false;
//
//      }
//
//      d1->get_graphics()->set_interpolation_mode(e_interpolation_mode_high_quality_bicubic);
//
//      d1->get_graphics()->StretchBlt(0, 0, d1.width(), d1.height(), point_i32->get_graphics(), 0, 0, point.width(), point.height());
//
//      image d2(w->puserinteraction->create_new, this);
//
//      if (!d2->create(54, 54))
//      {
//
//         return false;
//
//      }
//
//      d2->get_graphics()->set_interpolation_mode(e_interpolation_mode_high_quality_bicubic);
//
//      d2->get_graphics()->StretchBlt(0, 0, d2.width(), d2.height(), point_i32->get_graphics(), 0, 0, point.width(), point.height());
//
//      memory m(w->puserinteraction->get_context_application());
//
//      int length = 2 + d1->area() + 2 + d2->area();
//
//      m.set_size(length * 4);
//
//      unsigned int * pcr = (unsigned int *)m.get_data();
//
//      pcr[0] = d1.width();
//
//      pcr[1] = d1.height();
//
//      int c = d1->area();
//
//      for (int i = 0; i < c; i++)
//      {
//
//         pcr[i + 2] = d1->m_pcolorref[i];
//
//      }
//
//      memsize o;
//
//      o = 2 + d1->area();
//
//      pcr[o] = d2.width();
//
//      pcr[o + 1] = d2.height();
//
//      c = d2->area();
//
//      for (int i = 0; i < c; i++)
//      {
//
//         pcr[i + o + 2] = d2->m_pcolorref[i];
//
//      }
//
//      Display * display = w->display();
//
//      Atom net_wm_icon = XInternAtom(display, "_NET_WM_ICON", False);
//
//      Atom cardinal = XInternAtom(display, "CARDINAL", False);
//
//      int status = XChangeProperty(display, w->window(), net_wm_icon, cardinal, 32, PropModeReplace, (const unsigned char *)pcr, length);
//
//      if (status != 0)
//      {
//
//         //file_put_contents("/home/camilo/window.txt", __str((int)w->window()));
//         return false;
//
//      }
//#endif
//      return true;
//
//   }
//

   //i32 window::store_name(const char * psz)
   //{

   //   windowing_output_debug_string("\nwindow::store_name");

   //   xdisplay d(display());

   //   int i = XStoreName(display(), window(), psz);

   //   windowing_output_debug_string("\nwindow::store_name END");

   //   return i;

   //}


   //i32 window::select_input(i32 iInput)
   //{

   //   windowing_output_debug_string("\nwindow::select_input");

   //   xdisplay d(display());

   //   int i = XSelectInput(display(), window(), iInput);

   //   windowing_output_debug_string("\nwindow::select_input END");

   //   return i;

   //}


   //i32 window::select_all_input()
   //{

   //   windowing_output_debug_string("\nwindow::select_all_input");

   //   xdisplay d(display());

   //   int i = select_input(ExposureMask | ButtonPressMask);

   //   windowing_output_debug_string("\nwindow::select_all_input");

   //   return i;

   //}


   void window::post_nc_destroy()
   {


      WNDPROC pfnWndProc = WNDPROC(::GetWindowLongPtr(get_hwnd(), GWLP_WNDPROC));

      if (WNDPROC(::GetWindowLongPtr(get_hwnd(), GWLP_WNDPROC)) == pfnWndProc)
      {

         WNDPROC pfnSuper = *GetSuperWndProcAddr();

         if (pfnSuper != nullptr)
         {

            ::SetWindowLongPtr(get_hwnd(), GWLP_WNDPROC, reinterpret_cast<iptr>(pfnSuper));

         }

      }

      m_pwindowing->remove_window(this);

      set_hwnd(nullptr);

      //if (!::is_null(this))
      //{

      //   oswindow_remove(display(), window());

      //}

   }


   ::windowing::window * window::get_parent() const
   {

      auto oswindow = get_parent_oswindow();

      auto pwindow = m_pwindowing->window(oswindow);

      return pwindow;

   }


   oswindow window::get_parent_oswindow() const
   {

      auto hwnd = get_hwnd();

      auto hwndParent = ::GetParent(hwnd);

      auto oswindowParent = __oswindow(hwndParent);

      return oswindowParent;

   }


   ::e_status window::set_parent(::windowing::window * pwindowParent)
   {

      auto hwnd = get_hwnd();

      HWND hwndParent = nullptr;

      if (::is_set(pwindowParent))
      {

         hwndParent = __hwnd(pwindowParent->get_oswindow());

      }

      if (!::SetParent(hwnd, hwndParent))
      {

         return ::error_failed;

      }

      return ::success;

   }


   ::e_status window::show_window(const ::e_display & edisplay, const ::e_activation & eactivation)
   {

      auto iShowWindow = windows_show_window(edisplay, eactivation);

      HWND hwnd = get_hwnd();

      if (!::ShowWindow(hwnd, iShowWindow))
      {

         return ::error_failed;

      }

      return ::success;

   }


   void window::full_screen(const ::rectangle_i32 & rectangle)
   {

      //::rectangle_i32 rBest;

      //int iMonitor = best_xinerama_monitor(puserinteraction, rectangle, rBest);

      //windowing_output_debug_string("\n::window::full_screen 1");

      //xdisplay d(display());

      //if (d.is_null())
      //{

      //   windowing_output_debug_string("\n::window::full_screen 1.1");

      //   return;

      //}

      //XWindowAttributes attr;

      //if (!XGetWindowAttributes(display(), window(), &attr))
      //{

      //   windowing_output_debug_string("\n::window::full_screen 1.2");

      //   fflush(stdout);

      //   return;

      //}

      //::rectangle_i32 rWindow;

      //rWindow.left = attr.x;
      //rWindow.top = attr.y;
      //rWindow.right = attr.x + attr.width;
      //rWindow.bottom = attr.y + attr.height;

      //if (rBest != rWindow)
      //{

      //   puserinteraction->place(rBest);

      //   XMoveResizeWindow(d, m_window, rBest.left, rBest.top, rBest.width(), rBest.height());

      //}

      //if (attr.map_state == IsViewable)
      //{

      //   mapped_net_state_raw(true, d, window(), m_iScreen, intern_atom(net_wm_state_fullscreen, false), 0);

      //}
      //else
      //{

      //   unmapped_net_state_raw(d, window(), intern_atom(net_wm_state_fullscreen, false), 0);

      //   XMapWindow(display(), window());

      //}

      //windowing_output_debug_string("\n::window::full_screen 2");

      //::fflush(stdout);

   }


   void window::exit_iconify()
   {

      //xdisplay d(display());

      //if (d.is_null())
      //{

      //   windowing_output_debug_string("\n::window::exit_iconify 1.1");

      //   return;

      //}

      //XWindowAttributes attr;

      //if (!XGetWindowAttributes(display(), window(), &attr))
      //{

      //   windowing_output_debug_string("\n::window::exit_full_screen 1.2");

      //   fflush(stdout);

      //   return;

      //}

      //if (attr.map_state == IsViewable)
      //{

      //   mapped_net_state_raw(false, d, window(), m_iScreen, intern_atom(net_wm_state_hidden, false), 0);

      //}

   }

   
   void window::exit_full_screen()
   {

      //xdisplay d(display());

      //if (d.is_null())
      //{

      //   windowing_output_debug_string("\n::window::exit_full_screen 1.1");

      //   return;

      //}

      //XWindowAttributes attr;

      //if (!XGetWindowAttributes(display(), window(), &attr))
      //{

      //   windowing_output_debug_string("\n::window::exit_full_screen 1.2");

      //   fflush(stdout);

      //   return;

      //}

      //if (attr.map_state == IsViewable)
      //{

      //   mapped_net_state_raw(false, d, window(), m_iScreen, intern_atom("_NET_WM_STATE_FULLSCREEN", false), 0);

      //}

   }


   void window::exit_zoomed()
   {

      //sync_lock sl(x11_mutex());

      //xdisplay d(display());

      //if (d.is_null())
      //{

      //   windowing_output_debug_string("\n::window::exit_zoomed 1.1");

      //   return;

      //}

      //XWindowAttributes attr;

      //if (!XGetWindowAttributes(display(), window(), &attr))
      //{

      //   windowing_output_debug_string("\n::window::exit_zoomed 1.2");

      //   fflush(stdout);

      //   return;

      //}

      //if (attr.map_state == IsViewable)
      //{

      //   mapped_net_state_raw(false, d, window(), m_iScreen,
      //      intern_atom("_NET_WM_STATE_MAXIMIZED_HORZ", false),
      //      intern_atom("_NET_WM_STATE_MAXIMIZED_VERT", false));

      //}

   }


   ::e_status window::set_focus()
   {

      HWND hwnd = get_hwnd();

      if (!::SetFocus(hwnd))
      {

         return ::error_failed;

      }

      return ::success;

   }


   //__pointer(window) window::get_active_window()
   //{

   //   __throw(interface_only_exception());

   //   return nullptr;

   //}


   ::e_status window::set_active_window()
   {

      HWND hwnd = get_hwnd();

      if (!::SetActiveWindow(hwnd))
      {

         return ::error_failed;

      }

      return ::success;

   }


   ::e_status window::set_capture()
   {

      HWND hwnd = get_hwnd();

      if (!::SetCapture(hwnd))
      {

         return ::error_failed;

      }

      return ::success;

   }


   ::e_status window::destroy_window()
   {

      HWND hwnd = get_hwnd();

      if (!::DestroyWindow(hwnd))
      {

         return ::error_failed;

      }

      return ::success;

   }


   //::e_status window::show_window(const ::e_display & edisplay, const ::e_activation & eactivation)
   //{

   //   int nCmdShow = windows_show_window(edisplay, eactivation);

   //   if(!::ShowWindow(get_hwnd(), nCmdShow))
   //   {

   //      return ::error_failed;

   //   }

   //   return ::success;

   //}


   iptr window::get_window_long_ptr(i32 nIndex) const
   {

      HWND hwnd = get_hwnd();

      auto iptr = GetWindowLongPtr(hwnd, nIndex);

      return iptr;

   }


   bool window::set_window_long_ptr(i32 nIndex, iptr i)
   {

      HWND hwnd = get_hwnd();

      SetWindowLongPtr(hwnd, nIndex, i);

      return true;

   }


   bool window::_001ClientToScreen(POINT_I32 * ppoint)
   {

      HWND hwnd = get_hwnd();

      ClientToScreen(hwnd, (POINT *) ppoint);

      return true;

   }


   bool window::_001ScreenToClient(POINT_I32 * ppoint)
   {

      HWND hwnd = get_hwnd();

      ScreenToClient(hwnd, (POINT *)ppoint);

      return true;

   }


   //long window::get_state()
   //{

   //   windowing_output_debug_string("\n::window::get_state 1");

   //   xdisplay d(display());

   //   static const long WM_STATE_ELEMENTS = 2L;

   //   unsigned long nitems = 0;

   //   unsigned long leftover = 0;

   //   Atom atomWmState = 0;

   //   Atom actual_type = 0;

   //   i32 actual_format = 0;

   //   i32 status = 0;

   //   unsigned char * point = nullptr;

   //   if (d.m_pdata->m_atomWmState == None)
   //   {

   //      d.m_pdata->m_atomWmState = d.intern_atom("WM_STATE", false);

   //   }

   //   atomWmState = d.m_pdata->m_atomWmState;

   //   status = XGetWindowProperty(d, m_window, atomWmState, 0L, WM_STATE_ELEMENTS, False, AnyPropertyType, &actual_type,
   //      &actual_format, &nitems, &leftover, &point);

   //   if (status == 0)
   //   {

   //      long lStatus = -1;

   //      if (point != nullptr)
   //      {

   //         lStatus = (long)*point_i32;

   //      }

   //      XFree(point);

   //      windowing_output_debug_string("\n::window::get_state 1.1");

   //      return lStatus;

   //   }

   //   windowing_output_debug_string("\n::window::get_state 2");

   //   return -1;

   //}


   //bool window::is_iconic()
   //{

   //   HWND hwnd = get_hwnd();

   //   auto bIconic = ::IsIconic(hwnd);

   //   return bIconic != FALSE;

   //}


   bool window::is_window()
   {

      HWND hwnd = get_hwnd();

      auto bWindow = ::IsWindow(hwnd);

      return bWindow != FALSE;

   }


   bool window::is_window_visible()
   {

      HWND hwnd = get_hwnd();

      auto bWindowVisible = ::IsWindowVisible(hwnd);

      return bWindowVisible != FALSE;

   }


   bool window::is_destroying()
   {

      if (::is_null(this))
      {

         return true;

      }

      if (m_pimpl == nullptr)
      {

         return true;

      }

      auto puserinteraction = m_pimpl->m_puserinteraction;

      if (!puserinteraction->m_bUserPrimitiveOk)
      {

         return true;

      }

      return false;

   }

#undef SET_WINDOW_POS_LOG

   bool window::set_window_pos(class ::zorder zorder, i32 x, i32 y, i32 cx, i32 cy, ::u32 nFlags)
   {

      HWND hwnd = get_hwnd();

      auto pwindowing = windowing();

      HWND hwndInsertAfter = pwindowing->zorder_to_hwnd(zorder);

      auto bSetWindowPos = ::SetWindowPos(hwnd, hwndInsertAfter, x, y, cx, cy, nFlags);

      return bSetWindowPos != FALSE;

   }


//   bool window::_set_window_pos(class ::zorder zorder, i32 x, i32 y, i32 cx, i32 cy, ::u32 nFlags)
//   {
//
//      sync_lock sl(x11_mutex());
//
//      windowing_output_debug_string("\n::window::set_window_pos 1");
//
//      auto pdisplay = display();
//
//      xdisplay d(pdisplay);
//
//      XWindowAttributes attrs = {};
//
//      Window w = window();
//
//      if (!XGetWindowAttributes(pdisplay, w, &attrs))
//      {
//
//         windowing_output_debug_string("\n::window::set_window_pos 1.1 xgetwindowattr failed");
//
//         return false;
//
//      }
//
//      if (nFlags & SWP_SHOWWINDOW)
//      {
//
//         if (attrs.map_state == IsUnmapped)
//         {
//
//            windowing_output_debug_string("\n::window::set_window_pos Mapping Window 1.2");
//
//            XMapWindow(display(), window());
//
//         }
//
//         if (!XGetWindowAttributes(display(), window(), &attrs))
//         {
//
//            windowing_output_debug_string("\n::window::set_window_pos 1.3 xgetwindowattr failed");
//
//            return false;
//
//         }
//
//      }
//
//      bool bMove = !(nFlags & SWP_NOMOVE);
//
//      bool bSize = !(nFlags & SWP_NOSIZE);
//
//      if (bMove)
//      {
//
//         if (bSize)
//         {
//
//            windowing_output_debug_string("\n::window::set_window_pos Move Resize Window 1.4");
//
//#ifdef SET_WINDOW_POS_LOG
//
//            INFO("XMoveResizeWindow (%d, %d) - (%d, %d)", x, y, cx, cy);
//
//#endif
//
//            if (cx <= 0 || cy <= 0)
//            {
//
//               cx = 1;
//
//               cy = 1;
//
//#ifdef SET_WINDOW_POS_LOG
//
//               INFO("Changing parameters... (%d, %d) - (%d, %d)", x, y, cx, cy);
//
//#endif
//
//            }
//
//            XMoveResizeWindow(display(), window(), x, y, cx, cy);
//
//         }
//         else
//         {
//
//            windowing_output_debug_string("\n::window::set_window_pos Move Window 1.4.1");
//
//            XMoveWindow(display(), window(), x, y);
//
//         }
//
//      }
//      else if (bSize)
//      {
//
//         windowing_output_debug_string("\n::window::set_window_pos Resize Window 1.4.2");
//
//         XResizeWindow(display(), window(), cx, cy);
//
//      }
//
//      //   if(bMove || bSize)
//      //   {
//      //
//      //      if(attrs.override_redirect)
//      //      {
//      //
//      //         if(!(puserinteraction->m_ewindowflag & e_window_flag_arbitrary_positioning))
//      //         {
//      //
//      //            XSetWindowAttributes set;
//      //
//      //            set.override_redirect = False;
//      //
//      //            if(!XChangeWindowAttributes(display(), window(), CWOverrideRedirect, &set))
//      //            {
//      //
//      //               INFO("linux::window::_native_create_window_ex failed to clear override_redirect");
//      //
//      //            }
//      //
//      //         }
//      //
//      //      }
//      //
//      //   }
//
//
//      if (nFlags & SWP_HIDEWINDOW)
//      {
//
//         if (attrs.map_state == IsViewable)
//         {
//
//            windowing_output_debug_string("\n::window::set_window_pos Withdraw Window 1.4.3");
//
//            XWithdrawWindow(display(), window(), m_iScreen);
//
//         }
//
//      }
//
//      if (!XGetWindowAttributes(display(), window(), &attrs))
//      {
//
//         windowing_output_debug_string("\n::window::set_window_pos xgetwndattr 1.4.4");
//
//         return false;
//
//      }
//
//      if (attrs.map_state == IsViewable || (nFlags & SWP_SHOWWINDOW))
//      {
//
//         if (!(nFlags & SWP_NOZORDER))
//         {
//
//            if (zorder.m_ezorder == e_zorder_top_most)
//            {
//
//               if (m_iaNetWmState[net_wm_state_above] != 1)
//               {
//
//                  wm_state_above_raw(this, true);
//
//               }
//
//               XRaiseWindow(display(), window());
//
//            }
//            else if (zorder.m_ezorder == e_zorder_top)
//            {
//
//               if (m_iaNetWmState[net_wm_state_above] != 0
//                  || m_iaNetWmState[net_wm_state_below] != 0
//                  || m_iaNetWmState[net_wm_state_hidden] != 0
//                  || m_iaNetWmState[net_wm_state_maximized_horz] != 0
//                  || m_iaNetWmState[net_wm_state_maximized_vert] != 0
//                  || m_iaNetWmState[net_wm_state_fullscreen] != 0)
//               {
//
//                  wm_state_clear_raw(this, false);
//
//               }
//
//               XRaiseWindow(display(), window());
//
//            }
//            else if (zorder.m_ezorder == e_zorder_bottom)
//            {
//
//               if (m_iaNetWmState[net_wm_state_below] != 1)
//               {
//
//                  wm_state_below_raw(this, true);
//
//               }
//
//               XLowerWindow(display(), window());
//
//            }
//
//         }
//
//         puserinteraction->ModifyStyle(0, WS_VISIBLE, 0);
//
//      }
//      else
//      {
//
//         puserinteraction->ModifyStyle(WS_VISIBLE, 0, 0);
//
//      }
//
//      //m_pimpl->on_change_visibility();
//
//      windowing_output_debug_string("\n::window::set_window_pos 2");
//
//      return 1;
//
//   }


   ::e_status window::set_cursor(::windowing::cursor * pcursor)
   {

      HCURSOR hcursor = (HCURSOR) pcursor->get_os_data();

      if (hcursor == nullptr)
      {

         return ::error_failed;

      }

      if (!::SetCursor(hcursor))
      {

         return ::error_failed;

      }

      return ::success;

   }


   //bool window::reset(::windowing::cursor * pcursor, ::aura::session * psession)
   //{

   //   sync_lock sl(psession->mutex());

   //   if (::is_null(puserinteraction))
   //   {

   //      return false;

   //   }

   //   App(puserinteraction).window_set_mouse_cursor(puserinteraction->get_hwnd(), 0);

   //   return true;

   //}


   void window::message_handler(::message::base * pbase)
   {
      
      if (pbase->m_id == (enum_message)WM_SYSCOMMAND)
      {

         if (pbase->m_wparam == SC_SCREENSAVE)
         {

            auto puserinteraction = m_pimpl->m_puserinteraction;

            if (puserinteraction && !puserinteraction->_001CanEnterScreenSaver())
            {

               pbase->m_bRet = true;

               pbase->m_lresult = 0;

            }

            return;

         }
         else if (pbase->m_wparam == SC_MAXIMIZE)
         {

            auto puserinteraction = m_pimpl->m_puserinteraction;

            if (puserinteraction && !puserinteraction->_001Maximize())
            {

               pbase->m_bRet = true;

               pbase->m_lresult = 0;

            }

            return;
         }
         else if (pbase->m_wparam == SC_RESTORE)
         {

            auto puserinteraction = m_pimpl->m_puserinteraction;

            if (puserinteraction && !puserinteraction->_001Restore())
            {

               pbase->m_bRet = true;

               pbase->m_lresult = 0;

            }

            return;
         }

      }

      auto puserinteraction = m_pimpl->m_puserinteraction;

      if (puserinteraction)
      {

         puserinteraction->message_handler(pbase);

      }

   }


//   int_bool window::set_mouse_cursor(hwnd window, hcursor hcursor)
//   {
//
//#ifdef WINDOWS_DESKTOP
//
//      UNREFERENCED_PARAMETER(window);
//
//      if (!::SetCursor(hcursor))
//      {
//
//         return false;
//
//      }
//
//#endif
//
//      return true;
//
//   }


   bool window::get_window_placement(WINDOWPLACEMENT * pwndpl)
   {

      pwndpl->length = sizeof(WINDOWPLACEMENT);

      return ::GetWindowPlacement(get_hwnd(), pwndpl) != false;

   }


   bool window::set_window_placement(const WINDOWPLACEMENT * pwndpl)
   {

      auto puserinteraction = m_pimpl->m_puserinteraction;

      sync_lock sl(puserinteraction->mutex());

      ASSERT(::IsWindow(get_hwnd()));

      ((WINDOWPLACEMENT *)pwndpl)->length = sizeof(WINDOWPLACEMENT);

      return ::SetWindowPlacement(get_hwnd(), pwndpl) != false;

   }


   ::e_status window::set_foreground_window()
   {

      HWND hwnd = get_hwnd();

      if (!::SetForegroundWindow(hwnd))
      {

         return ::error_failed;

      }

      return ::success;

      //// special activate logic for floating toolbars and palettes
      //auto pActiveWnd = GetForegroundWindow();

      //if (!pActiveWnd || !((pActiveWnd)->get_hwnd() == get_hwnd() || ::IsChild((pActiveWnd)->get_hwnd(), get_hwnd())))
      //{
      //   // clicking on floating frame when it does not have
      //   // focus itself -- activate the toplevel frame instead.

      //   auto puserinteractionTopLevel = get_top_level();

      //   if (puserinteractionTopLevel)
      //   {

      //      puserinteractionTopLevel->SetForegroundWindow();

      //   }

   }


   void window::_001OnPaint(::message::message * pmessage)
   {

      //HideCaret();

      //PAINTSTRUCT ps;
      //HDC hdc1 = BeginPaint(m_oswindow, &ps);
      //// TODO: Add any drawing code that uses hdc here...
      //EndPaint(m_oswindow, &ps);

      //return
      //   ;
      //if (GetExStyle() & WS_EX_LAYERED)
      //{


      //   return;

      //}

      __pointer(::message::base) pbase(pmessage);

      ::rectangle_i32 rectWindow;

      auto puserinteraction = m_pimpl->m_puserinteraction;

      puserinteraction->layout().design().screen_rect(rectWindow);

      PAINTSTRUCT paint;

      __memset(&paint, 0, sizeof(paint));

      HDC hdc = ::BeginPaint(get_hwnd(), &paint);

      ::SelectClipRgn(hdc, nullptr);

      ::rectangle_i32 rectPaint;

      ::rectangle_i32 rectUpdate;

      rectPaint = paint.rcPaint;


      __throw(todo());

      //if (rectPaint.is_null() || (GetExStyle() & WS_EX_LAYERED))
      //{

      //   rectUpdate = rectWindow;

      //   rectPaint = rectWindow;

      //   puserinteraction->_001ScreenToClient(rectPaint, ::user::e_layout_design);

      //}
      //else
      //{

      //   rectUpdate = rectPaint;

      //}

      //if (m_pgraphics.is_null())
      //{

      //   update_graphics_resources();

      //}

      //__pointer(::windows::buffer) pbuffer = m_pgraphics;

      //ASSERT(pbuffer.is_set());

      //single_lock sl(pbuffer->mutex());

      //auto & buffer = pbuffer->m_osbuffera[!pbuffer->m_iCurrentBuffer];

      //::BitBlt(hdc, rectUpdate.left, rectUpdate.top, rectUpdate.width(), rectUpdate.height(), buffer.m_hdc, 0, 0, SRCCOPY);








      //if (m_spgraphics.is_set())
      //{

      //   ::draw2d::graphics_pointer & pgraphics = m_spgraphics->on_begin_draw();

      //   if (pgraphics != nullptr)
      //   {

      //      try
      //      {

      //         _001Print(pgraphics);

      //      }
      //      catch (...)
      //      {


      //      }

      //      ::draw2d::graphics_pointer g(e_create);

      //      try
      //      {

      //         if (pgraphics != nullptr && g->attach_hdc(hdc))
      //         {

      //            pgraphics->SetViewportOrg(0, 0);

      //            g->BitBlt(rectPaint.left, rectPaint.top, rectPaint.width(), rectPaint.height(), pgraphics, rectUpdate.left, rectUpdate.top);

      //         }

      //      }
      //      catch (...)
      //      {

      //      }

      //   }

      //}

      ::EndPaint(get_hwnd(), &paint);

      pmessage->m_bRet = true;

      pbase->m_lresult = 0;

      //ShowCaret();

   }


   //::user::interaction * window::set_parent(::user::interaction * pWndNewParent)
   //{

   //   return nullptr;

   //   //ASSERT(::IsWindow(get_hwnd()));

   //   //return System.ui_from_handle(::SetParent(get_hwnd(), pWndNewParent->get_hwnd()));

   //}


   //::user::interaction * window::get_parent() const
   //{

   //   return nullptr;

   //   //if (!::IsWindow(get_hwnd()))
   //   //{

   //   //   return nullptr;

   //   //}

   //   //if (get_hwnd() == nullptr)
   //   //{

   //   //   return nullptr;

   //   //}

   //   //HWND hwndParent = ::GetParent(get_hwnd());

   //   //if (hwndParent == nullptr)
   //   //{

   //   //   return nullptr;

   //   //}

   //   //return System.ui_from_handle(hwndParent);

   //}


   bool window::is_iconic()
   {

      if (_get_ex_style() & WS_EX_LAYERED)
      {
         
         auto puserinteraction = m_pimpl->m_puserinteraction;

         return puserinteraction->layout().sketch().display() == ::e_display_iconic;

      }
      else
      {

         if (!::IsWindow(get_hwnd()))
         {

            return false;

         }

         return ::IsIconic(get_hwnd()) != false;

      }

   }


   iptr window::_get_style() const
   {
      
      return get_window_long_ptr(GWL_STYLE);

   }


   iptr window::_get_ex_style() const
   {

      return get_window_long_ptr(GWL_EXSTYLE);

   }


   bool window::_set_style(iptr nStyle)
   {

      return set_window_long_ptr(GWL_STYLE, nStyle);

   }


   bool window::_set_ex_style(iptr nExStyle)
   {

      return set_window_long_ptr(GWL_EXSTYLE, nExStyle);

   }


   ::windowing::window * window::get_owner() const
   {

      oswindow oswindow = get_owner_oswindow();

      auto pwindow = m_pwindowing->window(oswindow);

      return pwindow;

   }


   oswindow window::get_owner_oswindow() const
   {

      if (!::IsWindow(get_hwnd()))
      {

         return nullptr;

      }

      if (get_hwnd() == nullptr)
      {

         return nullptr;

      }

      HWND hwndOwner = ::GetWindow(get_hwnd(), GW_OWNER);

      if (hwndOwner == nullptr)
      {

         return get_parent_oswindow();

      }

      return __oswindow(hwndOwner);

   }


   ::e_status window::set_owner(::windowing::window * pWndNewOwner)
   {

      auto hwnd = get_hwnd();

      HWND hwndOwner = nullptr;

      if (::is_set(pWndNewOwner))
      {

         hwndOwner = __hwnd(pWndNewOwner->get_oswindow());

      }

      if (!::SetWindowLongPtr(hwnd, GWLP_HWNDPARENT, (LONG_PTR) hwndOwner))
      {

         return ::error_failed;

      }

      return ::success;

   }



   //::i32 window::get_window_long(i32 nIndex) const
   //{
   //   return ::GetWindowLong(get_hwnd(), nIndex);
   //}

   //::i32 window::set_window_long(i32 nIndex, ::i32 lValue)
   //{
   //   return ::SetWindowLong(get_hwnd(), nIndex, lValue);
   //}


   //iptr window::get_window_long_ptr(i32 nIndex) const
   //{

   //   return ::GetWindowLongPtr(get_hwnd(), nIndex);

   //}


   //iptr window::set_window_long_ptr(i32 nIndex, iptr lValue)
   //{

   //   return ::SetWindowLongPtr(get_hwnd(), nIndex, lValue);

   //}


   // window
      /* window::operator hwnd() const
      { return this == nullptr ? nullptr : get_hwnd(); }*/
   //bool window::operator==(const window & wnd) const
   //{

   //   return (((window *)&wnd)->get_hwnd()) == ((window *)this)->get_hwnd();

   //}


   //bool window::operator!=(const window & wnd) const
   //{

   //   return (((window *)&wnd)->get_hwnd()) != ((window *)this)->get_hwnd();

   //}


   //u32 window::GetStyle() const
   //{

   //   if (!::IsWindow(get_hwnd()))
   //      return 0;

   //   return (u32)::GetWindowLong(get_hwnd(), GWL_STYLE);

   //}


   void window::default_message_handler(::message::base * pbase)
   {

      
      HWND hwnd = __hwnd(pbase->m_oswindow);

      UINT message = pbase->m_id.u32();

      WPARAM wparam = pbase->m_wparam;

      LPARAM lparam = pbase->m_lparam;

      LRESULT lresult = ::DefWindowProcW(hwnd, message, wparam, lparam);

      pbase->m_lresult = lresult;

   }


   bool window::modify_ex_style(uptr dwRemove, uptr dwAdd, ::u32 nFlags)
   {

      auto nExStyle = _get_ex_style();

      nExStyle &= ~dwRemove;

      nExStyle |= dwAdd;

      _set_ex_style(nExStyle);

      if (nFlags)
      {

         ::SetWindowPos(get_hwnd(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | nFlags);

      }

      return true;

   }


   bool window::modify_style(uptr dwRemove, uptr dwAdd, ::u32 nFlags)
   {

      auto nStyle = _get_style();

      nStyle &= ~dwRemove;

      nStyle |= dwAdd;

      _set_style(nStyle);

      if (nFlags)
      {

         ::SetWindowPos(get_hwnd(), 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOMOVE | nFlags);

      }

      return true;

   }


   //bool window::ModifyStyleEx(u32 dwRemove, u32 dwAdd, ::u32 nFlags)
   //{

   //   if (!::IsWindow(get_hwnd()))
   //      return false;

   //   return ModifyStyleEx(get_hwnd(), dwRemove, dwAdd, nFlags);

   //}


   lresult window::send_message(const ::id & id, wparam wParam, lparam lParam)
   {

      return ::SendMessage(get_hwnd(), id.umessage(), wParam, lParam);

   }


   bool window::post_message(const ::id & id, wparam wParam, lparam lParam)
   {

      HWND hwnd = get_hwnd();

      ::u32 message = id.umessage();

      wparam wparam = wParam;

      return ::PostMessage(hwnd, message, wparam, lParam.m_lparam) != false;

   }


   //bool window::DragDetect(const ::point_i32 & point) const
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return ::DragDetect(((window *)this)->get_hwnd(), point_i32) != false;

   //}


   void window::set_window_text(const char * pszString)
   {

      DWORD_PTR lresult = 0;

      auto puserinteraction = m_pimpl->m_puserinteraction;

      puserinteraction->m_strWindowText = pszString;

      wstring wstrText(puserinteraction->m_strWindowText);

      const unichar * pwszText = wstrText;

      if (!::SendMessageTimeoutW(get_hwnd(), WM_SETTEXT, 0, (LONG_PTR)pwszText, SMTO_ABORTIFHUNG, 500, &lresult))
      {

         return;

      }

      string str;

      get_window_text(str);

   }


   strsize window::get_window_text(char * pszString, strsize nMaxCount)
   {

      string str;

      get_window_text(str);

      ansi_count_copy(pszString, str, (size_t)min(nMaxCount, str.get_length()));

      return str.get_length();

   }


   void window::get_window_text(string & str)
   {

      DWORD_PTR lresult = 0;

      if (!::SendMessageTimeoutW(get_hwnd(), WM_GETTEXTLENGTH, 0, 0, SMTO_ABORTIFHUNG, 90, &lresult))
         return;

      wstring wstr;

      if (!::SendMessageTimeoutW(get_hwnd(), WM_GETTEXT, (lparam)wstr.get_string_buffer(lresult + 1), lresult + 1, SMTO_ABORTIFHUNG, 90, &lresult))
         return;

      str = wstr;

   }


   strsize window::get_window_text_length()
   {

      ASSERT(::IsWindow(get_hwnd()));

      return ::GetWindowTextLength(get_hwnd());

   }


   //void window::DragAcceptFiles(bool bAccept)
   //{
   //   ASSERT(::IsWindow(get_hwnd()));
   //   ::DragAcceptFiles(get_hwnd(), bAccept);
   //}


   ::u32 window::ArrangeIconicWindows()
   {

      ASSERT(::IsWindow(get_hwnd()));
      return ::ArrangeIconicWindows(get_hwnd());

   }


   //i32 window::SetWindowRgn(HRGN hRgn,bool bRedraw)
   //{

   //   //ASSERT(::IsWindow(get_hwnd())); return ::SetWindowRgn(get_hwnd(),hRgn,bRedraw);

   //}


   //i32 window::GetWindowRgn(HRGN hRgn)
   //{

   //   //ASSERT(::IsWindow(get_hwnd()) && hRgn != nullptr); return ::GetWindowRgn(get_hwnd(),hRgn);

   //}


      //void window::bring_to_top(::e_display edisplay)
      //{

      //   if (get_parent() == nullptr)
      //   {

      //      // place the window on top except for certain nCmdShow

      //      if (is_visible(edisplay)
      //         && edisplay_command(edisplay) != e_display_iconic &&
      //         !no_activate(edisplay))
      //      {

      //         HWND hwnd = get_hwnd();

      //         hwnd = ::GetLastActivePopup(hwnd);

      //         ::user::interaction * puserinteraction = System.ui_from_handle(hwnd);

      //         if (puserinteraction == nullptr)
      //            BringWindowToTop();
      //         else
      //            puserinteraction->BringWindowToTop();

      //      }

      //   }

      //}

      // MSDN
      // Brings the specified window to the top of the Z order.
      // If the window is a top-level window, it is activated.
      // If the window is a child window, the top-level parent
      // window associated with the child window is activated.
      //
      // Use the BringWindowToTop function to uncover any window
      // that is partially or completely obscured by other windows.
      // Calling this function is similar to calling the set_window_pos
      // function to change a window's position in the Z order.
      // BringWindowToTop does not make a window a top-level window.

      //bool window::BringWindowToTop()
      //{

      //   bool bOk = ::BringWindowToTop(get_hwnd()) != false;


      //   if (GetExStyle() & WS_EX_LAYERED)
      //   {

      //      if (!(GetExStyle() & WS_EX_TOPMOST))
      //      {

      //         ::set_window_pos(get_hwnd(), (HWND)e_zorder_top, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

      //         //::set_window_pos(get_hwnd(),(HWND)e_zorder_top,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

      //         //::set_window_pos(get_hwnd(),(HWND)e_zorder_top_most,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

      //         //::set_window_pos(get_hwnd(),(HWND)e_zorder_top,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

      //      }

      //   }

      //   return bOk;

      //}


   //void window::MapWindowPoints(::user::window * pwndTo, POINT_I32 * pPoint, ::u32 nCount)
   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   ::MapWindowPoints(get_hwnd(), pwndTo->get_hwnd(), pPoint, nCount);

   //}


   //void window::MapWindowPoints(::user::window * pwndTo, RECTANGLE_I32 * prectangle)

   //{
   //   ASSERT(::IsWindow(get_hwnd()));
   //   ::MapWindowPoints(get_hwnd(), pwndTo->get_hwnd(), (POINT_I32 *)prectangle, 2);

   //}



   void window::UpdateWindow()
   {
      ::UpdateWindow(get_hwnd());
   }

   void window::SetRedraw(bool bRedraw)
   {
      ASSERT(::IsWindow(get_hwnd()));
      ::SendMessage(get_hwnd(), WM_SETREDRAW, bRedraw, 0);
   }

   bool window::GetUpdateRect(RECTANGLE_I32 * prectangle, bool bErase)
   {

      ASSERT(::IsWindow(get_hwnd()));
      return ::GetUpdateRect(get_hwnd(), (RECT *) prectangle, bErase) != false;

   }


   i32 window::GetUpdateRgn(::draw2d::region * pRgn, bool bErase)
   {

      //ASSERT(::IsWindow(get_hwnd()));
      //return ::GetUpdateRgn(get_hwnd(), (HRGN)pRgn->get_os_data(), bErase);

      return 0;

   }


   void window::Invalidate(bool bErase)
   {
      ASSERT(::IsWindow(get_hwnd()));
      ::InvalidateRect(get_hwnd(), nullptr, bErase);
   }

   void window::InvalidateRect(const RECTANGLE_I32 * rectangle, bool bErase)

   {
      ASSERT(::IsWindow(get_hwnd()));
      ::InvalidateRect(get_hwnd(), (RECT *) rectangle, bErase);

   }


   void window::InvalidateRgn(::draw2d::region * pRgn, bool bErase)
   {

      //ASSERT(::IsWindow(get_hwnd()));
      //::InvalidateRgn(get_hwnd(), (HRGN)pRgn->get_os_data(), bErase);

   }


   void window::ValidateRect(const RECTANGLE_I32 * rectangle)

   {

      ASSERT(::IsWindow(get_hwnd()));

      ::ValidateRect(get_hwnd(), (RECT *) rectangle);


   }


   void window::ValidateRgn(::draw2d::region * pRgn)
   {

      ASSERT(::IsWindow(get_hwnd()));

      //::ValidateRgn(get_hwnd(), (HRGN)pRgn->get_os_data());

   }


   void window::on_visual_applied()
   {

      //rects_from_os();

      //::user::window::on_visual_applied();

   }


   // void window::window_apply_visual(const ::user::window_state & windowstate)
   // {

   //    return ::user::window::window_apply_visual(windowstate);

   // }


   //bool window::_is_window_visible()
   //{

   //   try
   //   {

   //      if (!::IsWindowVisible(get_hwnd()))
   //      {

   //         return false;

   //      }

   //      return true;

   //   }
   //   catch (...)
   //   {

   //      return false;

   //   }

   //}


   void window::ShowOwnedPopups(bool bShow)
   {

      ASSERT(::IsWindow(get_hwnd()));

      ::ShowOwnedPopups(get_hwnd(), bShow);

   }




   __pointer(::draw2d::graphics) window::GetDCEx(::draw2d::region * prgnClip, u32 flags)
   {

      ASSERT(::IsWindow(get_hwnd()));

      auto pgraphics = __create < ::draw2d::graphics >();

      __throw(todo());

      //pgraphics->attach(::GetDCEx(get_hwnd(), (HRGN)prgnClip->get_os_data(), flags));

      return pgraphics;


   }


   bool window::LockWindowUpdate()
   {

      ASSERT(::IsWindow(get_hwnd()));

      return ::LockWindowUpdate(get_hwnd()) != false;

   }

   void window::UnlockWindowUpdate()
   {

      ASSERT(::IsWindow(get_hwnd()));

      ::LockWindowUpdate(nullptr);

   }


   bool window::RedrawWindow(const ::rectangle_i32 & rectUpdate, ::draw2d::region * prgnUpdate, ::u32 flags)
   {

      //if (m_bDestroyImplOnly)
      //{

      //   return false;

      //}

      auto puserinteraction = m_pimpl->m_puserinteraction;

      if (!puserinteraction->is_window_visible(::user::e_layout_sketch))
      {

         return true;

      }

      if (!(_get_ex_style() & WS_EX_LAYERED))
      {

         ::RedrawWindow(get_hwnd(), nullptr, nullptr, flags);

      }

      if (flags & RDW_UPDATENOW)
      {





         //if (GetExStyle() & WS_EX_LAYERED)
         {

            //_001UpdateWindow();

            //return true;

         }
         //else
         //{

         //   if (!is_window_visible() && !m_bShowFlags)
         //      return false;

         //   if (!is_window_visible())
         //   {

         //      if (m_iShowFlags & SWP_SHOWWINDOW)
         //      {

         //         display(e_display_normal);

         //      }

         //   }

         //   return ::set_need_redraw(get_hwnd(), pRectUpdate, prgnUpdate == nullptr ? nullptr : (HRGN)prgnUpdate->get_os_data(),

         //      flags | RDW_NOERASE | RDW_NOFRAME | RDW_INVALIDATE) != false;

         //}

      }
      else
      {

         puserinteraction->set_need_redraw();

      }

      return true;

   }





   bool window::EnableScrollBar(i32 nSBFlags, ::u32 nArrowFlags)
   {

      ASSERT(::IsWindow(get_hwnd()));

      return ::EnableScrollBar(get_hwnd(), nSBFlags, nArrowFlags) != false;

   }

   //bool window::DrawAnimatedRects(i32 idAni, const LPRECTprcFrom, const LPRECTlprcTo)

   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   return ::DrawAnimatedRects(get_hwnd(), idAni, prcFrom, lprcTo) != false;


   //}

   bool window::DrawCaption(::draw2d::graphics_pointer & pgraphics, const rectangle_i32 & prc, ::u32 uFlags)

   {

      ASSERT(::IsWindow(get_hwnd()));

      ::exception::throw_not_implemented();
      return false;
      //      return ::DrawCaption(get_hwnd(), (HDC)(dynamic_cast<::windows::graphics * >(pgraphics))->get_hwnd(), prc, uFlags) != false;


   }


   bool window::is_this_enabled()
   {

      return ::IsWindowEnabled(get_hwnd()) != false;

   }


   bool window::enable_window(bool bEnable)
   {

      return ::EnableWindow(get_hwnd(), bEnable) != false;

   }


   //::e_status window::set_focus()
   //{

   //   HWND hwnd = get_hwnd();

   //   if (!::SetFocus(hwnd))
   //   {

   //      return ::error_failed;
   //      
   //   }

   //   return ::success;

   //}


   bool window::has_focus()
   {

      HWND hwnd = get_hwnd();

      if (::GetFocus() == hwnd)
      {

         return false;

      }

      return true;

   }


   bool window::is_active()
   {

      ithread_t ithread = 0;

      auto puserinteraction = m_pimpl->m_puserinteraction;

      if (puserinteraction && puserinteraction->m_pthreadUserInteraction)
      {

         ithread = puserinteraction->m_pthreadUserInteraction->get_ithread();

      }

      GUITHREADINFO info = {};

      info.cbSize = sizeof(GUITHREADINFO);

      HWND hwndActive;

      if (GetGUIThreadInfo(ithread, &info))
      {

         hwndActive = info.hwndActive;

      }
      else
      {

         hwndActive = ::GetActiveWindow();

      }

      if (hwndActive == get_hwnd())
      {

         return true;

      }

      //auto puserinteraction = System.ui_from_handle(oswindowActive);

      //if (::is_set(puserinteraction))
      //{

      //   if (puserinteraction->m_pthreadUserInteraction == puserinteraction->m_pthreadUserInteraction)
      //   {

      //      if (puserinteraction->m_ewindowflag & e_window_flag_satellite_window)
      //      {

      //         return true;

      //      }

      //   }

      //}

      return false;

   }


   //bool window::SetFocus()
   //{

   //   if (!::IsWindow(get_hwnd()))
   //   {

   //      return false;

   //   }

   //   if (!::IsWindowVisible(get_hwnd()))
   //   {

   //      return false;

   //   }

   //   puserinteraction->post_routine(__routine([this]()
   //      {

   //         HWND hwnd = ::SetFocus(get_hwnd());

   //         ::output_debug_string("::windowing_win32::window::SetFocus ::SetFocus(" + ::hex::lower_from((iptr)hwnd) + ")");

   //         return ::success;

   //      }));

   //   return true;

   //}


   //::user::interaction * window::get_desktop_window()
   //{

   //   return System.ui_from_handle(::get_desktop_window());

   //}


   // helper for radio buttons
   //i32 window::GetCheckedRadioButton(i32 nIDFirstButton, i32 nIDLastButton)
   //{
   //   for (i32 nID = nIDFirstButton; nID <= nIDLastButton; nID++)
   //   {
   //      if (IsDlgButtonChecked(nID))
   //         return nID; // id that matched
   //   }
   //   return 0; // invalid ID
   //}


   //void window::CheckDlgButton(i32 nIDButton, ::u32 nCheck)
   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   ::CheckDlgButton(get_hwnd(), nIDButton, nCheck);

   //}


   //void window::CheckRadioButton(i32 nIDFirstButton, i32 nIDLastButton, i32 nIDCheckButton)
   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   ::CheckRadioButton(get_hwnd(), nIDFirstButton, nIDLastButton, nIDCheckButton);

   //}


   //i32 window::DlgDirList(TCHAR * pPathSpec, i32 nIDListBox, i32 nIDStaticPath, ::u32 nFileType)

   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   return ::DlgDirList(get_hwnd(), pPathSpec, nIDListBox, nIDStaticPath, nFileType);


   //}


   //i32 window::DlgDirListComboBox(TCHAR * pPathSpec, i32 nIDComboBox, i32 nIDStaticPath, ::u32 nFileType)

   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   return ::DlgDirListComboBox(get_hwnd(), pPathSpec, nIDComboBox, nIDStaticPath, nFileType);


   //}


   //bool window::DlgDirSelect(TCHAR * pString, i32 nSize, i32 nIDListBox)

   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   return ::DlgDirSelectEx(get_hwnd(), pString, nSize, nIDListBox) != false;


   //}


   //bool window::DlgDirSelectComboBox(TCHAR * pString, i32 nSize, i32 nIDComboBox)

   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   return ::DlgDirSelectComboBoxEx(get_hwnd(), pString, nSize, nIDComboBox) != false;


   //}


   //void window::get_child_by_id(id id, hwnd * poswindow_) const
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   ASSERT(poswindow_ != nullptr);

   //   *poswindow_ = ::GetDlgItem(((window *)this)->get_hwnd(), (i32)id);

   //}


   //::u32 window::GetChildByIdInt(i32 nID, BOOL * pTrans, bool bSigned) const

   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return ::GetDlgItemInt(((window *)this)->get_hwnd(), nID, pTrans, bSigned);


   //}


   //i32 window::GetChildByIdText(i32 nID, TCHAR * pStr, i32 nMaxCount) const

   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return ::GetDlgItemText(((window *)this)->get_hwnd(), nID, pStr, nMaxCount);


   //}


   //::user::window * window::GetNextDlgGroupItem(::user::window * pWndCtl, bool bPrevious) const
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return System.ui_from_handle(::GetNextDlgGroupItem(((window *)this)->get_hwnd(), pWndCtl->get_hwnd(), bPrevious));

   //}


   //::user::window * window::GetNextDlgTabItem(::user::window * pWndCtl, bool bPrevious) const
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return System.ui_from_handle(::GetNextDlgTabItem(((window *)this)->get_hwnd(), pWndCtl->get_hwnd(), bPrevious));

   //}


   //::u32 window::IsDlgButtonChecked(i32 nIDButton) const
   //{
   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));
   //   return ::IsDlgButtonChecked(((window *)this)->get_hwnd(), nIDButton);
   //}


   //lparam window::SendDlgItemMessage(i32 nID, ::u32 message, wparam wParam, lparam lParam)
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return ::SendDlgItemMessage(((window *)this)->get_hwnd(), message, message, wParam, lParam);

   //}


   //void window::SetDlgItemInt(i32 nID, ::u32 nValue, bool bSigned)
   //{
   //   ASSERT(::IsWindow(get_hwnd()));
   //   ::SetDlgItemInt(get_hwnd(), nID, nValue, bSigned);
   //}
   //void window::SetDlgItemText(i32 nID, const char * pszString)

   //{
   //   ASSERT(::IsWindow(get_hwnd()));
   //   ::SetDlgItemTextW(get_hwnd(), nID, wstring(pszString));

   //}
   //i32 window::ScrollWindowEx(i32 dx, i32 dy,
   //   const RECTANGLE_I32 * pRectScroll, const RECTANGLE_I32 * lpRectClip,

   //   ::draw2d::region * prgnUpdate, RECTANGLE_I32 * pRectUpdate, ::u32 flags)

   //{

   //   //ASSERT(::IsWindow(get_hwnd()));
   //   //return ::ScrollWindowEx(get_hwnd(), dx, dy, pRectScroll, lpRectClip,

   //   //                        (HRGN)prgnUpdate->get_os_data(), pRectUpdate, flags);

   //   return 0;


   //}

   //void window::ShowScrollBar(::u32 nBar, bool bShow)
   //{
   //   ASSERT(::IsWindow(get_hwnd()));
   //   ::ShowScrollBar(get_hwnd(), nBar, bShow);
   //}
   //::user::interaction * window::ChildWindowFromPoint(const ::point_i32 & point)
   //{
   //   ASSERT(::IsWindow(get_hwnd()));

   //   return  System.ui_from_handle(::ChildWindowFromPoint(get_hwnd(), point_i32));


   //}

   //::user::interaction * window::ChildWindowFromPoint(const ::point_i32 & point, ::u32 nFlags)
   //{
   //   ASSERT(::IsWindow(get_hwnd()));

   //   return  System.ui_from_handle(::ChildWindowFromPointEx(get_hwnd(), point, nFlags));


   //}



   ::windowing::window * window::get_next_window(::u32 nFlag)
   {

      HWND hwnd = get_hwnd();

      HWND hwndNext = ::GetNextWindow(hwnd, nFlag);

      auto pwindowing = windowing();

      auto pwindowNext = pwindowing->_window(hwndNext);

      return pwindowNext;

   }


   ::windowing::window * window::get_top_window() const
   {

      HWND hwnd = get_hwnd();

      HWND hwndTop = ::GetTopWindow(hwnd);

      auto pwindowing = windowing();

      auto pwindowTop = pwindowing->_window(hwndTop);

      return pwindowTop;

   }


   ::windowing::window * window::get_window(::u32 nCmd) const
   {

      HWND hwnd = get_hwnd();

      HWND hwndGet = ::GetWindow(hwnd, nCmd);

      auto pwindowing = windowing();

      auto pwindowGet = pwindowing->_window(hwndGet);

      return pwindowGet;

   }


   ::windowing::window * window::get_last_active_popup() const
   {

      HWND hwnd = get_hwnd();

      HWND hwndLastActivePopup = ::GetLastActivePopup(hwnd);

      auto pwindowing = windowing();

      auto pwindowLastActivePopup = pwindowing->_window(hwndLastActivePopup);

      return pwindowLastActivePopup;

   }


   //bool window::FlashWindow(bool bInvert)
   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   return ::FlashWindow(get_hwnd(), bInvert) != false;

   //}


   //bool window::ChangeClipboardChain(hwnd oswindow_Next)
   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   return ::ChangeClipboardChain(get_hwnd(), oswindow_Next) != false;

   //}


   //hwnd window::SetClipboardViewer()
   //{

   //   ASSERT(::IsWindow(get_hwnd()));

   //   return ::SetClipboardViewer(get_hwnd());

   //}


   //bool window::open_clipboard()
   //{

   //   if (m_bClipboardOpened)
   //   {

   //      return true;

   //   }

   //   if (!::OpenClipboard(get_hwnd()))
   //   {

   //      return false;

   //   }

   //   m_bClipboardOpened = true;

   //   return true;

   //}


   //bool window::close_clipboard()
   //{

   //   if (!m_bClipboardOpened)
   //   {

   //      return false;

   //   }

   //   if (!::CloseClipboard())
   //   {

   //      return false;

   //   }

   //   m_bClipboardOpened = false;

   //   return true;

   //}


   //::user::interaction * window::GetOpenClipboardWindow()
   //{

   //   return System.ui_from_handle(::GetOpenClipboardWindow());

   //}


   //::user::interaction * window::GetClipboardOwner()
   //{

   //   return System.ui_from_handle(::GetClipboardOwner());

   //}

   
   //::user::interaction * window::GetClipboardViewer()
   //{

   //   return System.ui_from_handle(::GetClipboardViewer());

   //}


   point_i32 window::GetCaretPos()
   {

      ::point_i32 point;

      ::GetCaretPos((POINT *)&point);

      return point;

   }


   void window::SetCaretPos(const ::point_i32 & point)
   {

      ::SetCaretPos(point.x, point.y);

   }


   void window::HideCaret()
   {

      ::HideCaret(get_hwnd());

   }


   void window::ShowCaret()
   {

      ::ShowCaret(get_hwnd());

   }

   
   //hicon window::SetIcon(hicon hIcon, bool bBigIcon)
   //{

   //   return (hicon)send_message(WM_SETICON, bBigIcon, (lparam)hIcon);

   //}

   //hicon window::GetIcon(bool bBigIcon) const
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return (hicon)const_cast <::windowing_win32::window *> (this)->send_message(WM_GETICON, bBigIcon);

   //}

   //void window::Print(::draw2d::graphics_pointer & pgraphics, u32 dwFlags) const
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   ::exception::throw_not_implemented();
   //   //      const_cast < ::windowing_win32::window * > (this)->send_message(WM_PRINT, (wparam)(dynamic_cast<::windows::graphics * >(pgraphics))->get_hwnd(), (lparam) dwFlags);

   //}

   //void window::PrintClient(::draw2d::graphics_pointer & pgraphics, u32 dwFlags) const
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   ::exception::throw_not_implemented();
   //   //const_cast < ::windowing_win32::window * > (this)->send_message(WM_PRINTCLIENT, (wparam)(dynamic_cast<::windows::graphics * >(pgraphics))->get_hwnd(), (lparam) dwFlags);

   //}

   //bool window::SetWindowContextHelpId(u32 dwContextHelpId)
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return ::SetWindowContextHelpId(((window *)this)->get_hwnd(), dwContextHelpId) != false;

   //}

   //u32 window::GetWindowContextHelpId() const
   //{

   //   ASSERT(::IsWindow(((window *)this)->get_hwnd()));

   //   return ::GetWindowContextHelpId(((window *)this)->get_hwnd());

   //}


   // Default message map implementations
      //void window::OnActivateApp(bool, u32)
      //{
      //   Default();
      //}
      //void window::OnActivate(::u32, ::user::window *, bool)
      //{
      //   Default();
      //}
      //void window::OnCancelMode()
      //{
      //   Default();
      //}
      //void window::OnChildActivate()
      //{
      //   Default();
      //}
      //void window::OnClose()
      //{
      //   Default();
      //}
      //void window::OnContextMenu(::user::window *, point_i32)
      //{
      //   Default();
      //}

      //bool window::OnCopyData(::user::window *, COPYDATASTRUCT*)
      //{

      //   return Default() != false;

      //}

      //void window::OnEnable(bool)
      //{
      //   Default();
      //}
      //void window::OnEndSession(bool)
      //{
      //   Default();
      //}

      //bool window::OnEraseBkgnd(::draw2d::graphics *)
      //{

      //   return Default() != false;

      //}

      //void window::OnGetMinMaxInfo(MINMAXINFO*)
      //{
      //   Default();
      //}
      //void window::OnIconEraseBkgnd(::draw2d::graphics *)
      //{
      //   Default();
      //}


   void window::_001OnSetFocus(::message::message * pbase)
   {

      //m_bFocusImpl = true;

      //if (!m_bSystemCaret)
      //{

      //   m_bSystemCaret = true;

      //}

   }


   void window::_001OnKillFocus(::message::message * pmessage)
   {

      //m_bFocusImpl = false;

      //if (m_bSystemCaret)
      //{

      //   m_bSystemCaret = false;

      //   ::DestroyCaret();

      //}

   }


   //lresult window::OnMenuChar(::u32, ::u32, ::user::menu*)
   //{

   //   return Default();

   //}


   //void window::OnMenuSelect(::u32, ::u32, HMENU)
   //{

   //   Default();

   //}


   //void window::OnMove(i32, i32)
   //{

   //   Default();

   //}


   //hcursor window::OnQueryDragIcon()
   //{

   //   return (hcursor)Default();

   //}

   //bool window::OnQueryEndSession()
   //{

   //   return Default() != false;

   //}

   //bool window::OnQueryNewPalette()
   //{

   //   return Default() != false;

   //}

   //bool window::OnQueryOpen()
   //{

   //   return Default() != false;

   //}

   void window::_001OnSetCursor(::message::message * pmessage)
   {

      //__pointer(::message::base) pbase(pmessage);

      //auto psession = Session;

      //auto pcursor = psession->get_cursor();

      //if (pcursor != nullptr && pcursor->m_ecursor != cursor_system)
      //{

      //   pcursor->set_current(puserinteraction, psession);

      //}

      //pbase->m_lresult = 1;

      //pbase->m_bRet = true;

   }


   //void window::OnShowWindow(bool, ::u32)
   //{

   //   Default();

   //}


   //void window::OnSize(::u32, i32, i32)
   //{

   //   Default();

   //}


   //void window::OnTCard(::u32, u32)
   //{

   //   Default();

   //}


   void window::_001OnWindowPosChanging(::message::message * pmessage)
   {

      return;


   }


   void window::_001OnWindowPosChanged(::message::message * pmessage)
   {

      WINDOWPOS * pwindowpos = (WINDOWPOS *)pmessage->m_lparam.m_lparam;

      output_debug_string("\ninteraction_impl::_001OnWindowPosChanged");

      if (::IsIconic(get_hwnd()))
      {

         auto puserinteraction = m_pimpl->m_puserinteraction;

         if (puserinteraction->layout().sketch().display() != ::e_display_iconic)
         {

            output_debug_string(" IsIconic or not IsIconic, thats the question window::_001OnWindowPosChanged");

         }

      }
      else if (::IsZoomed(get_hwnd()))
      {

         auto puserinteraction = m_pimpl->m_puserinteraction;

         if (puserinteraction->layout().window().display() != ::e_display_zoomed)
         {

            output_debug_string(" IsZoomed or not IsZoomed, thats the question window::_001OnWindowPosChanged");

         }

      }


      //if (m_bDestroyImplOnly)
      //{

      //   return;

      //}

      auto puserinteraction = m_pimpl->m_puserinteraction;

      if (puserinteraction->layout().m_eflag)
      {

         return;

      }

      ::point_i32 point(pwindowpos->x, pwindowpos->y);

      bool bMove = false;

      if (puserinteraction->layout().sketch().origin() != point)
      {

         if (puserinteraction->layout().is_moving())
         {

            INFO("Window is Moving :: _001OnMove");

         }

         puserinteraction->layout().sketch().origin() = point;

         bMove = true;

      }

      ::size_i32 size(pwindowpos->cx, pwindowpos->cy);

      bool bSize = false;

      if (puserinteraction->layout().sketch().size() != size)
      {

         puserinteraction->layout().sketch().size() = size;

         bSize = true;

      }

      if (bMove)
      {

         puserinteraction->set_reposition();

         puserinteraction->set_need_redraw();

      }

      if (bSize)
      {

         puserinteraction->set_need_layout();

      }

      if (bMove || bSize)
      {

         puserinteraction->set_need_redraw();

      }


      pmessage->m_bRet = true;

   }


   void window::_001OnGetMinMaxInfo(::message::message * pmessage)
   {

      __pointer(::message::base) pbase(pmessage);

   }

   //   void window::OnDropFiles(HDROP)
   //   {
   //      Default();
   //   }
   //   void window::OnPaletteIsChanging(::user::window *)
   //   {
   //      Default();
   //   }
   //
   //   bool window::OnNcActivate(bool)
   //   {
   //
   //      return Default() != false;
   //
   //   }
   //
   //   void window::OnNcCalcSize(bool, NCCALCSIZE_PARAMS*)
   //   {
   //      Default();
   //   }
   //
   //   bool window::OnNcCreate(::user::system *)
   //   {
   //
   //      return Default() != false;
   //
   //   }
   //
   //   lresult window::OnNcHitTest(const point_i32 & )
   //   {
   //      return Default();
   //   }
   //   void window::OnNcLButtonDblClk(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcLButtonDown(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcLButtonUp(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcMButtonDblClk(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcMButtonDown(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcMButtonUp(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcMouseMove(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcPaint()
   //   {
   //      Default();
   //   }
   //   void window::OnNcRButtonDblClk(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcRButtonDown(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnNcRButtonUp(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnSysChar(::u32, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnSysCommand(::u32, lparam)
   //   {
   //      Default();
   //   }
   //   void window::OnSysDeadChar(::u32, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnSysKeyDown(::u32, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnSysKeyUp(::u32, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnCompacting(::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnFontChange()
   //   {
   //      Default();
   //   }
   //   void window::OnPaletteChanged(::user::window *)
   //   {
   //      Default();
   //   }
   //   void window::OnSpoolerStatus(::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnTimeChange()
   //   {
   //      Default();
   //   }
   //   void window::OnChar(::u32, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnDeadChar(::u32, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnKeyDown(::u32, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnKeyUp(::u32, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnLButtonDblClk(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnLButtonDown(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnLButtonUp(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnMButtonDblClk(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnMButtonDown(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnMButtonUp(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   i32 window::OnMouseActivate(::user::window *, ::u32, ::u32)
   //   {
   //      return (i32)Default();
   //   }
   //   void window::OnMouseMove(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //
   //   bool window::OnMouseWheel(::u32, i16, const point_i32&)
   //   {
   //
   //      return Default() != false;
   //
   //   }
   //
   //   lresult window::OnRegisteredMouseWheel(wparam, lparam)
   //   {
   //      return Default();
   //   }
   //   void window::OnRButtonDblClk(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnRButtonDown(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnRButtonUp(::u32, const point_i32&)
   //   {
   //      Default();
   //   }
   //   void window::OnTimer(uptr)
   //   {
   //      Default();
   //   }
   //   void window::OnInitMenu(::user::menu*)
   //   {
   //      Default();
   //   }
   //   void window::OnInitMenuPopup(::user::menu*, ::u32, bool)
   //   {
   //      Default();
   //   }
   //   void window::OnAskCbFormatName(__in ::u32 nMaxCount, __out_ecount_z(nMaxCount) char * lpszName)
   //   {
   //      (nMaxCount);
   //      if (nMaxCount > 0)
   //      {
   //         /* defwindow proc should do this for us, but to be safe, we'll do it here too */
   //         lpszName[0] = '\0';
   //      }
   //      Default();
   //   }
   //   void window::OnChangeCbChain(hwnd, hwnd)
   //   {
   //      Default();
   //   }
   //   void window::OnDestroyClipboard()
   //   {
   //      Default();
   //   }
   //   void window::OnDrawClipboard()
   //   {
   //      Default();
   //   }
   //   void window::OnHScrollClipboard(::user::window *, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnPaintClipboard(::user::window *, HGLOBAL)
   //   {
   //      Default();
   //   }
   //   void window::OnRenderAllFormats()
   //   {
   //      Default();
   //   }
   //   void window::OnRenderFormat(::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnSizeClipboard(::user::window *, HGLOBAL)
   //   {
   //      Default();
   //   }
   //   void window::OnVScrollClipboard(::user::window *, ::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   ::u32 window::OnGetDlgCode()
   //   {
   //      return (::u32)Default();
   //   }
   //   void window::OnMDIActivate(bool, ::user::window *, ::user::window *)
   //   {
   //      Default();
   //   }
   //   void window::OnEnterMenuLoop(bool)
   //   {
   //      Default();
   //   }
   //   void window::OnExitMenuLoop(bool)
   //   {
   //      Default();
   //   }
   //// Win4 support
   //   void window::OnStyleChanged(i32, LPSTYLESTRUCT)
   //   {
   //      Default();
   //   }
   //   void window::OnStyleChanging(i32, LPSTYLESTRUCT)
   //   {
   //      Default();
   //   }
   //   void window::OnSizing(::u32, RECTANGLE_I32 *)
   //   {
   //      Default();
   //   }
   //   void window::OnMoving(::u32, RECTANGLE_I32 *)
   //   {
   //      Default();
   //   }
   //   void window::OnCaptureChanged(::user::window *)
   //   {
   //      Default();
   //   }
   //
   //   bool window::OnDeviceChange(::u32, uptr)
   //   {
   //
   //      return Default() != false;
   //
   //   }
   //
   //   void window::OnWinIniChange(const char *)
   //   {
   //      Default();
   //   }
   //   void window::OnChangeUIState(::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   void window::OnUpdateUIState(::u32, ::u32)
   //   {
   //      Default();
   //   }
   //   ::u32 window::OnQueryUIState()
   //   {
   //      return (::u32)Default();
   //   }

   // window dialog data support
   //    void window::do_data_exchange(CDataExchange*)
   //   { } // default does nothing

   // window modality support

   void window::BeginModalState()
   {

      ::EnableWindow(get_hwnd(), false);

   }

   void window::EndModalState()
   {

      ::EnableWindow(get_hwnd(), true);

   }

   // frame_window
      /*    void frame_window::DelayUpdateFrameTitle()
      { m_nIdleFlags |= idleTitle; }
      void frame_window::DelayRecalcLayout(bool bNotify)
      { m_nIdleFlags |= (idleLayout | (bNotify ? idleNotify : 0)); };
      bool frame_window::InModalState() const
      { return m_cModalStack != 0; }
      void frame_window::set_title(const char * pszTitle)

      { m_strTitle = pszTitle; }

      string frame_window::get_title() const
      { return m_strTitle; }
      */



   void window::CloseWindow()
   {
      ASSERT(::IsWindow(get_hwnd()));
      ::CloseWindow(get_hwnd());
   }

   bool window::OpenIcon()
   {

      ASSERT(::IsWindow(get_hwnd()));

      return ::OpenIcon(get_hwnd()) != false;

   }


   //////////////////////////////////////////////////////////////////////////////
   //// UI related window functions


   //hwnd window::get_safe_owner(hwnd hParent, hwnd * pWndTop)
   //{
   //   // get window to start with
   //   HWND hwnd = hParent;
   //   if (hwnd == nullptr)
   //   {
   //      /* trans      __pointer(::user::frame_window) pFrame = channel::GetRoutingFrame_();
   //      if (pFrame != nullptr)
   //      hwnd = pFrame->get_hwnd();
   //      else
   //      hwnd = System.m_puiMain->get_hwnd();*/
   //   }

   //   // a popup window cannot be owned by a child window
   //   while (hwnd != nullptr && (::GetWindowLong(hwnd, GWL_STYLE) & WS_CHILD))
   //   {

   //      hwnd = ::GetParent(hwnd);

   //   }

   //   // determine toplevel window to disable as well
   //   ::hwnd oswindow_Top = hwnd;
   //   ::hwnd oswindow_Temp = hwnd;
   //   for (;;)
   //   {
   //      if (oswindow_Temp == nullptr)
   //         break;
   //      else
   //         oswindow_Top = oswindow_Temp;
   //      oswindow_Temp = ::GetParent(oswindow_Top);
   //   }

   //   // get last active popup of first non-child that was found
   //   if (hParent == nullptr && hwnd != nullptr)
   //      hwnd = ::GetLastActivePopup(hwnd);

   //   // disable and store top level parent window if specified
   //   if (pWndTop != nullptr)
   //   {
   //      if (oswindow_Top != nullptr && ::IsWindowEnabled(oswindow_Top) && oswindow_Top != hwnd)
   //      {
   //         *pWndTop = oswindow_Top;
   //         ::EnableWindow(oswindow_Top, false);
   //      }
   //      else
   //         *pWndTop = nullptr;
   //   }

   //   return hwnd;    // return the owner as hwnd
   //}


   //lresult CALLBACK __cbt_filter_hook(i32 code, wparam wParam, lparam lParam)
   //{

   //   if (code != HCBT_CREATEWND)
   //   {

   //      goto call_next_hook;

   //   }

   //   ASSERT(lParam != 0);

   //   ::user::system * pcs = (::user::system *) ((LPCBT_CREATEWND)lParam)->lpcs;

   //   ASSERT(pcs != nullptr);

   //   ::windowing_win32::window * pwnd = thread_set("wnd_init");

   //   if (pwnd != nullptr || (!(pcs->style & WS_CHILD)))
   //   {

   //      thread_set("wnd_init") = nullptr;

   //      // Note: special check to avoid subclassing the IME window
   //      //if (gen_DBCS)
   //      {
   //         // check for cheap CS_IME style first...
   //         if (GetClassLongW((hwnd)wParam, GCL_STYLE) & CS_IME)
   //         {

   //            goto call_next_hook;

   //         }

   //         const wchar_t * pszClassName;

   //         wchar_t szClassName[128] = {};

   //         if(uptr(pcs->lpszClass) > 0xffff)
   //         {

   //            pszClassName = pcs->lpszClass;

   //         }
   //         else
   //         {

   //            ::GlobalGetAtomNameW((ATOM)(uptr)pcs->lpszClass, szClassName, _countof(szClassName));

   //            pszClassName = szClassName;

   //         }

   //         // a little more expensive to test this way, but necessary...
   //         if (wide_find_string_case_insensitive(pszClassName, L"ime") != 0)
   //         {
   //          
   //            goto call_next_hook;

   //         }

   //      }

   //      ASSERT(wParam != 0);

   //      ::HWND hwnd = (::hwnd) wParam;

   //      if (pwnd != nullptr)
   //      {

   //         ASSERT(oswindow_get(hwnd) == nullptr);

   //         pwnd->puserinteraction->m_pimpl = pwnd;

   //         pwnd->attach(hwnd);

   //         pwnd->pre_subclass_window();

   //         WNDPROC * ppwndprocSuper = pwnd->GetSuperWndProcAddr();

   //         ASSERT(ppwndprocSuper != nullptr);

   //         WNDPROC pwndprocOld = (WNDPROC)SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (uptr)get_window_procedure();

   //         ASSERT(pwndprocOld != nullptr);

   //         if (pwndprocOld != get_window_procedure())
   //         {

   //            *ppwndprocSuper = pwndprocOld;

   //         }

   //      }

   //   }

   //call_next_hook:

   //   lresult lResult = CallNextHookEx(t_hHookOldCbtFilter, code, wParam, lParam);

   //   return lResult;

   //}


   void window::_001OnEraseBkgnd(::message::message * pmessage)
   {
      __pointer(::message::erase_bkgnd) perasebkgnd(pmessage);
      perasebkgnd->m_bRet = true;
      perasebkgnd->set_result(true);
   }


   //void window::_001OnTriggerMouseInside()
   //{

   //   ::user::window::_001OnTriggerMouseInside();

   //   TRACKMOUSEEVENT tme = { sizeof(tme) };
   //   tme.dwFlags = TME_LEAVE;
   //   tme.hwndTrack = get_hwnd();
   //   TrackMouseEvent(&tme);

   //}


   void window::set_viewport_org(::draw2d::graphics_pointer & pgraphics)
   {

      // graphics will be already set its view port to the window for linux - cairo with xlib

      pgraphics->SetViewportOrg(::point_i32());

   }


   void window::on_set_parent(::user::interaction * puserinteraction) {

      __throw(interface_only_exception());
   }

    bool window::get_rect_normal(RECTANGLE_I32 * prectangle) {

       __throw(interface_only_exception());
       return false;
    }

    //// virtual void register_drop_target();
    //void window::show_task(bool bShow)
    //{

    //   __throw(interface_only_exception());

    //}
    //

    void window::window_show_change_visibility(::e_display edisplay, ::e_activation eactivation)
    {

       __throw(interface_only_exception());

    }



   /*

/*   void window::_001DeferPaintLayeredWindowBackground(::image * pimage)
   {


   }

   */



   //bool window::get_rect_normal(RECTANGLE_I32 * prectangle)

   //{

   //   WINDOWPLACEMENT wp;

   //   __zero(wp);

   //   if (!GetWindowPlacement(&wp))
   //      return false;

   //   *prectangle = wp.rcNormalPosition;


   //   return true;

   //}


   //DWORD WINAPI drop_target(LPVOID point_i32)
   //{

   //   window * pimpl = (window *)point_i32;

   //   HRESULT hr = OleInitialize(nullptr);

   //   hr = ::RegisterDragDrop(pimpl->get_hwnd(), pimpl);

   //   if (SUCCEEDED(CoCreateInstance(CLSID_DragDropHelper, nullptr,
   //      CLSCTX_INPROC_SERVER,
   //      IID_IDropTargetHelper,
   //      (void **)&pimpl->m_piDropHelper)))
   //   {
   //      pimpl->m_bUseDnDHelper = true;
   //   }

   //   MSG msg;

   //   while (::GetMessage(&msg, nullptr, 0, 0xffffffffu))
   //   {

   //      TranslateMessage(&msg);

   //      DispatchMessage(&msg);

   //   }

   //   return 0;

   //}


   //void window::register_drop_target()
   //{
   //   m_bUseDnDHelper = false;

   //   ::CreateThread(nullptr, 0, drop_target, this, 0, nullptr);


   //}


   void window::show_task(bool bShow)
   {

      sync_lock sl(mutex());

      // https://www.daniweb.com/programming/software-development/threads/457564/mfc-application-disablehide-taskbar-icon

      if (bShow)
      {

         modify_ex_style(WS_EX_TOOLWINDOW, 0, SWP_FRAMECHANGED);

      }
      else
      {

         modify_ex_style(0, WS_EX_TOOLWINDOW, SWP_FRAMECHANGED);

      }

      defer_co_initialize_ex(false);

      comptr < ITaskbarList>                     tasklist;

      HRESULT hr = tasklist.CoCreateInstance(CLSID_TaskbarList, nullptr, CLSCTX_INPROC_SERVER);

      if (SUCCEEDED(hr) && SUCCEEDED(hr = tasklist->HrInit()))
      {

         if (bShow)
         {

            hr = tasklist->AddTab(get_hwnd());

            TRACE("result = %d", hr);

         }
         else
         {

            tasklist->DeleteTab(get_hwnd());

         }

      }

   }


   /*bool window::set_window_pos(class ::user::zorder zorder, i32 x, i32 y, i32 cx, i32 cy, ::u32 nFlags)
   {

      if (!::user::window::set_window_pos(zorder, x, y, cx, cy, nFlags))
      {

         return false;

      }

      if (!(GetExStyle() & WS_EX_LAYERED))
      {


         if (!::set_window_pos(get_hwnd(), zorder, x, y, cx, cy, nFlags))
         {

            return false;

         }

      }

      return true;

   }
*/


   //void window::window_show_change_visibility(::e_display edisplay, ::e_activation eactivation)
   //{

   //   ::user::window::window_show_change_visibility(edisplay, eactivation);

   //}


   void window::_001OnActivate(::message::message * pmessage)
   {

      BOOL enabled = false;

      DwmIsCompositionEnabled(&enabled);

      //data->composition_enabled = enabled;

      if (enabled)
      {

         /* The u needs a frame to show a shadow, so give it the smallest amount of frame possible */

         MARGINS m = { 0,0,0,0 };

         ::u32  dw = DWMNCRP_ENABLED;

         HWND hwnd = get_hwnd();

         DwmExtendFrameIntoClientArea(hwnd, &m);

         DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &dw, sizeof(::u32));

      }
      else
      {

      }

   }


   void window::_001OnDwmNcRenderingChanged(::message::message * pmessage)
   {

      //#ifdef WINDOWS_DESKTOP

      __pointer(::message::base) pbase(pmessage);

      wparam wparam;

      lparam lparam;

      wparam = pbase->m_wparam;

      lparam = pbase->m_lparam;

      //return Default();

      BOOL enabled = false;

      DwmIsCompositionEnabled(&enabled);

      //data->composition_enabled = enabled;

      if (enabled)
      {

         /* The u needs a frame to show a shadow, so give it the smallest amount of frame possible */

         MARGINS m = { 0,0,0,0 };

         ::u32  dw = DWMNCRP_ENABLED;

         HWND hwnd = get_hwnd();

         DwmExtendFrameIntoClientArea(hwnd, &m);

         DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &dw, sizeof(::u32));

      }
      else
      {

      }

      //update_region(data);
      //      long dwEx = ::GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE);
      //      dwEx &= ~(WS_EX_LAYERED);
      //      ::SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,dwEx);
      //      ::InvalidateRect(GetSafeHwnd(),nullptr,true);
      //      ::UpdateWindow(GetSafeHwnd());
      //      //MoveAnchorsImmediatelly(hwndDlg);
      pbase->m_bRet = true;

      pbase->m_lresult = 0;

      //#endif

   }


   void window::_001OnNcCalcSize(::message::message * pmessage)
   {

      __pointer(::message::nc_calc_size) pcalcsize(pmessage);

      BOOL bCalcValidRects = pcalcsize->GetCalcValidRects();

      NCCALCSIZE_PARAMS * pncsp = (NCCALCSIZE_PARAMS *) pcalcsize->m_pNCCALCSIZE_PARAMS;

      // TODO: Add your message handler code here and/or call default
      //if(bCalcValidRects)
      //{
      //   TRACE("1");
      //   pncsp->rgrc[0].left = lpncsp->lppos->x + 1;

      //   pncsp->rgrc[0].right = lpncsp->lppos->x + lpncsp->lppos->cx - 1;

      //   pncsp->rgrc[0].top = lpncsp->lppos->y + 32;

      //   pncsp->rgrc[0].bottom = lpncsp->lppos->y + lpncsp->lppos->cy - 1;

      //}
      //else
      //{
      //   CRect * prectangle = (CRect *) pncsp;

      //   prectangle->top += 32;
      //   prectangle->left++;
      //   prectangle->bottom--;
      //   prectangle->right--;

      //   TRACE("2");
      //}
      const rectangle_i32 & nonclient = pncsp->rgrc[0];

      //CMiniFrameWnd::OnNcCalcSize(bCalcValidRects, pncsp);

      const rectangle_i32 & client = pncsp->rgrc[0];

      if (is_zoomed())
      {

         WINDOWINFO wi = {};
         wi.cbSize = sizeof(wi);

         HWND hwnd = get_hwnd();

         ::GetWindowInfo(hwnd, &wi);

         /* Maximized windows always have a non-client border that hangs over
         the edge of the screen, so the size_i32 proposed by e_message_nccalcsize is
         fine. Just adjust the top border to remove the u title. */
         pncsp->rgrc[0].left = client.left;

         pncsp->rgrc[0].top = nonclient.top + wi.cyWindowBorders;

         pncsp->rgrc[0].right = client.right;

         pncsp->rgrc[0].bottom = client.bottom;


         HMONITOR mon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY);
         MONITORINFO mi = {};
         mi.cbSize = sizeof(mi);
         GetMonitorInfoW(mon, &mi);

         /* If the client rectangle_i32 is the same as the monitor's rectangle,
         the shell assumes that the u has gone fullscreen, so it removes
         the topmost attribute from any auto-hide appbars, making them
         inaccessible. To avoid this, reduce the size_i32 of the client area by
         one pixel on a certain edge. The edge is chosen based on which side
         of the monitor is likely to contain an auto-hide appbar, so the
         missing client area is covered by it. */
         if (EqualRect(&pncsp->rgrc[0], &mi.rcMonitor))

         {
            if (has_autohide_appbar(ABE_BOTTOM, mi.rcMonitor))
               pncsp->rgrc[0].bottom--;

            else if (has_autohide_appbar(ABE_LEFT, mi.rcMonitor))
               pncsp->rgrc[0].left++;

            else if (has_autohide_appbar(ABE_TOP, mi.rcMonitor))
               pncsp->rgrc[0].top++;

            else if (has_autohide_appbar(ABE_RIGHT, mi.rcMonitor))
               pncsp->rgrc[0].right--;

         }
      }
      else
      {
         /* For the non-maximized case, set the output const rectangle_i32 & to what it was
         before e_message_nccalcsize modified it. This will make the client size_i32 the
         same as the non-client size. */
         __copy(pncsp->rgrc[0],nonclient);


      }


      //__pointer(::message::base) pbase(pmessage);

      pcalcsize->m_lresult = 0;

      pcalcsize->m_bRet = true;

      //#endif

   }



   //void window::default_message_handler(::message::base * pbase)
   //{

   //   if (get_hwnd() == nullptr)
   //   {

   //      return;

   //   }

   //   WNDPROC pfnWndProc = *GetSuperWndProcAddr();

   //   lresult lresult = 0;

   //   if (pfnWndProc == nullptr)
   //   {

   //      lresult = ::DefWindowProcW(m_oswindow, (::u32)pmessage->m_id.i64(), pmessage->m_wparam, pmessage->m_lparam);

   //   }
   //   else
   //   {

   //      lresult = ::CallWindowProc(pfnWndProc, m_oswindow, (::u32)pmessage->m_id.i64(), pmessage->m_wparam, pmessage->m_lparam);

   //   }

   //   pmessage->m_lresult = lresult;

   //}


   ::e_status window::set_icon(::windowing::icon * picon)
   {

      HICON hiconSmall = nullptr;

      HICON hiconBig = nullptr;

      if (::is_set(picon))
      {

         hiconSmall = (HICON) picon->get_os_data(size_i32(16, 16));

         hiconBig = (HICON) picon->get_os_data(size_i32(16, 16));

      }

      SendMessage(get_hwnd(), (::u32)WM_SETICON, ICON_SMALL, 0);

      SendMessage(get_hwnd(), (::u32)WM_SETICON, ICON_BIG, 0);

      if (hiconSmall)
      {

         SendMessage(get_hwnd(), (::u32)WM_SETICON, ICON_SMALL, (lparam)hiconSmall);

      }

      if (hiconBig)
      {

         SendMessage(get_hwnd(), (::u32)WM_SETICON, ICON_BIG, (lparam)hiconBig);

      }

      return true;

   }


//   void window::message_handler(::message::base * pbase)
//   {
//
//      if (::is_set(puserinteraction))
//      {
//
//         puserinteraction->pre_translate_message(pbase);
//
//      }
//
//      if (pbase->m_bRet)
//      {
//
//         return;
//
//      }
//
//      ::u32 message;
//
//      message = pbase->m_id.umessage();
//
//      m_uiMessage = message;
//
//      m_wparam = pbase->m_wparam;
//
//      m_lparam = pbase->m_lparam;
//
//      if (message == WM_IME_SETCONTEXT)
//      {
//
//         if (m_wparam == 1)
//         {
//
//            m_lparam &= ~ISC_SHOWUICOMPOSITIONWINDOW;
//
//         }
//
//         pbase->m_lresult = ::DefWindowProcW(m_oswindow, m_uiMessage, m_wparam, m_lparam);
//
//         pbase->m_bRet = true;
//
//         return;
//
//      }
//
//      bool bUserElementalOk = !m_bDestroyImplOnly && puserinteraction && puserinteraction->m_bUserPrimitiveOk;
//
//      if (message == e_message_key_down ||
//         message == e_message_key_up ||
//         message == e_message_char ||
//         message == e_message_sys_key_down ||
//         message == e_message_sys_key_up ||
//         message == e_message_sys_char ||
//         message == WM_IME_KEYDOWN ||
//         message == WM_IME_SETCONTEXT ||
//         message == WM_IME_SELECT ||
//         message == WM_IME_KEYUP ||
//         message == WM_IME_CHAR ||
//         message == WM_IME_STARTCOMPOSITION ||
//         message == WM_IME_COMPOSITION ||
//         message == WM_IME_COMPOSITIONFULL ||
//         message == WM_IME_NOTIFY ||
//         message == WM_IME_ENDCOMPOSITION)
//      {
//
//         __pointer(::message::key) pkey(pbase);
//
//         if (message == e_message_key_down)
//         {
//
//            output_debug_string("\n Key Down Event ");
//
//         }
//
//         auto psession = Session;
//
//         if (message == e_message_key_down || message == e_message_sys_key_down)
//         {
//            try
//            {
//               psession->set_key_pressed(pkey->m_ekey, true);
//            }
//            catch (...)
//            {
//            }
//         }
//         else if (message == e_message_key_up || message == e_message_sys_key_up)
//         {
//
//            try
//            {
//
//               psession->set_key_pressed(pkey->m_ekey, false);
//
//            }
//            catch (...)
//            {
//
//            }
//
//         }
//
//      }
//
//      if (message == e_message_timer)
//      {
//         //         puserinteraction->get_context_application()->step_timer();
//      }
//      else if (message == e_message_left_button_down)
//      {
//         ::rectangle_i32 rectClient;
//         ::GetClientRect(get_hwnd(), rectClient);
//         ::rectangle_i32 rectWindow;
//         ::GetWindowRect(get_hwnd(), rectWindow);
//         ::rectangle_i32 rectRegion;
//         HRGN hrgn = CreateRectRgn(0, 0, 0, 0);
//         int regionType = ::GetWindowRgn(get_hwnd(), hrgn);
//         if (regionType != ERROR)
//         {
//            ::GetRgnBox(hrgn, rectRegion);
//         }
//         ::DeleteObject(hrgn); /* finished with region */
//         WINDOWPLACEMENT wp;
//         __zero(wp);
//         wp.length = sizeof(WINDOWPLACEMENT);
//         ::GetWindowPlacement(get_hwnd(), &wp);
//         bool bZoomed = ::IsZoomed(get_hwnd()) != false;
//         bool bIconic = ::IsIconic(get_hwnd()) != false;
//      }
//      else if (message == e_message_set_cursor
//         || message == e_message_non_client_mouse_move)
//      {
//         //output_debug_string(".");
//      }
//      else
//      {
//
//#ifdef __DEBUG
//
//         if (0)
//         {
//            switch (message)
//            {
//            case e_message_create:
//               TRACE("e_message_create wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_window_position_changing:
//               TRACE("e_message_window_position_changing wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_window_position_changed:
//               TRACE("e_message_window_position_changed wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_activate:
//               TRACE("e_message_activate wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case WM_ACTIVATEAPP:
//               TRACE("WM_ACTIVATEAPP wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_mouse_activate:
//               TRACE("e_message_mouse_activate wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_ncactivate:
//               TRACE("e_message_ncactivate wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_set_focus:
//               TRACE("e_message_set_focus wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_kill_focus:
//               TRACE("e_message_kill_focus wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_move:
//               TRACE("e_message_move wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            case e_message_size:
//               TRACE("e_message_size wparam=%08x lparam=%08x", pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            default:
//               TRACE("MESSAGE %08x wparam=%08x lparam=%08x", message, pbase->m_wparam, pbase->m_lparam);
//
//               break;
//            }
//
//         }
//
//#endif //__DEBUG
//
//      }
//
//      //auto psession = Session;
//
//
//
//      /*      else if(message == CA2M_BERGEDGE)
//      {
//      if(pbase->m_wparam == BERGEDGE_GETAPP)
//      {
//      __pointer(::aura::application)* ppapp= (__pointer(::aura::application)*) pbase->m_lparam;
//      *ppapp = get_context_application();
//      pbase->m_bRet = true;
//      return;
//      }
//      }*/
//      //pbase->set_lresult(0);
//
//      if (message == e_message_mouse_leave)
//      {
//
//         auto papexsession = get_context_session();
//
//         ::aura::session * psession = nullptr;
//
//         if (papexsession)
//         {
//
//            psession = papexsession->m_paurasession;
//
//         }
//
//         if (psession && psession->m_puiCapture)
//         {
//
//            psession->m_puiCapture->_000OnMouseLeave(pbase);
//
//         }
//         else if (puserinteraction)
//         {
//
//            puserinteraction->_000OnMouseLeave(pbase);
//
//         }
//
//         return;
//
//      }
//
//      if (message == e_message_left_button_down ||
//         message == e_message_left_button_up ||
//         message == e_message_middle_button_down ||
//         message == e_message_middle_button_up ||
//         message == e_message_right_button_down ||
//         message == e_message_right_button_up ||
//         message == e_message_left_button_double_click ||
//         message == e_message_mouse_move ||
//         message == e_message_non_client_mouse_move ||
//         message == e_message_mouse_wheel)
//      {
//
//         message::mouse * pmouse = dynamic_cast <::message::mouse *> (pbase);
//
//         if (message >= WM_MOUSEFIRST
//            && message <= WM_MOUSELAST
//            && m_bTranslateMouseMessageCursor
//            && !pmouse->m_bTranslated)
//         {
//
//            pmouse->m_bTranslated = true;
//
//            ::ClientToScreen(get_hwnd(), &pmouse->m_point);
//
//         }
//
//         if (message == e_message_left_button_down)
//         {
//
//            TRACE("e_message_left_button_down");
//
//            string strType = ::str::demangle(puserinteraction->type_name());
//
//            if (strType.contains_ci("combo_list"))
//            {
//
//               ::output_debug_string("combo_list e_message_left_button_down");
//
//            }
//
//         }
//         else if (message == e_message_left_button_up)
//         {
//
//            TRACE("e_message_left_button_up");
//
//         }
//         else if (message == e_message_non_client_left_button_up)
//         {
//
//            TRACE("e_message_non_client_left_button_up");
//
//         }
//         else if (message == e_message_non_client_left_button_down)
//         {
//
//            TRACE("e_message_non_client_left_button_down");
//
//            string strType;
//
//            if (strType.contains_ci("combo_list"))
//            {
//
//               ::output_debug_string("combo_list e_message_non_client_left_button_down");
//
//            }
//
//         }
//
//         auto papexsession = get_context_session();
//
//         ::aura::session * psession = nullptr;
//
//         if (papexsession)
//         {
//
//            psession = papexsession->m_paurasession;
//
//         }
//
//         if (psession)
//         {
//
//            psession->on_ui_mouse_message(pmouse);
//
//         }
//
//         if (message == e_message_mouse_move)
//         {
//            // We are at the message handler procedure.
//            // mouse messages originated from message handler and that are mouse move events should end up with the correct cursor.
//            // So the procedure starts by setting to the default cursor,
//            // what forces, at the end of message processing, setting the bergedge cursor to the default cursor, if no other
//            // handler has set it to another one.
//            pmouse->m_ecursor = cursor_default;
//
//            //INFO("windows::e_message_mouse_move(%d,%d)", pmouse->m_point.x, pmouse->m_point.y);
//
//            string strType;
//
//            if (puserinteraction)
//            {
//
//               strType = ::str::demangle(puserinteraction->type_name());
//
//               if (strType.contains_ci("combo_list"))
//               {
//
//                  //::output_debug_string("combo_list e_message_mouse_move");
//
//               }
//
//            }
//
//         }
//         else if (message == e_message_non_client_mouse_move)
//         {
//            // We are at the message handler procedure.
//            // mouse messages originated from message handler and that are mouse move events should end up with the correct cursor.
//            // So the procedure starts by setting to the default cursor,
//            // what forces, at the end of message processing, setting the bergedge cursor to the default cursor, if no other
//            // handler has set it to another one.
//            pmouse->m_ecursor = cursor_default;
//         }
//
//         if (psession)
//         {
//
//            auto puiCapture = psession->m_puiCapture;
//
//            if (::is_set(puiCapture))
//            {
//
//               puiCapture->_000OnMouse(pmouse);
//
//               return;
//
//            }
//
//         }
//
//         _008OnMouse(pmouse);
//
//         return;
//
//      }
//
//      if (message == MESSAGE_OLE_DRAGENTER ||
//         message == MESSAGE_OLE_DRAGOVER ||
//         message == MESSAGE_OLE_DRAGLEAVE ||
//         message == MESSAGE_OLE_DRAGDROP)
//      {
//
//         message::drag_and_drop * pdrag = (::message::drag_and_drop *)pbase;
//
//         auto puserinteraction = pdrag->userinteraction();
//
//         //hwnd_array hwnda;
//         //user::interaction_pointer_array wnda;
//         //wnda = *System.m_puiptraFrame;
//         //hwnda = wnda.get_hwnda();
//         //user::window_util::SortByZOrder(hwnda);
//         //for (i32 i = 0; i < hwnda.get_size(); i++)
//         //{
//         //   __pointer(::user::interaction) puserinteraction = wnda.find_first(hwnda[i]);
//         //   if (puserinteraction != nullptr)
//         //   {
//
//         if (puserinteraction)
//         {
//            puserinteraction->_000OnDrag(pdrag);
//            if (pdrag->m_bRet)
//               return;
//
//         }
//         /*        }
//              }*/
//         return;
//      }
//      if (message == e_message_key_down ||
//         message == e_message_key_up ||
//         message == e_message_char ||
//         message == e_message_sys_key_down ||
//         message == e_message_sys_key_up ||
//         message == e_message_sys_char ||
//         message == WM_IME_KEYDOWN ||
//         message == WM_IME_KEYUP ||
//         message == WM_IME_CHAR ||
//         message == WM_IME_SELECT ||
//         message == WM_IME_SETCONTEXT ||
//         message == WM_IME_STARTCOMPOSITION ||
//         message == WM_IME_COMPOSITION ||
//         message == WM_IME_COMPOSITIONFULL ||
//         message == WM_IME_NOTIFY ||
//         message == WM_IME_ENDCOMPOSITION ||
//         message == WM_INPUTLANGCHANGE)
//      {
//
//
//         message::key * pkey = (::message::key *)pbase;
//
//         __pointer(::user::interaction) puiFocus;
//
//         auto papexsession = get_context_session();
//
//         ::aura::session * psession = nullptr;
//
//         if (papexsession)
//         {
//
//            psession = papexsession->m_paurasession;
//
//         }
//
//         if (psession)
//         {
//
//            puiFocus = puserinteraction->get_keyboard_focus();
//
//         }
//
//         if (puiFocus && puiFocus->IsWindow() && puiFocus != puserinteraction)
//         {
//
//            puiFocus->send(pkey);
//
//            if (pbase->m_bRet)
//            {
//
//               return;
//
//            }
//
//         }
//         else if (!pkey->m_bRet)
//         {
//
//            if (puserinteraction != nullptr)
//            {
//
//               puserinteraction->_000OnKey(pkey);
//
//               if (pbase->m_bRet)
//               {
//
//                  return;
//
//               }
//
//            }
//
//         }
//
//         //m_wparam-
//
//         //m_lparam = pbase->m_lparam;
//
//         //pbase->set_lresult(::default_window_procedure(message, pbase->m_wparam, pbase->m_lparam));
//
//         //return;
//
//      }
//
//      if (message == e_message_event)
//      {
//
//         puserinteraction->on_control_event(pbase);
//
//         return;
//
//      }
//
//      ::user::window::message_handler(pbase);
//
//      //if(pmessage->m_bRet && !pbase->m_bDoSystemDefault)
//
//      if (pbase->m_bRet)
//      {
//
//         return;
//
//      }
//
//      if (message == WM_IME_STARTCOMPOSITION ||
//         message == WM_IME_COMPOSITION ||
//         message == WM_IME_COMPOSITIONFULL ||
//         message == WM_IME_NOTIFY ||
//         message == WM_IME_ENDCOMPOSITION ||
//         message == WM_IME_SELECT ||
//         message == WM_IME_SETCONTEXT)
//      {
//
//         //return;
//
//      }
//
//      //if (bUserElementalOk && pbase->m_bWindowProc)
//      //{
//      //   
//      //   if (puserinteraction != nullptr)
//      //   {
//      //      
//      //      puserinteraction->default_window_procedure(pbase);
//
//      //   }
//      //   else
//      //   {
//      //      
//      //      pbase->set_lresult(::DefWindowProcW(m_oswindow, pbase->m_id, pbase->m_wparam, pbase->m_lparam));
//
//      //   }
//
//      //}
//
//   }


   bool window::get_window_rect(RECTANGLE_I32 * prectangle)
   {

      RECT rect;

      if (!::GetWindowRect(get_hwnd(), &rect))
      {

         return false;

      }

      __copy(prectangle, rect);

      return true;

   }


   WNDPROC* window::GetSuperWndProcAddr()
   {

      // Note: it is no longer necessary to override GetSuperWndProcAddr
      //  for each control class with a different WNDCLASS.
      //  This implementation now uses instance data, such that the previous
      //  WNDPROC can be anything.

      return &m_pfnSuper;

   }


   bool window::get_client_rect(RECTANGLE_I32 * prectangle)
   {

      RECT rect;

      if (!::GetClientRect(get_hwnd(), &rect))
      {

         return false;

      }

      __copy(prectangle, rect);

      return true;

   }



   void window::non_top_most_upper_window_rects(::rect_array & recta)
   {

      /// from top to bottom
      top_level_enum toplevelenum;

      auto phwnda = toplevelenum.m_phwnda;

      HWND hwnd = get_hwnd();

      index iFind = phwnda->find_first(hwnd);

      if (iFind < 0)
      {

         return;

      }

      ::rectangle_i32 rHigher;

      for (iFind--; iFind >= 0; iFind--)
      {

         hwnd = phwnda->element_at(iFind);

         int iExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);

         if ((iExStyle & WS_EX_TOPMOST) == WS_EX_TOPMOST)
         {

            continue;

         }

         if (::GetWindowRect(hwnd, (RECT *) &rHigher))
         {

            recta.add(rHigher);

         }

      }

   }


   ::e_status window::finish(::context_object * pcontextobjectFinish)
   {

      auto estatus = set_finish(pcontextobjectFinish);

      if (estatus == success)
      {

         on_finish();

      }

      return estatus;

   }




   ::e_status window::set_tool_window(bool bSet)
   {

      if (bSet)
      {

         modify_ex_style(0, WS_EX_TOOLWINDOW, SWP_FRAMECHANGED);

      }
      else
      {

         modify_ex_style(WS_EX_TOOLWINDOW, 0, SWP_FRAMECHANGED);

      }

      return ::success;

   }


   //void window::_task_transparent_mouse_event()
   //{

   //   auto ptask = ::get_task();

   //   ::point_i32 pointCursor;

   //   auto pimpl = m_pimpl.cast < window>();

   //   HWND hwnd = get_safe_handle();

   //   while (ptask->thread_get_run())
   //   {

   //      ::GetCursorPos((POINT *)&pointCursor);

   //      if (pimpl->m_pointCursor == pointCursor)
   //      {

   //         sleep(30_ms);

   //         continue;

   //      }

   //      pimpl->m_pointCursor = pointCursor;

   //      lparam lparam;

   //      ::ScreenToClient(hwnd, (POINT *)&pointCursor);

   //      RECTANGLE_I32 rectClient;

   //      ::GetClientRect(hwnd, (RECT *)&rectClient);

   //      if (!PtInRect(&rectClient, (POINT *)&pointCursor))
   //      {

   //         sleep(100_ms);

   //         continue;

   //      }

   //      lparam = MAKELPARAM(pointCursor.x, pointCursor.y);

   //      output_debug_string("transparent_mouse_event:x=" + __str(pointCursor.x) + ",y=" + __str(pointCursor.y) + "\n");

   //      pimpl->call_message_handler(e_message_mouse_move, 0, lparam);

   //   }

   //}



} // namespace windows


bool is_registered_windows_message(::u32 message)
{

   return message >= 0xc000 && message <= 0xffff;

}


lresult CALLBACK WndProc(HWND hwnd, ::u32 message, wparam wparam, lparam lparam);


int g_iCol = 0;


lresult CALLBACK __window_procedure(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{

   ::windowing::window * pwindow = (::windowing::window *) ::GetWindowLongPtr(hwnd, 0);

   if (!pwindow)
   {

      if (message == WM_NCCREATE)
      {

         LPCREATESTRUCTW pcreatestructw = (LPCREATESTRUCTW)lparam;

         ::user::system * psystem = (::user::system *)pcreatestructw->lpCreateParams;

         pwindow = psystem->m_pwindow;

         if (!pwindow)
         {

            return 0;


         }
          
         ::SetWindowLongPtr(hwnd, 0, (LONG_PTR) pwindow);

         pwindow->set_oswindow(__oswindow(hwnd));

      }
      else
      {

         return 0;

      }

   }

   lresult lresult = 0;

   if (is_registered_windows_message(message))
   {

      lresult = ::DefWindowProcW(hwnd, message, wparam, lparam);

      return lresult;

   }

   auto pimpl = pwindow->m_pimpl;

   if (pimpl)
   {

      if (pimpl->__windows_message_bypass(pwindow, message, wparam, lparam, lresult))
      {

         return lresult;

      }

   }

   //return ::DefWindowProcW(hwnd, message, wparam, lparam);

   if (::get_context_system() == nullptr)
   {

      return 0;

   }

   if (&System == nullptr)
   {

      return 0;

   }

   pimpl->m_uiMessage = message;

   pimpl->m_wparam = wparam;

   pimpl->m_lparam = lparam;

   auto puserinteraction = pimpl->m_puserinteraction;

   if (message == e_message_activate)
   {

      output_debug_string("test");

   }

   if (message == e_message_left_button_up)
   {

      ::output_debug_string("e_message_left_button_up");

   }

   if (message == e_message_mouse_move)
   {

      if (lparam == pimpl->m_lparamLastMouseMove)
      {

         return 0;

      }

      pimpl->m_lparamLastMouseMove = lparam;

      ::point_i32 pointCursor;

      ::GetCursorPos((POINT *) &pointCursor);

      if (pimpl->m_pointCursor == pointCursor)
      {

         return 0;

      }

      pimpl->m_pointCursor = pointCursor;

   }
   else if (message == e_message_timer)
   {

      if (wparam == e_timer_transparent_mouse_event)
      {

         ::point_i32 pointCursor;

         ::GetCursorPos((POINT *) &pointCursor);

         if (pimpl->m_pointCursor == pointCursor)
         {

            return 0;

         }

         pimpl->m_pointCursor = pointCursor;

         ::lparam lparam;

         ::ScreenToClient(hwnd, (POINT *) &pointCursor);

         lparam = MAKELPARAM(pointCursor.x, pointCursor.y);

         pimpl->call_message_handler(e_message_mouse_move, 0, lparam);

      }
      else
      {

         // ignoring Timer Event

         output_debug_string("iTE\n");

      }

      return 0;

   }

   if (pimpl->m_bDestroyImplOnly || ::is_null(puserinteraction))
   {

      auto pbase = pimpl->get_message_base((enum_message)message, wparam, lparam);

      try
      {

         pimpl->message_handler(pbase);

      }
      catch (...)
      {

      }

      if (!pbase->m_bRet)
      {

         pimpl->default_message_handler(pbase);

      }

      lresult = pbase->m_lresult;

   }
   else if (::is_set(puserinteraction))
   {

      if (message == WM_GETTEXT)
      {

         return ::DefWindowProcW(hwnd, message, wparam, lparam);

      }
      else if (message == WM_GETTEXTLENGTH)
      {

         return ::DefWindowProcW(hwnd, message, wparam, lparam);

      }
      else if (message == WM_SETTEXT)
      {

         return ::DefWindowProcW(hwnd, message, wparam, lparam);

      }

      auto pbase = puserinteraction->get_message_base((enum_message)message, wparam, lparam);

      try
      {

         puserinteraction->message_handler(pbase);

      }
      catch (...)
      {

      }

      if (!pbase->m_bRet)
      {

         puserinteraction->default_message_handler(pbase);

      }

      lresult = pbase->m_lresult;

   }
   else
   {

      lresult = ::DefWindowProcW(hwnd, message, wparam, lparam);

   }

   return lresult;

}






//CLASS_DECL_WINDOWING_WIN32 bool hook_window_create(::windowing_win32::window * pwindow)
//{
//
//   if (pwindow == nullptr)
//   {
//
//      return false;
//
//   }
//
//   if (pwindow->get_hwnd() != nullptr)
//   {
//
//      return false;
//
//   }
//
//   if (t_hHookOldCbtFilter == nullptr)
//   {
//
//      t_hHookOldCbtFilter = ::SetWindowsHookExW(WH_CBT, windows::__cbt_filter_hook, nullptr, ::GetCurrentThreadId());
//
//      if (t_hHookOldCbtFilter == nullptr)
//      {
//
//         return false;
//
//      }
//
//   }
//
//   if (t_hHookOldCbtFilter == nullptr)
//   {
//
//      return false;
//
//   }
//
//   thread_set("wnd_init") = pwindow;
//
//   if (thread_set("wnd_init") == nullptr)   // hook not already in progress
//   {
//
//      return false;
//
//   }
//
//   if (thread_set("wnd_init") != pwindow)
//   {
//
//      return false;
//
//   }
//
//   return true;
//
//}
//
//
//CLASS_DECL_WINDOWING_WIN32 bool unhook_window_create()
//{
//
//   if (thread_set("wnd_init") != nullptr)
//   {
//
//      thread_set("wnd_init") = nullptr;
//
//      return false;   // was not successfully hooked
//
//   }
//
//   return true;
//
//}
//

//void CLASS_DECL_WINDOWING_WIN32 _handle_activate(::windowing::window * pwindow, wparam nState, ::windowing::window * pWndOther)
//{
//
//   ASSERT(pwindow);
//
//   // send WM_ACTIVATETOPLEVEL when top-level parents change
//   if (!(pwindow->get_style() & WS_CHILD))
//   {
//
//      __pointer(::user::interaction) pTopLevel = pwindow->get_top_level();
//
//      if (pTopLevel && (!pWndOther || !::IsWindow((pWndOther)->get_hwnd()) || pTopLevel != (pWndOther)->get_top_level()))
//      {
//         // lParam points to window getting the e_message_activate message and
//         //  oswindow_Other from the e_message_activate.
//         hwnd oswindow_2[2];
//         oswindow_2[0] = (pwindow)->get_hwnd();
//         if (!pWndOther)
//         {
//            oswindow_2[1] = nullptr;
//         }
//         else
//         {
//
//            oswindow_2[1] = (pWndOther)->get_hwnd();
//
//         }
//
//         // send it...
//         pTopLevel->send_message(WM_ACTIVATETOPLEVEL, nState, (lparam)&oswindow_2[0]);
//
//      }
//
//   }
//
//}


void __term_windowing()
{

   //if (t_hHookOldCbtFilter != nullptr)
   //{

   //   ::UnhookWindowsHookEx(t_hHookOldCbtFilter);

   //   t_hHookOldCbtFilter = nullptr;

   //}

}


CLASS_DECL_WINDOWING_WIN32 bool windows_register_class(WNDCLASSEXW * pwndclass);





wstring windows_calc_icon_window_class(::user::interaction * puserinteraction, u32 dwDefaultStyle, const char * pszMatter)
{

   string strPath = Ctx(puserinteraction).dir().matter(pszMatter, "icon.ico");

   hicon hIcon = (hicon) ::LoadImageW(nullptr, wstring(Ctx(puserinteraction).get_matter_path(strPath)), IMAGE_ICON, 256, 256, LR_LOADFROMFILE);

   wstring strClass = windows_get_user_interaction_window_class(puserinteraction);

   if (hIcon != nullptr)
   {

      // will fill pszClassName with default WNDCLASS name

      // ignore instance handle from pre_create_window.

      WNDCLASSEXW wndcls;

      if (strClass.get_length() > 0 && GetClassInfoExW((HINSTANCE) System.m_hinstance, strClass, &wndcls) && wndcls.hIcon != hIcon)
      {

         // register a very similar WNDCLASS

         return windows_register_window_class(get_context_application(), wndcls.style, wndcls.hCursor, wndcls.hbrBackground, hIcon);

      }

   }

   return strClass;

}




wstring CLASS_DECL_WINDOWING_WIN32 windows_get_user_interaction_window_class(::user::interaction * puserinteraction)
{

   ::user::interaction::enum_type etype = puserinteraction->get_window_type();

   WNDCLASSEXW wndcls;

   __memset(&wndcls, 0, sizeof(WNDCLASSEXW));   // start with nullptr defaults

   wndcls.lpfnWndProc = windows_user_interaction_impl_get_window_procedure();

   wndcls.hInstance = GetModuleHandleW(L"windowing_win32.dll");

   //INITCOMMONCONTROLSEX init;
   //init.dwSize = sizeof(init);

   if (etype == ::user::interaction::type_frame || etype == ::user::interaction::type_view)
   {
      
      // SDI Frame or MDI Child windows or views - normal colors
      
      wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
      
      //wndcls.style = CS_HREDRAW | CS_VREDRAW;
      
      wndcls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

      if (windows_register_with_icon(&wndcls, L"ca2_frame", 0))
      {

         return wndcls.lpszClassName;

      }

   }


   return windows_register_window_class(puserinteraction->get_context_application(), 0);

}



bool CLASS_DECL_WINDOWING_WIN32 windows_register_with_icon(WNDCLASSEXW * pwndclass, const unichar * pszClassName, ::u32 nIDIcon)
{

   pwndclass->lpszClassName = pszClassName;

   pwndclass->hIcon = ::LoadIconW(nullptr, MAKEINTRESOURCEW(32512));

   return windows_register_class(pwndclass);

}


//CLASS_DECL_WINDOWING_WIN32 WNDPROC get_window_procedure();


CLASS_DECL_WINDOWING_WIN32 wstring windows_register_window_class(::layered * pobjectContext, ::u32 nClassStyle, hcursor hCursor, HBRUSH hbrBackground, hicon hIcon)
{

   ::apex::application * papp = ::get_context_application(pobjectContext);

   const int iLen = 4096;

   wstring wstrClassName;

   HINSTANCE hinstance = ::GetModuleHandleW(L"windowing_win32.dll");



   {

      LPWSTR lpwsz = wstrClassName.get_string_buffer(iLen);

      if (hCursor == nullptr && hbrBackground == nullptr && hIcon == nullptr)
      {

         C_RUNTIME_ERRORCHECK_SPRINTF(_snwprintf_s(lpwsz, iLen, iLen - 1, L"windows_interaction_impl:%p,%x", hinstance, nClassStyle));

      }
      else
      {

         C_RUNTIME_ERRORCHECK_SPRINTF(_snwprintf_s(lpwsz, iLen, iLen - 1, L"windows_interaction_impl:%p,%x,%p,%p,%p", hinstance, nClassStyle, hCursor, hbrBackground, hIcon));

      }

      wstrClassName.release_string_buffer();

   }

   // see if the class already exists
   WNDCLASSEXW wndcls = {};

   if (::GetClassInfoExW((HINSTANCE)hinstance, wstrClassName, &wndcls))
   {
      // already registered, assert everything is good
      ASSERT(wndcls.style == nClassStyle);

      // NOTE: We have to trust that the hIcon, hbrBackground, and the
      //  hCursor are semantically the same, because sometimes Windows does
      //  some internal translation or copying of those handles before
      //  storing them in the internal WNDCLASS retrieved by GetClassInfo.
      return wstrClassName;

   }
   wndcls.cbSize = sizeof(wndcls);
   // otherwise we need to register a new class
   wndcls.style = nClassStyle;
   wndcls.lpfnWndProc = windows_user_interaction_impl_get_window_procedure();

   wndcls.cbClsExtra = wndcls.cbWndExtra = 40;
   wndcls.hInstance = hinstance;
   //wndcls.hIcon = hIcon;
   //wndcls.hCursor = hCursor;
   wndcls.hCursor = nullptr;
   wndcls.hbrBackground = hbrBackground;
   wndcls.lpszMenuName = nullptr;

   wndcls.lpszClassName = wstrClassName;

   if (!windows_register_class(&wndcls))
   {

      __throw(resource_exception());

   }

   // return thread-local pointer
   return wstrClassName;

}




lresult CALLBACK WndProc(HWND hWnd, const ::id & id, wparam wParam, lparam lParam);

//bool windows_register_class(HINSTANCE hinstance)
//{
//   
//   WNDCLASSEXW wcex = {};
//
//   wcex.cbSize = sizeof(WNDCLASSEXW);
//   wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
//   wcex.lpfnWndProc = windows_user_interaction_impl_get_window_procedure();
//   wcex.cbClsExtra = 0;
//   wcex.cbWndExtra = 0;
//   wcex.hInstance = hinstance;
//   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
//   wcex.lpszClassName = L"windows_interaction_impl";
//
//   if (!RegisterClassExW(&wcex))
//   {
//
//      return false;
//
//   }
//
//   return true;
//
//}
//

CLASS_DECL_WINDOWING_WIN32 bool windows_register_class(WNDCLASSEXW* pwndclass)
{

   WNDCLASSEXW wndcls;

   if (GetClassInfoExW(pwndclass->hInstance, pwndclass->lpszClassName, &wndcls))

   {

      return true;

   }

   pwndclass->cbSize = sizeof(WNDCLASSEXW);

   if (!::RegisterClassExW(pwndclass))
   {

      ::u32 dw = GetLastError();

      return false;

   }

   bool bRet = true;

   return bRet;

}


CLASS_DECL_WINDOWING_WIN32 WNDPROC windows_user_interaction_impl_get_window_procedure()
{

   return &__window_procedure;

}


