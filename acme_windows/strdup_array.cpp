#include "framework.h"
#include "acme/primitive/collection/strdup_array.h"
//#include "acme/operating_system.h"

namespace acme { namespace windows { 
platform_char** node::get_envp(wcsdup_array & wcsdupa)
{

   auto lpvEnv = ::GetEnvironmentStringsW();

   if (lpvEnv == NULL)
   {

      return nullptr;

   }

   // Variable strings are separated by NULL byte, and the block is terminated by a NULL byte.

   for (auto lpszVariable = (LPWSTR)lpvEnv; *lpszVariable; lpszVariable++)
   {

      wstring wstrVariable(lpszVariable);

      auto pszVariable = wcsdup(wstrVariable);

      wcsdupa.m_wszptra.add(pszVariable);

      while (*lpszVariable)
      {

         lpszVariable++;

      }

   }

   if (!FreeEnvironmentStrings(lpvEnv))
   {

      ::output_debug_string("GetEnvironmentStrings() failed.");

   }

   return wcsdupa.m_wszptra.get_data();

}
}
}