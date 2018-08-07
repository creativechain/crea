#pragma once

#include <creativecoin/chain/evaluator.hpp>

#include <creativecoin/private_message/private_message_operations.hpp>
#include <creativecoin/private_message/private_message_plugin.hpp>

namespace creativecoin { namespace private_message {

CREA_DEFINE_PLUGIN_EVALUATOR( private_message_plugin, creativecoin::private_message::private_message_plugin_operation, private_message )

} }
