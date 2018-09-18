#pragma once

#include <fc/container/flat.hpp>
#include <crea/protocol/operations.hpp>
#include <crea/protocol/transaction.hpp>

#include <fc/string.hpp>

namespace crea { namespace app {

using namespace fc;

void operation_get_impacted_accounts(
   const crea::protocol::operation& op,
   fc::flat_set<protocol::account_name_type>& result );

void transaction_get_impacted_accounts(
   const crea::protocol::transaction& tx,
   fc::flat_set<protocol::account_name_type>& result
   );

} } // crea::app
