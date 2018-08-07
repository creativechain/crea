#pragma once

#include <fc/container/flat.hpp>
#include <creativecoin/protocol/operations.hpp>
#include <creativecoin/protocol/transaction.hpp>

#include <fc/string.hpp>

namespace creativecoin { namespace app {

using namespace fc;

void operation_get_impacted_accounts(
   const creativecoin::protocol::operation& op,
   fc::flat_set<protocol::account_name_type>& result );

void transaction_get_impacted_accounts(
   const creativecoin::protocol::transaction& tx,
   fc::flat_set<protocol::account_name_type>& result
   );

} } // creativecoin::app
