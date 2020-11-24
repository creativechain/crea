#pragma once
#include <crea/chain/crea_object_types.hpp>

namespace crea { namespace chain {

  using crea::protocol::block_id_type;

  /**
    *  @brief tracks minimal information about past blocks to implement TaPOS
    *  @ingroup object
    *
    *  When attempting to calculate the validity of a transaction we need to
    *  lookup a past block and check its block hash and the time it occurred
    *  so we can calculate whether the current transaction is valid and at
    *  what time it should expire.
    */
  class block_summary_object : public object< block_summary_object_type, block_summary_object >
  {
    CHAINBASE_OBJECT( block_summary_object );
    public:
      CHAINBASE_DEFAULT_CONSTRUCTOR( block_summary_object )

      block_id_type  block_id;
  };

  typedef multi_index_container<
    block_summary_object,
    indexed_by<
      ordered_unique< tag< by_id >,
        const_mem_fun< block_summary_object, block_summary_object::id_type, &block_summary_object::get_id > >
    >,
    allocator< block_summary_object >
  > block_summary_index;

} } // crea::chain

#ifdef ENABLE_MIRA
namespace mira {

template<> struct is_static_length< crea::chain::block_summary_object > : public boost::true_type {};

} // mira
#endif

FC_REFLECT( crea::chain::block_summary_object, (id)(block_id) )
CHAINBASE_SET_INDEX_TYPE( crea::chain::block_summary_object, crea::chain::block_summary_index )
