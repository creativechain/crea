#pragma once

#include <crea/plugins/database_api/database_api.hpp>
#include <crea/plugins/block_api/block_api.hpp>
#include <crea/plugins/account_history_api/account_history_api.hpp>
#include <crea/plugins/account_by_key_api/account_by_key_api.hpp>
#include <crea/plugins/network_broadcast_api/network_broadcast_api.hpp>
#include <crea/plugins/tags_api/tags_api.hpp>
#include <crea/plugins/follow_api/follow_api.hpp>
#include <crea/plugins/reputation_api/reputation_api.hpp>
#include <crea/plugins/market_history_api/market_history_api.hpp>
#include <crea/plugins/witness_api/witness_api.hpp>

#include <crea/plugins/condenser_api/condenser_api_legacy_objects.hpp>

#include <fc/optional.hpp>
#include <fc/variant.hpp>
#include <fc/vector.hpp>
#include <fc/api.hpp>

namespace crea { namespace plugins { namespace condenser_api {

using std::vector;
using fc::variant;
using fc::optional;

using namespace chain;

namespace detail{ class condenser_api_impl; }

struct discussion_index
{
   string           category;         /// category by which everything is filtered
   vector< string > trending;         /// trending posts over the last 24 hours
   vector< string > payout;           /// pending posts by payout
   vector< string > payout_comments;  /// pending comments by payout
   vector< string > trending30;       /// pending lifetime payout
   vector< string > created;          /// creation date
   vector< string > responses;        /// creation date
   vector< string > updated;          /// creation date
   vector< string > active;           /// last update or reply
   vector< string > votes;            /// last update or reply
   vector< string > cashout;          /// last update or reply
   vector< string > maturing;         /// about to be paid out
   vector< string > best;             /// total lifetime payout
   vector< string > hot;              /// total lifetime payout
   vector< string > promoted;         /// pending lifetime payout
};

struct api_limit_order_object
{
   api_limit_order_object( const limit_order_object& o ) :
      id( o.id ),
      created( o.created ),
      expiration( o.expiration ),
      seller( o.seller ),
      orderid( o.orderid ),
      for_sale( o.for_sale ),
      sell_price( o.sell_price )
   {}

   api_limit_order_object(){}

   limit_order_id_type  id;
   time_point_sec       created;
   time_point_sec       expiration;
   account_name_type    seller;
   uint32_t             orderid = 0;
   share_type           for_sale;
   legacy_price         sell_price;
   double               real_price = 0;
   bool                 rewarded   = false;
};


struct api_operation_object
{
   api_operation_object() {}
   api_operation_object( const account_history::api_operation_object& obj, const legacy_operation& l_op ) :
      trx_id( obj.trx_id ),
      block( obj.block ),
      trx_in_block( obj.trx_in_block ),
      virtual_op( obj.virtual_op ),
      timestamp( obj.timestamp ),
      op( l_op )
   {}

   transaction_id_type  trx_id;
   uint32_t             block = 0;
   uint32_t             trx_in_block = 0;
   uint32_t             op_in_trx = 0;
   uint32_t             virtual_op = 0;
   fc::time_point_sec   timestamp;
   legacy_operation     op;
};

struct api_account_object
{
   api_account_object( const database_api::api_account_object& a ) :
      id( a.id ),
      name( a.name ),
      owner( a.owner ),
      active( a.active ),
      posting( a.posting ),
      memo_key( a.memo_key ),
      json_metadata( a.json_metadata ),
      proxy( a.proxy ),
      last_owner_update( a.last_owner_update ),
      last_account_update( a.last_account_update ),
      created( a.created ),
      mined( a.mined ),
      recovery_account( a.recovery_account ),
      reset_account( a.reset_account ),
      last_account_recovery( a.last_account_recovery ),
      comment_count( a.comment_count ),
      lifetime_vote_count( a.lifetime_vote_count ),
      post_count( a.post_count ),
      can_vote( a.can_vote ),
      voting_flowbar( a.voting_flowbar ),
      balance( legacy_asset::from_asset( a.balance ) ),
      savings_balance( legacy_asset::from_asset( a.savings_balance ) ),
      cbd_balance( legacy_asset::from_asset( a.cbd_balance ) ),
      cbd_seconds( a.cbd_seconds ),
      cbd_seconds_last_update( a.cbd_seconds_last_update ),
      cbd_last_interest_payment( a.cbd_last_interest_payment ),
      savings_cbd_balance( legacy_asset::from_asset( a.savings_cbd_balance ) ),
      savings_cbd_seconds( a.savings_cbd_seconds ),
      savings_cbd_seconds_last_update( a.savings_cbd_seconds_last_update ),
      savings_cbd_last_interest_payment( a.savings_cbd_last_interest_payment ),
      savings_withdraw_requests( a.savings_withdraw_requests ),
      reward_cbd_balance( legacy_asset::from_asset( a.reward_cbd_balance ) ),
      reward_crea_balance( legacy_asset::from_asset( a.reward_crea_balance ) ),
      reward_vesting_balance( legacy_asset::from_asset( a.reward_vesting_balance ) ),
      reward_vesting_crea( legacy_asset::from_asset( a.reward_vesting_crea ) ),
      curation_rewards( a.curation_rewards ),
      posting_rewards( a.posting_rewards ),
      vesting_shares( legacy_asset::from_asset( a.vesting_shares ) ),
      delegated_vesting_shares( legacy_asset::from_asset( a.delegated_vesting_shares ) ),
      received_vesting_shares( legacy_asset::from_asset( a.received_vesting_shares ) ),
      vesting_withdraw_rate( legacy_asset::from_asset( a.vesting_withdraw_rate ) ),
      next_vesting_withdrawal( a.next_vesting_withdrawal ),
      withdrawn( a.withdrawn ),
      to_withdraw( a.to_withdraw ),
      withdraw_routes( a.withdraw_routes ),
      witnesses_voted_for( a.witnesses_voted_for ),
      last_post( a.last_post ),
      last_root_post( a.last_root_post ),
      last_vote_time( a.last_vote_time ),
      post_bandwidth( a.post_bandwidth ),
      follower_count(a.follower_count),
      following_count(a.following_count),
      pending_claimed_accounts( a.pending_claimed_accounts )
   {
      voting_power = _compute_voting_power(a);
      proxied_vsf_votes.insert( proxied_vsf_votes.end(), a.proxied_vsf_votes.begin(), a.proxied_vsf_votes.end() );
   }

   api_account_object(){}

   uint16_t _compute_voting_power( const database_api::api_account_object& a );

   account_id_type   id;

   account_name_type name;
   authority         owner;
   authority         active;
   authority         posting;
   public_key_type   memo_key;
   string            json_metadata;
   account_name_type proxy;

   time_point_sec    last_owner_update;
   time_point_sec    last_account_update;

