#pragma once

#include <crea/chain/util/asset.hpp>
#include <crea/chain/crea_objects.hpp>

#include <crea/protocol/asset.hpp>
#include <crea/protocol/config.hpp>
#include <crea/protocol/types.hpp>
#include <crea/protocol/misc_utilities.hpp>

#include <fc/reflect/reflect.hpp>

#include <fc/uint128.hpp>

namespace crea { namespace chain { namespace util {

using crea::protocol::asset;
using crea::protocol::price;
using crea::protocol::share_type;

using fc::uint128_t;

struct comment_reward_context
{
   share_type rshares;
   uint16_t   reward_weight = 0;
   asset      max_cbd;
   uint128_t  total_reward_shares2;
   asset      total_reward_fund_crea;
   price      current_crea_price;
   protocol::curve_id   reward_curve = protocol::quadratic;
   uint128_t  content_constant = CREA_CONTENT_CONSTANT_HF0;
};

uint64_t get_rshare_reward( const comment_reward_context& ctx );

inline uint128_t get_content_constant_s()
{
   return CREA_CONTENT_CONSTANT_HF0; // looking good for posters
}

uint128_t evaluate_reward_curve( const uint128_t& rshares, const protocol::curve_id& curve = protocol::quadratic, const uint128_t& content_constant = CREA_CONTENT_CONSTANT_HF0 );

inline bool is_comment_payout_dust( const price& p, uint64_t crea_payout )
{
   return to_cbd( p, asset( crea_payout, CREA_SYMBOL ) ) < CREA_MIN_PAYOUT_CBD;
}

} } } // crea::chain::util

FC_REFLECT( crea::chain::util::comment_reward_context,
   (rshares)
   (reward_weight)
   (max_cbd)
   (total_reward_shares2)
   (total_reward_fund_crea)
   (current_crea_price)
   (reward_curve)
   (content_constant)
   )
