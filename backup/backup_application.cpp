﻿#include "framework.h"


namespace backup
{


   application::application(::object * pobject) :
      ::object(this),
      ::thread(this),
      ::aura::application(pobject),
      ::axis::application(pobject),
      ::base::application(pobject),
      ::aura::application(pobject),
      ::production::application(pobject),
      ::sphere::application(pobject)
   {
      m_strAppName            = "backup";
      m_strBaseSupportId      = "ca2_backup";
   }

   application::~application()
   {
   }

   bool application::initialize_application()
   {

      if(!::aura::application::init_instance())
         return false;


      m_eversion = ::production::production::version_backup;


      create_factory <document > ();
      create_factory <frame > ();
      create_factory <view > ();
      create_factory <pane_view > ();

      puser->filemanager()->m_strLevelUp = "levelup";


      m_pdocumenttemplateMain = new ::user::single_document_template(
      this,
      "production/frame",
      __type(document),
      __type(frame),
      __type(pane_view));

      return true;
   }

   void application::term_instance()
   {

      try
      {

         production::application::term_instance();

      }
      catch (...)
      {

         m_result.add(error_failed);

      }

   }


   bool application::bergedge_start()
   {
      return m_pdocumenttemplateMain->open_document_file(this, nullptr, true) != nullptr;
   }


   void application::_001OnFileNew()
   {

      document_manager()->_001OnFileNew();

   }


} //namespace backup