   time_point_sec    created;
   bool              mined = false;
   account_name_type recovery_account;
   account_name_type reset_account;
   time_point_sec    last_account_recovery;
   uint32_t          comment_count = 0;
   uint32_t          lifetime_vote_count = 0;
   uint32_t          post_count = 0;

   bool              can_vote = false;
   util::flowbar     voting_flowbar;
   uint16_t          voting_power = 0;

   legacy_asset      balance;
   legacy_asset      savings_balance;

   legacy_asset      cbd_balance;
   uint128_t         cbd_seconds;
   time_point_sec    cbd_seconds_last_update;
   time_point_sec    cbd_last_interest_payment;

   legacy_asset      savings_cbd_balance;
   uint128_t         savings_cbd_seconds;
   time_point_sec    savings_cbd_seconds_last_update;
   time_point_sec    savings_cbd_last_interest_payment;

   uint8_t           savings_withdraw_requests = 0;

   legacy_asset      reward_cbd_balance;
   legacy_asset      reward_crea_balance;
   legacy_asset      reward_vesting_balance;
   legacy_asset      reward_vesting_crea;

   share_type        curation_rewards;
   share_type        posting_rewards;

   legacy_asset      vesting_shares;
   legacy_asset      delegated_vesting_shares;
   legacy_asset      received_vesting_shares;
   legacy_asset      vesting_withdraw_rate;
   time_point_sec    next_vesting_withdrawal;
   share_type        withdrawn;
   share_type        to_withdraw;
   uint16_t          withdraw_routes = 0;

   vector< share_type > proxied_vsf_votes;

   uint16_t          witnesses_voted_for = 0;

   time_point_sec    last_post;
   time_point_sec    last_root_post;
   time_point_sec    last_vote_time;
   uint32_t          post_bandwidth = 0;
   uint32_t          follower_count = 0;
   uint32_t          following_count = 0;

   share_type        pending_claimed_accounts = 0;
};

struct extended_account : public api_account_object
{
   extended_account(){}
   extended_account( const database_api::api_account_object& a ) :
      api_account_object( a ) {}

   share_type                                               average_bandwidth;
   share_type                                               lifetime_bandwidth;
   time_point_sec                                           last_bandwidth_update;
   share_type                                               average_market_bandwidth;
   share_type                                               lifetime_market_bandwidth;
   time_point_sec                                           last_market_bandwidth_update;

   legacy_asset                                             vesting_balance;  /// convert vesting_shares to vesting crea
   share_type                                               reputation = 0;
   map< uint64_t, api_operation_object >   transfer_history; /// transfer to/from vesting
   map< uint64_t, api_operation_object >   market_history;   /// limit order / cancel / fill
   map< uint64_t, api_operation_object >   post_history;
   map< uint64_t, api_operation_object >   vote_history;
   map< uint64_t, api_operation_object >   other_history;
   set< string >                                            witness_votes;
   vector< tags::tag_count_object >                         tags_usage;
   vector< follow::reblog_count >                           guest_bloggers;

   optional< map< uint32_t, api_limit_order_object > >      open_orders;
   optional< vector< string > >                             comments;         /// permlinks for this user
   optional< vector< string > >                             blog;             /// blog posts for this user
   optional< vector< string > >                             feed;             /// feed posts for this user
   optional< vector< string > >                             recent_replies;   /// blog posts for this user
   optional< vector< string > >                             recommended;      /// posts recommened for this user
};

struct api_download_granted_object
{
   api_download_granted_object( const database_api::api_download_granted_object& o ):
       id( o.id ), payment_date( o.payment_date ), comment_author( o.comment_author ),
       comment_permlink( o.comment_permlink ), price( legacy_asset::from_asset( o.price ) )
   {

   }
   api_download_granted_object(){}

   download_granted_id_type  id;

   time_point_sec            payment_date;
   account_name_type         comment_author;
   string                    comment_permlink;
   string                    resource = "";
   legacy_asset              price;

};

struct api_comment_download_object
{
   api_comment_download_object( const database_api::api_comment_download_object& o ):
       id( o.id ), resource( o.resource ), name( o.name ),
       type( o.type ), size( o.size ), times_downloaded( o.times_downloaded), password( o.password ),
       price( o.price )
   {
      author = o.author;
      permlink = o.permlink;
   }
   api_comment_download_object(){}

   comment_download_id_type  id;

   account_name_type         author;
   string                    permlink;
   string                    resource;
   string                    name;
   string                    type;
   uint32_t                  size = 0;
   uint32_t                  times_downloaded = 0;
   string                    password;
   asset                     price;


};

struct api_comment_object
{
   api_comment_object( const database_api::api_comment_object& c ):
      id( c.id ),
      category( c.category ),
      parent_author( c.parent_author ),
      parent_permlink( c.parent_permlink ),
      author( c.author ),
      permlink( c.permlink ),
      title( c.title ),
      body( c.body ),
      json_metadata( c.json_metadata ),
      last_update( c.last_update ),
      created( c.created ),
      active( c.active ),
      last_payout( c.last_payout ),
      depth( c.depth ),
      children( c.children ),
      net_rshares( c.net_rshares ),
      abs_rshares( c.abs_rshares ),
      vote_rshares( c.vote_rshares ),
      children_abs_rshares( c.children_abs_rshares ),
      cashout_time( c.cashout_time ),
      max_cashout_time( c.max_cashout_time ),
      total_vote_weight( c.total_vote_weight ),
      reward_weight( c.reward_weight ),
      total_payout_value( legacy_asset::from_asset( c.total_payout_value ) ),
      curator_payout_value( legacy_asset::from_asset( c.curator_payout_value ) ),
      author_rewards( c.author_rewards ),
      net_votes( c.net_votes ),
      root_author( c.root_author ),
      root_permlink( c.root_permlink ),
      max_accepted_payout( legacy_asset::from_asset( c.max_accepted_payout ) ),
      percent_crea_dollars( c.percent_crea_dollars ),
      allow_replies( c.allow_replies ),
      allow_votes( c.allow_votes ),
      allow_curation_rewards( c.allow_curation_rewards ),
      download( c.download )
   {
      for( auto& route : c.beneficiaries )
      {
         beneficiaries.push_back( route );
      }
   }

   api_comment_object(){}

   comment_id_type   id;
   string            category;
   string            parent_author;
   string            parent_permlink;
   string            author;
   string            permlink;

   string            title;
   string            body;
   api_comment_download_object download;
   string            json_metadata;
   time_point_sec    last_update;
   time_point_sec    created;
   time_point_sec    active;
   time_point_sec    last_payout;

   uint8_t           depth = 0;
   uint32_t          children = 0;

   share_type        net_rshares;
   share_type        abs_rshares;
   share_type        vote_rshares;

