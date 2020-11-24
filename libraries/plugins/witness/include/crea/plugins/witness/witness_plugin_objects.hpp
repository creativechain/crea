#pragma once

#include <crea/chain/crea_object_types.hpp>

#ifndef CREA_WITNESS_SPACE_ID
#define CREA_WITNESS_SPACE_ID 19
#endif

namespace crea { namespace chain {
struct by_account;
} }

namespace crea { namespace plugins { namespace witness {

using namespace crea::chain;

enum witness_object_types
{
  witness_custom_op_object_type          = ( CREA_WITNESS_SPACE_ID << 8 )
};

class witness_custom_op_object : public object< witness_custom_op_object_type, witness_custom_op_object >
{
  CHAINBASE_OBJECT( witness_custom_op_object );
  public:
    CHAINBASE_DEFAULT_CONSTRUCTOR( witness_custom_op_object )

    account_name_type    account;
    uint32_t             count = 0;
};

typedef multi_index_container<
  witness_custom_op_object,
  indexed_by<
    ordered_unique< tag< by_id >,
      const_mem_fun< witness_custom_op_object, witness_custom_op_object::id_type, &witness_custom_op_object::get_id > >,
    ordered_unique< tag< by_account >,
      member< witness_custom_op_object, account_name_type, &witness_custom_op_object::account > >
  >,
  allocator< witness_custom_op_object >
> witness_custom_op_index;

} } }

FC_REFLECT( crea::plugins::witness::witness_custom_op_object,
  (id)
  (account)
  (count)
  )
CHAINBASE_SET_INDEX_TYPE( crea::plugins::witness::witness_custom_op_object, crea::plugins::witness::witness_custom_op_index )
