#include <creativecoin/plugins/follow/follow_operations.hpp>

#include <creativecoin/protocol/operation_util_impl.hpp>

namespace creativecoin { namespace plugins{ namespace follow {

void follow_operation::validate()const
{
   FC_ASSERT( follower != following, "You cannot follow yourself" );
}

void reblog_operation::validate()const
{
   FC_ASSERT( account != author, "You cannot reblog your own content" );
}

} } } //creativecoin::plugins::follow

CREA_DEFINE_OPERATION_TYPE( creativecoin::plugins::follow::follow_plugin_operation )
