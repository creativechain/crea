#pragma once
#include <crea/chain/crea_fwd.hpp>

#include <crea/protocol/version.hpp>

#include <crea/chain/crea_object_types.hpp>

namespace crea { namespace chain {

  using chainbase::t_vector;

  class hardfork_property_object : public object< hardfork_property_object_type, hardfork_property_object >
  {
    CHAINBASE_OBJECT( hardfork_property_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( hardfork_property_object, (processed_hardforks) )

      using t_processed_hardforks = t_vector< fc::time_point_sec >;

      t_processed_hardforks       processed_hardforks;
      uint32_t                    last_hardfork = 0;
      protocol::hardfork_version  current_hardfork_version;
      protocol::hardfork_version  next_hardfork;
      fc::time_point_sec          next_hardfork_time;

    CHAINBASE_UNPACK_CONSTRUCTOR(hardfork_property_object, (processed_hardforks));
  };

  typedef multi_index_container<
    hardfork_property_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< hardfork_property_object, hardfork_property_object::id_type, &hardfork_property_object::get_id > >
    >,
    allocator< hardfork_property_object >
  > hardfork_property_index;

} } // crea::chain

FC_REFLECT( crea::chain::hardfork_property_object,
  (id)(processed_hardforks)(last_hardfork)(current_hardfork_version)
  (next_hardfork)(next_hardfork_time) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::hardfork_property_object, crea::chain::hardfork_property_index )
