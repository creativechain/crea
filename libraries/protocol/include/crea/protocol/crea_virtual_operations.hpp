#pragma once
#include <crea/protocol/base.hpp>
#include <crea/protocol/block_header.hpp>
#include <crea/protocol/asset.hpp>

#include <fc/utf8.hpp>

namespace crea { namespace protocol {

  struct author_reward_operation : public virtual_operation {
    author_reward_operation(){}
    author_reward_operation( const account_name_type& a, const string& p, const asset& s, const asset& st, const asset& v, const asset& c )
      :author(a), permlink(p), cbd_payout(s), crea_payout(st), vesting_payout(v), curators_vesting_payout(c) {}

    account_name_type author;
    string            permlink;
    asset             cbd_payout;
    asset             crea_payout;
    asset             vesting_payout;
    asset             curators_vesting_payout;
  };


  struct curation_reward_operation : public virtual_operation
  {
    curation_reward_operation(){}
    curation_reward_operation( const string& c, const asset& r, const string& a, const string& p )
      :curator(c), reward(r), comment_author(a), comment_permlink(p) {}

    account_name_type curator;
    asset             reward;
    account_name_type comment_author;
    string            comment_permlink;
  };


  struct comment_reward_operation : public virtual_operation
  {
    comment_reward_operation() = default;
    comment_reward_operation( const account_name_type& a, const string& pl, const asset& p, share_type ar,
                              const asset& tpv, const asset& cpv, const asset& bpv )
      : author(a), permlink(pl), payout(p), author_rewards(ar),
        total_payout_value( tpv ), curator_payout_value( cpv ), beneficiary_payout_value( bpv ) {}

    account_name_type author;
    string            permlink;
    asset             payout;
    share_type        author_rewards;
    asset             total_payout_value;
    asset             curator_payout_value;
    asset             beneficiary_payout_value;
  };


  struct liquidity_reward_operation : public virtual_operation
  {
    liquidity_reward_operation( string o = string(), asset p = asset() )
    :owner(o), payout(p) {}

    account_name_type owner;
    asset             payout;
  };


  struct interest_operation : public virtual_operation
  {
    interest_operation( const string& o = "", const asset& i = asset(0,CBD_SYMBOL) )
      :owner(o),interest(i){}

    account_name_type owner;
    asset             interest;
  };


  struct fill_convert_request_operation : public virtual_operation
  {
    fill_convert_request_operation(){}
    fill_convert_request_operation( const string& o, const uint32_t id, const asset& in, const asset& out )
      :owner(o), requestid(id), amount_in(in), amount_out(out) {}

    account_name_type owner;
    uint32_t          requestid = 0;
    asset             amount_in;
    asset             amount_out;
  };


  struct fill_vesting_withdraw_operation : public virtual_operation
  {
    fill_vesting_withdraw_operation(){}
    fill_vesting_withdraw_operation( const string& f, const string& t, const asset& w, const asset& d )
      :from_account(f), to_account(t), withdrawn(w), deposited(d) {}

    account_name_type from_account;
    account_name_type to_account;
    asset             withdrawn;
    asset             deposited;
  };


  struct shutdown_witness_operation : public virtual_operation
  {
    shutdown_witness_operation(){}
    shutdown_witness_operation( const string& o ):owner(o) {}

    account_name_type owner;
  };


  struct fill_order_operation : public virtual_operation
  {
    fill_order_operation(){}
    fill_order_operation( const string& c_o, uint32_t c_id, const asset& c_p, const string& o_o, uint32_t o_id, const asset& o_p )
    :current_owner(c_o), current_orderid(c_id), current_pays(c_p), open_owner(o_o), open_orderid(o_id), open_pays(o_p) {}

    account_name_type current_owner;
    uint32_t          current_orderid = 0;
    asset             current_pays;
    account_name_type open_owner;
    uint32_t          open_orderid = 0;
    asset             open_pays;
  };


  struct fill_transfer_from_savings_operation : public virtual_operation
  {
    fill_transfer_from_savings_operation() {}
    fill_transfer_from_savings_operation( const account_name_type& f, const account_name_type& t, const asset& a, const uint32_t r, const string& m )
      :from(f), to(t), amount(a), request_id(r), memo(m) {}

