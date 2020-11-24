#pragma once
#include <crea/chain/crea_fwd.hpp>

#include <crea/protocol/authority.hpp>
#include <crea/protocol/crea_operations.hpp>

#include <crea/chain/crea_object_types.hpp>
#include <crea/chain/witness_objects.hpp>
#include <crea/chain/account_object.hpp>

#include <crea/chain/util/tiny_asset.hpp>

#include <fc/crypto/ripemd160.hpp>

#define CREA_ROOT_POST_PARENT_ID crea::chain::account_id_type::null_id()

namespace crea { namespace chain {

  using protocol::beneficiary_route_type;
  using chainbase::t_vector;
  using chainbase::t_pair;
#ifdef CREA_ENABLE_SMT
  using protocol::votable_asset_info;
#endif

  class comment_object : public object< comment_object_type, comment_object >
  {
    CHAINBASE_OBJECT( comment_object );
    public:
      using author_and_permlink_hash_type = fc::ripemd160;

      template< typename Allocator >
      comment_object( allocator< Allocator > a, uint64_t _id,
        const account_object& _author, const std::string& _permlink,
        fc::optional< std::reference_wrapper< const comment_object > > _parent_comment );

      //returns comment identification hash
      const author_and_permlink_hash_type& get_author_and_permlink_hash() const { return author_and_permlink_hash; }

      static author_and_permlink_hash_type compute_author_and_permlink_hash(
        account_id_type author_account_id, const std::string& permlink );

      //returns id of root comment (self when top comment)
      comment_id_type get_root_id() const { return root_comment; }
      //returns id of parent comment (null_id() when top comment)
      comment_id_type get_parent_id() const { return parent_comment; }
      //tells if comment is top comment
      bool is_root() const { return parent_comment == comment_id_type::null_id(); }

      //returns comment depth (distance to root)
      uint16_t get_depth() const { return depth; }

    private:
      comment_id_type root_comment;
      comment_id_type parent_comment;

      author_and_permlink_hash_type author_and_permlink_hash;

      uint16_t        depth = 0; //looks like a candidate for removal (see https://github.com/steemit/steem/issues/767 )

      CHAINBASE_UNPACK_CONSTRUCTOR(comment_object);
  };

  template< typename Allocator >
  inline comment_object::comment_object( allocator< Allocator > a, uint64_t _id,
    const account_object& _author, const std::string& _permlink,
    fc::optional< std::reference_wrapper< const comment_object > > _parent_comment
  )
    : id ( _id )
  {
    author_and_permlink_hash = compute_author_and_permlink_hash( _author.get_id(), _permlink );

    if ( _parent_comment.valid() )
    {
      parent_comment = ( *_parent_comment ).get().get_id();
      root_comment = ( *_parent_comment ).get().get_root_id();
      depth = ( *_parent_comment ).get().get_depth() + 1;
    }
    else
    {
      parent_comment = comment_id_type::null_id();
      root_comment = id;
    }
  }

  inline comment_object::author_and_permlink_hash_type comment_object::compute_author_and_permlink_hash(
    account_id_type author_account_id, const std::string& permlink )
  {
    return fc::ripemd160::hash( permlink + "@" + std::to_string( author_account_id ) );
  }

  /*
    Helper class related to `comment_object` - members needed for payout calculation.

    Objects of this class can be removed, it depends on `cashout_time`
    when `cashout_time == fc::time_point_sec::maximum()`
  */
  class comment_cashout_object : public object< comment_cashout_object_type, comment_cashout_object >
  {
    CHAINBASE_OBJECT( comment_cashout_object );
    public:
      template< typename Allocator >
      comment_cashout_object( allocator< Allocator > a, uint64_t _id,
        const comment_object& _comment, const account_object& _author, const std::string& _permlink,
        const time_point_sec& _creation_time, const time_point_sec& _cashout_time, uint16_t _reward_weight = 0 )
        : id( _comment.get_id() ), //note that it is possible because relation is 1->{0,1} so we can share id
        author_id( _author.get_id() ), permlink( a ), active( _creation_time ),
        last_payout( time_point_sec::min() ), created( _creation_time ), cashout_time( _cashout_time ),
        max_cashout_time( time_point_sec::maximum() ), reward_weight( _reward_weight ), beneficiaries( a )
#ifdef CREA_ENABLE_SMT
        , allowed_vote_assets( a )
#endif
      {
        from_string( permlink, _permlink );
        FC_ASSERT( _creation_time <= _cashout_time );
      }