   share_type        children_abs_rshares;
   time_point_sec    cashout_time;
   time_point_sec    max_cashout_time;
   uint64_t          total_vote_weight = 0;

   uint16_t          reward_weight = 0;

   legacy_asset      total_payout_value;
   legacy_asset      curator_payout_value;

   share_type        author_rewards;

   int32_t           net_votes = 0;

   account_name_type root_author;
   string            root_permlink;

   legacy_asset      max_accepted_payout;
   uint16_t          percent_crea_dollars = 0;
   bool              allow_replies = false;
   bool              allow_votes = false;
   bool              allow_curation_rewards = false;
   vector< beneficiary_route_type > beneficiaries;
};

struct extended_dynamic_global_properties
{
   extended_dynamic_global_properties() {}
   extended_dynamic_global_properties( const database_api::api_dynamic_global_property_object& o ) :
      head_block_number( o.head_block_number ),
      head_block_id( o.head_block_id ),
      time( o.time ),
      current_witness( o.current_witness ),
      total_pow( o.total_pow ),
      num_pow_witnesses( o.num_pow_witnesses ),
      virtual_supply( legacy_asset::from_asset( o.virtual_supply ) ),
      current_supply( legacy_asset::from_asset( o.current_supply ) ),
      confidential_supply( legacy_asset::from_asset( o.confidential_supply ) ),
      current_cbd_supply( legacy_asset::from_asset( o.current_cbd_supply ) ),
      confidential_cbd_supply( legacy_asset::from_asset( o.confidential_cbd_supply ) ),
      total_vesting_fund_crea( legacy_asset::from_asset( o.total_vesting_fund_crea ) ),
      total_vesting_shares( legacy_asset::from_asset( o.total_vesting_shares ) ),
      total_reward_fund_crea( legacy_asset::from_asset( o.total_reward_fund_crea ) ),
      total_reward_shares2( o.total_reward_shares2 ),
      pending_rewarded_vesting_shares( legacy_asset::from_asset( o.pending_rewarded_vesting_shares ) ),
      pending_rewarded_vesting_crea( legacy_asset::from_asset( o.pending_rewarded_vesting_crea ) ),
      cbd_interest_rate( o.cbd_interest_rate ),
      cbd_print_rate( o.cbd_print_rate ),
      maximum_block_size( o.maximum_block_size ),
      current_aslot( o.current_aslot ),
      recent_slots_filled( o.recent_slots_filled ),
      participation_count( o.participation_count ),
      last_irreversible_block_num( o.last_irreversible_block_num ),
      vote_power_reserve_rate( o.vote_power_reserve_rate ),
      delegation_return_period( o.delegation_return_period ),
      reverse_auction_seconds( o.reverse_auction_seconds ),
      cbd_stop_percent( o.cbd_stop_percent ),
      cbd_start_percent( o.cbd_start_percent )
   {}

   uint32_t          head_block_number = 0;
   block_id_type     head_block_id;
   time_point_sec    time;
   account_name_type current_witness;

   uint64_t          total_pow = -1;

   uint32_t          num_pow_witnesses = 0;

   legacy_asset      virtual_supply;
   legacy_asset      current_supply;
   legacy_asset      confidential_supply;
   legacy_asset      current_cbd_supply;
   legacy_asset      confidential_cbd_supply;
   legacy_asset      total_vesting_fund_crea;
   legacy_asset      total_vesting_shares;
   legacy_asset      total_reward_fund_crea;
   fc::uint128       total_reward_shares2;
   legacy_asset      pending_rewarded_vesting_shares;
   legacy_asset      pending_rewarded_vesting_crea;

   uint16_t          cbd_interest_rate = 0;
   uint16_t          cbd_print_rate = CREA_100_PERCENT;

   uint32_t          maximum_block_size = 0;
   uint64_t          current_aslot = 0;
   fc::uint128_t     recent_slots_filled;
   uint8_t           participation_count = 0;

   uint32_t          last_irreversible_block_num = 0;
    
   uint32_t          vote_power_reserve_rate = CREA_INITIAL_VOTE_POWER_RATE;
   uint32_t          delegation_return_period = CREA_DELEGATION_RETURN_PERIOD_HF0;

   uint64_t          reverse_auction_seconds = 0;

   uint16_t          cbd_stop_percent = 0;
   uint16_t          cbd_start_percent = 0;

   int32_t           average_block_size = 0;
   int64_t           current_reserve_ratio = 1;
   uint128_t         max_virtual_bandwidth = 0;
};

struct api_witness_object
{
   api_witness_object() {}
   api_witness_object( const database_api::api_witness_object& w ) :
      id( w.id ),
      owner( w.owner ),
      created( w.created ),
      url( w.url ),
      total_missed( w.total_missed ),
      last_aslot( w.last_aslot ),
      last_confirmed_block_num( w.last_confirmed_block_num ),
      pow_worker( w.pow_worker ),
      signing_key( w.signing_key ),
      props( w.props ),
      cbd_exchange_rate( w.cbd_exchange_rate ),
      last_cbd_exchange_update( w.last_cbd_exchange_update ),
      votes( w.votes ),
      virtual_last_update( w.virtual_last_update ),
      virtual_position( w.virtual_position ),
      virtual_scheduled_time( w.virtual_scheduled_time ),
      last_work( w.last_work ),
      running_version( w.running_version ),
      hardfork_version_vote( w.hardfork_version_vote ),
      hardfork_time_vote( w.hardfork_time_vote ),
      available_witness_account_subsidies( w.available_witness_account_subsidies )
   {}

   witness_id_type  id;
   account_name_type       owner;
   time_point_sec          created;
   string                  url;
   uint32_t                total_missed = 0;
   uint64_t                last_aslot = 0;
   uint64_t                last_confirmed_block_num = 0;
   uint64_t                pow_worker;
   public_key_type         signing_key;
   api_chain_properties    props;
   legacy_price            cbd_exchange_rate;
   time_point_sec          last_cbd_exchange_update;
   share_type              votes;
   fc::uint128_t           virtual_last_update;
   fc::uint128_t           virtual_position;
   fc::uint128_t           virtual_scheduled_time = fc::uint128_t::max_value();
   digest_type             last_work;
   version                 running_version;
   hardfork_version        hardfork_version_vote;
   time_point_sec          hardfork_time_vote = CREA_GENESIS_TIME;
   int64_t                 available_witness_account_subsidies = 0;
};

struct api_witness_schedule_object
{
   api_witness_schedule_object() {}
   api_witness_schedule_object( const database_api::api_witness_schedule_object& w ) :
      id( w.id ),
      current_virtual_time( w.current_virtual_time ),
      next_shuffle_block_num( w.next_shuffle_block_num ),
      num_scheduled_witnesses( w.num_scheduled_witnesses ),
      elected_weight( w.elected_weight ),
      timeshare_weight( w.timeshare_weight ),
      miner_weight( w.miner_weight ),
      witness_pay_normalization_factor( w.witness_pay_normalization_factor ),
      median_props( w.median_props ),
      majority_version( w.majority_version ),
      max_voted_witnesses( w.max_voted_witnesses ),
      max_miner_witnesses( w.max_miner_witnesses ),
      max_runner_witnesses( w.max_runner_witnesses ),
      hardfork_required_witnesses( w.hardfork_required_witnesses ),
      account_subsidy_rd( w.account_subsidy_rd ),
      account_subsidy_witness_rd( w.account_subsidy_witness_rd ),
      min_witness_account_subsidy_decay( w.min_witness_account_subsidy_decay )
   {
      current_shuffled_witnesses.insert( current_shuffled_witnesses.begin(), w.current_shuffled_witnesses.begin(), w.current_shuffled_witnesses.end() );
   }

