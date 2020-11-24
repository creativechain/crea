#pragma once
#include <crea/chain/crea_fwd.hpp>

#include <crea/protocol/authority.hpp>
#include <crea/protocol/crea_operations.hpp>
#include <crea/protocol/misc_utilities.hpp>

#include <crea/chain/crea_object_types.hpp>

#include <boost/multiprecision/cpp_int.hpp>


namespace crea { namespace chain {

  using crea::protocol::asset;
  using crea::protocol::price;
  using crea::protocol::asset_symbol_type;
  using chainbase::t_deque;

  typedef protocol::fixed_string< 16 > reward_fund_name_type;

  /**
    *  This object is used to track pending requests to convert CBD to CREA
    */
  class convert_request_object : public object< convert_request_object_type, convert_request_object >
  {
    CHAINBASE_OBJECT( convert_request_object );
    public:
      template< typename Allocator >
      convert_request_object( allocator< Allocator > a, uint64_t _id,
        const account_name_type& _owner, const asset& _amount, const time_point_sec& _conversion_time, uint32_t _requestid )
        : id( _id ), owner( _owner ), requestid( _requestid ), amount( _amount ), conversion_date( _conversion_time )
      {}

      //amount of CBD to be converted to CREA
      const asset& get_convert_amount() const { return amount; }

      account_name_type owner; //< TODO: can be replaced with account_id_type
      uint32_t          requestid = 0; ///< id set by owner, the owner,requestid pair must be unique
      asset             amount; //< TODO: can be replaced with CBD_asset
      time_point_sec    conversion_date; ///< at this time the feed_history_median_price * amount

    CHAINBASE_UNPACK_CONSTRUCTOR(convert_request_object);
  };


  class escrow_object : public object< escrow_object_type, escrow_object >
  {
    CHAINBASE_OBJECT( escrow_object );
    public:
      template< typename Allocator >
      escrow_object( allocator< Allocator > a, uint64_t _id,
        const account_name_type& _from, const account_name_type& _to, const account_name_type& _agent,
        const asset& _crea_amount, const asset& _cbd_amount, const asset& _fee,
        const time_point_sec& _ratification_deadline, const time_point_sec& _escrow_expiration, uint32_t _escrow_transfer_id )
        : id( _id ), escrow_id( _escrow_transfer_id ), from( _from ), to( _to ), agent( _agent ),
        ratification_deadline( _ratification_deadline ), escrow_expiration( _escrow_expiration ),
        cbd_balance( _cbd_amount ), crea_balance( _crea_amount ), pending_fee( _fee )
      {}

      //CREA portion of transfer balance
      const asset& get_crea_balance() const { return crea_balance; }
      //CBD portion of transfer balance
      const asset& get_cbd_balance() const { return cbd_balance; }
      //fee offered to escrow (can be either in CREA or CBD)
      const asset& get_fee() const { return pending_fee; }

      bool is_approved() const { return to_approved && agent_approved; }

      uint32_t          escrow_id = 20;
      account_name_type from; //< TODO: can be replaced with account_id_type
      account_name_type to; //< TODO: can be replaced with account_id_type
      account_name_type agent; //< TODO: can be replaced with account_id_type
      time_point_sec    ratification_deadline;
      time_point_sec    escrow_expiration;
      asset             cbd_balance; //< TODO: can be replaced with CBD_asset
      asset             crea_balance; //< TODO: can be replaced with CREA_asset
      asset             pending_fee; //fee can use CREA of CBD
      bool              to_approved = false; //< TODO: can be replaced with bit field along with all flags
      bool              agent_approved = false;
      bool              disputed = false;

    CHAINBASE_UNPACK_CONSTRUCTOR(escrow_object);
  };


  class savings_withdraw_object : public object< savings_withdraw_object_type, savings_withdraw_object >
  {
    CHAINBASE_OBJECT( savings_withdraw_object );
    public:
      template< typename Allocator >
      savings_withdraw_object( allocator< Allocator > a, uint64_t _id,
        const account_name_type& _from, const account_name_type& _to, const asset& _amount,
        const string& _memo, const time_point_sec& _time_of_completion, uint32_t _request_id )
        : id( _id ), from( _from ), to( _to ), memo( a ), request_id( _request_id ),
        amount( _amount ), complete( _time_of_completion )
      {
#ifndef IS_LOW_MEM
        from_string( memo, _memo );
#endif
      }

