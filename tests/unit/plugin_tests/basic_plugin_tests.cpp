#if defined IS_TEST_NET && !defined ENABLE_MIRA && !defined ENABLE_STD_ALLOCATOR
#include <boost/test/unit_test.hpp>

#include <crea/plugins/account_by_key/account_by_key_objects.hpp>
#include <crea/plugins/account_history_rocksdb/account_history_rocksdb_objects.hpp>
//#include <crea/plugins/block_log_info/block_log_info_objects.hpp>
#include <crea/plugins/market_history/market_history_plugin.hpp>
#include <crea/plugins/rc/rc_objects.hpp>
#include <crea/plugins/reputation/reputation_objects.hpp>
#include <crea/plugins/transaction_status/transaction_status_objects.hpp>
#include <crea/plugins/witness/witness_plugin_objects.hpp>

#include "../db_fixture/database_fixture.hpp"

using namespace crea::chain;
using namespace crea::protocol;
using namespace crea::plugins;

BOOST_FIXTURE_TEST_SUITE( basic_plugin_tests, clean_database_fixture )

BOOST_AUTO_TEST_CASE( plugin_object_size )
{
  try
  {
    BOOST_CHECK_EQUAL( sizeof( account_by_key::key_lookup_object ), 56 );
    //3.4M, lasting, expected 3*account_object on average

    BOOST_CHECK_EQUAL( sizeof( account_history_rocksdb::volatile_operation_object ), 112 );
    //temporary, at most as many as operations in reversible blocks

    //BOOST_CHECK_EQUAL( sizeof( block_log_info::block_log_hash_state_object ), 0 );
    //BOOST_CHECK_EQUAL( sizeof( block_log_info::block_log_pending_message_object ), 0 );

    BOOST_CHECK_EQUAL( sizeof( market_history::bucket_object ), 96
#ifdef CREA_ENABLE_SMT
      + 8
#endif
    );
    //temporary, regulated amount, ~13k
    BOOST_CHECK_EQUAL( sizeof( market_history::order_history_object ), 88 );
    //permanent, growing with executed limit_order_object, 2.5M atm

    BOOST_CHECK_EQUAL( sizeof( rc::rc_resource_param_object ), 368 );
    //singleton
    BOOST_CHECK_EQUAL( sizeof( rc::rc_pool_object ), 48 );
    //singleton
    BOOST_CHECK_EQUAL( sizeof( rc::rc_account_object ), 64 );
    //permanent, as many as account_object, 1.3M atm
    BOOST_CHECK_EQUAL( sizeof( rc::rc_delegation_pool_object ), 40 );
    //unused
    BOOST_CHECK_EQUAL( sizeof( rc::rc_indel_edge_object ), 48 );
    //unused
    BOOST_CHECK_EQUAL( sizeof( rc::rc_outdel_drc_edge_object ), 64 );
    //unused

    BOOST_CHECK_EQUAL( sizeof( reputation::reputation_object ), 32 );
    //lasting, as many as account_object, 1.3M atm

    BOOST_CHECK_EQUAL( sizeof( transaction_status::transaction_status_object ), 28 );
    //temporary, depends on tracking flag, cuts out data from too old blocks
    
    BOOST_CHECK_EQUAL( sizeof( witness::witness_custom_op_object ), 32 );
    //temporary, at most as many as account_object affected by custom ops in single block
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_SUITE_END()
#endif
