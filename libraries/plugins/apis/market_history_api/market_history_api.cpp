#include <crea/plugins/market_history_api/market_history_api_plugin.hpp>
#include <crea/plugins/market_history_api/market_history_api.hpp>

#include <crea/chain/crea_objects.hpp>

#define ASSET_TO_REAL( asset ) (double)( asset.amount.value )

namespace crea { namespace plugins { namespace market_history {

namespace detail {

using namespace crea::plugins::market_history;

class market_history_api_impl
{
   public:
      market_history_api_impl() : _db( appbase::app().get_plugin< crea::plugins::chain::chain_plugin >().db() ) {}

    DECLARE_API_IMPL(
      (get_ticker)
      (get_volume)
      (get_order_book)
      (get_trade_history)
      (get_recent_trades)
      (get_market_history)
      (get_market_history_buckets)
    )

    chain::database& _db;
};

DEFINE_API_IMPL( market_history_api_impl, get_ticker )
{
  get_ticker_return result;

  const auto& bucket_idx = _db.get_index< bucket_index, by_bucket >();
  auto itr = bucket_idx.lower_bound( boost::make_tuple( 0, _db.head_block_time() - 86400 ) );

   if( itr != bucket_idx.end() )
   {
      auto open = ASSET_TO_REAL( asset( itr->non_crea.open, CBD_SYMBOL ) ) / ASSET_TO_REAL( asset( itr->crea.open, CREA_SYMBOL ) );
      itr = bucket_idx.lower_bound( boost::make_tuple( 0, _db.head_block_time() ) );
      result.latest = ASSET_TO_REAL( asset( itr->non_crea.close, CBD_SYMBOL ) ) / ASSET_TO_REAL( asset( itr->crea.close, CREA_SYMBOL ) );
      result.percent_change = ( (result.latest - open ) / open ) * 100;
   }

  auto orders = get_order_book( get_order_book_args{ 1 } );
  if( orders.bids.empty() == false)
    result.highest_bid = orders.bids[0].real_price;
  if( orders.asks.empty() == false)
    result.lowest_ask = orders.asks[0].real_price;

   auto volume = get_volume( get_volume_args() );
   result.crea_volume = volume.crea_volume;
   result.cbd_volume = volume.cbd_volume;

  return result;
}

DEFINE_API_IMPL( market_history_api_impl, get_volume )
{
  const auto& bucket_idx = _db.get_index< bucket_index, by_bucket >();
  auto itr = bucket_idx.lower_bound( boost::make_tuple( 0, _db.head_block_time() - 86400 ) );
  get_volume_return result;

  if( itr == bucket_idx.end() )
    return result;

   uint32_t bucket_size = itr->seconds;
   do
   {
      result.crea_volume.amount += itr->crea.volume;
      result.cbd_volume.amount += itr->non_crea.volume;

    ++itr;
  } while( itr != bucket_idx.end() && itr->seconds == bucket_size );

  return result;
}

DEFINE_API_IMPL( market_history_api_impl, get_order_book )
{
  FC_ASSERT( args.limit <= 500 );

   const auto& order_idx = _db.get_index< chain::limit_order_index, chain::by_price >();
   auto itr = order_idx.lower_bound( price::max( CBD_SYMBOL, CREA_SYMBOL ) );

  get_order_book_return result;

   while( itr != order_idx.end() && itr->sell_price.base.symbol == CBD_SYMBOL && result.bids.size() < args.limit )
   {
      order cur;
      cur.order_price = itr->sell_price;
      cur.real_price = ASSET_TO_REAL( itr->sell_price.base ) / ASSET_TO_REAL( itr->sell_price.quote );
      cur.crea = ( asset( itr->for_sale, CBD_SYMBOL ) * itr->sell_price ).amount;
      cur.cbd = itr->for_sale;
      cur.created = itr->created;
      result.bids.push_back( cur );
      ++itr;
   }

   itr = order_idx.lower_bound( price::max( CREA_SYMBOL, CBD_SYMBOL ) );

   while( itr != order_idx.end() && itr->sell_price.base.symbol == CREA_SYMBOL && result.asks.size() < args.limit )
   {
      order cur;
      cur.order_price = itr->sell_price;
      cur.real_price = ASSET_TO_REAL( itr->sell_price.quote ) / ASSET_TO_REAL( itr->sell_price.base );
      cur.crea = itr->for_sale;
      cur.cbd = ( asset( itr->for_sale, CREA_SYMBOL ) * itr->sell_price ).amount;
      cur.created = itr->created;
      result.asks.push_back( cur );
      ++itr;
   }

  return result;
}

DEFINE_API_IMPL( market_history_api_impl, get_trade_history )
{
  FC_ASSERT( args.limit <= 1000 );
  const auto& bucket_idx = _db.get_index< order_history_index, by_time >();
  auto itr = bucket_idx.lower_bound( args.start );

  get_trade_history_return result;

  while( itr != bucket_idx.end() && itr->time <= args.end && result.trades.size() < args.limit )
  {
    market_trade trade;
    trade.date = itr->time;
    trade.current_pays = itr->op.current_pays;
    trade.open_pays = itr->op.open_pays;
    result.trades.push_back( trade );
    ++itr;
  }

  return result;
}

DEFINE_API_IMPL( market_history_api_impl, get_recent_trades )
{
  FC_ASSERT( args.limit <= 1000 );
  const auto& order_idx = _db.get_index< order_history_index, by_time >();
  auto itr = order_idx.rbegin();

  get_recent_trades_return result;

  while( itr != order_idx.rend() && result.trades.size() < args.limit )
  {
    market_trade trade;
    trade.date = itr->time;
    trade.current_pays = itr->op.current_pays;
    trade.open_pays = itr->op.open_pays;
    result.trades.push_back( trade );
    ++itr;
  }

  return result;
}

DEFINE_API_IMPL( market_history_api_impl, get_market_history )
{
  const auto& bucket_idx = _db.get_index< bucket_index, by_bucket >();
  auto itr = bucket_idx.lower_bound( boost::make_tuple( args.bucket_seconds, args.start ) );

  get_market_history_return result;

  while( itr != bucket_idx.end() && itr->seconds == args.bucket_seconds && itr->open < args.end )
  {
    result.buckets.push_back( itr->copy_chain_object() );

    ++itr;
  }

  return result;
}

DEFINE_API_IMPL( market_history_api_impl, get_market_history_buckets )
{
   get_market_history_buckets_return result;
   result.bucket_sizes = appbase::app().get_plugin< crea::plugins::market_history::market_history_plugin >().get_tracked_buckets();
   return result;
}


} // detail

market_history_api::market_history_api(): my( new detail::market_history_api_impl() )
{
   JSON_RPC_REGISTER_API( CREA_MARKET_HISTORY_API_PLUGIN_NAME );
}

market_history_api::~market_history_api() {}

DEFINE_READ_APIS( market_history_api,
  (get_ticker)
  (get_volume)
  (get_order_book)
  (get_trade_history)
  (get_recent_trades)
  (get_market_history)
  (get_market_history_buckets)
)

} } } // crea::plugins::market_history
