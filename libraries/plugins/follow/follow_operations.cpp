#include <crea/plugins/follow/follow_operations.hpp>

#include <crea/protocol/operation_util_impl.hpp>

namespace crea { namespace plugins{ namespace follow {

void follow_operation::validate()const
{
   FC_ASSERT( follower != following, "You cannot follow yourself" );
}

void reblog_operation::validate()const
{
   FC_ASSERT( account != author, "You cannot reblog your own content" );
}

} } } //crea::plugins::follow

CREA_DEFINE_OPERATION_TYPE( crea::plugins::follow::follow_plugin_operation )
