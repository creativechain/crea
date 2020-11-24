#pragma once
#include <crea/chain/crea_fwd.hpp>
#include <crea/plugins/debug_node/debug_node_plugin.hpp>
#include <crea/plugins/json_rpc/json_rpc_plugin.hpp>

#include <appbase/application.hpp>

#define CREA_DEBUG_NODE_API_PLUGIN_NAME "debug_node_api"


namespace crea { namespace plugins { namespace debug_node {

using namespace appbase;

class debug_node_api_plugin : public appbase::plugin< debug_node_api_plugin >
{
public:
  APPBASE_PLUGIN_REQUIRES(
    (crea::plugins::debug_node::debug_node_plugin)
    (crea::plugins::json_rpc::json_rpc_plugin)
  )

  debug_node_api_plugin();
  virtual ~debug_node_api_plugin();

  static const std::string& name() { static std::string name = CREA_DEBUG_NODE_API_PLUGIN_NAME; return name; }

  virtual void set_program_options( options_description& cli, options_description& cfg ) override;

  virtual void plugin_initialize( const variables_map& options ) override;
  virtual void plugin_startup() override;
  virtual void plugin_shutdown() override;

  std::shared_ptr< class debug_node_api > api;
};

} } } // crea::plugins::debug_node