    account_name_type from;
    account_name_type to;
    asset             amount;
    uint32_t          request_id = 0;
    string            memo;
  };

  struct hardfork_operation : public virtual_operation
  {
    hardfork_operation() {}
    hardfork_operation( uint32_t hf_id ) : hardfork_id( hf_id ) {}

    uint32_t         hardfork_id = 0;
  };

  struct comment_payout_update_operation : public virtual_operation
  {
    comment_payout_update_operation() {}
    comment_payout_update_operation( const account_name_type& a, const string& p ) : author( a ), permlink( p ) {}

    account_name_type author;
    string            permlink;
  };

  struct effective_comment_vote_operation : public virtual_operation
  {
    effective_comment_vote_operation() = default;
    effective_comment_vote_operation(const account_name_type& _voter, const account_name_type& _author, const string& _permlink) :
      voter(_voter), author(_author), permlink(_permlink) {}

    account_name_type voter;
    account_name_type author;
    string            permlink;
    uint64_t          weight = 0; ///< defines the score this vote receives, used by vote payout calc. 0 if a negative vote or changed votes.
    int64_t           rshares = 0; ///< The number of rshares this vote is responsible for
    uint64_t          total_vote_weight = 0; ///< the total weight of voting rewards, used to calculate pro-rata share of curation payouts
    //potential payout of related comment at the moment of this vote
    asset             pending_payout = asset( 0, CBD_SYMBOL ); //supplemented by account history RocksDB plugin (needed by HiveMind)
  };

  struct ineffective_delete_comment_operation : public virtual_operation
  {
    ineffective_delete_comment_operation() = default;
    ineffective_delete_comment_operation(const account_name_type& _author, const string& _permlink) :
      author(_author), permlink(_permlink) {}

    account_name_type author;
    string            permlink;
  };

  struct return_vesting_delegation_operation : public virtual_operation
  {
    return_vesting_delegation_operation() {}
    return_vesting_delegation_operation( const account_name_type& a, const asset& v ) : account( a ), vesting_shares( v ) {}

    account_name_type account;
    asset             vesting_shares;
  };

  struct comment_benefactor_reward_operation : public virtual_operation
  {
    comment_benefactor_reward_operation() {}
    comment_benefactor_reward_operation( const account_name_type& b, const account_name_type& a, const string& p, const asset& s, const asset& st, const asset& v )
      : benefactor( b ), author( a ), permlink( p ), cbd_payout( s ), crea_payout( st ), vesting_payout( v ) {}

    account_name_type benefactor;
    account_name_type author;
    string            permlink;
    asset             cbd_payout;
    asset             crea_payout;
    asset             vesting_payout;
  };

  struct producer_reward_operation : public virtual_operation
  {
    producer_reward_operation(){}
    producer_reward_operation( const string& p, const asset& v ) : producer( p ), vesting_shares( v ) {}

    account_name_type producer;
    asset             vesting_shares;

  };

  struct clear_null_account_balance_operation : public virtual_operation
  {
    vector< asset >   total_cleared;
  };

  struct consolidate_treasury_balance_operation : public virtual_operation
  {
    vector< asset >   total_moved;
  };

  struct delayed_voting_operation : public virtual_operation
  {
    delayed_voting_operation(){}
    delayed_voting_operation( const account_name_type& _voter, const ushare_type _votes ) : voter( _voter ), votes( _votes ) {}

    account_name_type    voter;
    ushare_type          votes = 0;
  };

  struct sps_fund_operation : public virtual_operation
  {
    sps_fund_operation() {}
    sps_fund_operation( const account_name_type& _fund, const asset& v ) : fund_account( _fund ), additional_funds( v ) {}

    account_name_type fund_account;
    asset additional_funds;
  };

  struct sps_convert_operation : public virtual_operation
  {
    sps_convert_operation() {}
    sps_convert_operation(account_name_type f, const asset& c, const asset& a) : fund_account(f), crea_amount_in( c ), cbd_amount_out( a ) {}

    account_name_type fund_account;
    asset crea_amount_in;
    asset cbd_amount_out;
  };

