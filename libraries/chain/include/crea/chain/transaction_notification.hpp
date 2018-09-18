#pragma once

#include <crea/protocol/block.hpp>

namespace crea { namespace chain {

struct transaction_notification
{
   transaction_notification( const crea::protocol::signed_transaction& tx ) : transaction(tx)
   {
      transaction_id = tx.id();
   }

   crea::protocol::transaction_id_type          transaction_id;
   const crea::protocol::signed_transaction&    transaction;
};

} }