      //amount of savings to withdraw (CREA or CBD)
      const asset& get_withdraw_amount() const { return amount; }

      account_name_type from; //< TODO: can be replaced with account_id_type
      account_name_type to; //< TODO: can be replaced with account_id_type
      shared_string     memo;
      uint32_t          request_id = 0;
      asset             amount; //can be expressed in CREA or CBD
      time_point_sec    complete;

    CHAINBASE_UNPACK_CONSTRUCTOR(savings_withdraw_object, (memo));
  };


  /**
    *  If last_update is greater than 1 week, then volume gets reset to 0
    *
    *  When a user is a maker, their volume increases
    *  When a user is a taker, their volume decreases
    *
    *  Every 1000 blocks, the account that has the highest volume_weight() is paid the maximum of
    *  1000 CREA or 1000 * virtual_supply / (100*blocks_per_year) aka 10 * virtual_supply / blocks_per_year
    *
    *  After being paid volume gets reset to 0
    */
  class liquidity_reward_balance_object : public object< liquidity_reward_balance_object_type, liquidity_reward_balance_object >
  {
    CHAINBASE_OBJECT( liquidity_reward_balance_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( liquidity_reward_balance_object )

      int64_t get_crea_volume() const { return crea_volume; }
      int64_t get_cbd_volume() const { return cbd_volume; }

      account_id_type   owner;
      int64_t           crea_volume = 0;
      int64_t           cbd_volume = 0;
      uint128_t         weight = 0;

      time_point_sec    last_update = fc::time_point_sec::min(); /// used to decay negative liquidity balances. block num

      /// this is the sort index
      uint128_t volume_weight()const
      {
        return crea_volume * cbd_volume * is_positive();
      }

      uint128_t min_volume_weight()const
      {
        return std::min(crea_volume,cbd_volume) * is_positive();
      }

      void update_weight( bool hf9 )
      {
          weight = hf9 ? min_volume_weight() : volume_weight();
      }

      inline int is_positive()const
      {
        return ( crea_volume > 0 && cbd_volume > 0 ) ? 1 : 0;
      }
    CHAINBASE_UNPACK_CONSTRUCTOR(liquidity_reward_balance_object);
  };


  /**
    *  This object gets updated once per hour, on the hour
    */
  class feed_history_object  : public object< feed_history_object_type, feed_history_object >
  {
    CHAINBASE_OBJECT( feed_history_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( feed_history_object, (price_history) )

      price current_median_history; ///< the current median of the price history, used as the base for convert operations

      using t_price_history = t_deque< price >;

      t_deque< price > price_history; ///< tracks this last week of median_feed one per hour

    CHAINBASE_UNPACK_CONSTRUCTOR(feed_history_object, (price_history));
  };


  /**
    *  @brief an offer to sell a amount of a asset at a specified exchange rate by a certain time
    *  @ingroup object
    *  @ingroup protocol
    *  @ingroup market
    *
    *  This limit_order_objects are indexed by @ref expiration and is automatically deleted on the first block after expiration.
    */
  class limit_order_object : public object< limit_order_object_type, limit_order_object >
  {
    CHAINBASE_OBJECT( limit_order_object );
    public:
      template< typename Allocator >
      limit_order_object( allocator< Allocator > a, uint64_t _id,
        const account_name_type& _seller, const asset& _amount_to_sell, const price& _sell_price,
        const time_point_sec& _creation_time, const time_point_sec& _expiration_time, uint32_t _orderid )
        : id( _id ), created( _creation_time ), expiration( _expiration_time ), seller( _seller ),
        orderid( _orderid ), for_sale( _amount_to_sell.amount ), sell_price( _sell_price )
      {
        FC_ASSERT( _amount_to_sell.symbol == _sell_price.base.symbol );
      }

      pair< asset_symbol_type, asset_symbol_type > get_market() const
      {
        return sell_price.base.symbol < sell_price.quote.symbol ?
          std::make_pair( sell_price.base.symbol, sell_price.quote.symbol ) :
          std::make_pair( sell_price.quote.symbol, sell_price.base.symbol );
      }

      asset amount_for_sale() const { return asset( for_sale, sell_price.base.symbol ); }
      asset amount_to_receive() const { return amount_for_sale() * sell_price; }

