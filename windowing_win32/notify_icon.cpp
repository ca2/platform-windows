#include "framework.h"
#include "aura/user/_user.h"


static ::user::notify_icon * g_pnotifyiconLast = nullptr;


#ifdef LINUX

#include "aura/os/linux/appindicator.h"

#endif


namespace windowing_win32
{


   notify_icon::notify_icon()
   {

      g_pnotifyiconLast = this;

#ifdef WINDOWS_DESKTOP

      m_nid.cbSize = sizeof(m_nid);

#elif defined(LINUX)

      m_pindicator = nullptr;

#endif

      m_bCreated = false;

   }


   notify_icon::~notify_icon()
   {

      destroy_window();

      if (g_pnotifyiconLast == this)
      {

         g_pnotifyiconLast = nullptr;

      }

   }


   void notify_icon::install_message_routing(::channel * pchannel)
   {

#ifdef WINDOWS_DESKTOP

      ::user::interaction::install_message_routing(pchannel);

      MESSAGE_LINK(MessageNotifyIcon, pchannel, this, &notify_icon::_001OnNotifyIconMessage);
      MESSAGE_LINK(e_message_destroy, pchannel, this, &notify_icon::_001OnDestroy);

#endif

   }


   ::e_status notify_icon::create_notify_icon(::u32 uId, ::user::notify_icon_listener * plistener, ::windowing::icon * picon)
   {

      if (m_bCreated)
      {

         return ::success_none;

      }

      m_strId.Format("notify_icon_%d", uId);

      m_strId = "ca2-" + picon->get_tray_icon_name() + "-" + m_strId;

      if (!create_message_queue(m_strId))
      {

         return false;

      }

      m_uiId = uId;

      m_nid.hWnd = __hwnd(get_oswindow());
      m_nid.uID = uId;
      m_nid.hIcon = (HICON) picon->get_os_data(::size_i32(16, 16));
      m_nid.uFlags = NIF_ICON | NIF_MESSAGE;
      m_nid.uCallbackMessage = MessageNotifyIcon;


      m_plistener = plistener;

      if (!Shell_NotifyIcon(NIM_ADD, &m_nid))
      {

         m_plistener = nullptr;

         start_destroying_window();

         return false;

      }

      m_bCreated = true;

      return true;

   }


   ::e_status notify_icon::modify_icon(::windowing::icon * picon)
   {

      if (!m_bCreated)
      {

         return error_failed;

      }

      m_nid.hIcon = (HICON) picon->get_os_data(::size_i32(16, 16));

      m_nid.uFlags = NIF_ICON;

      if (!Shell_NotifyIcon(NIM_MODIFY, &m_nid))
      {

         return false;

      }

      m_piconCurrent = picon;

      return ::success;

   }


   ::e_status notify_icon::add_hidden_window(::user::interaction * puserinteraction)
   {

      auto estatus = ::user::notify_icon::add_hidden_window(puserinteraction);

      if (!estatus)
      {

         return estatus;

      }

      return estatus;

   }


#ifdef WINDOWS_DESKTOP


   void notify_icon::destroy_window()
   {

      m_nid.uFlags = 0;

      if (!Shell_NotifyIcon(NIM_DELETE, &m_nid))
      {

         //return false;

      }

      ::user::interaction::destroy_window();

   }


   void notify_icon::erase_all_routes()
   {

      ::user::interaction::erase_all_routes();

   }


   void notify_icon::PostNcDestroy()
   {

      ::user::interaction::PostNcDestroy();


   }

#endif // defined(WINDOWS_DESKTOP)


   bool notify_icon::start_destroying_window()
   {

      if (!m_bCreated)
      {

         return false;

      }

      m_bCreated = false;

#ifdef WINDOWS_DESKTOP

      ::user::interaction::start_destroying_window();

      return true;


#elif defined(LINUX) && !defined(RASPBIAN)

      if (m_pindicator)
      {

         auto pnode = Node;

         pnode->appindicator_destroy(m_pindicator);

         m_pindicator = nullptr;

      }

      return true;

#elif defined(MACOS)

      notify_icon_destroy();

      return true;

#else


      __throw(todo);

      return true;

#endif

   }


   void notify_icon::_001OnDestroy(::message::message * pmessage)
   {

      output_debug_string("notify_icon::_001OnDestroy");

   }


   void notify_icon::_001OnNotifyIconMessage(::message::message * pmessage)
   {

      __pointer(::user::message) pusermessage(pmessage);

      if (pusermessage->m_lparam == e_message_left_button_down)
      {

         while (m_userinteractionaHidden.get_size() > 0)
         {

            try
            {

               __pointer(::user::interaction) pframe = (m_userinteractionaHidden.element_at(0));

               if (pframe != nullptr)
               {

                  pframe->display(e_display_normal);

               }
               else
               {

                  m_userinteractionaHidden.element_at(0)->display(e_display_normal);

               }

            }
            catch (...)
            {

            }

            m_userinteractionaHidden.erase_at(0);

         }

      }

      m_plistener->OnNotifyIconMessage(m_uiId, (::u32)pusermessage->m_lparam);

   }


   //void notify_icon::notify_icon_play(const char * action)
   //{


   //}


   ::e_status notify_icon::step()
   {

#if defined(LINUX)

      //      main_async([&]
      //      {
      //
      //         linux_g_direct_app_indicator_step(m_pindicator);
      //
      //      });

#endif

      return ::success;

   }


//#if defined(LINUX) || defined(MACOS)
//
//
//   int notify_icon::_get_notification_area_action_count()
//   {
//
//      return m_plistener->_get_notification_area_action_count();
//
//   }
//
//
//   const char * notify_icon::_get_notification_area_action_name(int iIndex)
//   {
//
//      return m_plistener->_get_notification_area_action_name(iIndex);
//
//   }
//
//
//   const char * notify_icon::_get_notification_area_action_id(int iIndex)
//   {
//
//      return m_plistener->_get_notification_area_action_id(iIndex);
//
//   }
//
//
//   const char * notify_icon::_get_notification_area_action_label(int iIndex)
//   {
//
//      return m_plistener->_get_notification_area_action_label(iIndex);
//
//   }
//
//
//   const char * notify_icon::_get_notification_area_action_accelerator(int iIndex)
//   {
//
//      return m_plistener->_get_notification_area_action_accelerator(iIndex);
//
//   }
//
//
//   const char * notify_icon::_get_notification_area_action_description(int iIndex)
//   {
//
//      return m_plistener->_get_notification_area_action_description(iIndex);
//
//   }
//
//
//   void notify_icon::call_notification_area_action(const char * pszId)
//   {
//
//      m_plistener->call_notification_area_action(pszId);
//
//   }
//
//
//#endif


} // namespace windowing_win32