      //returns id of associated comment
      comment_id_type get_comment_id() const { return comment_object::id_type( id ); }

      //returns creation time
      const time_point_sec& get_creation_time() const { return created; }

      account_id_type   author_id;
      shared_string     permlink;

      time_point_sec    active; ///< the last time this post was "touched" by voting or reply
      time_point_sec    last_payout;

      /// index on pending_payout for "things happning now... needs moderation"
      /// TRENDING = UNCLAIMED + PENDING
      share_type        net_rshares; // reward is proportional to rshares^2, this is the sum of all votes (positive and negative)
      share_type        abs_rshares; /// this is used to track the total abs(weight) of votes for the purpose of calculating cashout_time
      share_type        vote_rshares; /// Total positive rshares from all votes. Used to calculate delta weights. Needed to handle vote changing and removal.

      share_type        children_abs_rshares; /// this is used to calculate cashout time of a discussion.

      uint32_t          children = 0; ///< used to track the total number of children, grandchildren, etc...

    private:
      time_point_sec    created;
    public:
      time_point_sec    cashout_time; /// 24 hours from the weighted average of vote time
      time_point_sec    max_cashout_time;
      uint64_t          total_vote_weight = 0; /// the total weight of voting rewards, used to calculate pro-rata share of curation payouts

      uint16_t          reward_weight = 0;

      /** tracks the total payout this comment has received over time, measured in CBD */
      CBD_asset         total_payout_value = asset(0, CBD_SYMBOL);
      CBD_asset         curator_payout_value = asset(0, CBD_SYMBOL);
      CBD_asset         beneficiary_payout_value = asset( 0, CBD_SYMBOL );

      share_type        author_rewards = 0;

      int32_t           net_votes = 0;

      CBD_asset         max_accepted_payout = asset( 1000000000, CBD_SYMBOL );       /// CBD value of the maximum payout this post will receive
      uint16_t          percent_cbd = CREA_100_PERCENT; /// the percent of CBD to key, unkept amounts will be received as VESTS
      bool              allow_votes   = true;      /// allows a post to receive votes;
      bool              allow_curation_rewards = true;

      using t_beneficiaries = t_vector< beneficiary_route_type >;
      t_beneficiaries   beneficiaries;
#ifdef CREA_ENABLE_SMT
      using t_votable_assets = t_vector< t_pair< asset_symbol_type, votable_asset_info > >;
      t_votable_assets  allowed_vote_assets;

      CHAINBASE_UNPACK_CONSTRUCTOR(comment_cashout_object, (permlink)(beneficiaries)(allowed_vote_assets));
#else
      CHAINBASE_UNPACK_CONSTRUCTOR(comment_cashout_object, (permlink)(beneficiaries));
#endif
  };

  class comment_content_object : public object< comment_content_object_type, comment_content_object >
  {
    CHAINBASE_OBJECT( comment_content_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( comment_content_object, (title)(body)(json_metadata) )

      comment_id_type   comment;

      shared_string     title;
      shared_string     body;
      shared_string     json_metadata;

    CHAINBASE_UNPACK_CONSTRUCTOR(comment_content_object, (title)(body)(json_metadata));
  };

   class comment_download_object : public object< comment_download_object_type, comment_download_object >
   {
       CHAINBASE_OBJECT( comment_download_object );

       public:
          template< typename Allocator >
          comment_download_object (allocator< Allocator > a, uint64_t _id,
             const comment_object& _comment, const std::string& _resource, const std::string& _name,
             const std::string& _type, const std::string& _password) : id( _comment.get_id() )
          {
              from_string(resource, _resource);
              from_string(name, _name);
              from_string(type, _type);
              from_string(password, _password);
          }

