#include <crea/plugins/tags_api/tags_api_plugin.hpp>
#include <crea/plugins/tags_api/tags_api.hpp>
#include <crea/plugins/tags/tags_plugin.hpp>
#include <crea/plugins/follow_api/follow_api_plugin.hpp>
#include <crea/plugins/follow_api/follow_api.hpp>

#include <crea/chain/crea_object_types.hpp>
#include <crea/chain/util/reward.hpp>
#include <crea/chain/util/uint256.hpp>

namespace crea { namespace plugins { namespace tags {

namespace detail {

class tags_api_impl
{
   public:
      tags_api_impl() : _db( appbase::app().get_plugin< crea::plugins::chain::chain_plugin >().db() ) {}

      DECLARE_API_IMPL(
         (get_popular_tags)
         (get_tags_used_by_author)
         (get_discussion)
         (get_content_replies)
         (get_post_discussions_by_payout)
         (get_comment_discussions_by_payout)
         (get_discussions_by_popular)
         (get_discussions_by_now)
         (get_discussions_by_active)
         (get_discussions_by_cashout)
         (get_discussions_by_votes)
         (get_discussions_by_children)
         (get_discussions_by_skyrockets)
         (get_discussions_by_feed)
         (get_discussions_by_blog)
         (get_discussions_by_comments)
         (get_discussions_by_promoted)
         (get_replies_by_last_update)
         (get_discussions_by_author_before_date)
         (get_active_votes)
      )

    chain::database& _db;
    std::shared_ptr< crea::plugins::follow::follow_api > _follow_api;
};

DEFINE_API_IMPL( tags_api_impl, get_popular_tags )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_tags_used_by_author )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussion )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_content_replies )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_post_discussions_by_payout )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_comment_discussions_by_payout )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_popular )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_now )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_active )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_cashout )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_votes )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_children )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_skyrockets )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_feed )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_blog )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_comments )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_discussions_by_promoted )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_replies_by_last_update )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

get_discussions_by_author_before_date_return tags_api_impl::getDiscussionByAuthorBeforeDateCreated(const get_discussions_by_author_before_date_args& args)
{
  FC_ASSERT( false, "Supported by hivemind" );
}

DEFINE_API_IMPL( tags_api_impl, get_active_votes )
{
  FC_ASSERT( false, "Supported by hivemind" );
}

} // detail

tags_api::tags_api(): my( new detail::tags_api_impl() )
{
   JSON_RPC_REGISTER_API( CREA_TAGS_API_PLUGIN_NAME );
}

tags_api::~tags_api() {}

DEFINE_READ_APIS( tags_api,
   (get_popular_tags)
   (get_tags_used_by_author)
   (get_discussion)
   (get_content_replies)
   (get_post_discussions_by_payout)
   (get_comment_discussions_by_payout)
   (get_discussions_by_popular)
   (get_discussions_by_now)
   (get_discussions_by_active)
   (get_discussions_by_cashout)
   (get_discussions_by_votes)
   (get_discussions_by_children)
   (get_discussions_by_skyrockets)
   (get_discussions_by_feed)
   (get_discussions_by_blog)
   (get_discussions_by_comments)
   (get_discussions_by_promoted)
   (get_replies_by_last_update)
   (get_discussions_by_author_before_date)
   (get_active_votes)
)

} } } // crea::plugins::tags
