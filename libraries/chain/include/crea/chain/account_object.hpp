#pragma once
#include <crea/chain/crea_fwd.hpp>

#include <fc/fixed_string.hpp>

#include <crea/protocol/authority.hpp>
#include <crea/protocol/crea_operations.hpp>

#include <crea/chain/crea_object_types.hpp>
#include <crea/chain/witness_objects.hpp>
#include <crea/chain/shared_authority.hpp>
#include <crea/chain/util/flowbar.hpp>

#include <crea/chain/util/delayed_voting_processor.hpp>
#include <crea/chain/util/tiny_asset.hpp>
#include <boost/multi_index/composite_key.hpp>


#include <numeric>

namespace crea { namespace chain {

  using chainbase::t_vector;
  using crea::protocol::authority;

  class account_object : public object< account_object_type, account_object >
  {
    CHAINBASE_OBJECT( account_object );
    public:
      //constructor for creation of regular accounts
      template< typename Allocator >
      account_object( allocator< Allocator > a, uint64_t _id,
        const account_name_type& _name, const public_key_type& _memo_key,
        const time_point_sec& _creation_time, bool _mined,
        const account_name_type& _recovery_account,
        bool _fill_flow, const asset& incoming_delegation )
        : id( _id ), name( _name ), memo_key( _memo_key ), created( _creation_time ), mined( _mined ),
        recovery_account( _recovery_account ), delayed_votes( a )
      {
        received_vesting_shares += incoming_delegation;
        voting_flowbar.last_update_time = _creation_time.sec_since_epoch();
        downvote_flowbar.last_update_time = _creation_time.sec_since_epoch();
        if( _fill_flow )
          voting_flowbar.current_flow = CREA_100_PERCENT;
      }

      //minimal constructor used for creation of accounts at genesis and in tests
      template< typename Allocator >
      account_object( allocator< Allocator > a, uint64_t _id,
        const account_name_type& _name, const public_key_type& _memo_key = public_key_type() )
        : id( _id ), name( _name ), memo_key( _memo_key ), delayed_votes( a )
      {}

      //liquid CREA balance
      asset get_balance() const { return balance; }
      //CREA balance in savings
      asset get_savings() const { return savings_balance; }
      //unclaimed CREA rewards
      asset get_rewards() const { return reward_crea_balance; }

      //liquid CBD balance
      asset get_cbd_balance() const { return cbd_balance; }
      //CBD balance in savings
      asset get_cbd_savings() const { return savings_cbd_balance; }
      //unclaimed CBD rewards
      asset get_cbd_rewards() const { return reward_cbd_balance; }

      //all VESTS held by the account - use other routines to get active VESTS for specific uses
      asset get_vesting() const { return vesting_shares; }
      //VESTS that were delegated to other accounts
      asset get_delegated_vesting() const { return delegated_vesting_shares; }
      //VESTS that were borrowed from other accounts
      asset get_received_vesting() const { return received_vesting_shares; }
      //TODO: add routines for specific uses, f.e. get_witness_voting_power, get_proposal_voting_power, get_post_voting_power...
      //unclaimed VESTS rewards
      asset get_vest_rewards() const { return reward_vesting_balance; }
      //value of unclaimed VESTS rewards in CREA (CREA held on global balance)
      asset get_vest_rewards_as_crea() const { return reward_vesting_crea; }

      account_name_type name;
      public_key_type   memo_key;
      account_name_type proxy;

      time_point_sec    last_account_update;

      time_point_sec    created;
      bool              mined = true;
      account_name_type recovery_account;
      account_name_type reset_account = CREA_NULL_ACCOUNT;
      time_point_sec    last_account_recovery;
      uint32_t          comment_count = 0;
      uint32_t          lifetime_vote_count = 0;
      uint32_t          post_count = 0;

      bool              can_vote = true;
      util::flowbar     voting_flowbar;
      util::flowbar     downvote_flowbar;

      CREA_asset        balance = asset( 0, CREA_SYMBOL );  ///< total liquid shares held by this account
      CREA_asset        savings_balance = asset( 0, CREA_SYMBOL );  ///< total liquid shares held by this account

