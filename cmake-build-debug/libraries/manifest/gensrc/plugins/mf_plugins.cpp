#include <appbase/application.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <creativecoin/manifest/plugins.hpp>


#include <creativecoin/plugins/follow/follow_plugin.hpp>

#include <creativecoin/plugins/statsd/statsd_plugin.hpp>

#include <creativecoin/plugins/smt_test/smt_test_plugin.hpp>

#include <creativecoin/plugins/account_history/account_history_plugin.hpp>

#include <creativecoin/plugins/block_log_info/block_log_info_plugin.hpp>

#include <creativecoin/plugins/account_history_api/account_history_api_plugin.hpp>

#include <creativecoin/plugins/condenser_api/condenser_api_plugin.hpp>

#include <creativecoin/plugins/network_broadcast_api/network_broadcast_api_plugin.hpp>

#include <creativecoin/plugins/market_history_api/market_history_api_plugin.hpp>

#include <creativecoin/plugins/tags_api/tags_api_plugin.hpp>

#include <creativecoin/plugins/reputation_api/reputation_api_plugin.hpp>

#include <creativecoin/plugins/follow_api/follow_api_plugin.hpp>

#include <creativecoin/plugins/account_by_key_api/account_by_key_api_plugin.hpp>

#include <creativecoin/plugins/database_api/database_api_plugin.hpp>

#include <creativecoin/plugins/chain_api/chain_api_plugin.hpp>

#include <creativecoin/plugins/block_api/block_api_plugin.hpp>

#include <creativecoin/plugins/debug_node_api/debug_node_api_plugin.hpp>

#include <creativecoin/plugins/witness_api/witness_api_plugin.hpp>

#include <creativecoin/plugins/tags/tags_plugin.hpp>

#include <creativecoin/plugins/chain/chain_plugin.hpp>

#include <creativecoin/plugins/account_history_rocksdb/account_history_rocksdb_plugin.hpp>

#include <creativecoin/plugins/debug_node/debug_node_plugin.hpp>

#include <creativecoin/plugins/witness/witness_plugin.hpp>

#include <creativecoin/plugins/market_history/market_history_plugin.hpp>

#include <creativecoin/plugins/stats_export/stats_export_plugin.hpp>

#include <creativecoin/plugins/account_by_key/account_by_key_plugin.hpp>

#include <creativecoin/plugins/p2p/p2p_plugin.hpp>

#include <creativecoin/plugins/block_data_export/block_data_export_plugin.hpp>

#include <creativecoin/plugins/reputation/reputation_plugin.hpp>

#include <creativecoin/plugins/webserver/webserver_plugin.hpp>


namespace creativecoin { namespace plugins {

void register_plugins()
{
   
   appbase::app().register_plugin< creativecoin::plugins::follow::follow_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::statsd::statsd_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::smt_test::smt_test_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::account_history::account_history_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::block_log_info::block_log_info_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::account_history::account_history_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::condenser_api::condenser_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::network_broadcast_api::network_broadcast_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::market_history::market_history_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::tags::tags_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::reputation::reputation_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::follow::follow_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::account_by_key::account_by_key_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::database_api::database_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::chain::chain_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::block_api::block_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::debug_node::debug_node_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::witness::witness_api_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::tags::tags_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::chain::chain_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::account_history_rocksdb::account_history_rocksdb_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::debug_node::debug_node_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::witness::witness_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::market_history::market_history_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::stats_export::stats_export_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::account_by_key::account_by_key_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::p2p::p2p_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::block_data_export::block_data_export_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::reputation::reputation_plugin >();
   
   appbase::app().register_plugin< creativecoin::plugins::webserver::webserver_plugin >();
   
}

} }