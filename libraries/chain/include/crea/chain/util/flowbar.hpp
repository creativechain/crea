#pragma once

#include <cstdint>

#include <fc/saturation.hpp>
#include <fc/uint128.hpp>
#include <fc/time.hpp>
#include <fc/exception/exception.hpp>
#include <fc/reflect/reflect.hpp>

namespace crea { namespace chain { namespace util {

struct flowbar_params
{
  int64_t    max_flow         = 0;
  uint32_t   regen_time       = 0;

  flowbar_params() {}
  flowbar_params( int64_t mm, uint32_t rt )
    : max_flow(mm), regen_time(rt) {}

  void validate()const
  { try{
    FC_ASSERT( max_flow >= 0 );
  } FC_CAPTURE_AND_RETHROW( (max_flow) ) }
};

struct flowbar
{
  int64_t    current_flow     = 0;
  uint32_t   last_update_time = 0;

  flowbar() {}
  flowbar( int64_t m, uint32_t t )
    : current_flow(m), last_update_time(t) {}

  template< bool skip_cap_regen = false >
  void regenerate_flow( const flowbar_params& params, uint32_t now )
  {
    params.validate();

    FC_ASSERT( now >= last_update_time );
    uint32_t dt = now - last_update_time;
    if( current_flow >= params.max_flow )
    {
      current_flow = params.max_flow;
      last_update_time = now;
      return;
    }

    if( !skip_cap_regen )
      dt = (dt > params.regen_time) ? params.regen_time : dt;

    uint128_t max_flow_dt = uint64_t( params.max_flow >= 0 ? params.max_flow : 0 );
    max_flow_dt *= dt;
    uint64_t u_regen = (max_flow_dt / params.regen_time).to_uint64();
    FC_ASSERT( u_regen <= std::numeric_limits< int64_t >::max() );
    int64_t new_current_flow = fc::signed_sat_add( current_flow, int64_t( u_regen ) );
    current_flow = (new_current_flow > params.max_flow) ? params.max_flow : new_current_flow;

    last_update_time = now;
  }

  template< bool skip_cap_regen = false >
  void regenerate_flow( const flowbar_params& params, fc::time_point_sec now )
  {
    regenerate_flow< skip_cap_regen >( params, now.sec_since_epoch() );
  }

  bool has_flow( int64_t flow_needed )const
  {
    return (flow_needed <= 0) || (current_flow >= flow_needed);
  }

  bool has_flow( uint64_t flow_needed )const
  {
    FC_ASSERT( flow_needed <= std::numeric_limits< int64_t >::max() );
    return has_flow( (int64_t) flow_needed );
  }

  void use_flow( int64_t flow_used, int64_t min_flow = std::numeric_limits< uint64_t >::min() )
  {
    current_flow = fc::signed_sat_sub( current_flow, flow_used );

    if( current_flow < min_flow )
    {
      current_flow = min_flow;
    }
  }

  void use_flow( uint64_t flow_used, int64_t min_flow = std::numeric_limits< uint64_t >::min() )
  {
    FC_ASSERT( flow_used <= std::numeric_limits< int64_t >::max() );
    use_flow( (int64_t) flow_used, min_flow );
  }
};

template< typename T >
int64_t get_effective_vesting_shares( const T& account )
{
  int64_t effective_vesting_shares =
      account.vesting_shares.amount.value              // base vesting shares
    + account.received_vesting_shares.amount.value     // incoming delegations
    - account.delegated_vesting_shares.amount.value;   // outgoing delegations

  // If there is a power down occuring, also reduce effective vesting shares by this week's power down amount
  if( account.next_vesting_withdrawal != fc::time_point_sec::maximum() )
  {
    effective_vesting_shares -=
      std::min(
        account.vesting_withdraw_rate.amount.value,           // Weekly amount
        account.to_withdraw.value - account.withdrawn.value   // Or remainder
        );
  }

  return effective_vesting_shares;
}

template< typename PropType, typename AccountType >
void update_flowbar( const PropType& gpo, AccountType& account, bool downvote_flow = false, bool check_overflow = true, int64_t new_flow = 0 )
{
  auto effective_vests = util::get_effective_vesting_shares( account );
  try {
  flowbar_params params( effective_vests, CREA_VOTING_FLOW_REGENERATION_SECONDS );
  account.voting_flowbar.regenerate_flow( params, gpo.time );
  account.voting_flowbar.use_flow( -new_flow );
  } FC_CAPTURE_LOG_AND_RETHROW( (account)(effective_vests) )

  FC_TODO( "This hardfork check should not be needed. Remove after HF21 if that is the case." );
  // This is used as a hardfork check. Can be replaced with if( gpo.downvote_pool_percent ). Leaving as a hard check to be safe until after HF 21
  try{
  if( downvote_flow )
  {
    flowbar_params params;
    params.regen_time = CREA_VOTING_FLOW_REGENERATION_SECONDS;

    if( check_overflow )
    {
      params.max_flow = ( ( uint128_t( effective_vests ) * gpo.downvote_pool_percent ) / CREA_100_PERCENT ).to_int64();
    }
    else
    {
      FC_TODO( "Cleanup once we have verified the overflow has not permanently made it in to the chain" );
      uint128_t numerator = effective_vests * gpo.downvote_pool_percent;
      if( numerator.hi != 0 )
        elog( "NOTIFYALERT! max flow overflow made it in to the chain" );

      params.max_flow = ( effective_vests * gpo.downvote_pool_percent ) / CREA_100_PERCENT;
    }

    account.downvote_flowbar.regenerate_flow( params, gpo.time );
    account.downvote_flowbar.use_flow( ( -new_flow * gpo.downvote_pool_percent ) / CREA_100_PERCENT );
  }
  } FC_CAPTURE_LOG_AND_RETHROW( (account)(effective_vests) )
}

} } } // crea::chain::util

FC_REFLECT( crea::chain::util::flowbar,
  (current_flow)
  (last_update_time)
  )
