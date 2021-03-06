#pragma once


namespace write_text_win32
{


   // Windows GDI

   class CLASS_DECL_WRITE_TEXT_WIN32 font_enumeration :
      virtual public ::write_text::font_enumeration
   {
   public:


      HDC                                    m_hdc;
      wstring                                m_wstrTopicFaceName;


      font_enumeration();
      ~font_enumeration() override;


      ::e_status on_enumerate_fonts() override;


      virtual void enumerate_character_set(::write_text::font_enumeration_item * pitem);


      static int CALLBACK OLDFONTENUMPROCW(CONST LOGFONTW* plogfont, CONST TEXTMETRICW* ptextmetric, DWORD dwFontType, LPARAM lparam);


      static int CALLBACK OLDFONTENUMPROCW_character_set(CONST LOGFONTW* plogfont, CONST TEXTMETRICW* ptextmetric, DWORD dwFontType, LPARAM lparam);


   };


} // namespace write_text_win32



