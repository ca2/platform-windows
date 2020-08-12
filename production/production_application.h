#pragma once


namespace production
{


   class view;


   class CLASS_DECL_CA2_PRODUCTION application :
      virtual public ::sphere::application
   {
   public:


      view * m_pview;
      
      
      __pointer(::user::single_document_template) m_pdocumenttemplateMain;


      application(::object * pobject);
      virtual ~application();

      virtual void construct();

      virtual ::estatus init_instance() override;
      virtual void term_instance() override;

      virtual bool bergedge_start();

      virtual void on_request(::create * pcreate);

	   virtual void route_command_message(::user::command * pcommand);
   		

      virtual void OnFileManagerOpenFile(
         ::filemanager::data * pdata, 
         ::file::item_array & itema);

      production::e_version m_eversion;

   };


} // namespace production


