#ifdef IS_TEST_NET

#include <boost/test/unit_test.hpp>

#include <crea/chain/crea_fwd.hpp>

#include <crea/protocol/exceptions.hpp>
#include <crea/protocol/hardfork.hpp>
#include <crea/protocol/sps_operations.hpp>

#include <crea/chain/database.hpp>
#include <crea/chain/database_exceptions.hpp>
#include <crea/chain/crea_objects.hpp>

#include <crea/chain/util/reward.hpp>
#include <crea/chain/util/hf23_helper.hpp>

#include <crea/plugins/rc/rc_objects.hpp>
#include <crea/plugins/rc/resource_count.hpp>

#include <fc/macros.hpp>
#include <fc/crypto/digest.hpp>

#include "../db_fixture/database_fixture.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>

using namespace crea;
using namespace crea::chain;
using namespace crea::protocol;
using fc::string;

#define DELEGATED_VESTS( account ) db->get_account( account ).get_delegated_vesting().amount.value
#define RECEIVED_VESTS( account ) db->get_account( account ).get_received_vesting().amount.value

namespace
{

std::string asset_to_string( const asset& a )
{
  return crea::plugins::condenser_api::legacy_asset::from_asset( a ).to_string();
}

} // namespace


BOOST_FIXTURE_TEST_SUITE( hf23_tests, hf23_database_fixture )

