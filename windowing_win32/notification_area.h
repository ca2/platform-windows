#pragma once


namespace windowing_win32
{


   struct CLASS_DECL_WINDOWING_WIN32 TRAYDATA
   {

      HWND           hwnd;
      UINT           uID;
      UINT           uCallbackMessage;
      UINT           Reserved[2];
      HICON          hIcon;


   };


   struct CLASS_DECL_WINDOWING_WIN32 TrayItemInfo :
      virtual public object
   {

      HWND           m_hwnd;
      ::u32          m_uID;
      ::u32          m_uCallbackMessage;
      string         m_strTip;
      string         m_strProcessPath;
      bool           m_bVisible;


   };


   CLASS_DECL_WINDOWING_WIN32 char GetDriveLetter(const char * pDevicePath);

   CLASS_DECL_WINDOWING_WIN32 HWND find_tray_toolbar_window();


   class CLASS_DECL_WINDOWING_WIN32 notification_area :
      virtual public ::context_object
   {
   public:


      HWND                             m_hwndTray;
      __pointer(::image_list)          m_pil16;
      
      __pointer_array(TrayItemInfo)    m_infoa;
      HFONT                            m_hfontHidden;
      HFONT                            m_hfontRegular;


      notification_area();
      virtual ~notification_area();

      virtual void Initialize(::context_object * pcontextobject);

      void ListTrayIcons(i32 defindex = 0);
      void EditCopy(i32 iItem);
      void DoubleClick(i32 iItem);
      void RightClick(i32 iItem);
      void LeftClick(i32 iItem);
      void MoveLeft(i32 iItem);
      void MoveRight(i32 iItem);
      void Refresh();
      void PostMessageToTrayIcon(i32 iItem, LPARAM lParam);


   };


} // namespace windowing_win32



