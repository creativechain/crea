#pragma once

#include <crea/protocol/types.hpp>

#include <fc/reflect/reflect.hpp>

namespace crea { namespace protocol {
struct signed_transaction;
} } // crea::protocol

namespace crea { namespace plugins { namespace rc {

using crea::protocol::account_name_type;
using crea::protocol::signed_transaction;

account_name_type get_resource_user( const signed_transaction& tx );

} } } // crea::plugins::rc
