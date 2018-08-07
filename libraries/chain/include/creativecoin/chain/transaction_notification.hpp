#pragma once

#include <creativecoin/protocol/block.hpp>

namespace creativecoin { namespace chain {

struct transaction_notification
{
   transaction_notification( const creativecoin::protocol::signed_transaction& tx ) : transaction(tx)
   {
      transaction_id = tx.id();
   }

   creativecoin::protocol::transaction_id_type          transaction_id;
   const creativecoin::protocol::signed_transaction&    transaction;
};

} }