      time_point_sec    created;
      time_point_sec    expiration;
      account_name_type seller; //< TODO: can be replaced with account_id_type
      uint32_t          orderid = 0;
      share_type        for_sale; ///< asset id is sell_price.base.symbol
      price             sell_price;

      CHAINBASE_UNPACK_CONSTRUCTOR(limit_order_object);
  };


  /**
    * @breif a route to send withdrawn vesting shares.
    */
  class withdraw_vesting_route_object : public object< withdraw_vesting_route_object_type, withdraw_vesting_route_object >
  {
    CHAINBASE_OBJECT( withdraw_vesting_route_object, true );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( withdraw_vesting_route_object )

      account_name_type from_account;
      account_name_type to_account;
      uint16_t          percent = 0;
      bool              auto_vest = false;

    CHAINBASE_UNPACK_CONSTRUCTOR(withdraw_vesting_route_object);
  };


  class decline_voting_rights_request_object : public object< decline_voting_rights_request_object_type, decline_voting_rights_request_object >
  {
    CHAINBASE_OBJECT( decline_voting_rights_request_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( decline_voting_rights_request_object )

      account_name_type account;
      time_point_sec    effective_date;

    CHAINBASE_UNPACK_CONSTRUCTOR(decline_voting_rights_request_object);
  };

  class reward_fund_object : public object< reward_fund_object_type, reward_fund_object >
  {
    CHAINBASE_OBJECT( reward_fund_object );
    public:
      template< typename Allocator >
      reward_fund_object( allocator< Allocator > a, uint64_t _id,
        const string& _name, const asset& _balance, const time_point_sec& _creation_time, const uint128_t& _claims = 0 )
        : id( _id ), name( _name ), reward_balance( _balance ), recent_claims( _claims ), last_update( _creation_time )
      {}

      //amount of CREA in reward fund
      const asset& get_reward_balance() const { return reward_balance; }

      reward_fund_name_type   name;
      asset                   reward_balance = asset( 0, CREA_SYMBOL );
      uint128_t               recent_claims = 0;
      time_point_sec          last_update;
      uint128_t               content_constant = CREA_CONTENT_CONSTANT_HF0;
      uint16_t                percent_curation_rewards = CREA_1_PERCENT * 25;
      uint16_t                percent_content_rewards = CREA_100_PERCENT;
      protocol::curve_id      author_reward_curve = protocol::curve_id::quadratic;
      protocol::curve_id      curation_reward_curve = protocol::curve_id::bounded_curation;

    CHAINBASE_UNPACK_CONSTRUCTOR(reward_fund_object);
  };

  struct by_price;
  struct by_expiration;
  struct by_account;
  typedef multi_index_container<
    limit_order_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< limit_order_object, limit_order_object::id_type, &limit_order_object::get_id > >,
      ordered_unique< tag< by_expiration >,
        composite_key< limit_order_object,
          member< limit_order_object, time_point_sec, &limit_order_object::expiration >,
          const_mem_fun< limit_order_object, limit_order_object::id_type, &limit_order_object::get_id >
        >
      >,
      ordered_unique< tag< by_price >,
        composite_key< limit_order_object,
          member< limit_order_object, price, &limit_order_object::sell_price >,
          const_mem_fun< limit_order_object, limit_order_object::id_type, &limit_order_object::get_id >
        >,
        composite_key_compare< std::greater< price >, std::less< limit_order_id_type > >
      >,
      ordered_unique< tag< by_account >,
        composite_key< limit_order_object,
          member< limit_order_object, account_name_type, &limit_order_object::seller >,
          member< limit_order_object, uint32_t, &limit_order_object::orderid >
        >
      >
    >,
    allocator< limit_order_object >
  > limit_order_index;

  struct by_owner;
  struct by_conversion_date;
  typedef multi_index_container<
    convert_request_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< convert_request_object, convert_request_object::id_type, &convert_request_object::get_id > >,
      ordered_unique< tag< by_conversion_date >,
        composite_key< convert_request_object,
          member< convert_request_object, time_point_sec, &convert_request_object::conversion_date >,
          const_mem_fun< convert_request_object, convert_request_object::id_type, &convert_request_object::get_id >
        >
      >,
      ordered_unique< tag< by_owner >,
        composite_key< convert_request_object,
          member< convert_request_object, account_name_type, &convert_request_object::owner >,
          member< convert_request_object, uint32_t, &convert_request_object::requestid >
        >
      >
    >,
    allocator< convert_request_object >
  > convert_request_index;