      /**
        *  CBD Deposits pay interest based upon the interest rate set by witnesses. The purpose of these
        *  fields is to track the total (time * cbd_balance) that it is held. Then at the appointed time
        *  interest can be paid using the following equation:
        *
        *  interest = interest_rate * cbd_seconds / seconds_per_year
        *
        *  Every time the cbd_balance is updated the cbd_seconds is also updated. If at least
        *  CREA_CBD_INTEREST_COMPOUND_INTERVAL_SEC has passed since cbd_last_interest_payment then
        *  interest is added to cbd_balance.
        *
        *  @defgroup cbd_data CBD Balance Data
        */
      ///@{
      CBD_asset         cbd_balance = asset( 0, CBD_SYMBOL ); /// total CBD balance
      uint128_t         cbd_seconds; ///< total CBD * how long it has been held
      time_point_sec    cbd_seconds_last_update; ///< the last time the cbd_seconds was updated
      time_point_sec    cbd_last_interest_payment; ///< used to pay interest at most once per month


      CBD_asset         savings_cbd_balance = asset( 0, CBD_SYMBOL ); /// total CBD balance
      uint128_t         savings_cbd_seconds; ///< total CBD * how long it has been held
      time_point_sec    savings_cbd_seconds_last_update; ///< the last time the cbd_seconds was updated
      time_point_sec    savings_cbd_last_interest_payment; ///< used to pay interest at most once per month

      uint8_t           savings_withdraw_requests = 0;
      ///@}

      CBD_asset         reward_cbd_balance = asset( 0, CBD_SYMBOL );
      CREA_asset        reward_crea_balance = asset( 0, CREA_SYMBOL );
      VEST_asset        reward_vesting_balance = asset( 0, VESTS_SYMBOL );
      CREA_asset        reward_vesting_crea = asset( 0, CREA_SYMBOL );

      share_type        curation_rewards = 0;
      share_type        posting_rewards = 0;

      VEST_asset        vesting_shares = asset( 0, VESTS_SYMBOL ); ///< total vesting shares held by this account, controls its voting power
      VEST_asset        delegated_vesting_shares = asset( 0, VESTS_SYMBOL );
      VEST_asset        received_vesting_shares = asset( 0, VESTS_SYMBOL );

      VEST_asset        vesting_withdraw_rate = asset( 0, VESTS_SYMBOL ); ///< at the time this is updated it can be at most vesting_shares/104
      time_point_sec    next_vesting_withdrawal = fc::time_point_sec::maximum(); ///< after every withdrawal this is incremented by 1 week
      share_type        withdrawn = 0; /// Track how many shares have been withdrawn
      share_type        to_withdraw = 0; /// Might be able to look this up with operation history.
      uint16_t          withdraw_routes = 0; //max 10, why is it 16bit?
      uint16_t          pending_transfers = 0; //for now max is 255, but it might change

      fc::array<share_type, CREA_MAX_PROXY_RECURSION_DEPTH> proxied_vsf_votes;// = std::vector<share_type>( CREA_MAX_PROXY_RECURSION_DEPTH, 0 ); ///< the total VFS votes proxied to this account

      uint16_t          witnesses_voted_for = 0; //max 30, why is it 16bit?

      time_point_sec    last_post;
      time_point_sec    last_root_post = fc::time_point_sec::min();
      time_point_sec    last_post_edit;
      time_point_sec    last_vote_time;
      uint32_t          post_bandwidth = 0;

      share_type        pending_claimed_accounts = 0;

      using t_delayed_votes = t_vector< delayed_votes_data >;
      /*
        Holds sum of VESTS per day.
        VESTS from day `X` will be matured after `X` + 30 days ( because `CREA_DELAYED_VOTING_TOTAL_INTERVAL_SECONDS` == 30 days )
      */
      t_delayed_votes   delayed_votes;
      /*
        Total sum of VESTS from `delayed_votes` collection.
        It's a helper variable needed for better performance.
      */
      ushare_type       sum_delayed_votes = 0;

      time_point_sec get_the_earliest_time() const
      {
        if( !delayed_votes.empty() )
          return ( delayed_votes.begin() )->time;
        else
          return time_point_sec::maximum();
      }