  // TODO : Fix legacy error itr != to_full_tag.end(): Invalid operation name: hardfork_crea {"n":"hardfork_crea"}
  struct hardfork_crea_operation : public virtual_operation
  {
    hardfork_crea_operation() {}
    hardfork_crea_operation( const account_name_type& acc, const account_name_type& _treasury,
      const asset& s, const asset& st, const asset& v, const asset& cs )
      : account( acc ), treasury( _treasury ), cbd_transferred( s ), crea_transferred( st ), vests_converted( v ), total_crea_from_vests(cs)
    {}

    account_name_type account;
    account_name_type treasury;
    asset             cbd_transferred;
    asset             crea_transferred;
    asset             vests_converted; // Amount of converted vests
    asset             total_crea_from_vests; // Resulting CREA from conversion
  };

  struct hardfork_crea_restore_operation : public virtual_operation
  {
    hardfork_crea_restore_operation() {}
    hardfork_crea_restore_operation( const account_name_type& acc, const account_name_type& _treasury, const asset& s, const asset& st )
      : account( acc ), treasury( _treasury ), cbd_transferred( s ), crea_transferred( st ) {}

    account_name_type account;
    account_name_type treasury;
    asset             cbd_transferred;
    asset             crea_transferred;
  };

} } //crea::protocol

FC_REFLECT( crea::protocol::author_reward_operation, (author)(permlink)(cbd_payout)(crea_payout)(vesting_payout)(curators_vesting_payout) )
FC_REFLECT( crea::protocol::curation_reward_operation, (curator)(reward)(comment_author)(comment_permlink) )
FC_REFLECT( crea::protocol::comment_reward_operation, (author)(permlink)(payout)(author_rewards)(total_payout_value)(curator_payout_value)(beneficiary_payout_value) )
FC_REFLECT( crea::protocol::fill_convert_request_operation, (owner)(requestid)(amount_in)(amount_out) )
FC_REFLECT( crea::protocol::liquidity_reward_operation, (owner)(payout) )
FC_REFLECT( crea::protocol::interest_operation, (owner)(interest) )
FC_REFLECT( crea::protocol::fill_vesting_withdraw_operation, (from_account)(to_account)(withdrawn)(deposited) )
FC_REFLECT( crea::protocol::shutdown_witness_operation, (owner) )
FC_REFLECT( crea::protocol::fill_order_operation, (current_owner)(current_orderid)(current_pays)(open_owner)(open_orderid)(open_pays) )
FC_REFLECT( crea::protocol::fill_transfer_from_savings_operation, (from)(to)(amount)(request_id)(memo) )
FC_REFLECT( crea::protocol::hardfork_operation, (hardfork_id) )
FC_REFLECT( crea::protocol::comment_payout_update_operation, (author)(permlink) )
FC_REFLECT( crea::protocol::effective_comment_vote_operation, (voter)(author)(permlink)(weight)(rshares)(total_vote_weight)(pending_payout))
FC_REFLECT( crea::protocol::ineffective_delete_comment_operation, (author)(permlink))
FC_REFLECT( crea::protocol::return_vesting_delegation_operation, (account)(vesting_shares) )
FC_REFLECT( crea::protocol::comment_benefactor_reward_operation, (benefactor)(author)(permlink)(cbd_payout)(crea_payout)(vesting_payout) )
FC_REFLECT( crea::protocol::producer_reward_operation, (producer)(vesting_shares) )
FC_REFLECT( crea::protocol::clear_null_account_balance_operation, (total_cleared) )
FC_REFLECT( crea::protocol::consolidate_treasury_balance_operation, ( total_moved ) )
FC_REFLECT( crea::protocol::delayed_voting_operation, (voter)(votes) )
FC_REFLECT( crea::protocol::sps_fund_operation, (fund_account)(additional_funds) )
FC_REFLECT( crea::protocol::sps_convert_operation, (fund_account)(crea_amount_in)(cbd_amount_out) )
FC_REFLECT( crea::protocol::hardfork_crea_operation, (account)(treasury)(cbd_transferred)(crea_transferred)(vests_converted)(total_crea_from_vests) )
FC_REFLECT( crea::protocol::hardfork_crea_restore_operation, (account)(treasury)(cbd_transferred)(crea_transferred) )