  struct by_owner;
  struct by_volume_weight;

  typedef multi_index_container<
    liquidity_reward_balance_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< liquidity_reward_balance_object, liquidity_reward_balance_object::id_type, &liquidity_reward_balance_object::get_id > >,
      ordered_unique< tag< by_owner >,
        member< liquidity_reward_balance_object, account_id_type, &liquidity_reward_balance_object::owner > >,
      ordered_unique< tag< by_volume_weight >,
        composite_key< liquidity_reward_balance_object,
            member< liquidity_reward_balance_object, fc::uint128, &liquidity_reward_balance_object::weight >,
            member< liquidity_reward_balance_object, account_id_type, &liquidity_reward_balance_object::owner >
        >,
        composite_key_compare< std::greater< fc::uint128 >, std::less< account_id_type > >
      >
    >,
    allocator< liquidity_reward_balance_object >
  > liquidity_reward_balance_index;

  typedef multi_index_container<
    feed_history_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< feed_history_object, feed_history_object::id_type, &feed_history_object::get_id > >
    >,
    allocator< feed_history_object >
  > feed_history_index;

  struct by_withdraw_route;
  struct by_destination;
  typedef multi_index_container<
    withdraw_vesting_route_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< withdraw_vesting_route_object, withdraw_vesting_route_object::id_type, &withdraw_vesting_route_object::get_id > >,
      ordered_unique< tag< by_withdraw_route >,
        composite_key< withdraw_vesting_route_object,
          member< withdraw_vesting_route_object, account_name_type, &withdraw_vesting_route_object::from_account >,
          member< withdraw_vesting_route_object, account_name_type, &withdraw_vesting_route_object::to_account >
        >,
        composite_key_compare< std::less< account_name_type >, std::less< account_name_type > >
      >,
      ordered_unique< tag< by_destination >,
        composite_key< withdraw_vesting_route_object,
          member< withdraw_vesting_route_object, account_name_type, &withdraw_vesting_route_object::to_account >,
          const_mem_fun< withdraw_vesting_route_object, withdraw_vesting_route_object::id_type, &withdraw_vesting_route_object::get_id >
        >
      >
    >,
    allocator< withdraw_vesting_route_object >
  > withdraw_vesting_route_index;

  struct by_from_id;
  struct by_ratification_deadline;
  typedef multi_index_container<
    escrow_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< escrow_object, escrow_object::id_type, &escrow_object::get_id > >,
      ordered_unique< tag< by_from_id >,
        composite_key< escrow_object,
          member< escrow_object, account_name_type, &escrow_object::from >,
          member< escrow_object, uint32_t, &escrow_object::escrow_id >
        >
      >,
      ordered_unique< tag< by_ratification_deadline >,
        composite_key< escrow_object,
          const_mem_fun< escrow_object, bool, &escrow_object::is_approved >,
          member< escrow_object, time_point_sec, &escrow_object::ratification_deadline >,
          const_mem_fun< escrow_object, escrow_object::id_type, &escrow_object::get_id >
        >,
        composite_key_compare< std::less< bool >, std::less< time_point_sec >, std::less< escrow_id_type > >
      >
    >,
    allocator< escrow_object >
  > escrow_index;

  struct by_from_rid;
  struct by_to_complete;
  struct by_complete_from_rid;
  typedef multi_index_container<
    savings_withdraw_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< savings_withdraw_object, savings_withdraw_object::id_type, &savings_withdraw_object::get_id > >,
      ordered_unique< tag< by_from_rid >,
        composite_key< savings_withdraw_object,
          member< savings_withdraw_object, account_name_type, &savings_withdraw_object::from >,
          member< savings_withdraw_object, uint32_t, &savings_withdraw_object::request_id >
        >
      >,
      ordered_unique< tag< by_complete_from_rid >,
        composite_key< savings_withdraw_object,
          member< savings_withdraw_object, time_point_sec, &savings_withdraw_object::complete >,
          member< savings_withdraw_object, account_name_type, &savings_withdraw_object::from >,
          member< savings_withdraw_object, uint32_t, &savings_withdraw_object::request_id >
        >
      >,
      ordered_unique< tag< by_to_complete >,
        composite_key< savings_withdraw_object,
          member< savings_withdraw_object, account_name_type, &savings_withdraw_object::to >,
          member< savings_withdraw_object, time_point_sec, &savings_withdraw_object::complete >,
          const_mem_fun< savings_withdraw_object, savings_withdraw_object::id_type, &savings_withdraw_object::get_id >
        >
      >
    >,
    allocator< savings_withdraw_object >
  > savings_withdraw_index;

