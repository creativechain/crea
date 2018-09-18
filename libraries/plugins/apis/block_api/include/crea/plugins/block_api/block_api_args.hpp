#pragma once
#include <crea/plugins/block_api/block_api_objects.hpp>

#include <crea/protocol/types.hpp>
#include <crea/protocol/transaction.hpp>
#include <crea/protocol/block_header.hpp>

#include <crea/plugins/json_rpc/utility.hpp>

namespace crea { namespace plugins { namespace block_api {

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

} } } // crea::block_api

FC_REFLECT( crea::plugins::block_api::get_block_header_args,
   (block_num) )

FC_REFLECT( crea::plugins::block_api::get_block_header_return,
   (header) )

FC_REFLECT( crea::plugins::block_api::get_block_args,
   (block_num) )

FC_REFLECT( crea::plugins::block_api::get_block_return,
   (block) )

