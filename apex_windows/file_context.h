#pragma once


CLASS_DECL_APEX_WINDOWS bool vfxFullPath(unichar * pszPathOut, const unichar * pszFileIn);

CLASS_DECL_APEX_WINDOWS bool vfxFullPath(wstring & wstrFullPath, const wstring & wstrPath);
CLASS_DECL_APEX_WINDOWS ::u32 vfxGetFileName(const unichar * pszPathName, unichar * pszTitle, ::u32 nMax);

CLASS_DECL_APEX_WINDOWS void vfxGetModuleShortFileName(HINSTANCE hInst, string & strShortName);
CLASS_DECL_APEX_WINDOWS void vfxGetModuleShortFileName(HINSTANCE hInst, string & strShortName);


namespace windows
{


   class CLASS_DECL_APEX_WINDOWS file_context :
      virtual public ::file_context
   {
   public:


      __reference(file_system)      m_pfilesystem;
      __reference(dir_system)       m_pdirsystem;


      file_context();
      virtual ~file_context();


      virtual ::e_status initialize(::object * pobject) override;


      virtual ::e_status init_system() override;


      virtual bool get_status(const ::file::path & path, ::file::file_status & status);
      virtual ::extended::status set_status(const ::file::path & path, const ::file::file_status & status);



      virtual bool FullPath(string & str, const char * pszFileIn);

      virtual bool FullPath(wstring & wstrFullPath, const wstring & wstrPath);
      virtual ::u32 GetFileName(const char * pszPathName, string & str);

      virtual void GetModuleShortFileName(HINSTANCE hInst, string & strShortName);
      
      virtual ::payload length(const ::file::path & path) override;
      virtual ::payload length(const ::file::path & path, ::payload * pvarQuery) override;

      virtual ::extended::status move(const ::file::path & pszNew, const ::file::path & psz) override;

      virtual ::extended::status del(const ::file::path & psz) override;

      virtual bool is_read_only(const ::file::path & psz) override;

      virtual ::extended::transport < ::file::file > resource_get_file(const ::file::path & path) override;

      virtual bool get_last_write_time(FILETIME * pfiletime, const string & strFilename);

      virtual ::e_status update_module_path() override;

      virtual file_result get_file(const ::payload & varFile, const ::file::e_open & nOpenFlags) override;



      virtual ::file::path dropbox_info_json() override;

      //virtual ::file::path onedrive_global_ini(::object * pobject) override;
      //virtual ::file::path onedrive_cid_ini(::object * pobject) override;


   };


} // namespace windows