          //returns id of associated comment
          comment_id_type get_comment_id() const { return comment_object::id_type( id ); }

          shared_string     resource;
          shared_string     name;
          shared_string     type;
          uint32_t          size = 0;
          uint32_t          times_downloaded = 0;
          shared_string     password;
          asset             price;

       CHAINBASE_UNPACK_CONSTRUCTOR(comment_download_object, (resource)(name)(type)(password));
   };

   class download_granted_object : public object< download_granted_object_type, download_granted_object >
   {
       CHAINBASE_OBJECT( download_granted_object );

       public:
          template< typename Allocator >
          download_granted_object (allocator< Allocator > a, uint64_t _id,
             const comment_download_object& _comment_download, const account_object& _downloader) :
             id ( _id ), download( _comment_download.get_id() ), downloader_id ( _downloader.get_id() )
          {
          }
          comment_download_id_type  download;

          time_point_sec            payment_date;
          account_id_type           downloader_id;
          account_id_type           comment_author_id;
          shared_string             comment_permlink;
          asset                     price;

       CHAINBASE_UNPACK_CONSTRUCTOR(download_granted_object, (comment_permlink));

   };

   /**
    * Used to parse download data
    */
   struct comment_download_data {

       string resource;
       string name;
       string type;
       uint32_t size;
       string price;
   };

   /**
    * This index maintains the set of voter/comment pairs that have been used, voters cannot
    * vote on the same comment more than once per payout period.
    */
  class comment_vote_object : public object< comment_vote_object_type, comment_vote_object>
  {
    CHAINBASE_OBJECT( comment_vote_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( comment_vote_object )

      account_id_type   voter;
      comment_id_type   comment;
      uint64_t          weight = 0; ///< defines the score this vote receives, used by vote payout calc. 0 if a negative vote or changed votes.
      int64_t           rshares = 0; ///< The number of rshares this vote is responsible for
      int16_t           vote_percent = 0; ///< The percent weight of the vote
      time_point_sec    last_update; ///< The time of the last update of the vote
      int8_t            num_changes = 0;

    CHAINBASE_UNPACK_CONSTRUCTOR(comment_vote_object);
  };

  struct by_comment_voter;
  struct by_voter_comment;
  typedef multi_index_container<
    comment_vote_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< comment_vote_object, comment_vote_object::id_type, &comment_vote_object::get_id > >,
      ordered_unique< tag< by_comment_voter >,
        composite_key< comment_vote_object,
          member< comment_vote_object, comment_id_type, &comment_vote_object::comment>,
          member< comment_vote_object, account_id_type, &comment_vote_object::voter>
        >
      >,
      ordered_unique< tag< by_voter_comment >,
        composite_key< comment_vote_object,
          member< comment_vote_object, account_id_type, &comment_vote_object::voter>,
          member< comment_vote_object, comment_id_type, &comment_vote_object::comment>
        >
      >
    >,
    allocator< comment_vote_object >
  > comment_vote_index;


  struct by_permlink; /// author, perm
  struct by_root;
  struct by_author_last_update;

  /**
    * @ingroup object_index
    */
  typedef multi_index_container<
    comment_object,
    indexed_by<
      /// CONSENSUS INDICES - used by evaluators
      ordered_unique< tag< by_id >,
        const_mem_fun< comment_object, comment_object::id_type, &comment_object::get_id > >,
      ordered_unique< tag< by_permlink >, /// used by consensus to find posts referenced in ops
        const_mem_fun< comment_object, const comment_object::author_and_permlink_hash_type&, &comment_object::get_author_and_permlink_hash > >,
      ordered_unique< tag< by_root >,
        composite_key< comment_object,
          const_mem_fun< comment_object, comment_id_type, &comment_object::get_root_id >,
          const_mem_fun< comment_object, comment_object::id_type, &comment_object::get_id >
        >
      >
    >,
    allocator< comment_object >
  > comment_index;

  struct by_cashout_time; /// cashout_time

