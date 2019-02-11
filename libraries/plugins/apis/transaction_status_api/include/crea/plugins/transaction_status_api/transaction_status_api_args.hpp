#pragma once

#include <crea/protocol/types.hpp>
#include <crea/protocol/transaction.hpp>
#include <crea/protocol/block_header.hpp>

#include <crea/plugins/json_rpc/utility.hpp>
#include <crea/plugins/transaction_status/transaction_status_objects.hpp>

namespace crea { namespace plugins { namespace transaction_status_api {

struct find_transaction_args
{
   chain::transaction_id_type transaction_id;
   fc::optional< fc::time_point_sec > expiration;
};

struct find_transaction_return
{
   transaction_status::transaction_status status;
   fc::optional< uint32_t > block_num;
};

} } } // crea::transaction_status_api

FC_REFLECT( crea::plugins::transaction_status_api::find_transaction_args, (transaction_id)(expiration) )
FC_REFLECT( crea::plugins::transaction_status_api::find_transaction_return, (status)(block_num) )
