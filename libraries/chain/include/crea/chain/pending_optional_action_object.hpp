#pragma once
#include <crea/chain/crea_fwd.hpp>

#include <crea/protocol/required_automated_actions.hpp>

#include <crea/chain/crea_object_types.hpp>

namespace crea { namespace chain {

using crea::protocol::optional_automated_action;

class pending_optional_action_object : public object< pending_optional_action_object_type, pending_optional_action_object >
{
  CHAINBASE_OBJECT( pending_optional_action_object );
  public:
    CHAINBASE_DEFAULT_CONSTRUCTOR( pending_optional_action_object )

    time_point_sec             execution_time;
    optional_automated_action  action;

  CHAINBASE_UNPACK_CONSTRUCTOR(pending_optional_action_object);
};

typedef multi_index_container<
  pending_optional_action_object,
  indexed_by<
    ordered_unique< tag< by_id >,
      const_mem_fun< pending_optional_action_object, pending_optional_action_object::id_type, &pending_optional_action_object::get_id > >,
    ordered_unique< tag< by_execution >,
      composite_key< pending_optional_action_object,
        member< pending_optional_action_object, time_point_sec, &pending_optional_action_object::execution_time >,
        const_mem_fun< pending_optional_action_object, pending_optional_action_object::id_type, &pending_optional_action_object::get_id >
      >
    >
  >,
  allocator< pending_optional_action_object >
> pending_optional_action_index;

} } //crea::chain

FC_REFLECT( crea::chain::pending_optional_action_object,
        (id)(execution_time)(action) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::pending_optional_action_object, crea::chain::pending_optional_action_index )