      share_type get_real_vesting_shares() const
      {
        FC_ASSERT( sum_delayed_votes.value <= vesting_shares.amount, "",
                ( "sum_delayed_votes",     sum_delayed_votes )
                ( "vesting_shares.amount", vesting_shares.amount )
                ( "account",               name ) );

        return asset( vesting_shares.amount - sum_delayed_votes.value, VESTS_SYMBOL ).amount;
      }

      /// This function should be used only when the account votes for a witness directly
      share_type        witness_vote_weight()const {
        return std::accumulate( proxied_vsf_votes.begin(),
                        proxied_vsf_votes.end(),
                        get_real_vesting_shares()
                        );
      }
      share_type        proxied_vsf_votes_total()const {
        return std::accumulate( proxied_vsf_votes.begin(),
                        proxied_vsf_votes.end(),
                        share_type() );
      }

    CHAINBASE_UNPACK_CONSTRUCTOR(account_object, (delayed_votes));
  };

  class account_metadata_object : public object< account_metadata_object_type, account_metadata_object >
  {
    CHAINBASE_OBJECT( account_metadata_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( account_metadata_object, (json_metadata)(posting_json_metadata) )

      account_id_type   account;
      shared_string     json_metadata;
      shared_string     posting_json_metadata;

    CHAINBASE_UNPACK_CONSTRUCTOR(account_metadata_object, (json_metadata)(posting_json_metadata));
  };

  class account_authority_object : public object< account_authority_object_type, account_authority_object >
  {
    CHAINBASE_OBJECT( account_authority_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( account_authority_object, (owner)(active)(posting) )

      account_name_type account;

      shared_authority  owner;   ///< used for backup control, can set owner or active
      shared_authority  active;  ///< used for all monetary operations, can set active or posting
      shared_authority  posting; ///< used for voting and posting

      time_point_sec    last_owner_update;

    CHAINBASE_UNPACK_CONSTRUCTOR(account_authority_object, (owner)(active)(posting));
  };

  class vesting_delegation_object : public object< vesting_delegation_object_type, vesting_delegation_object >
  {
    CHAINBASE_OBJECT( vesting_delegation_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( vesting_delegation_object )

      //amount of delegated VESTS
      const asset& get_vesting() const { return vesting_shares; }

      account_name_type delegator;
      account_name_type delegatee;
      asset             vesting_shares = asset( 0, VESTS_SYMBOL );
      time_point_sec    min_delegation_time;

    CHAINBASE_UNPACK_CONSTRUCTOR(vesting_delegation_object);
  };

  class vesting_delegation_expiration_object : public object< vesting_delegation_expiration_object_type, vesting_delegation_expiration_object >
  {
    CHAINBASE_OBJECT( vesting_delegation_expiration_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( vesting_delegation_expiration_object )

      //amount of expiring delegated VESTS
      const asset& get_vesting() const { return vesting_shares; }

      account_name_type delegator;
      asset             vesting_shares = asset( 0, VESTS_SYMBOL );
      time_point_sec    expiration;

    CHAINBASE_UNPACK_CONSTRUCTOR(vesting_delegation_expiration_object);
  };

  class owner_authority_history_object : public object< owner_authority_history_object_type, owner_authority_history_object >
  {
    CHAINBASE_OBJECT( owner_authority_history_object );
    public:
      template< typename Allocator >
      owner_authority_history_object( allocator< Allocator > a, uint64_t _id,
        const account_object& _account, const shared_authority& _previous_owner, const time_point_sec& _creation_time )
        : id( _id ), account( _account.name ), previous_owner_authority( allocator< shared_authority >( a ) ),
        last_valid_time( _creation_time )
      {
        previous_owner_authority = _previous_owner;
      }

      account_name_type account;
      shared_authority  previous_owner_authority;
      time_point_sec    last_valid_time;
    CHAINBASE_UNPACK_CONSTRUCTOR(owner_authority_history_object, (previous_owner_authority));
  };

  class account_recovery_request_object : public object< account_recovery_request_object_type, account_recovery_request_object >
  {
    CHAINBASE_OBJECT( account_recovery_request_object );
    public:
      template< typename Allocator >
      account_recovery_request_object( allocator< Allocator > a, uint64_t _id,
        const account_name_type& _account_to_recover, const authority& _new_owner_authority, const time_point_sec& _expiration_time )
        : id( _id ), account_to_recover( _account_to_recover ), new_owner_authority( allocator< shared_authority >( a ) ),
        expires( _expiration_time )
      {
        new_owner_authority = _new_owner_authority;
      }