  typedef multi_index_container<
    comment_cashout_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< comment_cashout_object, comment_cashout_object::id_type, &comment_cashout_object::get_id > >,
      ordered_unique< tag< by_cashout_time >,
        composite_key< comment_cashout_object,
          member< comment_cashout_object, time_point_sec, &comment_cashout_object::cashout_time>,
          const_mem_fun< comment_cashout_object, comment_cashout_object::id_type, &comment_cashout_object::get_id >
        >
      >
    >,
    allocator< comment_cashout_object >
  > comment_cashout_index;

  struct by_comment;
  struct by_download;
  struct by_downloader;

  typedef multi_index_container<
    comment_content_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< comment_content_object, comment_content_object::id_type, &comment_content_object::get_id > >,
      ordered_unique< tag< by_comment >,
        member< comment_content_object, comment_id_type, &comment_content_object::comment > >
    >,
    allocator< comment_content_object >
  > comment_content_index;

   typedef multi_index_container<
      comment_download_object,
      indexed_by<
         ordered_unique< tag< by_id >,
           const_mem_fun< comment_download_object, comment_download_id_type, &comment_download_object::id > >,
         ordered_unique< tag< by_comment >,
           const_mem_fun< comment_download_object, comment_id_type, &comment_download_object::comment > >
      >,
      allocator< comment_download_object >
   > comment_download_index;

   typedef multi_index_container<
      download_granted_object,
        indexed_by<
           ordered_unique< tag< by_id >,
             const_mem_fun< download_granted_object, download_granted_id_type, &download_granted_object::id > >,
           ordered_unique< tag< by_download >,
              composite_key< download_granted_object,
                 member< download_granted_object, download_granted_id_type, &download_granted_object::id >,
                 member< download_granted_object, comment_download_id_type, &download_granted_object::download >
              >
           >,
           ordered_unique< tag< by_downloader >,
              composite_key< download_granted_object,
                 member< download_granted_object, account_name_type, &download_granted_object::comment_author >,
                 member< download_granted_object, shared_string, &download_granted_object::comment_permlink >,
                 member< download_granted_object, account_name_type, &download_granted_object::downloader >
              >,
              composite_key_compare< std::less< account_name_type >, strcmp_less, std::less< account_name_type > >
           >
        >,
      allocator< download_granted_object >
   > download_granted_index;

} } // crea::chain

#ifdef ENABLE_MIRA
namespace mira {

template<> struct is_static_length< crea::chain::comment_vote_object > : public boost::true_type {};

} // mira
#endif

FC_REFLECT( crea::chain::comment_object,
          (id)(root_comment)(parent_comment)
          (author_and_permlink_hash)(depth)
        )

CHAINBASE_SET_INDEX_TYPE( crea::chain::comment_object, crea::chain::comment_index )

FC_REFLECT( crea::chain::comment_cashout_object,
          (id)(author_id)(permlink)
          (active)(last_payout)
          (net_rshares)(abs_rshares)(vote_rshares)(children_abs_rshares)
          (children)(created)(cashout_time)(max_cashout_time)
          (total_vote_weight)(reward_weight)(total_payout_value)(curator_payout_value)(beneficiary_payout_value)
          (author_rewards)(net_votes)
          (max_accepted_payout)(percent_cbd)(allow_votes)(allow_curation_rewards)
          (beneficiaries)
#ifdef CREA_ENABLE_SMT
          (allowed_vote_assets)
#endif
        )

CHAINBASE_SET_INDEX_TYPE( crea::chain::comment_cashout_object, crea::chain::comment_cashout_index )

FC_REFLECT( crea::chain::comment_content_object,
        (id)(comment)(title)(body)(json_metadata) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::comment_content_object, crea::chain::comment_content_index )

FC_REFLECT( crea::chain::download_granted_object,
            (id)(payment_date)(downloader)(comment_author)(comment_permlink)(price) )

CHAINBASE_SET_INDEX_TYPE( crea::chain::download_granted_object, crea::chain::download_granted_index )

FC_REFLECT( crea::chain::comment_download_data,
            (resource)(name)(type)(size)(price) )

FC_REFLECT( crea::chain::comment_download_object,
            (id)(comment)(resource)(name)(type)(size)(times_downloaded)(password)(price) )

CHAINBASE_SET_INDEX_TYPE( crea::chain::comment_download_object, crea::chain::comment_download_index )

FC_REFLECT( crea::chain::comment_vote_object,
          (id)(voter)(comment)(weight)(rshares)(vote_percent)(last_update)(num_changes)
        )
CHAINBASE_SET_INDEX_TYPE( crea::chain::comment_vote_object, crea::chain::comment_vote_index )

namespace helpers
{
  using crea::chain::shared_string;

