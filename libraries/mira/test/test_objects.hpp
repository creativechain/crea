#pragma once

#include <chainbase/chainbase.hpp>

#include <fc/reflect/reflect.hpp>
#include <fc/reflect/variant.hpp>

#include <crea/protocol/fixed_string.hpp>

#include <mira/index_adapter.hpp>
#include <mira/ordered_index.hpp>
#include <mira/tag.hpp>
#include <mira/member.hpp>
#include <mira/indexed_by.hpp>
#include <mira/composite_key.hpp>
#include <mira/mem_fun.hpp>

enum test_object_type
{
  book_object_type,
  single_index_object_type,
  test_object_type,
  test_object2_type,
  test_object3_type,
  account_object_type
};

class book : public chainbase::object< book_object_type, book >
{
  CHAINBASE_OBJECT( book );

public:
  CHAINBASE_DEFAULT_CONSTRUCTOR( book )

  int a = 0;
  int b = 1;

  int sum()const { return a + b; }

};
typedef chainbase::oid_ref< book > book_id_type;


namespace fc {
namespace raw {
template<typename Stream>
inline void pack(Stream& s, const book&)
  {
  }

template<typename Stream>
inline void unpack(Stream& s, book& id, uint32_t depth = 0)
  {
  }
}
}


using chainbase::by_id;
struct by_a;
struct by_b;
struct by_sum;

typedef mira::multi_index_adapter<
  book,
  mira::multi_index::indexed_by<
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_id >,
      mira::multi_index::const_mem_fun< book, book::id_type, &book::get_id > >,
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_a >,
      mira::multi_index::member< book, int, &book::a > >,
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_b >,
      mira::multi_index::composite_key< book,
        mira::multi_index::member< book, int, &book::b >,
        mira::multi_index::member< book, int, &book::a >
      >,
      mira::multi_index::composite_key_compare< std::greater< int >, std::less< int > >
    >,
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_sum >, mira::multi_index::const_mem_fun< book, int, &book::sum > >
  >,
  chainbase::allocator< book >
> book_index;

class single_index_object : public chainbase::object< single_index_object_type, single_index_object >
{
  CHAINBASE_OBJECT( single_index_object );

public:
  CHAINBASE_DEFAULT_CONSTRUCTOR( single_index_object )

};
typedef chainbase::oid_ref< single_index_object > single_index_id_type;

typedef mira::multi_index_adapter<
  single_index_object,
  mira::multi_index::indexed_by<
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_id >,
      mira::multi_index::const_mem_fun< single_index_object, single_index_object::id_type, &single_index_object::get_id > >
  >,
  chainbase::allocator< single_index_object >
> single_index_index;

class test_object : public chainbase::object< test_object_type, test_object >
{
  CHAINBASE_OBJECT( test_object );

public:
  CHAINBASE_DEFAULT_CONSTRUCTOR( test_object, (name) )
  void set_id( int _id ) { id = id_type( _id ); }

  uint32_t val = 0;
  std::string name;

};
typedef chainbase::oid_ref< test_object > test_id_type;

struct composited_ordered_idx;

typedef mira::multi_index_adapter<
  test_object,
  mira::multi_index::indexed_by<
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_id >,
      mira::multi_index::const_mem_fun< test_object, test_object::id_type, &test_object::get_id > >,
    mira::multi_index::ordered_unique< mira::multi_index::tag< composited_ordered_idx >,
      mira::multi_index::composite_key< test_object,
        mira::multi_index::member< test_object, std::string, &test_object::name >,
        mira::multi_index::member< test_object, uint32_t, &test_object::val >
      >
    >
  >,
  chainbase::allocator< test_object >
> test_object_index;

class test_object2 : public chainbase::object< test_object2_type, test_object2 >
{
  CHAINBASE_OBJECT( test_object2 );

public:
  CHAINBASE_DEFAULT_CONSTRUCTOR( test_object2 )
  void set_id( int _id ) { id = id_type( _id ); }

  uint32_t val = 0;

};
typedef chainbase::oid_ref< test_object2 > test2_id_type;

struct composite_ordered_idx2;

