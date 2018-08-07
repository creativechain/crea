#pragma once
#include <creativecoin/plugins/json_rpc/json_rpc_plugin.hpp>
#include <creativecoin/plugins/database_api/database_api_plugin.hpp>
#include <creativecoin/plugins/block_api/block_api_plugin.hpp>

#define CREA_CONDENSER_API_PLUGIN_NAME "condenser_api"

namespace creativecoin { namespace plugins { namespace condenser_api {

using namespace appbase;

class condenser_api_plugin : public appbase::plugin< condenser_api_plugin >
{
public:
   APPBASE_PLUGIN_REQUIRES( (creativecoin::plugins::json_rpc::json_rpc_plugin)(creativecoin::plugins::database_api::database_api_plugin) )

   condenser_api_plugin();
   virtual ~condenser_api_plugin();

   static const std::string& name() { static std::string name = CREA_CONDENSER_API_PLUGIN_NAME; return name; }

   virtual void set_program_options( options_description& cli, options_description& cfg ) override;

   virtual void plugin_initialize( const variables_map& options ) override;
   virtual void plugin_startup() override;
   virtual void plugin_shutdown() override;

   std::shared_ptr< class condenser_api > api;
};

} } } // creativecoin::plugins::condenser_api
