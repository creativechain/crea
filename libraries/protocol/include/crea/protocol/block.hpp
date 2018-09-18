#pragma once
#include <crea/protocol/block_header.hpp>
#include <crea/protocol/transaction.hpp>

namespace crea { namespace protocol {

   struct signed_block : public signed_block_header
   {
      checksum_type calculate_merkle_root()const;
      vector<signed_transaction> transactions;
   };

} } // crea::protocol

FC_REFLECT_DERIVED( crea::protocol::signed_block, (crea::protocol::signed_block_header), (transactions) )
