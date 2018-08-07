#pragma once
#include <creativecoin/protocol/block_header.hpp>
#include <creativecoin/protocol/transaction.hpp>

namespace creativecoin { namespace protocol {

   struct signed_block : public signed_block_header
   {
      checksum_type calculate_merkle_root()const;
      vector<signed_transaction> transactions;
   };

} } // creativecoin::protocol

FC_REFLECT_DERIVED( creativecoin::protocol::signed_block, (creativecoin::protocol::signed_block_header), (transactions) )
