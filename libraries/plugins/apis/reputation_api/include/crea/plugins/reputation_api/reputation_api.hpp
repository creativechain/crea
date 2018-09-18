#pragma once
#include <crea/plugins/json_rpc/utility.hpp>
#include <crea/plugins/reputation/reputation_objects.hpp>
#include <crea/plugins/database_api/database_api_objects.hpp>

#include <crea/protocol/types.hpp>

#include <fc/optional.hpp>
#include <fc/variant.hpp>
#include <fc/vector.hpp>

namespace crea { namespace plugins { namespace reputation {

using crea::protocol::account_name_type;

namespace detail
{
   class reputation_api_impl;
}

struct account_reputation
{
   account_name_type             account;
   crea::protocol::share_type   reputation;
};

struct get_account_reputations_args
{
   account_name_type account_lower_bound;
   uint32_t          limit = 1000;
};

struct get_account_reputations_return
{
   vector< account_reputation > reputations;
};

class reputation_api
{
   public:
      reputation_api();
      ~reputation_api();

      DECLARE_API(
         (get_account_reputations)
      )

   private:
      std::unique_ptr< detail::reputation_api_impl > my;
};

} } } // crea::plugins::reputation

FC_REFLECT( crea::plugins::reputation::account_reputation,
            (account)(reputation) );

FC_REFLECT( crea::plugins::reputation::get_account_reputations_args,
            (account_lower_bound)(limit) );

FC_REFLECT( crea::plugins::reputation::get_account_reputations_return,
            (reputations) );