      account_name_type account_to_recover;
      shared_authority  new_owner_authority;
      time_point_sec    expires;
    CHAINBASE_UNPACK_CONSTRUCTOR(account_recovery_request_object, (new_owner_authority));
  };

  class change_recovery_account_request_object : public object< change_recovery_account_request_object_type, change_recovery_account_request_object >
  {
    CHAINBASE_OBJECT( change_recovery_account_request_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( change_recovery_account_request_object )

      account_name_type account_to_recover;
      account_name_type recovery_account;
      time_point_sec    effective_on;
    CHAINBASE_UNPACK_CONSTRUCTOR(change_recovery_account_request_object);
  };

  struct by_proxy;
  struct by_next_vesting_withdrawal;
  struct by_delayed_voting;
  /**
    * @ingroup object_index
    */
  typedef multi_index_container<
    account_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< account_object, account_object::id_type, &account_object::get_id > >,
      ordered_unique< tag< by_name >,
        member< account_object, account_name_type, &account_object::name > >,
      ordered_unique< tag< by_proxy >,
        composite_key< account_object,
          member< account_object, account_name_type, &account_object::proxy >,
          member< account_object, account_name_type, &account_object::name >
        > /// composite key by proxy
      >,
      ordered_unique< tag< by_next_vesting_withdrawal >,
        composite_key< account_object,
          member< account_object, time_point_sec, &account_object::next_vesting_withdrawal >,
          member< account_object, account_name_type, &account_object::name >
        > /// composite key by_next_vesting_withdrawal
      >,
      ordered_unique< tag< by_delayed_voting >,
        composite_key< account_object,
          const_mem_fun< account_object, time_point_sec, &account_object::get_the_earliest_time >,
          const_mem_fun< account_object, account_object::id_type, &account_object::get_id >
        >
      >
    >,
    allocator< account_object >
  > account_index;

  struct by_account;

  typedef multi_index_container <
    account_metadata_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< account_metadata_object, account_metadata_object::id_type, &account_metadata_object::get_id > >,
      ordered_unique< tag< by_account >,
        member< account_metadata_object, account_id_type, &account_metadata_object::account > >
    >,
    allocator< account_metadata_object >
  > account_metadata_index;

  typedef multi_index_container <
    owner_authority_history_object,
    indexed_by <
      ordered_unique< tag< by_id >,
        const_mem_fun< owner_authority_history_object, owner_authority_history_object::id_type, &owner_authority_history_object::get_id > >,
      ordered_unique< tag< by_account >,
        composite_key< owner_authority_history_object,
          member< owner_authority_history_object, account_name_type, &owner_authority_history_object::account >,
          member< owner_authority_history_object, time_point_sec, &owner_authority_history_object::last_valid_time >,
          const_mem_fun< owner_authority_history_object, owner_authority_history_object::id_type, &owner_authority_history_object::get_id >
        >,
        composite_key_compare< std::less< account_name_type >, std::less< time_point_sec >, std::less< owner_authority_history_id_type > >
      >
    >,
    allocator< owner_authority_history_object >
  > owner_authority_history_index;

  struct by_last_owner_update;

  typedef multi_index_container <
    account_authority_object,
    indexed_by <
      ordered_unique< tag< by_id >,
        const_mem_fun< account_authority_object, account_authority_object::id_type, &account_authority_object::get_id > >,
      ordered_unique< tag< by_account >,
        composite_key< account_authority_object,
          member< account_authority_object, account_name_type, &account_authority_object::account >,
          const_mem_fun< account_authority_object, account_authority_object::id_type, &account_authority_object::get_id >
        >,
        composite_key_compare< std::less< account_name_type >, std::less< account_authority_id_type > >
      >,
      ordered_unique< tag< by_last_owner_update >,
        composite_key< account_authority_object,
          member< account_authority_object, time_point_sec, &account_authority_object::last_owner_update >,
          const_mem_fun< account_authority_object, account_authority_object::id_type, &account_authority_object::get_id >
        >,
        composite_key_compare< std::greater< time_point_sec >, std::less< account_authority_id_type > >
      >
    >,
    allocator< account_authority_object >
  > account_authority_index;

