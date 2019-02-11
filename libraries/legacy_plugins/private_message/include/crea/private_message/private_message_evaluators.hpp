#pragma once

#include <crea/chain/evaluator.hpp>

#include <crea/private_message/private_message_operations.hpp>
#include <crea/private_message/private_message_plugin.hpp>

namespace crea { namespace private_message {

CREA_DEFINE_PLUGIN_EVALUATOR( private_message_plugin, crea::private_message::private_message_plugin_operation, private_message )

} }