  struct by_account;
  struct by_effective_date;
  typedef multi_index_container<
    decline_voting_rights_request_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< decline_voting_rights_request_object, decline_voting_rights_request_object::id_type, &decline_voting_rights_request_object::get_id > >,
      ordered_unique< tag< by_account >,
        member< decline_voting_rights_request_object, account_name_type, &decline_voting_rights_request_object::account >
      >,
      ordered_unique< tag< by_effective_date >,
        composite_key< decline_voting_rights_request_object,
          member< decline_voting_rights_request_object, time_point_sec, &decline_voting_rights_request_object::effective_date >,
          member< decline_voting_rights_request_object, account_name_type, &decline_voting_rights_request_object::account >
        >,
        composite_key_compare< std::less< time_point_sec >, std::less< account_name_type > >
      >
    >,
    allocator< decline_voting_rights_request_object >
  > decline_voting_rights_request_index;

  typedef multi_index_container<
    reward_fund_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< reward_fund_object, reward_fund_object::id_type, &reward_fund_object::get_id > >,
      ordered_unique< tag< by_name >,
        member< reward_fund_object, reward_fund_name_type, &reward_fund_object::name > >
    >,
    allocator< reward_fund_object >
  > reward_fund_index;

} } // crea::chain

#ifdef ENABLE_MIRA
namespace mira {

template<> struct is_static_length< crea::chain::convert_request_object > : public boost::true_type {};
template<> struct is_static_length< crea::chain::escrow_object > : public boost::true_type {};
template<> struct is_static_length< crea::chain::liquidity_reward_balance_object > : public boost::true_type {};
template<> struct is_static_length< crea::chain::limit_order_object > : public boost::true_type {};
template<> struct is_static_length< crea::chain::withdraw_vesting_route_object > : public boost::true_type {};
template<> struct is_static_length< crea::chain::decline_voting_rights_request_object > : public boost::true_type {};
template<> struct is_static_length< crea::chain::reward_fund_object > : public boost::true_type {};

} // mira
#endif

#include <crea/chain/comment_object.hpp>
#include <crea/chain/account_object.hpp>

FC_REFLECT( crea::chain::limit_order_object,
          (id)(created)(expiration)(seller)(orderid)(for_sale)(sell_price) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::limit_order_object, crea::chain::limit_order_index )

FC_REFLECT( crea::chain::feed_history_object,
          (id)(current_median_history)(price_history) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::feed_history_object, crea::chain::feed_history_index )

FC_REFLECT( crea::chain::convert_request_object,
          (id)(owner)(requestid)(amount)(conversion_date) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::convert_request_object, crea::chain::convert_request_index )

FC_REFLECT( crea::chain::liquidity_reward_balance_object,
          (id)(owner)(crea_volume)(cbd_volume)(weight)(last_update) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::liquidity_reward_balance_object, crea::chain::liquidity_reward_balance_index )

FC_REFLECT( crea::chain::withdraw_vesting_route_object,
          (id)(from_account)(to_account)(percent)(auto_vest) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::withdraw_vesting_route_object, crea::chain::withdraw_vesting_route_index )

FC_REFLECT( crea::chain::savings_withdraw_object,
          (id)(from)(to)(memo)(request_id)(amount)(complete) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::savings_withdraw_object, crea::chain::savings_withdraw_index )

FC_REFLECT( crea::chain::escrow_object,
          (id)(escrow_id)(from)(to)(agent)
          (ratification_deadline)(escrow_expiration)
          (cbd_balance)(crea_balance)(pending_fee)
          (to_approved)(agent_approved)(disputed) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::escrow_object, crea::chain::escrow_index )

FC_REFLECT( crea::chain::decline_voting_rights_request_object,
          (id)(account)(effective_date) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::decline_voting_rights_request_object, crea::chain::decline_voting_rights_request_index )

FC_REFLECT( crea::chain::reward_fund_object,
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
CHAINBASE_SET_INDEX_TYPE( crea::chain::reward_fund_object, crea::chain::reward_fund_index )
