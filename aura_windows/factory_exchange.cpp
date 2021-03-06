#include "framework.h"


extern "C"
void apex_windows_factory_exchange(::factory_map * pfactorymap);


extern "C"
void aura_windows_common_factory_exchange(::factory_map * pfactorymap);


extern "C"
void aura_windows_factory_exchange(::factory_map * pfactorymap)
{

   apex_windows_factory_exchange(pfactorymap);

   aura_windows_common_factory_exchange(pfactorymap);

   //create_factory < ::windows::console, ::console::console >();
   //create_factory < ::windows::crypto, ::crypto::crypto >();
   //create_factory < ::windows::ip_enum, ::net::ip_enum >();


   //create_factory < ::windows::buffer, ::graphics::graphics >();
   //pfactorymap->create_factory < ::windows::interaction_impl, ::user::interaction_impl >();

   //create_factory < ::file::os_watcher, ::file::watcher >();
   //create_factory < ::file::os_watch, ::file::watch >();

   //create_factory < ::windows::copydesk, ::user::copydesk >();
   //create_factory < ::windows::shell_open, ::shell_open >();
   //create_factory < ::windows::interprocess_communication_base, ::interprocess_communication::base >();
   //create_factory < ::windows::interprocess_communication_tx, ::interprocess_communication::tx >();
   //create_factory < ::windows::interprocess_communication_rx, ::interprocess_communication::rx >();
   //create_factory < ::windows::interprocess_communication, ::interprocess_communication::interprocess_communication >();


   pfactorymap->create_factory < ::windows::interaction_impl, ::user::interaction_impl >();

   pfactorymap->create_factory < ::aura::windows::node, ::acme::node >();


}




