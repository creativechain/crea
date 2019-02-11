#pragma once

#include <crea/protocol/transaction.hpp>
#include <crea/protocol/optional_automated_actions.hpp>

#include <fc/int_array.hpp>
#include <fc/reflect/reflect.hpp>
#include <vector>

#define CREA_NUM_RESOURCE_TYPES     5

namespace crea { namespace plugins { namespace rc {

enum rc_resource_types
{
   resource_history_bytes,
   resource_new_accounts,
   resource_market_bytes,
   resource_state_bytes,
   resource_execution_time
};

typedef fc::int_array< int64_t, CREA_NUM_RESOURCE_TYPES > resource_count_type;

struct count_resources_result
{
   resource_count_type                            resource_count;
};

void count_resources(
   const crea::protocol::signed_transaction& tx,
   count_resources_result& result );

void count_resources(
   const crea::protocol::optional_automated_action&,
   count_resources_result& result );

} } } // crea::plugins::rc

FC_REFLECT_ENUM( crea::plugins::rc::rc_resource_types,
    (resource_history_bytes)
    (resource_new_accounts)
    (resource_market_bytes)
    (resource_state_bytes)
    (resource_execution_time)
   )

FC_REFLECT( crea::plugins::rc::count_resources_result,
   (resource_count)
)

FC_REFLECT_TYPENAME( crea::plugins::rc::resource_count_type )