   witness_schedule_id_type      id;
   fc::uint128_t                 current_virtual_time;
   uint32_t                      next_shuffle_block_num = 1;
   vector< account_name_type >   current_shuffled_witnesses;
   uint8_t                       num_scheduled_witnesses = 1;
   uint8_t                       elected_weight = 1;
   uint8_t                       timeshare_weight = 5;
   uint8_t                       miner_weight = 1;
   uint32_t                      witness_pay_normalization_factor = 25;
   api_chain_properties          median_props;
   version                       majority_version;
   uint8_t                       max_voted_witnesses           = CREA_MAX_VOTED_WITNESSES_HF0;
   uint8_t                       max_miner_witnesses           = CREA_MAX_MINER_WITNESSES_HF0;
   uint8_t                       max_runner_witnesses          = CREA_MAX_RUNNER_WITNESSES_HF0;
   uint8_t                       hardfork_required_witnesses   = CREA_HARDFORK_REQUIRED_WITNESSES;

   rd_dynamics_params            account_subsidy_rd;
   rd_dynamics_params            account_subsidy_witness_rd;
   int64_t                       min_witness_account_subsidy_decay = 0;
};

struct api_feed_history_object
{
   api_feed_history_object() {}
   api_feed_history_object( const database_api::api_feed_history_object& f ) :
      current_median_history( f.current_median_history )
   {
      for( auto& p : f.price_history )
      {
         price_history.push_back( legacy_price( p ) );
      }
   }

   feed_history_id_type   id;
   legacy_price           current_median_history;
   deque< legacy_price >  price_history;
};

struct api_reward_fund_object
{
   api_reward_fund_object() {}
   api_reward_fund_object( const database_api::api_reward_fund_object& r ) :
      id( r.id ),
      name( r.name ),
      reward_balance( legacy_asset::from_asset( r.reward_balance ) ),
      recent_claims( r.recent_claims ),
      last_update( r.last_update ),
      content_constant( r.content_constant ),
      percent_curation_rewards( r.percent_curation_rewards ),
      percent_content_rewards( r.percent_content_rewards ),
      author_reward_curve( r.author_reward_curve ),
      curation_reward_curve( r.curation_reward_curve )
   {}

   reward_fund_id_type     id;
   reward_fund_name_type   name;
   legacy_asset            reward_balance;
   fc::uint128_t           recent_claims = 0;
   time_point_sec          last_update;
   uint128_t               content_constant = 0;
   uint16_t                percent_curation_rewards = 0;
   uint16_t                percent_content_rewards = 0;
   protocol::curve_id      author_reward_curve = protocol::linear;
   protocol::curve_id      curation_reward_curve = protocol::square_root;
};

struct api_escrow_object
{
   api_escrow_object() {}
   api_escrow_object( const database_api::api_escrow_object& e ) :
      id( e.id ),
      escrow_id( e.escrow_id ),
      from( e.from ),
      to( e.to ),
      agent( e.agent ),
      ratification_deadline( e.ratification_deadline ),
      escrow_expiration( e.escrow_expiration ),
      cbd_balance( legacy_asset::from_asset( e.cbd_balance ) ),
      crea_balance( legacy_asset::from_asset( e.crea_balance ) ),
      pending_fee( legacy_asset::from_asset( e.pending_fee ) ),
      to_approved( e.to_approved ),
      disputed( e.disputed ),
      agent_approved( e.agent_approved )
   {}

   escrow_id_type    id;
   uint32_t          escrow_id = 20;
   account_name_type from;
   account_name_type to;
   account_name_type agent;
   time_point_sec    ratification_deadline;
   time_point_sec    escrow_expiration;
   legacy_asset      cbd_balance;
   legacy_asset      crea_balance;
   legacy_asset      pending_fee;
   bool              to_approved = false;
   bool              disputed = false;
   bool              agent_approved = false;
};

struct api_savings_withdraw_object
{
   api_savings_withdraw_object() {}
   api_savings_withdraw_object( const database_api::api_savings_withdraw_object& s ) :
      id( s.id ),
      from( s.from ),
      to( s.to ),
      memo( s.memo ),
      request_id( s.request_id ),
      amount( legacy_asset::from_asset( s.amount ) ),
      complete( s.complete )
   {}

   savings_withdraw_id_type id;

   account_name_type from;
   account_name_type to;
   string            memo;
   uint32_t          request_id = 0;
   legacy_asset      amount;
   time_point_sec    complete;
};

struct api_vesting_delegation_object
{
   api_vesting_delegation_object() {}
   api_vesting_delegation_object( const database_api::api_vesting_delegation_object& v ) :
      id( v.id ),
      delegator( v.delegator ),
      delegatee( v.delegatee ),
      vesting_shares( legacy_asset::from_asset( v.vesting_shares ) ),
      min_delegation_time( v.min_delegation_time )
   {}

   vesting_delegation_id_type id;
   account_name_type delegator;
   account_name_type delegatee;
   legacy_asset      vesting_shares;
   time_point_sec    min_delegation_time;
};

struct api_vesting_delegation_expiration_object
{
   api_vesting_delegation_expiration_object() {}
   api_vesting_delegation_expiration_object( const database_api::api_vesting_delegation_expiration_object& v ) :
      id( v.id ),
      delegator( v.delegator ),
      vesting_shares( legacy_asset::from_asset( v.vesting_shares ) ),
      expiration( v.expiration )
   {}

   vesting_delegation_expiration_id_type id;
   account_name_type delegator;
   legacy_asset      vesting_shares;
   time_point_sec    expiration;
};

struct api_convert_request_object
{
   api_convert_request_object() {}
   api_convert_request_object( const database_api::api_convert_request_object& c ) :
      id( c.id ),
      owner( c.owner ),
      requestid( c.requestid ),
      amount( legacy_asset::from_asset( c.amount ) ),
      conversion_date( c.conversion_date )
   {}


