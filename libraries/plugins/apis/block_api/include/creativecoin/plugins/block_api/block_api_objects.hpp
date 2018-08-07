#pragma once
#include <creativecoin/chain/account_object.hpp>
#include <creativecoin/chain/block_summary_object.hpp>
#include <creativecoin/chain/comment_object.hpp>
#include <creativecoin/chain/global_property_object.hpp>
#include <creativecoin/chain/history_object.hpp>
#include <creativecoin/chain/creativecoin_objects.hpp>
#include <creativecoin/chain/transaction_object.hpp>
#include <creativecoin/chain/witness_objects.hpp>
#include <creativecoin/chain/database.hpp>

namespace creativecoin { namespace plugins { namespace block_api {

using namespace creativecoin::chain;

struct api_signed_block_object : public signed_block
{
   api_signed_block_object( const signed_block& block ) : signed_block( block )
   {
      block_id = id();
      signing_key = signee();
      transaction_ids.reserve( transactions.size() );
      for( const signed_transaction& tx : transactions )
         transaction_ids.push_back( tx.id() );
   }
   api_signed_block_object() {}

   block_id_type                 block_id;
   public_key_type               signing_key;
   vector< transaction_id_type > transaction_ids;
};

} } } // creativecoin::plugins::database_api

FC_REFLECT_DERIVED( creativecoin::plugins::block_api::api_signed_block_object, (creativecoin::protocol::signed_block),
                     (block_id)
                     (signing_key)
                     (transaction_ids)
                  )
