#pragma once
#include <crea/protocol/required_automated_actions.hpp>

#include <crea/chain/crea_object_types.hpp>

#include <boost/multi_index/composite_key.hpp>

namespace crea { namespace chain {

using crea::protocol::optional_automated_action;

class pending_optional_action_object : public object< pending_optional_action_object_type, pending_optional_action_object >
{
   pending_optional_action_object() = delete;

   public:
      template< typename Constructor, typename Allocator >
      pending_optional_action_object( Constructor&& c, allocator< Allocator > a )
      {
         c( *this );
      }

      id_type                    id;

      time_point_sec             execution_time;
      optional_automated_action  action;
};

typedef multi_index_container<
   pending_optional_action_object,
   indexed_by<
      ordered_unique< tag< by_id >, member< pending_optional_action_object, pending_optional_action_id_type, &pending_optional_action_object::id > >,
      ordered_unique< tag< by_execution >,
         composite_key< pending_optional_action_object,
            member< pending_optional_action_object, time_point_sec, &pending_optional_action_object::execution_time >,
            member< pending_optional_action_object, pending_optional_action_id_type, &pending_optional_action_object::id >
         >
      >
   >,
   allocator< pending_optional_action_object >
> pending_optional_action_index;

} } //crea::chain

FC_REFLECT( crea::chain::pending_optional_action_object,
            (id)(execution_time)(action) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::pending_optional_action_object, crea::chain::pending_optional_action_index )