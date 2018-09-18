#pragma once
#include <crea/plugins/follow/follow_plugin.hpp>
#include <crea/plugins/json_rpc/json_rpc_plugin.hpp>

#include <appbase/application.hpp>

#define CREA_FOLLOW_API_PLUGIN_NAME "follow_api"


namespace crea { namespace plugins { namespace follow {

using namespace appbase;

class follow_api_plugin : public appbase::plugin< follow_api_plugin >
{
public:
   APPBASE_PLUGIN_REQUIRES(
      (crea::plugins::follow::follow_plugin)
      (crea::plugins::json_rpc::json_rpc_plugin)
   )

   follow_api_plugin();
   virtual ~follow_api_plugin();

   static const std::string& name() { static std::string name = CREA_FOLLOW_API_PLUGIN_NAME; return name; }

   virtual void set_program_options( options_description& cli, options_description& cfg ) override;

   virtual void plugin_initialize( const variables_map& options ) override;
   virtual void plugin_startup() override;
   virtual void plugin_shutdown() override;

   std::shared_ptr< class follow_api > api;
};

} } } // crea::plugins::follow