   convert_request_id_type id;

   account_name_type owner;
   uint32_t          requestid = 0;
   legacy_asset      amount;
   time_point_sec    conversion_date;
};

struct discussion : public api_comment_object
{
   discussion() {}

   discussion( const api_comment_object& c ) : api_comment_object( c ) {}

   discussion( const tags::discussion& d ) :
      api_comment_object( d ),
      url( d.url ),
      root_title( d.root_title ),
      pending_payout_value( legacy_asset::from_asset( d.pending_payout_value ) ),
      total_pending_payout_value( legacy_asset::from_asset( d.total_pending_payout_value ) ),
      active_votes( d.active_votes ),
      replies( d.replies ),
      author_reputation( d.author_reputation ),
      promoted( legacy_asset::from_asset( d.promoted ) ),
      body_length( d.body_length ),
      reblogged_by( d.reblogged_by ),
      first_reblogged_by( d.first_reblogged_by ),
      first_reblogged_on( d.first_reblogged_on )
   {}

   string                        url; /// /category/@rootauthor/root_permlink#author/permlink
   string                        root_title;
   legacy_asset                  pending_payout_value; ///< cbd
   legacy_asset                  total_pending_payout_value; ///< cbd including replies
   vector< tags::vote_state >    active_votes;
   vector< string >              replies; ///< author/slug mapping
   share_type                    author_reputation = 0;
   legacy_asset                  promoted;
   uint32_t                      body_length = 0;
   vector< account_name_type >   reblogged_by;
   optional< account_name_type > first_reblogged_by;
   optional< time_point_sec >    first_reblogged_on;
};

struct tag_index
{
   vector< tags::tag_name_type > trending; /// pending payouts
};

struct api_tag_object
{
   api_tag_object( const tags::api_tag_object& o ) :
      name( o.name ),
      total_payouts( legacy_asset::from_asset( o.total_payouts ) ),
      net_votes( o.net_votes ),
      top_posts( o.top_posts ),
      comments( o.comments ),
      trending( o.trending ) {}

   api_tag_object() {}

   string               name;
   legacy_asset         total_payouts;
   int32_t              net_votes = 0;
   uint32_t             top_posts = 0;
   uint32_t             comments = 0;
   fc::uint128          trending = 0;
};

struct state
{
   string                                             current_route;

   extended_dynamic_global_properties                 props;

   tag_index                                          tag_idx;

   /**
    * "" is the global tags::discussion index
    */
   map< string, discussion_index >                    discussion_idx;

   map< string, api_tag_object >                tags;

   /**
    *  map from account/slug to full nested tags::discussion
    */
   map< string, discussion >                          content;
   map< string, extended_account >                    accounts;

   map< string, api_witness_object >                  witnesses;
   api_witness_schedule_object                        witness_schedule;
   legacy_price                                       feed_price;
   string                                             error;
};

struct scheduled_hardfork
{
   hardfork_version     hf_version;
   fc::time_point_sec   live_time;
};

struct account_vote
{
   string         authorperm;
   uint64_t       weight = 0;
   int64_t        rshares = 0;
   int16_t        percent = 0;
   time_point_sec time;
};

enum withdraw_route_type
{
   incoming,
   outgoing,
   all
};

typedef vector< variant > get_version_args;

struct get_version_return
{
   get_version_return() {}
   get_version_return( fc::string bc_v, fc::string s_v, fc::string fc_v )
      :blockchain_version( bc_v ), crea_revision( s_v ), fc_revision( fc_v ) {}

   fc::string blockchain_version;
   fc::string crea_revision;
   fc::string fc_revision;
};

typedef map< uint32_t, api_operation_object > get_account_history_return_type;

typedef vector< variant > broadcast_transaction_synchronous_args;

struct broadcast_transaction_synchronous_return
{
   broadcast_transaction_synchronous_return() {}
   broadcast_transaction_synchronous_return( transaction_id_type txid, int32_t bn, int32_t tn, bool ex )
   : id(txid), block_num(bn), trx_num(tn), expired(ex) {}

   transaction_id_type   id;
   int32_t               block_num = 0;
   int32_t               trx_num   = 0;
   bool                  expired   = false;
};

struct comment_feed_entry
{
   comment_feed_entry( const follow::comment_feed_entry& c ) :
      comment( c.comment ),
      reblog_on( c.reblog_on ),
      entry_id( c.entry_id )
   {
      reblog_by.resize( c.reblog_by.size() );

      for( auto& a : c.reblog_by )
      {
         reblog_by.push_back( a );
      }
   }

   comment_feed_entry() {}

   api_comment_object            comment;
   vector< account_name_type >   reblog_by;
   time_point_sec                reblog_on;
   uint32_t                      entry_id = 0;
};

struct comment_blog_entry
{
   comment_blog_entry( const follow::comment_blog_entry& c ) :
      comment( c.comment ),
      blog( c.blog ),
      reblog_on( c.reblog_on ),
      entry_id( c.entry_id )
   {}

   comment_blog_entry() {}

   api_comment_object   comment;
   string               blog;
   time_point_sec       reblog_on;
   uint32_t             entry_id = 0;
};

struct ticker
{
   ticker() {}
   ticker( const market_history::get_ticker_return& t ) :
      latest( t.latest ),
      lowest_ask( t.lowest_ask ),
      highest_bid( t.highest_bid ),
      percent_change( t.percent_change ),
      crea_volume( legacy_asset::from_asset( t.crea_volume ) ),
      cbd_volume( legacy_asset::from_asset( t.cbd_volume ) )
   {}

   double         latest = 0;
   double         lowest_ask = 0;
   double         highest_bid = 0;
   double         percent_change = 0;
   legacy_asset   crea_volume;
   legacy_asset   cbd_volume;
};

struct volume
{
   volume() {}
   volume( const market_history::get_volume_return& v ) :
      crea_volume( legacy_asset::from_asset( v.crea_volume ) ),
      cbd_volume( legacy_asset::from_asset( v.cbd_volume ) )
   {}

   legacy_asset   crea_volume;
   legacy_asset   cbd_volume;
};

struct order
{
   order() {}
   order( const market_history::order& o ) :
      order_price( o.order_price ),
      real_price( o.real_price ),
      crea( o.crea ),
      cbd( o.cbd ),
      created( o.created )
   {}

   legacy_price   order_price;
   double         real_price;
   share_type     crea;
   share_type     cbd;
   time_point_sec created;
};

struct order_book
{
   order_book() {}
   order_book( const market_history::get_order_book_return& book )
   {
      for( auto& b : book.bids ) bids.push_back( order( b ) );
      for( auto& a : book.asks ) asks.push_back( order( a ) );
   }