  struct by_delegation;

  typedef multi_index_container <
    vesting_delegation_object,
    indexed_by <
      ordered_unique< tag< by_id >,
        const_mem_fun< vesting_delegation_object, vesting_delegation_object::id_type, &vesting_delegation_object::get_id > >,
      ordered_unique< tag< by_delegation >,
        composite_key< vesting_delegation_object,
          member< vesting_delegation_object, account_name_type, &vesting_delegation_object::delegator >,
          member< vesting_delegation_object, account_name_type, &vesting_delegation_object::delegatee >
        >,
        composite_key_compare< std::less< account_name_type >, std::less< account_name_type > >
      >
    >,
    allocator< vesting_delegation_object >
  > vesting_delegation_index;

  struct by_expiration;
  struct by_account_expiration;

  typedef multi_index_container <
    vesting_delegation_expiration_object,
    indexed_by <
      ordered_unique< tag< by_id >,
        const_mem_fun< vesting_delegation_expiration_object, vesting_delegation_expiration_object::id_type, &vesting_delegation_expiration_object::get_id > >,
      ordered_unique< tag< by_expiration >,
        composite_key< vesting_delegation_expiration_object,
          member< vesting_delegation_expiration_object, time_point_sec, &vesting_delegation_expiration_object::expiration >,
          const_mem_fun< vesting_delegation_expiration_object, vesting_delegation_expiration_object::id_type, &vesting_delegation_expiration_object::get_id >
        >,
        composite_key_compare< std::less< time_point_sec >, std::less< vesting_delegation_expiration_id_type > >
      >,
      ordered_unique< tag< by_account_expiration >,
        composite_key< vesting_delegation_expiration_object,
          member< vesting_delegation_expiration_object, account_name_type, &vesting_delegation_expiration_object::delegator >,
          member< vesting_delegation_expiration_object, time_point_sec, &vesting_delegation_expiration_object::expiration >,
          const_mem_fun< vesting_delegation_expiration_object, vesting_delegation_expiration_object::id_type, &vesting_delegation_expiration_object::get_id >
        >,
        composite_key_compare< std::less< account_name_type >, std::less< time_point_sec >, std::less< vesting_delegation_expiration_id_type > >
      >
    >,
    allocator< vesting_delegation_expiration_object >
  > vesting_delegation_expiration_index;

  struct by_expiration;

  typedef multi_index_container <
    account_recovery_request_object,
    indexed_by <
      ordered_unique< tag< by_id >,
        const_mem_fun< account_recovery_request_object, account_recovery_request_object::id_type, &account_recovery_request_object::get_id > >,
      ordered_unique< tag< by_account >,
        member< account_recovery_request_object, account_name_type, &account_recovery_request_object::account_to_recover >
      >,
      ordered_unique< tag< by_expiration >,
        composite_key< account_recovery_request_object,
          member< account_recovery_request_object, time_point_sec, &account_recovery_request_object::expires >,
          member< account_recovery_request_object, account_name_type, &account_recovery_request_object::account_to_recover >
        >,
        composite_key_compare< std::less< time_point_sec >, std::less< account_name_type > >
      >
    >,
    allocator< account_recovery_request_object >
  > account_recovery_request_index;

  struct by_effective_date;

