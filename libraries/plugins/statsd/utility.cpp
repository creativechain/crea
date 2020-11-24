#include <crea/plugins/statsd/utility.hpp>

namespace crea { namespace plugins{ namespace statsd { namespace util {

bool statsd_enabled()
{
  static bool enabled = appbase::app().find_plugin< statsd_plugin >() != nullptr;
  return enabled;
}

const statsd_plugin& get_statsd()
{
  static const statsd_plugin& statsd = appbase::app().get_plugin< statsd_plugin >();
  return statsd;
}

} } } } // crea::plugins::statsd::util