   vector< order > bids;
   vector< order > asks;
};

struct market_trade
{
   market_trade() {}
   market_trade( const market_history::market_trade& t ) :
      date( t.date ),
      current_pays( legacy_asset::from_asset( t.current_pays ) ),
      open_pays( legacy_asset::from_asset( t.open_pays ) )
   {}

   time_point_sec date;
   legacy_asset   current_pays;
   legacy_asset   open_pays;
};

#define DEFINE_API_ARGS( api_name, arg_type, return_type )  \
typedef arg_type api_name ## _args;                         \
typedef return_type api_name ## _return;

/*               API,                                    args,                return */
DEFINE_API_ARGS( get_trending_tags,                      vector< variant >,   vector< api_tag_object > )
DEFINE_API_ARGS( get_state,                              vector< variant >,   state )
DEFINE_API_ARGS( get_active_witnesses,                   vector< variant >,   vector< account_name_type > )
DEFINE_API_ARGS( get_block_header,                       vector< variant >,   optional< block_header > )
DEFINE_API_ARGS( get_block,                              vector< variant >,   optional< legacy_signed_block > )
DEFINE_API_ARGS( get_ops_in_block,                       vector< variant >,   vector< api_operation_object > )
DEFINE_API_ARGS( get_config,                             vector< variant >,   fc::variant_object )
DEFINE_API_ARGS( get_dynamic_global_properties,          vector< variant >,   extended_dynamic_global_properties )
DEFINE_API_ARGS( get_chain_properties,                   vector< variant >,   api_chain_properties )
DEFINE_API_ARGS( get_current_median_history_price,       vector< variant >,   legacy_price )
DEFINE_API_ARGS( get_feed_history,                       vector< variant >,   api_feed_history_object )
DEFINE_API_ARGS( get_witness_schedule,                   vector< variant >,   api_witness_schedule_object )
DEFINE_API_ARGS( get_hardfork_version,                   vector< variant >,   hardfork_version )
DEFINE_API_ARGS( get_next_scheduled_hardfork,            vector< variant >,   scheduled_hardfork )
DEFINE_API_ARGS( get_reward_fund,                        vector< variant >,   api_reward_fund_object )
DEFINE_API_ARGS( get_key_references,                     vector< variant >,   vector< vector< account_name_type > > )
DEFINE_API_ARGS( get_accounts,                           vector< variant >,   vector< extended_account > )
DEFINE_API_ARGS( get_account_references,                 vector< variant >,   vector< account_id_type > )
DEFINE_API_ARGS( lookup_account_names,                   vector< variant >,   vector< optional< api_account_object > > )
DEFINE_API_ARGS( lookup_accounts,                        vector< variant >,   set< string > )
DEFINE_API_ARGS( get_account_count,                      vector< variant >,   uint64_t )
DEFINE_API_ARGS( get_owner_history,                      vector< variant >,   vector< database_api::api_owner_authority_history_object > )
DEFINE_API_ARGS( get_recovery_request,                   vector< variant >,   optional< database_api::api_account_recovery_request_object > )
DEFINE_API_ARGS( get_escrow,                             vector< variant >,   optional< api_escrow_object > )
DEFINE_API_ARGS( get_withdraw_routes,                    vector< variant >,   vector< database_api::api_withdraw_vesting_route_object > )
DEFINE_API_ARGS( get_account_bandwidth,                  vector< variant >,   optional< witness::api_account_bandwidth_object > )
DEFINE_API_ARGS( get_savings_withdraw_from,              vector< variant >,   vector< api_savings_withdraw_object > )
DEFINE_API_ARGS( get_savings_withdraw_to,                vector< variant >,   vector< api_savings_withdraw_object > )
DEFINE_API_ARGS( get_vesting_delegations,                vector< variant >,   vector< api_vesting_delegation_object > )
DEFINE_API_ARGS( get_expiring_vesting_delegations,       vector< variant >,   vector< api_vesting_delegation_expiration_object > )
DEFINE_API_ARGS( get_witnesses,                          vector< variant >,   vector< optional< api_witness_object > > )
DEFINE_API_ARGS( get_conversion_requests,                vector< variant >,   vector< api_convert_request_object > )
DEFINE_API_ARGS( get_witness_by_account,                 vector< variant >,   optional< api_witness_object > )
DEFINE_API_ARGS( get_witnesses_by_vote,                  vector< variant >,   vector< api_witness_object > )
DEFINE_API_ARGS( lookup_witness_accounts,                vector< variant >,   vector< account_name_type > )
DEFINE_API_ARGS( get_open_orders,                        vector< variant >,   vector< api_limit_order_object > )
DEFINE_API_ARGS( get_witness_count,                      vector< variant >,   uint64_t )
DEFINE_API_ARGS( get_transaction_hex,                    vector< variant >,   string )
DEFINE_API_ARGS( get_transaction,                        vector< variant >,   legacy_signed_transaction )
DEFINE_API_ARGS( get_required_signatures,                vector< variant >,   set< public_key_type > )
DEFINE_API_ARGS( get_potential_signatures,               vector< variant >,   set< public_key_type > )
DEFINE_API_ARGS( verify_authority,                       vector< variant >,   bool )
DEFINE_API_ARGS( verify_account_authority,               vector< variant >,   bool )
DEFINE_API_ARGS( get_active_votes,                       vector< variant >,   vector< tags::vote_state > )
DEFINE_API_ARGS( get_account_votes,                      vector< variant >,   vector< account_vote > )
DEFINE_API_ARGS( get_download,                           vector< variant >,   api_download_granted_object )
DEFINE_API_ARGS( get_content,                            vector< variant >,   discussion )
DEFINE_API_ARGS( get_content_replies,                    vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_tags_used_by_author,                vector< variant >,   vector< tags::tag_count_object > )
DEFINE_API_ARGS( get_post_discussions_by_payout,         vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_comment_discussions_by_payout,      vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_trending,            vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_created,             vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_active,              vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_cashout,             vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_votes,               vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_children,            vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_hot,                 vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_feed,                vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_blog,                vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_comments,            vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_promoted,            vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_replies_by_last_update,             vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_discussions_by_author_before_date,  vector< variant >,   vector< discussion > )
DEFINE_API_ARGS( get_account_history,                    vector< variant >,   get_account_history_return_type )
DEFINE_API_ARGS( broadcast_transaction,                  vector< variant >,   json_rpc::void_type )
DEFINE_API_ARGS( broadcast_block,                        vector< variant >,   json_rpc::void_type )
DEFINE_API_ARGS( get_followers,                          vector< variant >,   vector< follow::api_follow_object > )
DEFINE_API_ARGS( get_following,                          vector< variant >,   vector< follow::api_follow_object > )
DEFINE_API_ARGS( get_follow_count,                       vector< variant >,   follow::get_follow_count_return )
DEFINE_API_ARGS( get_feed_entries,                       vector< variant >,   vector< follow::feed_entry > )
DEFINE_API_ARGS( get_feed,                               vector< variant >,   vector< comment_feed_entry > )
DEFINE_API_ARGS( get_blog_entries,                       vector< variant >,   vector< follow::blog_entry > )
DEFINE_API_ARGS( get_blog,                               vector< variant >,   vector< comment_blog_entry > )
DEFINE_API_ARGS( get_account_reputations,                vector< variant >,   vector< reputation::account_reputation > )
DEFINE_API_ARGS( get_reblogged_by,                       vector< variant >,   vector< account_name_type > )
DEFINE_API_ARGS( get_blog_authors,                       vector< variant >,   vector< follow::reblog_count > )
DEFINE_API_ARGS( get_ticker,                             vector< variant >,   ticker )
DEFINE_API_ARGS( get_volume,                             vector< variant >,   volume )
DEFINE_API_ARGS( get_order_book,                         vector< variant >,   order_book )
DEFINE_API_ARGS( get_trade_history,                      vector< variant >,   vector< market_trade > )
DEFINE_API_ARGS( get_recent_trades,                      vector< variant >,   vector< market_trade > )
DEFINE_API_ARGS( get_market_history,                     vector< variant >,   vector< market_history::bucket_object > )
DEFINE_API_ARGS( get_market_history_buckets,             vector< variant >,   flat_set< uint32_t > )

#undef DEFINE_API_ARGS

class condenser_api
{
public:
   condenser_api();
   ~condenser_api();

