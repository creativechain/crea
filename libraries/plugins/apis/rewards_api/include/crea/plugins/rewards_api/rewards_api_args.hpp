#pragma once
#include <string>
#include <fc/uint128.hpp>
#include <crea/protocol/misc_utilities.hpp>
#include <crea/protocol/asset.hpp>
#include <crea/plugins/json_rpc/utility.hpp>

namespace crea { namespace plugins { namespace rewards_api {

struct simulate_curve_payouts_element {
  protocol::account_name_type  author;
  fc::string                   permlink;
  protocol::asset              payout;
};

struct simulate_curve_payouts_args
{
  protocol::curve_id curve;
  std::string        var1;
};

struct simulate_curve_payouts_return
{
  std::string                                   recent_claims;
  std::vector< simulate_curve_payouts_element > payouts;
};


} } } // crea::plugins::rewards_api

FC_REFLECT( crea::plugins::rewards_api::simulate_curve_payouts_element, (author)(permlink)(payout) )
FC_REFLECT( crea::plugins::rewards_api::simulate_curve_payouts_args, (curve)(var1) )
FC_REFLECT( crea::plugins::rewards_api::simulate_curve_payouts_return, (recent_claims)(payouts) )