typedef mira::multi_index_adapter<
  test_object2,
  mira::multi_index::indexed_by<
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_id >,
      mira::multi_index::const_mem_fun< test_object2, test_object2::id_type, &test_object2::get_id > >,
    mira::multi_index::ordered_unique< mira::multi_index::tag< composite_ordered_idx2 >,
      mira::multi_index::composite_key< test_object2,
        mira::multi_index::member< test_object2, uint32_t, &test_object2::val >,
        mira::multi_index::const_mem_fun< test_object2, test_object2::id_type, &test_object2::get_id >
      >
    >
  >,
  chainbase::allocator< test_object2 >
> test_object2_index;

class test_object3 : public chainbase::object< test_object3_type, test_object3 >
{
  CHAINBASE_OBJECT( test_object3 );

public:
  CHAINBASE_DEFAULT_CONSTRUCTOR( test_object3 )
  void set_id( int _id ) { id = id_type( _id ); }

  uint32_t val = 0;
  uint32_t val2 = 0;
  uint32_t val3 = 0;

};

typedef chainbase::oid_ref< test_object3 > test3_id_type;

struct composite_ordered_idx3a;
struct composite_ordered_idx3b;

typedef mira::multi_index_adapter<
  test_object3,
  mira::multi_index::indexed_by<
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_id >,
      mira::multi_index::const_mem_fun< test_object3, test_object3::id_type, &test_object3::get_id > >,
    mira::multi_index::ordered_unique< mira::multi_index::tag< composite_ordered_idx3a >,
      mira::multi_index::composite_key< test_object3,
        mira::multi_index::member< test_object3, uint32_t, &test_object3::val >,
        mira::multi_index::member< test_object3, uint32_t, &test_object3::val2 >
      >
    >,
    mira::multi_index::ordered_unique< mira::multi_index::tag< composite_ordered_idx3b >,
      mira::multi_index::composite_key< test_object3,
        mira::multi_index::member< test_object3, uint32_t, &test_object3::val >,
        mira::multi_index::member< test_object3, uint32_t, &test_object3::val3 >
      >
    >
  >,
  chainbase::allocator< test_object3 >
> test_object3_index;


typedef crea::protocol::fixed_string<16> account_name_type;

class account_object : public chainbase::object< account_object_type, account_object >
{
  CHAINBASE_OBJECT( account_object );

public:
  template< typename Allocator >
  account_object( Allocator&& a, uint64_t _id, const account_name_type& _name )
    : id( _id ), name( _name )
  {}

  account_name_type name;

  CHAINBASE_UNPACK_CONSTRUCTOR( account_object );
};
typedef chainbase::oid_ref< account_object > account_id_type;

struct by_name;

typedef mira::multi_index_adapter<
  account_object,
  mira::multi_index::indexed_by<
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_id >,
      mira::multi_index::const_mem_fun< account_object, account_object::id_type, &account_object::get_id > >,
    mira::multi_index::ordered_unique< mira::multi_index::tag< by_name >,
      mira::multi_index::member< account_object, account_name_type, &account_object::name > >
  >,
  chainbase::allocator< account_object >
> account_index;

FC_REFLECT( book::id_type, (_id) )
FC_REFLECT( book, (id)(a)(b) )
CHAINBASE_SET_INDEX_TYPE( book, book_index )

FC_REFLECT( single_index_object::id_type, (_id) )
FC_REFLECT( single_index_object, (id) )
CHAINBASE_SET_INDEX_TYPE( single_index_object, single_index_index )

FC_REFLECT( test_object::id_type, (_id) )
FC_REFLECT( test_object, (id)(val)(name) )
CHAINBASE_SET_INDEX_TYPE( test_object, test_object_index )

FC_REFLECT( test_object2::id_type, (_id) )
FC_REFLECT( test_object2, (id)(val) )
CHAINBASE_SET_INDEX_TYPE( test_object2, test_object2_index )

FC_REFLECT( test_object3::id_type, (_id) )
FC_REFLECT( test_object3, (id)(val)(val2)(val3) )
CHAINBASE_SET_INDEX_TYPE( test_object3, test_object3_index )

FC_REFLECT( account_object::id_type, (_id) )
FC_REFLECT( account_object, (id)(name) )
CHAINBASE_SET_INDEX_TYPE( account_object, account_index )