   DECLARE_API(
      (get_version)
      (get_trending_tags)
      (get_state)
      (get_active_witnesses)
      (get_block_header)
      (get_block)
      (get_ops_in_block)
      (get_config)
      (get_dynamic_global_properties)
      (get_chain_properties)
      (get_current_median_history_price)
      (get_feed_history)
      (get_witness_schedule)
      (get_hardfork_version)
      (get_next_scheduled_hardfork)
      (get_reward_fund)
      (get_key_references)
      (get_accounts)
      (get_account_references)
      (lookup_account_names)
      (lookup_accounts)
      (get_account_count)
      (get_owner_history)
      (get_recovery_request)
      (get_escrow)
      (get_withdraw_routes)
      (get_account_bandwidth)
      (get_savings_withdraw_from)
      (get_savings_withdraw_to)
      (get_vesting_delegations)
      (get_expiring_vesting_delegations)
      (get_witnesses)
      (get_conversion_requests)
      (get_witness_by_account)
      (get_witnesses_by_vote)
      (lookup_witness_accounts)
      (get_witness_count)
      (get_open_orders)
      (get_transaction_hex)
      (get_transaction)
      (get_required_signatures)
      (get_potential_signatures)
      (verify_authority)
      (verify_account_authority)
      (get_active_votes)
      (get_account_votes)
      (get_download)
      (get_content)
      (get_content_replies)
      (get_tags_used_by_author)
      (get_post_discussions_by_payout)
      (get_comment_discussions_by_payout)
      (get_discussions_by_trending)
      (get_discussions_by_created)
      (get_discussions_by_active)
      (get_discussions_by_cashout)
      (get_discussions_by_votes)
      (get_discussions_by_children)
      (get_discussions_by_hot)
      (get_discussions_by_feed)
      (get_discussions_by_blog)
      (get_discussions_by_comments)
      (get_discussions_by_promoted)
      (get_replies_by_last_update)
      (get_discussions_by_author_before_date)
      (get_account_history)
      (broadcast_transaction)
      (broadcast_transaction_synchronous)
      (broadcast_block)
      (get_followers)
      (get_following)
      (get_follow_count)
      (get_feed_entries)
      (get_feed)
      (get_blog_entries)
      (get_blog)
      (get_account_reputations)
      (get_reblogged_by)
      (get_blog_authors)
      (get_ticker)
      (get_volume)
      (get_order_book)
      (get_trade_history)
      (get_recent_trades)
      (get_market_history)
      (get_market_history_buckets)
   )

   private:
      friend class condenser_api_plugin;
      void api_startup();

      std::unique_ptr< detail::condenser_api_impl > my;
};

} } } // crea::plugins::condenser_api

FC_REFLECT( crea::plugins::condenser_api::discussion_index,
            (category)(trending)(payout)(payout_comments)(trending30)(updated)(created)(responses)(active)(votes)(maturing)(best)(hot)(promoted)(cashout) )

FC_REFLECT( crea::plugins::condenser_api::api_tag_object,
            (name)(total_payouts)(net_votes)(top_posts)(comments)(trending) )

FC_REFLECT( crea::plugins::condenser_api::state,
            (current_route)(props)(tag_idx)(tags)(content)(accounts)(witnesses)(discussion_idx)(witness_schedule)(feed_price)(error) )

FC_REFLECT( crea::plugins::condenser_api::api_limit_order_object,
            (id)(created)(expiration)(seller)(orderid)(for_sale)(sell_price)(real_price)(rewarded) )

FC_REFLECT( crea::plugins::condenser_api::api_operation_object,
             (trx_id)(block)(trx_in_block)(op_in_trx)(virtual_op)(timestamp)(op) )

FC_REFLECT( crea::plugins::condenser_api::api_account_object,
             (id)(name)(owner)(active)(posting)(memo_key)(json_metadata)(proxy)(last_owner_update)(last_account_update)
             (created)(mined)
             (recovery_account)(last_account_recovery)(reset_account)
             (comment_count)(lifetime_vote_count)(post_count)(can_vote)(voting_flowbar)(voting_power)
             (balance)
             (savings_balance)
             (cbd_balance)(cbd_seconds)(cbd_seconds_last_update)(cbd_last_interest_payment)
             (savings_cbd_balance)(savings_cbd_seconds)(savings_cbd_seconds_last_update)(savings_cbd_last_interest_payment)(savings_withdraw_requests)
             (reward_cbd_balance)(reward_crea_balance)(reward_vesting_balance)(reward_vesting_crea)
             (vesting_shares)(delegated_vesting_shares)(received_vesting_shares)(vesting_withdraw_rate)(next_vesting_withdrawal)(withdrawn)(to_withdraw)(withdraw_routes)
             (curation_rewards)
             (posting_rewards)
             (proxied_vsf_votes)(witnesses_voted_for)
             (last_post)(last_root_post)(last_vote_time)
             (post_bandwidth)(follower_count)(following_count)(pending_claimed_accounts)
          )

FC_REFLECT_DERIVED( crea::plugins::condenser_api::extended_account, (crea::plugins::condenser_api::api_account_object),
            (average_bandwidth)(lifetime_bandwidth)(last_bandwidth_update)(average_market_bandwidth)(lifetime_market_bandwidth)(last_market_bandwidth_update)
            (vesting_balance)(reputation)(transfer_history)(market_history)(post_history)(vote_history)(other_history)(witness_votes)(tags_usage)(guest_bloggers)(open_orders)(comments)(feed)(blog)(recent_replies)(recommended) )

