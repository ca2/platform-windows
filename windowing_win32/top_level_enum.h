#pragma once


namespace windowing_win32
{


   template < typename PRED >
   class top_level_boolean_pred
   {
   public:


      PRED     m_pred;
      HWND     m_hwnd;


      top_level_boolean_pred(PRED pred, HWND hwndDefault = nullptr) : m_pred(pred), m_hwnd(hwndDefault) {}


      bool found(HWND hwnd)
      {

         return m_pred(hwnd);

      }


#ifdef WINDOWS_DESKTOP

      static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
      {

         top_level_boolean_pred * ppred = (top_level_boolean_pred *)lParam;

         if (ppred->found(hwnd))
         {

            ppred->m_hwnd = hwnd;

            return false;

         }

         return true;

      }

#endif

   };


   template < typename PRED >
   bool top_level_contains_pred(PRED pred)
   {

      top_level_boolean_pred<PRED> boolean_pred(pred);

      EnumWindows(&top_level_boolean_pred<PRED>::EnumWindowsProc, (LPARAM)&boolean_pred);

      return boolean_pred.m_hwnd != nullptr;

   }


   string get_window_text_timeout(HWND hwnd, const ::duration & durationSendMessageMax)
   {

      DWORD_PTR dw = 0;

      //if (!SendMessageTimeoutW(hwnd, WM_GETTEXTLENGTH, 0, 0, SMTO_ABORTIFHUNG | SMTO_NOTIMEOUTIFNOTHUNG, 100, &dw))
      if (!SendMessageTimeoutW(hwnd, WM_GETTEXTLENGTH, 0, 0, SMTO_ABORTIFHUNG, __os(durationSendMessageMax), &dw))
      {

         return "";

      }

      if (!dw)
      {

         return "";

      }

      wstring wstr;

      auto pwsz = wstr.get_string_buffer(dw);

      if (!SendMessageTimeoutW(hwnd, WM_GETTEXT, dw + 1, (LPARAM)pwsz, SMTO_ABORTIFHUNG, __os(durationSendMessageMax), &dw))
      {

         return "";

      }

      wstr.release_string_buffer();

      return wstr;

   }


   bool top_level_contains_name(string str)
   {

      return top_level_contains_pred([str](HWND hwnd)
         {

            //PSEUDO-Code char sz[1024]; GetWindowTextA(sz,1024, oswindow); return !strcmp(sz, str.c_str());

            string strWindowText = get_window_text_timeout(hwnd, 50_ms);

            return strWindowText.contains_ci(str);

         });

   }


   bool visible_top_level_contains_name(string str)
   {

      return top_level_contains_pred([str](HWND hwnd)
         {

            //PSEUDO-Code char sz[1024]; GetWindowTextA(sz,1024, oswindow); return !strcmp(sz, str.c_str());

            if (!::IsWindowVisible(hwnd))
            {

               return false;

            }

            string strWindowText = get_window_text_timeout(hwnd, 50_ms);

            return strWindowText.contains_ci(str);

         });

   }

   bool visible_top_level_contains_all_names(string_array & stra)
   {

      return top_level_contains_pred([&stra](HWND hwnd)
         {

            //PSEUDO-Code char sz[1024]; GetWindowTextA(sz,1024, oswindow); return !strcmp(sz, str.c_str());

            if (!::IsWindowVisible(hwnd))
            {

               return false;

            }

            string strWindowText = get_window_text_timeout(hwnd, 50_ms);

            for (auto & str : stra)
            {

               if (!strWindowText.contains_ci(str))
               {

                  return false;

               }

            }

            return true;

         });

   }



   /// from top to bottom
   class CLASS_DECL_WINDOWING_WIN32 top_level_enum
   {
   public:


      __pointer(hwnd_array)         m_phwnda;
      bool                          m_bVisible;


      top_level_enum(bool bDestkop = true, bool bVisible = true);
      ~top_level_enum();


      /// from top to bottom
      static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);


   };


} // namespace windowing_win32


