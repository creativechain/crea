#include <crea/plugins/tags_api/tags_api_plugin.hpp>
#include <crea/plugins/tags_api/tags_api.hpp>


namespace crea { namespace plugins { namespace tags {

tags_api_plugin::tags_api_plugin() {}
tags_api_plugin::~tags_api_plugin() {}

void tags_api_plugin::set_program_options( options_description& cli, options_description& cfg ) {}

void tags_api_plugin::plugin_initialize( const variables_map& options )
{
  api = std::make_shared< tags_api >();
}

void tags_api_plugin::plugin_startup() {}
void tags_api_plugin::plugin_shutdown() {}

} } } // crea::plugins::tags