FC_REFLECT( crea::plugins::condenser_api::api_comment_download_object,
            (id)(author)(permlink)
            (resource)(name)(type)(size)(times_downloaded)(price)
)

FC_REFLECT( crea::plugins::condenser_api::api_download_granted_object,
            (id)(payment_date)(comment_author)(comment_permlink)(resource)(price)
)

FC_REFLECT( crea::plugins::condenser_api::api_comment_object,
             (id)(author)(permlink)
             (category)(parent_author)(parent_permlink)
             (title)(body)(download)(json_metadata)(last_update)(created)(active)(last_payout)
             (depth)(children)
             (net_rshares)(abs_rshares)(vote_rshares)
             (children_abs_rshares)(cashout_time)(max_cashout_time)
             (total_vote_weight)(reward_weight)(total_payout_value)(curator_payout_value)(author_rewards)(net_votes)
             (root_author)(root_permlink)
             (max_accepted_payout)(percent_crea_dollars)(allow_replies)(allow_votes)(allow_curation_rewards)
             (beneficiaries)
          )

FC_REFLECT( crea::plugins::condenser_api::extended_dynamic_global_properties,
            (head_block_number)(head_block_id)(time)
            (current_witness)(total_pow)(num_pow_witnesses)
            (virtual_supply)(current_supply)(confidential_supply)(current_cbd_supply)(confidential_cbd_supply)
            (total_vesting_fund_crea)(total_vesting_shares)
            (total_reward_fund_crea)(total_reward_shares2)(pending_rewarded_vesting_shares)(pending_rewarded_vesting_crea)
            (cbd_interest_rate)(cbd_print_rate)
            (maximum_block_size)(current_aslot)(recent_slots_filled)(participation_count)(last_irreversible_block_num)(vote_power_reserve_rate)
            (delegation_return_period)(reverse_auction_seconds)(cbd_stop_percent)(cbd_start_percent)
            (average_block_size)(current_reserve_ratio)(max_virtual_bandwidth) )

FC_REFLECT( crea::plugins::condenser_api::api_witness_object,
             (id)
             (owner)
             (created)
             (url)(votes)(virtual_last_update)(virtual_position)(virtual_scheduled_time)(total_missed)
             (last_aslot)(last_confirmed_block_num)(pow_worker)(signing_key)
             (props)
             (cbd_exchange_rate)(last_cbd_exchange_update)
             (last_work)
             (running_version)
             (hardfork_version_vote)(hardfork_time_vote)
             (available_witness_account_subsidies)
          )

FC_REFLECT( crea::plugins::condenser_api::api_witness_schedule_object,
             (id)
             (current_virtual_time)
             (next_shuffle_block_num)
             (current_shuffled_witnesses)
             (num_scheduled_witnesses)
             (elected_weight)
             (timeshare_weight)
             (miner_weight)
             (witness_pay_normalization_factor)
             (median_props)
             (majority_version)
             (max_voted_witnesses)
             (max_miner_witnesses)
             (max_runner_witnesses)
             (hardfork_required_witnesses)
             (account_subsidy_rd)
             (account_subsidy_witness_rd)
             (min_witness_account_subsidy_decay)
          )

FC_REFLECT( crea::plugins::condenser_api::api_feed_history_object,
             (id)
             (current_median_history)
             (price_history)
          )

FC_REFLECT( crea::plugins::condenser_api::api_reward_fund_object,
            (id)
            (name)
            (reward_balance)
            (recent_claims)
            (last_update)
            (content_constant)
            (percent_curation_rewards)
            (percent_content_rewards)
            (author_reward_curve)
            (curation_reward_curve)
         )

FC_REFLECT( crea::plugins::condenser_api::api_escrow_object,
             (id)(escrow_id)(from)(to)(agent)
             (ratification_deadline)(escrow_expiration)
             (cbd_balance)(crea_balance)(pending_fee)
             (to_approved)(agent_approved)(disputed) )

FC_REFLECT( crea::plugins::condenser_api::api_savings_withdraw_object,
             (id)
             (from)
             (to)
             (memo)
             (request_id)
             (amount)
             (complete)
          )

FC_REFLECT( crea::plugins::condenser_api::api_vesting_delegation_object,
            (id)(delegator)(delegatee)(vesting_shares)(min_delegation_time) )

FC_REFLECT( crea::plugins::condenser_api::api_vesting_delegation_expiration_object,
            (id)(delegator)(vesting_shares)(expiration) )

FC_REFLECT( crea::plugins::condenser_api::api_convert_request_object,
             (id)(owner)(requestid)(amount)(conversion_date) )

FC_REFLECT_DERIVED( crea::plugins::condenser_api::discussion, (crea::plugins::condenser_api::api_comment_object),
             (url)(root_title)(pending_payout_value)(total_pending_payout_value)
             (active_votes)(replies)(author_reputation)(promoted)
             (body_length)(reblogged_by)(first_reblogged_by)(first_reblogged_on)
          )

FC_REFLECT( crea::plugins::condenser_api::scheduled_hardfork,
            (hf_version)(live_time) )

FC_REFLECT( crea::plugins::condenser_api::account_vote,
            (authorperm)(weight)(rshares)(percent)(time) )

FC_REFLECT( crea::plugins::condenser_api::tag_index, (trending) )

FC_REFLECT_ENUM( crea::plugins::condenser_api::withdraw_route_type, (incoming)(outgoing)(all) )

FC_REFLECT( crea::plugins::condenser_api::get_version_return,
            (blockchain_version)(crea_revision)(fc_revision) )

FC_REFLECT( crea::plugins::condenser_api::broadcast_transaction_synchronous_return,
            (id)(block_num)(trx_num)(expired) )

FC_REFLECT( crea::plugins::condenser_api::comment_feed_entry,
            (comment)(reblog_by)(reblog_on)(entry_id) )

FC_REFLECT( crea::plugins::condenser_api::comment_blog_entry,
            (comment)(blog)(reblog_on)(entry_id) )

FC_REFLECT( crea::plugins::condenser_api::ticker,
            (latest)(lowest_ask)(highest_bid)(percent_change)(crea_volume)(cbd_volume) )

FC_REFLECT( crea::plugins::condenser_api::volume,
            (crea_volume)(cbd_volume) )

FC_REFLECT( crea::plugins::condenser_api::order,
            (order_price)(real_price)(crea)(cbd)(created) )

FC_REFLECT( crea::plugins::condenser_api::order_book,
            (bids)(asks) )

FC_REFLECT( crea::plugins::condenser_api::market_trade,
            (date)(current_pays)(open_pays) )
