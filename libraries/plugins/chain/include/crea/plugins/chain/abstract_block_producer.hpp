#pragma once

#include <fc/time.hpp>

#include <crea/chain/database.hpp>

namespace crea { namespace plugins { namespace chain {
   
class abstract_block_producer {
public:
   virtual ~abstract_block_producer() = default;

   virtual crea::chain::signed_block generate_block(
      fc::time_point_sec when,
      const crea::chain::account_name_type& witness_owner,
      const fc::ecc::private_key& block_signing_private_key,
      uint32_t skip = crea::chain::database::skip_nothing) = 0;
};

} } } // crea::plugins::chain
