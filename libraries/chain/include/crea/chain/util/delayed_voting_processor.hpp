#pragma once

#include <crea/chain/crea_object_types.hpp>

namespace crea { namespace chain {

namespace delayed_voting_messages
{
  constexpr const char* incorrect_head_time             = "head time must be greater or equal to last delayed voting time";
  constexpr const char* incorrect_sum_greater_equal     = "unexpected error: sum of delayed votings must be greater or equal to zero";
  constexpr const char* incorrect_sum_equal             = "unexpected error: sum of delayed votings must be equal to zero";
  constexpr const char* incorrect_erased_votes          = "unexpected error: number votes to be erased must be greater or equal to sum of delayed votings";
  constexpr const char* object_is_null                  = "unexpected error: objects are empty";
  constexpr const char* incorrect_votes_update          = "unexpected error: votes updating is incorrect";
  constexpr const char* incorrect_withdraw_data         = "unexpected error: withdraw data is inconsistent";
}

struct delayed_votes_data
{
  time_point_sec time;
  ushare_type    val = 0;

  bool operator==( const delayed_votes_data& obj ) const
  {
    return ( time == obj.time ) && ( val == obj.val );
  }
};

struct delayed_voting_processor
{
  template< typename COLLECTION_TYPE >
  static void add( COLLECTION_TYPE& items, ushare_type& sum, const time_point_sec& head_time, const ushare_type val )
  {
    /*
      A collection is filled gradually - every item in `items` is created each CREA_DELAYED_VOTING_INTERVAL_SECONDS time.

      Input data:
        2020-03-10 00:30:00 1000
        2020-03-10 05:00:00 2000
        2020-03-10 11:00:00 7000
        2020-03-11 01:00:00 6000
        2020-03-12 02:00:00 8000
        2020-03-12 02:00:00 200000

      Result:
        items[0] = {2020-03-10,  10000}
        items[1] = {2020-03-11,  6000}
        items[2] = {2020-03-12,  208000}
    */
    if( val == 0 )
      return;

    if( items.empty() )
    {
      sum += val;
      items.emplace_back( delayed_votes_data{ head_time, val } );
      return;
    }

    delayed_votes_data& back_obj = items.back();
    FC_ASSERT( head_time >= back_obj.time, "unexpected error: ${error}", ("error", delayed_voting_messages::incorrect_head_time ) );

    sum += val;

    if( head_time >= back_obj.time + CREA_DELAYED_VOTING_INTERVAL_SECONDS )
    {
      items.emplace_back( delayed_votes_data{ head_time, val } );
    }
    else
    {
      back_obj.val += val;
    }
  }

  template< typename COLLECTION_TYPE >
  static void erase_front( COLLECTION_TYPE& items, ushare_type& sum )
  {
    if( !items.empty() )
    {
      auto _begin = items.begin();

      FC_ASSERT( sum >= _begin->val, "unexpected error: ${error}", ("error", delayed_voting_messages::incorrect_sum_greater_equal ) );

      sum -= _begin->val;
      items.erase( items.begin() );
    }
    else
      FC_ASSERT( sum == 0, "unexpected error: ${error}", ("error", delayed_voting_messages::incorrect_sum_equal ) );
  }

  template< typename COLLECTION_TYPE >
  static void erase( COLLECTION_TYPE& items, ushare_type& sum, ushare_type count )
  {
    if( count == 0 )
      return;

    FC_ASSERT( count <= sum, "unexpected error: ${error}", ("error", delayed_voting_messages::incorrect_erased_votes ) );

    if( sum == count )
    {
      sum = 0;
      items.clear();
    }
    else
    {
      sum -= count;

      while( true )
      {
        auto&  obj = items.back();

        if( count >= obj.val )
        {
          count -= obj.val;
          items.pop_back();
        }
        else
        {
          obj.val -= count;
          break;
        }
      }
    }
  }

};

} } // namespace crea::chain

FC_REFLECT( crea::chain::delayed_votes_data,
          (time)(val)
        )
