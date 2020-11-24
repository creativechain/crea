#pragma once
#include <crea/chain/crea_object_types.hpp>

namespace crea { namespace plugins { namespace account_by_key {

using namespace std;
using namespace crea::chain;

using crea::protocol::public_key_type;
using crea::protocol::account_name_type;

#ifndef CREA_ACCOUNT_BY_KEY_SPACE_ID
#define CREA_ACCOUNT_BY_KEY_SPACE_ID 11
#endif

enum account_by_key_object_types
{
  key_lookup_object_type = ( CREA_ACCOUNT_BY_KEY_SPACE_ID << 8 )
};

class key_lookup_object : public object< key_lookup_object_type, key_lookup_object >
{
  CHAINBASE_OBJECT( key_lookup_object );
  public:
    CHAINBASE_DEFAULT_CONSTRUCTOR( key_lookup_object )

    public_key_type   key;
    account_name_type account;
};

typedef oid_ref< key_lookup_object > key_lookup_id_type;

struct by_key;

typedef multi_index_container<
  key_lookup_object,
  indexed_by<
    ordered_unique< tag< by_id >,
      const_mem_fun< key_lookup_object, key_lookup_object::id_type, &key_lookup_object::get_id > >,
    ordered_unique< tag< by_key >,
      composite_key< key_lookup_object,
        member< key_lookup_object, public_key_type, &key_lookup_object::key >,
        member< key_lookup_object, account_name_type, &key_lookup_object::account >
      >
    >
  >,
  allocator< key_lookup_object >
> key_lookup_index;

} } } // crea::plugins::account_by_key


FC_REFLECT( crea::plugins::account_by_key::key_lookup_object, (id)(key)(account) )
CHAINBASE_SET_INDEX_TYPE( crea::plugins::account_by_key::key_lookup_object, crea::plugins::account_by_key::key_lookup_index )
