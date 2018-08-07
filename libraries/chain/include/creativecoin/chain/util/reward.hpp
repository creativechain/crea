#pragma once

#include <creativecoin/chain/util/asset.hpp>
#include <creativecoin/chain/creativecoin_objects.hpp>

#include <creativecoin/protocol/asset.hpp>
#include <creativecoin/protocol/config.hpp>
#include <creativecoin/protocol/types.hpp>
#include <creativecoin/protocol/misc_utilities.hpp>

#include <fc/reflect/reflect.hpp>

#include <fc/uint128.hpp>

namespace creativecoin { namespace chain { namespace util {

using creativecoin::protocol::asset;
using creativecoin::protocol::price;
using creativecoin::protocol::share_type;

using fc::uint128_t;

struct comment_reward_context
{
   share_type rshares;
   uint16_t   reward_weight = 0;
   asset      max_sbd;
   uint128_t  total_reward_shares2;
   asset      total_reward_fund_creativecoin;
   price      current_creativecoin_price;
   protocol::curve_id   reward_curve = protocol::quadratic;
   uint128_t  content_constant = CREA_CONTENT_CONSTANT_HF0;
};

uint64_t get_rshare_reward( const comment_reward_context& ctx );

inline uint128_t get_content_constant_s()
{
   return CREA_CONTENT_CONSTANT_HF0; // looking good for posters
}

uint128_t evaluate_reward_curve( const uint128_t& rshares, const protocol::curve_id& curve = protocol::quadratic, const uint128_t& content_constant = CREA_CONTENT_CONSTANT_HF0 );

inline bool is_comment_payout_dust( const price& p, uint64_t creativecoin_payout )
{
   return to_sbd( p, asset( creativecoin_payout, CREA_SYMBOL ) ) < CREA_MIN_PAYOUT_SBD;
}

} } } // creativecoin::chain::util

FC_REFLECT( creativecoin::chain::util::comment_reward_context,
   (rshares)
   (reward_weight)
   (max_sbd)
   (total_reward_shares2)
   (total_reward_fund_creativecoin)
   (current_creativecoin_price)
   (reward_curve)
   (content_constant)
   )
