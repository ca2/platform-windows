﻿#include "framework.h"


namespace backup
{


   document::document(::context_object * pcontextobject) :
      ::object(pobject),
      ::user::document(pobject),
      ::data::data_container_base(pobject),
      production::document(pobject)
   {


   }


   document::~document()
   {


   }


} // namespace backup


