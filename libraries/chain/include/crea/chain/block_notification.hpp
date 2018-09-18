#pragma once

#include <crea/protocol/block.hpp>

namespace crea { namespace chain {

struct block_notification
{
   block_notification( const crea::protocol::signed_block& b ) : block(b)
   {
      block_id = b.id();
      block_num = block_header::num_from_id( block_id );
   }

   crea::protocol::block_id_type          block_id;
   uint32_t                                block_num = 0;
   const crea::protocol::signed_block&    block;
};

} }
