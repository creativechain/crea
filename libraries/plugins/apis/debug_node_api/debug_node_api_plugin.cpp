#include <crea/plugins/debug_node_api/debug_node_api_plugin.hpp>
#include <crea/plugins/debug_node_api/debug_node_api.hpp>


namespace crea { namespace plugins { namespace debug_node {

debug_node_api_plugin::debug_node_api_plugin() {}
debug_node_api_plugin::~debug_node_api_plugin() {}

void debug_node_api_plugin::set_program_options( options_description& cli, options_description& cfg ) {}

void debug_node_api_plugin::plugin_initialize( const variables_map& options )
{
   api = std::make_shared< debug_node_api >();
}

void debug_node_api_plugin::plugin_startup() {}
void debug_node_api_plugin::plugin_shutdown() {}

} } } // crea::plugins::debug_node