  template <>
  class index_statistic_provider<crea::chain::comment_index>
  {
  public:
    typedef crea::chain::comment_index IndexType;
    index_statistic_info gather_statistics(const IndexType& index, bool onlyStaticInfo) const
    {
      index_statistic_info info;
      gather_index_static_data(index, &info);
      return info;
    }
  };

  template <>
  class index_statistic_provider<crea::chain::comment_cashout_index>
  {
  public:
    typedef crea::chain::comment_cashout_index IndexType;
    typedef typename crea::chain::comment_cashout_object::t_beneficiaries t_beneficiaries;
#ifdef CREA_ENABLE_SMT
    typedef typename crea::chain::comment_cashout_object::t_votable_assets t_votable_assets;
#endif
    index_statistic_info gather_statistics(const IndexType& index, bool onlyStaticInfo) const
    {
      index_statistic_info info;
      gather_index_static_data(index, &info);

      if(onlyStaticInfo == false)
      {
        for(const auto& o : index)
        {
          info._item_additional_allocation += o.permlink.capacity()*sizeof(shared_string::value_type);
          info._item_additional_allocation += o.beneficiaries.capacity()*sizeof(t_beneficiaries::value_type);
#ifdef CREA_ENABLE_SMT
          info._item_additional_allocation += o.allowed_vote_assets.capacity()*sizeof(t_votable_assets::value_type);
#endif
        }
      }

      return info;
    }
  };

  template <>
  class index_statistic_provider<crea::chain::comment_content_index>
  {
  public:
    typedef crea::chain::comment_content_index IndexType;

    index_statistic_info gather_statistics(const IndexType& index, bool onlyStaticInfo) const
    {
      index_statistic_info info;
      gather_index_static_data(index, &info);

      if(onlyStaticInfo == false)
      {
        for(const auto& o : index)
        {
          info._item_additional_allocation += o.title.capacity()*sizeof(shared_string::value_type);
          info._item_additional_allocation += o.body.capacity()*sizeof(shared_string::value_type);
          info._item_additional_allocation += o.json_metadata.capacity()*sizeof(shared_string::value_type);
        }
      }

         return info;
      }
   };

    template <>
    class index_statistic_provider<crea::chain::comment_download_index>
    {
    public:
        typedef crea::chain::comment_download_index IndexType;

        index_statistic_info gather_statistics(const IndexType& index, bool onlyStaticInfo) const
        {
            index_statistic_info info;
            gather_index_static_data(index, &info);

            if(onlyStaticInfo == false)
            {
                for(const auto& o : index)
                {
                    info._item_additional_allocation += o.resource.capacity()*sizeof(shared_string::value_type);
                    info._item_additional_allocation += o.type.capacity()*sizeof(shared_string::value_type);
                    info._item_additional_allocation += o.name.capacity()*sizeof(shared_string::value_type);
                    info._item_additional_allocation += o.password.capacity()*sizeof(shared_string::value_type);
                    info._item_additional_allocation += sizeof(o.size);
                    info._item_additional_allocation += sizeof(o.times_downloaded);
                }
            }

            return info;
        }
    };

    template <>
    class index_statistic_provider<crea::chain::download_granted_index>
    {
    public:
        typedef crea::chain::download_granted_index IndexType;

        index_statistic_info gather_statistics(const IndexType& index, bool onlyStaticInfo) const
        {
            index_statistic_info info;
            gather_index_static_data(index, &info);

            if(onlyStaticInfo == false)
            {
                for(const auto& o : index)
                {
                    info._item_additional_allocation += o.comment_permlink.capacity()*sizeof(shared_string::value_type);
                }
            }

            return info;
        }
    };

} /// namespace helpers