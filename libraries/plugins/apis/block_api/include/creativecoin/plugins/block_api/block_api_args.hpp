#pragma once
#include <creativecoin/plugins/block_api/block_api_objects.hpp>

#include <creativecoin/protocol/types.hpp>
#include <creativecoin/protocol/transaction.hpp>
#include <creativecoin/protocol/block_header.hpp>

#include <creativecoin/plugins/json_rpc/utility.hpp>

namespace creativecoin { namespace plugins { namespace block_api {

/* get_block_header */

struct get_block_header_args
{
   uint32_t block_num;
};

struct get_block_header_return
{
   optional< block_header > header;
};

/* get_block */
struct get_block_args
{
   uint32_t block_num;
};

struct get_block_return
{
   optional< api_signed_block_object > block;
};

} } } // creativecoin::block_api

FC_REFLECT( creativecoin::plugins::block_api::get_block_header_args,
   (block_num) )

FC_REFLECT( creativecoin::plugins::block_api::get_block_header_return,
   (header) )

FC_REFLECT( creativecoin::plugins::block_api::get_block_args,
   (block_num) )

FC_REFLECT( creativecoin::plugins::block_api::get_block_return,
   (block) )