BOOST_AUTO_TEST_CASE( restore_accounts_02 )
{
  try
  {
    BOOST_TEST_MESSAGE( "Saving during HF23 and restoring balances during HF24" );

    ACTORS( (alice0)(alice1)(alice2)(alice3)(alice4)(dude) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    std::set< std::string > accounts{ "alice0", "alice1", "alice2", "alice3", "alice4" };

    struct tmp_data
    {
      std::string name;

      asset       balance;
      asset       cbd_balance;
    };
    auto cmp = []( const tmp_data& a, const tmp_data& b )
    {
      return std::strcmp( a.name.c_str(), b.name.c_str() ) < 0;
    };
    std::set< tmp_data, decltype( cmp ) > old_balances( cmp );
    hf23_helper::hf23_items _balances;

    uint32_t cnt = 1;
    for( auto& account : accounts )
    {
      FUND( account, asset( cnt * 1000,      CREA_SYMBOL ) );
      FUND( account, asset( cnt * 1000 + 1,  CBD_SYMBOL ) );
      ++cnt;

      old_balances.emplace( tmp_data{ account, get_balance( account ), get_cbd_balance( account ) } );
    }
    {
      FUND( "dude", asset( 68,   CREA_SYMBOL ) );
      FUND( "dude", asset( 78,   CBD_SYMBOL ) );
      generate_block();
    }
    {
      db->clear_accounts( _balances, accounts );

      for( auto& account : accounts )
      {
        auto& _acc = db->get_account( account );

        idump(( _acc.get_balance() ));
        idump(( _acc.get_cbd_balance() ));
        BOOST_REQUIRE( _acc.get_balance() == asset( 0, CREA_SYMBOL ) );
        BOOST_REQUIRE( _acc.get_cbd_balance() == asset( 0, CBD_SYMBOL ) );
      }
      BOOST_REQUIRE( get_balance( "dude" )      == asset( 68, CREA_SYMBOL ) );
      BOOST_REQUIRE( get_cbd_balance( "dude" )  == asset( 78, CBD_SYMBOL ) );
    }
    {
      auto accounts_ex = accounts;
      accounts_ex.insert( "dude" );
      accounts_ex.insert( "devil" );

      db->restore_accounts( _balances, accounts_ex );

      auto itr_old_balances = old_balances.begin();
      for( auto& account : accounts )
      {
        auto& _acc = db->get_account( account );

        BOOST_REQUIRE( account           == itr_old_balances->name );
        BOOST_REQUIRE( _acc.get_balance() == itr_old_balances->balance );
        BOOST_REQUIRE( _acc.get_cbd_balance() == itr_old_balances->cbd_balance );

        ++itr_old_balances;
      }
      BOOST_REQUIRE( get_balance( "dude" )      == asset( 68, CREA_SYMBOL ) );
      BOOST_REQUIRE( get_cbd_balance( "dude" )  == asset( 78, CBD_SYMBOL ) );
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( restore_accounts_01 )
{
  try
  {
    BOOST_TEST_MESSAGE( "Saving during HF23 and restoring balances during HF24" );

    ACTORS( (alice)(bob)(carol) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    FUND( "alice", ASSET( "1000.000 TESTS" ) );
    FUND( "alice", ASSET( "20.000 TBD" ) );
    FUND( "bob", ASSET( "900.000 TESTS" ) );
    FUND( "bob", ASSET( "10.000 TBD" ) );
    FUND( "carol", ASSET( "3600.000 TESTS" ) );
    FUND( "carol", ASSET( "3500.000 TBD" ) );

    hf23_helper::hf23_items _hf23_items;

    const std::set< std::string > accounts{ "alice", "bob" };

    {
      auto& _alice   = db->get_account( "alice" );
      auto& _bob     = db->get_account( "bob" );

      auto alice_balance      = _alice.get_balance();
      auto alice_cbd_balance  = _alice.get_cbd_balance();
      auto bob_balance        = _bob.get_balance();
      auto bob_cbd_balance    = _bob.get_cbd_balance();

      {
        hf23_helper::gather_balance( _hf23_items, _alice.name, _alice.get_balance(), _alice.get_cbd_balance() );
        db->adjust_balance( db->get_treasury_name(), _alice.get_balance() );
        db->adjust_balance( db->get_treasury_name(), _alice.get_cbd_balance() );
        db->adjust_balance( "alice", -_alice.get_balance() );
        db->adjust_balance( "alice", -_alice.get_cbd_balance() );
      }
      {
        hf23_helper::gather_balance( _hf23_items, _bob.name, _bob.get_balance(), _bob.get_cbd_balance() );
        db->adjust_balance( db->get_treasury_name(), _bob.get_balance() );
        db->adjust_balance( db->get_treasury_name(), _bob.get_cbd_balance() );
        db->adjust_balance( "bob", -_bob.get_balance() );
        db->adjust_balance( "bob", -_bob.get_cbd_balance() );
      }

      asset _2000 = ASSET( "2000.000 TESTS" );
      asset _10   = ASSET( "10.000 TBD" );
      asset _800  = ASSET( "800.000 TESTS" );
      asset _70   = ASSET( "70.000 TBD" );
      asset _5000 = ASSET( "5000.000 TESTS" );
      asset _4000 = ASSET( "4000.000 TBD" );

      {
        //Create another balances for `alice`
        db->adjust_balance( "carol", -_2000 );
        db->adjust_balance( "carol", -_10 );
        db->adjust_balance( "alice", _2000 );
        db->adjust_balance( "alice", _10 );

        generate_block();

        BOOST_REQUIRE( get_balance( "alice" )     == _2000 );
        BOOST_REQUIRE( get_cbd_balance( "alice" ) == _10 );
      }
      {
        //Create another balances for `bob`
        db->adjust_balance( "carol", -_800 );
        db->adjust_balance( "carol", -_70 );
        db->adjust_balance( "bob", _800 );
        db->adjust_balance( "bob", _70 );

        generate_block();

        BOOST_REQUIRE( get_balance( "bob" )       == _800 );
        BOOST_REQUIRE( get_cbd_balance( "bob" )   == _70 );
      }

      std::set< std::string > restored_accounts = { "bob", "alice", "carol" };

      auto treasury_balance_start      = db->get_treasury().get_balance();
      auto treasury_cbd_balance_start  = db->get_treasury().get_cbd_balance();

      {
        auto last_supply = db->get_dynamic_global_properties().get_current_cbd_supply();

        FUND( db->get_treasury_name(), _5000 );
        FUND( db->get_treasury_name(), _4000 );

        generate_block();

        auto interest = db->get_dynamic_global_properties().get_current_cbd_supply() - last_supply - _4000;

        BOOST_REQUIRE( db->get_treasury().get_balance() == _5000 + treasury_balance_start );
        BOOST_REQUIRE( db->get_treasury().get_cbd_balance() == _4000 + treasury_cbd_balance_start + interest );

        treasury_balance_start     += _5000;
        treasury_cbd_balance_start += _4000 + interest;
      }

      database_fixture::validate_database();

      db->restore_accounts( _hf23_items, restored_accounts );

      auto& _alice2           = db->get_account( "alice" );
      auto& _bob2             = db->get_account( "bob" );
      const auto& _treasury2 = db->get_treasury();

      BOOST_REQUIRE( _alice2.get_balance() == _2000 + alice_balance );
      BOOST_REQUIRE( _alice2.get_cbd_balance() == _10 + alice_cbd_balance );
      BOOST_REQUIRE( _bob2.get_balance() == _800 + bob_balance );
      BOOST_REQUIRE( _bob2.get_cbd_balance() == _70 + bob_cbd_balance );

      BOOST_REQUIRE( _treasury2.get_balance() == treasury_balance_start - ( alice_balance + bob_balance ) );
      BOOST_REQUIRE( _treasury2.get_cbd_balance() == treasury_cbd_balance_start - ( alice_cbd_balance + bob_cbd_balance ) );
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

hf23_helper::hf23_item get_balances( const account_object& obj )
{
  hf23_helper::hf23_item res;

  res.name          = obj.name;

  res.balance       = obj.get_balance();
  res.cbd_balance   = obj.get_cbd_balance();

  return res;
}

bool cmp_hf23_item( const hf23_helper::hf23_item& a, const hf23_helper::hf23_item& b )
{
  return a.name == b.name && a.balance == b.balance && a.cbd_balance == b.cbd_balance;
}

BOOST_AUTO_TEST_CASE( save_test_02 )
{
  try
  {
    BOOST_TEST_MESSAGE( "Saving one account balances" );

    ACTORS( (alice) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    FUND( "alice", ASSET( "1000.000 TESTS" ) );
    FUND( "alice", ASSET( "20.000 TBD" ) );

    hf23_helper::hf23_items _hf23_items;

    const std::set< std::string > accounts{ "alice", "bob" };

    {
      hf23_helper::gather_balance( _hf23_items, "alice", get_balance( "alice" ), get_cbd_balance( "alice" ) );
    }
    {
      BOOST_REQUIRE_EQUAL( _hf23_items.size(), 1u );

      auto alice_balances = get_balances( db->get_account( "alice" ) );

      hf23_helper::hf23_items cmp_balances = { alice_balances };

      BOOST_REQUIRE_EQUAL( _hf23_items.size(), cmp_balances.size() );
      BOOST_REQUIRE( cmp_hf23_item( *_hf23_items.begin(), *cmp_balances.begin() ) );
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( save_test_01 )
{
  try
  {
    BOOST_TEST_MESSAGE( "Saving accounts balances" );

    ACTORS( (alice)(bob) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    FUND( "alice", ASSET( "1000.000 TESTS" ) );
    FUND( "bob", ASSET( "2000.000 TESTS" ) );

    FUND( "alice", ASSET( "20.000 TBD" ) );

    const std::set< std::string > accounts{ "alice", "bob" };
    hf23_helper::hf23_items _hf23_items;

    {
      vest( "alice", "alice", ASSET( "10.000 TESTS" ), alice_private_key );
      vest( "bob", "bob", ASSET( "10.000 TESTS" ), bob_private_key );

      hf23_helper::gather_balance( _hf23_items, "alice", get_balance( "alice" ), get_cbd_balance( "alice" ) );
      hf23_helper::gather_balance( _hf23_items, "bob", get_balance( "bob" ), get_cbd_balance( "bob" ) );
    }
    {
      auto alice_balances = get_balances( db->get_account( "alice" ) );
      auto bob_balances = get_balances( db->get_account( "bob" ) );

      hf23_helper::hf23_items cmp_balances = { alice_balances, bob_balances };

      BOOST_REQUIRE_EQUAL( _hf23_items.size(), cmp_balances.size() );

      auto iter = _hf23_items.begin();
      auto cmp_iter = cmp_balances.begin();

      for( uint32_t i = 0; i < cmp_balances.size(); ++i )
      {
        BOOST_REQUIRE( cmp_hf23_item( *iter, *cmp_iter ) );
        ++iter;
        ++cmp_iter;
      }
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( basic_test_06 )
{
  try
  {
    BOOST_TEST_MESSAGE( "VEST delegations - object of type `vesting_delegation_expiration_object` is generated" );

    ACTORS( (alice)(bob) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    auto _10 = ASSET( "10.000 TESTS" );
    auto _20 = ASSET( "20.000 TESTS" );
    auto _1v = ASSET( "1.000000 VESTS" );
    auto _2v = ASSET( "2.000000 VESTS" );
    auto _3v = ASSET( "3.000000 VESTS" );
    auto _1000 = ASSET( "1000.000 TESTS" );

    FUND( "alice", _1000 );
    FUND( "bob", _1000 );

    {
      vest( "alice", "alice", _10, alice_private_key );
      vest( "bob", "bob", _20, bob_private_key );
    }
    {
      delegate_vest( "alice", "bob", _3v, alice_private_key );
      generate_block();

      const auto& idx = db->get_index< vesting_delegation_expiration_index, by_account_expiration >();
      BOOST_REQUIRE( idx.lower_bound( "alice" ) == idx.end() );
      BOOST_REQUIRE( idx.lower_bound( "bob" ) == idx.end() );
    }
    {
      delegate_vest( "alice", "bob", _3v, alice_private_key );

      const auto& idx = db->get_index< vesting_delegation_expiration_index, by_account_expiration >();
      BOOST_REQUIRE( idx.lower_bound( "alice" ) == idx.end() );
      BOOST_REQUIRE( idx.lower_bound( "bob" ) == idx.end() );
    }
    {
      delegate_vest( "alice", "bob", _2v, alice_private_key );

      const auto& idx = db->get_index< vesting_delegation_expiration_index, by_account_expiration >();
      BOOST_REQUIRE( idx.lower_bound( "alice" ) != idx.end() );
      BOOST_REQUIRE( idx.lower_bound( "bob" ) == idx.end() );
    }
    {
      delegate_vest( "alice", "bob", _1v, alice_private_key );

      const auto& idx = db->get_index< vesting_delegation_expiration_index, by_account_expiration >();
      BOOST_REQUIRE( idx.lower_bound( "alice" ) != idx.end() );
      BOOST_REQUIRE( idx.lower_bound( "bob" ) == idx.end() );
    }
    {
      const auto& _bob = db->get_account( "bob" );
      db->clear_account( _bob );

      const auto& idx = db->get_index< vesting_delegation_expiration_index, by_account_expiration >();
      BOOST_REQUIRE( idx.lower_bound( "alice" ) != idx.end() );
      BOOST_REQUIRE( idx.lower_bound( "bob" ) == idx.end() );
    }
    {
      const auto& _alice = db->get_account( "alice" );
      db->clear_account( _alice );

      const auto& idx = db->get_index< vesting_delegation_expiration_index, by_account_expiration >();
      BOOST_REQUIRE( idx.lower_bound( "alice" ) == idx.end() );
      BOOST_REQUIRE( idx.lower_bound( "bob" ) == idx.end() );
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( basic_test_05 )
{
  try
  {
    BOOST_TEST_MESSAGE( "VEST delegations - more complex scenarios" );

    ACTORS( (alice)(bob)(carol) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    auto _10 = ASSET( "10.000 TESTS" );
    auto _20 = ASSET( "20.000 TESTS" );
    auto _1v = ASSET( "1.000000 VESTS" );
    auto _2v = ASSET( "2.000000 VESTS" );
    auto _3v = ASSET( "3.000000 VESTS" );
    auto _1000 = ASSET( "1000.000 TESTS" );

    FUND( "alice", _1000 );
    FUND( "bob", _1000 );
    FUND( "carol", _1000 );

    {
      vest( "alice", "alice", _10, alice_private_key );
      vest( "bob", "bob", _20, bob_private_key );
      vest( "carol", "carol", _10, carol_private_key );
    }
    {
      delegate_vest( "alice", "bob", _1v, alice_private_key );
      delegate_vest( "alice", "bob", _2v, alice_private_key );

      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) == _2v.amount.value );

      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) == _2v.amount.value );
    }
    {
      delegate_vest( "alice", "carol", _1v, alice_private_key );

      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) == _3v.amount.value );

      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) == _2v.amount.value );
      BOOST_REQUIRE( RECEIVED_VESTS( "carol" ) == _1v.amount.value );
    }
    {
      delegate_vest( "carol", "bob", _1v, carol_private_key );

      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) == _3v.amount.value );
      BOOST_REQUIRE( DELEGATED_VESTS( "carol" ) == _1v.amount.value );

      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) == _3v.amount.value );
      BOOST_REQUIRE( RECEIVED_VESTS( "carol" ) == _1v.amount.value );
    }
    {
      const auto& _carol = db->get_account( "carol" );
      db->clear_account( _carol );

      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) == _3v.amount.value );
      BOOST_REQUIRE( DELEGATED_VESTS( "carol" ) == 0l );

      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) == _2v.amount.value );
      BOOST_REQUIRE( RECEIVED_VESTS( "carol" ) == _1v.amount.value );
    }
    {
      const auto& _alice = db->get_account( "alice" );
      db->clear_account( _alice );

      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) == 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "bob" ) == 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "carol" ) == 0l );

      BOOST_REQUIRE( RECEIVED_VESTS( "alice" ) == 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) == 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "carol" ) == 0l );
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( basic_test_04 )
{
  try
  {
    BOOST_TEST_MESSAGE( "VEST delegations" );

    ACTORS( (alice)(bob)(carol) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    auto _10 = ASSET( "10.000 TESTS" );
    auto _20 = ASSET( "20.000 TESTS" );
    auto _1v = ASSET( "1.000000 VESTS" );
    auto _2v = ASSET( "2.000000 VESTS" );
    auto _1000 = ASSET( "1000.000 TESTS" );

    FUND( "alice", _1000 );
    FUND( "bob", _1000 );
    FUND( "carol", _1000 );

    {
      vest( "alice", "alice", _10, alice_private_key );
      vest( "alice", "alice", _20, alice_private_key );

      delegate_vest( "alice", "bob", _1v, alice_private_key );
      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) != 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "bob" ) == 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "carol" ) == 0l );

      BOOST_REQUIRE( RECEIVED_VESTS( "alice" ) == 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) != 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "carol" ) == 0l );

      auto previous = DELEGATED_VESTS( "alice" );

      delegate_vest( "alice", "carol", _2v, alice_private_key );

      auto next = DELEGATED_VESTS( "alice" );
      BOOST_REQUIRE( next > previous );

      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) != 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "bob" ) == 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "carol" ) == 0l );

      BOOST_REQUIRE( RECEIVED_VESTS( "alice" ) == 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) != 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "carol" ) != 0l );
    }
    {
      const auto& _alice = db->get_account( "alice" );
      db->clear_account( _alice );

      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) == 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "bob" ) == 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "carol" ) == 0l );

      BOOST_REQUIRE( RECEIVED_VESTS( "alice" ) == 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) == 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "carol" ) == 0l );

      const auto& _bob = db->get_account( "bob" );
      db->clear_account( _bob );

      BOOST_REQUIRE( DELEGATED_VESTS( "alice" ) == 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "bob" ) == 0l );
      BOOST_REQUIRE( DELEGATED_VESTS( "carol" ) == 0l );

      BOOST_REQUIRE( RECEIVED_VESTS( "alice" ) == 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "bob" ) == 0l );
      BOOST_REQUIRE( RECEIVED_VESTS( "carol" ) == 0l );
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( basic_test_03 )
{
  try
  {
    BOOST_TEST_MESSAGE( "Vesting every account to anothers accounts" );

    ACTORS( (alice)(bob)(carol) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    auto _1 = ASSET( "1.000 TESTS" );
    auto _2 = ASSET( "2.000 TESTS" );
    auto _3 = ASSET( "3.000 TESTS" );
    auto _1000 = ASSET( "1000.000 TESTS" );

    FUND( "alice", _1000 );
    FUND( "bob", _1000 );
    FUND( "carol", _1000 );

    BOOST_REQUIRE( get_vesting( "alice" ) == get_vesting( "bob" ) );
    BOOST_REQUIRE( get_vesting( "bob" ) == get_vesting( "carol" ) );

    {
      vest( "alice", "bob", _1, alice_private_key );
      vest( "bob", "carol", _2, bob_private_key );
      vest( "carol", "alice", _3, carol_private_key );
      BOOST_REQUIRE_GT( get_vesting( "alice" ).amount.value, get_vesting( "carol" ).amount.value );
      BOOST_REQUIRE_GT( get_vesting( "carol" ).amount.value, get_vesting( "bob" ).amount.value );
    }
    {
      auto vest_bob = get_vesting( "bob" ).amount.value;
      auto vest_carol = get_vesting( "carol" ).amount.value;

      const auto& _alice = db->get_account( "alice" );
      db->clear_account( _alice );

      BOOST_REQUIRE( get_vesting( "alice" ).amount.value == 0l );
      BOOST_REQUIRE( get_vesting( "bob" ).amount.value == vest_bob );
      BOOST_REQUIRE( get_vesting( "carol" ).amount.value == vest_carol );

      const auto& _bob = db->get_account( "bob" );
      db->clear_account( _bob );

      BOOST_REQUIRE( get_vesting( "alice" ).amount.value == 0l );
      BOOST_REQUIRE( get_vesting( "bob" ).amount.value == 0l );
      BOOST_REQUIRE( get_vesting( "carol" ).amount.value == vest_carol );

      const auto& _carol = db->get_account( "carol" );
      db->clear_account( _carol );

      BOOST_REQUIRE( get_vesting( "alice" ).amount.value == 0l );
      BOOST_REQUIRE( get_vesting( "bob" ).amount.value == 0l );
      BOOST_REQUIRE( get_vesting( "carol" ).amount.value == 0l );
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( basic_test_02 )
{
  try
  {
    BOOST_TEST_MESSAGE( "Vesting" );

    ACTORS( (alice)(bob) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    auto _1 = ASSET( "1.000 TESTS" );
    auto _1000 = ASSET( "1000.000 TESTS" );

    FUND( "alice", _1000 );
    FUND( "bob", _1000 );

    BOOST_REQUIRE( get_vesting( "alice" ) == get_vesting( "bob" ) );

    {
      vest( "alice", "alice", _1, alice_private_key );
      BOOST_REQUIRE_GT( get_vesting( "alice" ).amount.value, get_vesting( "bob" ).amount.value );
    }
    {
      auto vest_bob = get_vesting( "bob" ).amount.value;

      const auto& _alice = db->get_account( "alice" );
      db->clear_account( _alice );

      BOOST_REQUIRE( get_vesting( "alice" ).amount.value == 0l );
      BOOST_REQUIRE( get_vesting( "bob" ).amount.value == vest_bob );

      const auto& _bob = db->get_account( "bob" );
      db->clear_account( _bob );

      BOOST_REQUIRE( get_vesting( "alice" ).amount.value == 0l );
      BOOST_REQUIRE( get_vesting( "bob" ).amount.value == 0l );
    }

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( basic_test_01 )
{
  try
  {
    BOOST_TEST_MESSAGE( "Calling clear_account" );

    ACTORS( (alice) )
    generate_block();

    const auto& _alice = db->get_account( "alice" );

    db->clear_account( _alice );

    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

#define CLEAR( who ) \
  if( !db->pending_transaction_session().valid() ) \
    db->pending_transaction_session() = db->start_undo_session(); \
  db->clear_account( db->get_account( who ) ); \
  database_fixture::validate_database()
//since generating block runs undo and reapplies transactions since last block (and clear_account is not a transaction)
//it is better to do it explicitly to avoid confusion when reading and debugging test
#define UNDO_CLEAR db->pending_transaction_session().reset()

BOOST_AUTO_TEST_CASE( escrow_cleanup_test )
{

#define REQUIRE_BALANCE( alice, bob, carol, treasury, getter, asset ) \
  BOOST_REQUIRE( getter( "alice" ) == ASSET( alice " " asset ) ); \
  BOOST_REQUIRE( getter( "bob" ) == ASSET( bob " " asset ) ); \
  BOOST_REQUIRE( getter( "carol" ) == ASSET( carol " " asset ) ); \
  BOOST_REQUIRE( getter( db->get_treasury_name() ) == ASSET( treasury " " asset ) )

  try
  {
    BOOST_TEST_MESSAGE( "Calling escrow_cleanup_test" );

    ACTORS( (alice)(bob)(carol) )
    generate_block();

    fund( "alice", ASSET( "10.000 TESTS" ) ); //<- note! extra 0.1 is in form of vests
    fund( "alice", ASSET( "10.100 TBD" ) ); //<- note! treasury will get extras from interest and sps-fund/inflation
    generate_block();
    REQUIRE_BALANCE( "10.000", "0.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "10.100", "0.000", "0.000", "0.027", get_cbd_balance, "TBD" );

    signed_transaction tx;
    tx.set_expiration( db->head_block_time() + CREA_MAX_TIME_UNTIL_EXPIRATION );
    {
      escrow_transfer_operation op;
      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.crea_amount = ASSET( "10.000 TESTS" );
      op.cbd_amount = ASSET( "10.000 TBD" );
      op.fee = ASSET( "0.100 TBD" );
      op.json_meta = "";
      op.ratification_deadline = db->head_block_time() + fc::seconds( CREA_BLOCK_INTERVAL * 10 );
      op.escrow_expiration = db->head_block_time() + fc::seconds( CREA_BLOCK_INTERVAL * 20 );
      tx.operations.push_back( op );
      sign( tx, alice_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "0.027", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) != nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 1 );
    generate_block();

    //escrow transfer requested but neither receiver nor agent approved yet
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "10.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "10.136", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    UNDO_CLEAR;

    {
      escrow_approve_operation op;
      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.who = "carol";
      tx.operations.push_back( op );
      sign( tx, carol_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "0.036", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) != nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 1 );
    generate_block();

    //escrow transfer approved by agent but not by receiver
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "10.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "10.145", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    UNDO_CLEAR;

    {
      escrow_approve_operation op;
      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.who = "bob";
      tx.operations.push_back( op );
      sign( tx, bob_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.100", "0.045", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) != nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 1 );
    generate_block();

    //escrow transfer approved by all parties (agent got fee) but the transfer itself wasn't released yet
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.000", "10.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.000", "0.100", "10.054", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    UNDO_CLEAR;

    {
      escrow_release_operation op;
      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.who = "alice";
      op.receiver = "bob";
      op.crea_amount = ASSET( "2.000 TESTS" );
      op.cbd_amount = ASSET( "3.000 TBD" );
      tx.operations.push_back( op );
      sign( tx, alice_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "2.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "3.000", "0.100", "0.054", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) != nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 1 );
    generate_block();

    //escrow transfer released partially by sender prior to escrow expiration
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "2.000", "0.000", "8.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "3.000", "0.100", "7.063", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    UNDO_CLEAR;

    {
      escrow_release_operation op;
      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.who = "bob";
      op.receiver = "alice";
      op.crea_amount = ASSET( "2.000 TESTS" );
      op.cbd_amount = ASSET( "3.000 TBD" );
      tx.operations.push_back( op );
      sign( tx, bob_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "2.000", "2.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "3.000", "3.000", "0.100", "0.063", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) != nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 1 );
    generate_block();

    //escrow transfer released partially by receiver prior to escrow expiration
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "2.000", "0.000", "8.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "3.000", "0.100", "7.072", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    UNDO_CLEAR;

    //after approvals it doesn't matter if the ratification expires

    {
      escrow_dispute_operation op;
      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.who = "bob";
      tx.operations.push_back( op );
      sign( tx, bob_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "2.000", "2.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "3.000", "3.000", "0.100", "0.072", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) != nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 1 );
    generate_block();

    //escrow transfer disputed by sender
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "2.000", "0.000", "8.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "3.000", "0.100", "7.081", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    UNDO_CLEAR;

    //after dispute it doesn't matter if the escrow expires
    
    {
      escrow_release_operation op;
      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.who = "carol";
      op.receiver = "bob";
      op.crea_amount = ASSET( "2.000 TESTS" );
      op.cbd_amount = ASSET( "3.000 TBD" );
      tx.operations.push_back( op );

      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.who = "carol";
      op.receiver = "alice";
      op.crea_amount = ASSET( "2.000 TESTS" );
      op.cbd_amount = ASSET( "1.000 TBD" );
      tx.operations.push_back( op );

      sign( tx, carol_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "4.000", "4.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "4.000", "6.000", "0.100", "0.081", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) != nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 1 );
    generate_block();

    //escrow transfer released partially by agent to sender and partially to receiver
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "4.000", "0.000", "6.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "6.000", "0.100", "4.090", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    UNDO_CLEAR;

    {
      escrow_release_operation op;
      op.from = "alice";
      op.to = "bob";
      op.agent = "carol";
      op.who = "carol";
      op.receiver = "bob";
      op.crea_amount = ASSET( "2.000 TESTS" );
      op.cbd_amount = ASSET( "0.000 TBD" );
      tx.operations.push_back( op );
      sign( tx, carol_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "4.000", "6.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "4.000", "6.000", "0.100", "0.090", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    generate_block();

    //escrow transfer released by agent to receiver and finished (transfer fully executed)
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "6.000", "0.000", "4.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "6.000", "0.100", "4.099", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_escrow( "alice", 30 ) == nullptr );
    BOOST_REQUIRE( db->get_account( "alice" ).pending_transfers == 0 );
    UNDO_CLEAR;
  }
  FC_LOG_AND_RETHROW()

#undef REQUIRE_BALANCE

}

BOOST_AUTO_TEST_CASE( limit_order_cleanup_test )
{

#define REQUIRE_BALANCE( alice, bob, treasury, getter, asset ) \
  BOOST_REQUIRE( getter( "alice" ) == ASSET( alice " " asset ) ); \
  BOOST_REQUIRE( getter( "bob" ) == ASSET( bob " " asset ) ); \
  BOOST_REQUIRE( getter( db->get_treasury_name() ) == ASSET( treasury " " asset ) )

  try
  {
    BOOST_TEST_MESSAGE( "Calling limit_order_cleanup_test" );
    ACTORS( (alice)(bob) )
    generate_block();

    fund( "alice", ASSET( "10.000 TESTS" ) ); //<- note! extra 0.1 is in form of vests
    fund( "bob", ASSET( "5.000 TBD" ) ); //<- note! treasury will get extras from interest and sps-fund/inflation
    generate_block();
    REQUIRE_BALANCE( "10.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "5.000", "0.027", get_cbd_balance, "TBD" );

    signed_transaction tx;
    tx.set_expiration( db->head_block_time() + CREA_MAX_TIME_UNTIL_EXPIRATION );
    {
      limit_order_create_operation op;
      op.owner = "alice";
      op.amount_to_sell = ASSET( "10.000 TESTS" );
      op.min_to_receive = ASSET( "5.000 TBD" );
      op.expiration = db->head_block_time() + fc::seconds( CREA_BLOCK_INTERVAL * 20 );
      tx.operations.push_back( op );
      sign( tx, alice_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "5.000", "0.027", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_limit_order( "alice", 0 ) != nullptr );
    generate_block();

    //limit order set but not matched yet
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "0.000", "10.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "5.000", "0.036", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_limit_order( "alice", 0 ) == nullptr );
    UNDO_CLEAR;

    {
      limit_order_create_operation op;
      op.owner = "bob";
      op.amount_to_sell = ASSET( "2.000 TBD" );
      op.min_to_receive = ASSET( "3.500 TESTS" );
      op.fill_or_kill = true;
      op.expiration = db->head_block_time() + fc::seconds( CREA_BLOCK_INTERVAL * 20 );
      tx.operations.push_back( op );
      sign( tx, bob_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "4.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "2.000", "3.000", "0.036", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_limit_order( "alice", 0 ) != nullptr );
    generate_block();

    //limit order partially matched
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "4.000", "6.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "3.000", "2.045", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_limit_order( "alice", 0 ) == nullptr );
    UNDO_CLEAR;

    {
      limit_order_create2_operation op;
      op.owner = "bob";
      op.amount_to_sell = ASSET( "3.000 TBD" );
      op.exchange_rate = ASSET( "2.000 TBD" ) / ASSET( "5.000 TESTS" );
      op.fill_or_kill = true;
      op.expiration = db->head_block_time() + fc::seconds( CREA_BLOCK_INTERVAL * 20 );
      tx.operations.push_back( op );
      sign( tx, bob_private_key );
      CREA_REQUIRE_THROW( db->push_transaction( tx, 0 ), fc::exception );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "4.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "2.000", "3.000", "0.045", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_limit_order( "alice", 0 ) != nullptr );
    generate_block();

    //limit order partially matched after counter-order in fill_or_kill mode that failed (no change)
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "4.000", "6.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "3.000", "2.054", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_limit_order( "alice", 0 ) == nullptr );
    UNDO_CLEAR;

    {
      limit_order_create2_operation op;
      op.owner = "bob";
      op.amount_to_sell = ASSET( "3.000 TBD" );
      op.exchange_rate = ASSET( "2.000 TBD" ) / ASSET( "4.000 TESTS" );
      op.fill_or_kill = true;
      op.expiration = db->head_block_time() + fc::seconds( CREA_BLOCK_INTERVAL * 20 );
      tx.operations.push_back( op );
      sign( tx, bob_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "10.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "5.000", "0.000", "0.054", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_limit_order( "alice", 0 ) == nullptr );
    generate_block();

    //limit order fully matched
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "10.000", "0.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.000", "5.063", get_cbd_balance, "TBD" );
    BOOST_REQUIRE( db->find_limit_order( "alice", 0 ) == nullptr );
    UNDO_CLEAR;
  }
  FC_LOG_AND_RETHROW()

#undef REQUIRE_BALANCE

}

BOOST_AUTO_TEST_CASE( convert_request_cleanup_test )
{

#define REQUIRE_BALANCE( alice, treasury, getter, asset ) \
  BOOST_REQUIRE( getter( "alice" ) == ASSET( alice " " asset ) ); \
  BOOST_REQUIRE( getter( db->get_treasury_name() ) == ASSET( treasury " " asset ) )

  try
  {
    BOOST_TEST_MESSAGE( "Calling convert_request_cleanup_test" );
    ACTORS( (alice) )
    generate_block();

    //note! extra 0.1 TESTS is in form of vests
    fund( "alice", ASSET( "5.000 TBD" ) ); //<- note! treasury will get extras from interest and sps-fund/inflation
    generate_block();
    REQUIRE_BALANCE( "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "5.000", "0.027", get_cbd_balance, "TBD" );

    signed_transaction tx;
    tx.set_expiration( db->head_block_time() + CREA_MAX_TIME_UNTIL_EXPIRATION );
    {
      convert_operation op;
      op.owner = "alice";
      op.amount = ASSET( "5.000 TBD" );
      tx.operations.push_back( op );
      sign( tx, alice_private_key );
      db->push_transaction( tx, 0 );
    }
    tx.clear();

    REQUIRE_BALANCE( "0.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.027", get_cbd_balance, "TBD" );
    generate_block();

    //conversion request created but not executed yet
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "0.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "5.036", get_cbd_balance, "TBD" );
    UNDO_CLEAR;

    generate_blocks( db->head_block_time() + CREA_CONVERSION_DELAY );

    REQUIRE_BALANCE( "5.000", "0.000", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.054", get_cbd_balance, "TBD" );
    generate_block();

    //conversion request executed
    CLEAR( "alice" );
    REQUIRE_BALANCE( "0.000", "5.100", get_balance, "TESTS" );
    REQUIRE_BALANCE( "0.000", "0.063", get_cbd_balance, "TBD" );
    UNDO_CLEAR;
  }
  FC_LOG_AND_RETHROW()

#undef REQUIRE_BALANCE

}

BOOST_AUTO_TEST_CASE( cbd_test_01 )
{
  try
  {
    ACTORS( (alice) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    BOOST_REQUIRE( get_cbd_balance( "alice" ) == ASSET( "0.000 TBD" ) );
    db->clear_account( db->get_account( "alice" ) );
    BOOST_REQUIRE( get_cbd_balance( "alice" ) == ASSET( "0.000 TBD" ) );
    database_fixture::validate_database();

    fund( "alice", ASSET( "1000.000 TBD" ) );
    auto alice_cbd = get_cbd_balance( "alice" );
    BOOST_REQUIRE( alice_cbd == ASSET( "1000.000 TBD" ) );
    db->clear_account( db->get_account( "alice" ) );
    BOOST_REQUIRE( get_cbd_balance( "alice" ) == ASSET( "0.000 TBD" ) );
    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( cbd_test_02 )
{
  try
  {
    ACTORS( (alice)(bob) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    BOOST_REQUIRE( get_cbd_balance( "alice" ) == ASSET( "0.000 TBD" ) );
    fund( "alice", ASSET( "1000.000 TBD" ) );
    auto start_time = db->get_account( "alice" ).cbd_seconds_last_update;
    auto alice_cbd = get_cbd_balance( "alice" );
    BOOST_TEST_MESSAGE( "treasury_cbd = " << asset_to_string( db->get_treasury().get_cbd_balance() ) );
    BOOST_TEST_MESSAGE( "alice_cbd = " << asset_to_string( alice_cbd ) );
    BOOST_REQUIRE( alice_cbd == ASSET( "1000.000 TBD" ) );

    generate_blocks( db->head_block_time() + fc::seconds( CREA_CBD_INTEREST_COMPOUND_INTERVAL_SEC ), true );

    {
    signed_transaction tx;
    transfer_operation transfer;
    transfer.to = "bob";
    transfer.from = "alice";
    transfer.amount = ASSET( "1.000 TBD" );
    tx.set_expiration( db->head_block_time() + CREA_MAX_TIME_UNTIL_EXPIRATION );
    tx.operations.push_back( transfer );
    sign( tx, alice_private_key );
    db->push_transaction( tx, 0 );
    }

    auto& gpo = db->get_dynamic_global_properties();
    auto interest_op = get_last_operations( 1 )[0].get< interest_operation >();

    BOOST_REQUIRE( gpo.get_cbd_interest_rate() > 0 );
    BOOST_REQUIRE( static_cast<uint64_t>(get_cbd_balance( "alice" ).amount.value) ==
      alice_cbd.amount.value - ASSET( "1.000 TBD" ).amount.value +
      ( ( ( ( uint128_t( alice_cbd.amount.value ) * ( db->head_block_time() - start_time ).to_seconds() ) / CREA_SECONDS_PER_YEAR ) *
        gpo.get_cbd_interest_rate() ) / CREA_100_PERCENT ).to_uint64() );
    BOOST_REQUIRE( interest_op.owner == "alice" );
    BOOST_REQUIRE( interest_op.interest.amount.value ==
      get_cbd_balance( "alice" ).amount.value - ( alice_cbd.amount.value - ASSET( "1.000 TBD" ).amount.value ) );
    database_fixture::validate_database();

    generate_blocks( db->head_block_time() + fc::seconds( CREA_CBD_INTEREST_COMPOUND_INTERVAL_SEC ), true );

    alice_cbd = get_cbd_balance( "alice" );
    BOOST_TEST_MESSAGE( "alice_cbd = " << asset_to_string( alice_cbd ) );
    BOOST_TEST_MESSAGE( "bob_cbd = " << asset_to_string( get_cbd_balance( "bob" ) ) );

    db->clear_account( db->get_account( "alice" ) );
    BOOST_REQUIRE( get_cbd_balance( "alice" ) == ASSET( "0.000 TBD" ) );
    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( savings_test_01 )
{
  try
  {
    ACTORS( (alice) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    fund( "alice", ASSET( "1000.000 TBD" ) );
    
    signed_transaction tx;
    transfer_to_savings_operation op;
    op.from = "alice";
    op.to = "alice";
    op.amount = ASSET( "1000.000 TBD" );
    tx.set_expiration( db->head_block_time() + CREA_MAX_TIME_UNTIL_EXPIRATION );
    tx.operations.push_back( op );
    sign( tx, alice_private_key );
    db->push_transaction( tx, 0 );

    BOOST_REQUIRE( get_cbd_savings( "alice" ) == ASSET( "1000.000 TBD" ) );
    db->clear_account( db->get_account( "alice" ) );
    BOOST_REQUIRE( get_cbd_savings( "alice" ) == ASSET( "0.000 TBD" ) );
    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_CASE( savings_test_02 )
{
  try
  {
    ACTORS( (alice)(bob) )
    generate_block();

    set_price_feed( price( ASSET( "1.000 TBD" ), ASSET( "1.000 TESTS" ) ) );
    generate_block();

    fund( "alice", ASSET( "1000.000 TBD" ) );

    signed_transaction tx;
    transfer_to_savings_operation op;
    op.from = "alice";
    op.to = "alice";
    op.amount = ASSET( "1000.000 TBD" );
    tx.set_expiration( db->head_block_time() + CREA_MAX_TIME_UNTIL_EXPIRATION );
    tx.operations.push_back( op );
    sign( tx, alice_private_key );
    db->push_transaction( tx, 0 );

    BOOST_REQUIRE( get_cbd_savings( "alice" ) == ASSET( "1000.000 TBD" ) );

    generate_blocks( db->head_block_time() + fc::seconds( CREA_CBD_INTEREST_COMPOUND_INTERVAL_SEC ), true );

    BOOST_TEST_MESSAGE( "alice_savings_cbd before clear = " << asset_to_string( get_cbd_savings( "alice" ) ) );
    db->clear_account( db->get_account( "alice" ) );
    BOOST_TEST_MESSAGE( "alice_savings_cbd after clear = " << asset_to_string( get_cbd_savings( "alice" ) ) );
    BOOST_REQUIRE( get_cbd_savings( "alice" ) == ASSET( "0.000 TBD" ) );
    database_fixture::validate_database();
  }
  FC_LOG_AND_RETHROW()
}

BOOST_AUTO_TEST_SUITE_END()

#endif
