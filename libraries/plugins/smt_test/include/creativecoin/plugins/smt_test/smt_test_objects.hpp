#pragma once
#include <creativecoin/chain/creativecoin_object_types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace creativecoin { namespace plugins { namespace smt_test {

using namespace std;
using namespace creativecoin::chain;

#ifndef CREA_SMT_TEST_SPACE_ID
#define CREA_SMT_TEST_SPACE_ID 13
#endif

enum smt_test_object_types
{
   smt_token_object_type = ( CREA_SMT_TEST_SPACE_ID << 8 )
};

class smt_token_object : public object< smt_token_object_type, smt_token_object >
{
   public:
      template< typename Constructor, typename Allocator >
      smt_token_object( Constructor&& c, allocator< Allocator > a )
      {
         c( *this );
      }

      id_type                 id;

      account_name_type       control_account;
      uint8_t                 decimal_places = 0;
      int64_t                 max_supply = CREA_MAX_SHARE_SUPPLY;

      time_point_sec          generation_begin_time;
      time_point_sec          generation_end_time;
      time_point_sec          announced_launch_time;
      time_point_sec          launch_expiration_time;
};

typedef smt_token_object::id_type smt_token_id_type;

using namespace boost::multi_index;

struct by_control_account;

typedef multi_index_container<
   smt_token_object,
   indexed_by<
      ordered_unique< tag< by_id >, member< smt_token_object, smt_token_id_type, &smt_token_object::id > >,
      ordered_unique< tag< by_control_account >,
         composite_key< smt_token_object,
            member< smt_token_object, account_name_type, &smt_token_object::control_account >
         >
      >
   >,
   allocator< smt_token_object >
> smt_token_index;

} } } // creativecoin::plugins::smt_test

FC_REFLECT( creativecoin::plugins::smt_test::smt_token_object,
   (id)
   (control_account)
   (decimal_places)
   (max_supply)
   (generation_begin_time)
   (generation_end_time)
   (announced_launch_time)
   (launch_expiration_time)
   )
CHAINBASE_SET_INDEX_TYPE( creativecoin::plugins::smt_test::smt_token_object, creativecoin::plugins::smt_test::smt_token_index )