  typedef multi_index_container <
    change_recovery_account_request_object,
    indexed_by <
      ordered_unique< tag< by_id >,
        const_mem_fun< change_recovery_account_request_object, change_recovery_account_request_object::id_type, &change_recovery_account_request_object::get_id > >,
      ordered_unique< tag< by_account >,
        member< change_recovery_account_request_object, account_name_type, &change_recovery_account_request_object::account_to_recover >
      >,
      ordered_unique< tag< by_effective_date >,
        composite_key< change_recovery_account_request_object,
          member< change_recovery_account_request_object, time_point_sec, &change_recovery_account_request_object::effective_on >,
          member< change_recovery_account_request_object, account_name_type, &change_recovery_account_request_object::account_to_recover >
        >,
        composite_key_compare< std::less< time_point_sec >, std::less< account_name_type > >
      >
    >,
    allocator< change_recovery_account_request_object >
  > change_recovery_account_request_index;
} }

#ifdef ENABLE_MIRA
namespace mira {

template<> struct is_static_length< crea::chain::vesting_delegation_object > : public boost::true_type {};
template<> struct is_static_length< crea::chain::vesting_delegation_expiration_object > : public boost::true_type {};
template<> struct is_static_length< crea::chain::change_recovery_account_request_object > : public boost::true_type {};

} // mira
#endif

FC_REFLECT( crea::chain::account_object,
          (id)(name)(memo_key)(proxy)(last_account_update)
          (created)(mined)
          (recovery_account)(last_account_recovery)(reset_account)
          (comment_count)(lifetime_vote_count)(post_count)(can_vote)(voting_flowbar)(downvote_flowbar)
          (balance)
          (savings_balance)
          (cbd_balance)(cbd_seconds)(cbd_seconds_last_update)(cbd_last_interest_payment)
          (savings_cbd_balance)(savings_cbd_seconds)(savings_cbd_seconds_last_update)(savings_cbd_last_interest_payment)(savings_withdraw_requests)
          (reward_crea_balance)(reward_cbd_balance)(reward_vesting_balance)(reward_vesting_crea)
          (vesting_shares)(delegated_vesting_shares)(received_vesting_shares)
          (vesting_withdraw_rate)(next_vesting_withdrawal)(withdrawn)(to_withdraw)(withdraw_routes)
          (pending_transfers)(curation_rewards)
          (posting_rewards)
          (proxied_vsf_votes)(witnesses_voted_for)
          (last_post)(last_root_post)(last_post_edit)(last_vote_time)(post_bandwidth)
          (pending_claimed_accounts)
          (delayed_votes)
          (sum_delayed_votes)
        )

CHAINBASE_SET_INDEX_TYPE( crea::chain::account_object, crea::chain::account_index )

FC_REFLECT( crea::chain::account_metadata_object,
          (id)(account)(json_metadata)(posting_json_metadata) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::account_metadata_object, crea::chain::account_metadata_index )

FC_REFLECT( crea::chain::account_authority_object,
          (id)(account)(owner)(active)(posting)(last_owner_update)
)
CHAINBASE_SET_INDEX_TYPE( crea::chain::account_authority_object, crea::chain::account_authority_index )

FC_REFLECT( crea::chain::vesting_delegation_object,
        (id)(delegator)(delegatee)(vesting_shares)(min_delegation_time) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::vesting_delegation_object, crea::chain::vesting_delegation_index )

FC_REFLECT( crea::chain::vesting_delegation_expiration_object,
        (id)(delegator)(vesting_shares)(expiration) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::vesting_delegation_expiration_object, crea::chain::vesting_delegation_expiration_index )

FC_REFLECT( crea::chain::owner_authority_history_object,
          (id)(account)(previous_owner_authority)(last_valid_time)
        )
CHAINBASE_SET_INDEX_TYPE( crea::chain::owner_authority_history_object, crea::chain::owner_authority_history_index )

FC_REFLECT( crea::chain::account_recovery_request_object,
          (id)(account_to_recover)(new_owner_authority)(expires)
        )
CHAINBASE_SET_INDEX_TYPE( crea::chain::account_recovery_request_object, crea::chain::account_recovery_request_index )

FC_REFLECT( crea::chain::change_recovery_account_request_object,
          (id)(account_to_recover)(recovery_account)(effective_on)
        )
CHAINBASE_SET_INDEX_TYPE( crea::chain::change_recovery_account_request_object, crea::chain::change_recovery_account_request_index )

namespace helpers
{
  template <>
  class index_statistic_provider<crea::chain::account_index>
  {
  public:
    typedef crea::chain::account_index IndexType;
    typedef typename crea::chain::account_object::t_delayed_votes t_delayed_votes;

    index_statistic_info gather_statistics(const IndexType& index, bool onlyStaticInfo) const
    {
      index_statistic_info info;
      gather_index_static_data(index, &info);

      if(onlyStaticInfo == false)
      {
        for(const auto& o : index)
        {
          info._item_additional_allocation += o.delayed_votes.capacity()*sizeof(t_delayed_votes::value_type);
        }
      }

      return info;
    }
  };

} /// namespace helpers
