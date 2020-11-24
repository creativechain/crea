#pragma once

#ifdef ENABLE_MIRA
#include <mira/index_adapter.hpp>
#include <mira/multi_index_container.hpp>
#include <mira/ordered_index.hpp>
#include <mira/tag.hpp>
#include <mira/member.hpp>
#include <mira/indexed_by.hpp>
#include <mira/composite_key.hpp>
#include <mira/mem_fun.hpp>
#endif

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/tag.hpp>
#include <mira/boost_adapter.hpp>

#include <boost/mpl/vector.hpp>
#include <type_traits>
#include <typeinfo>

namespace crea { namespace chain {

#ifndef ENABLE_MIRA
using boost::multi_index::multi_index_container;
using boost::multi_index::indexed_by;
using boost::multi_index::ordered_unique;
using boost::multi_index::tag;
using boost::multi_index::member;
using boost::multi_index::composite_key;
using boost::multi_index::composite_key_compare;
using boost::multi_index::const_mem_fun;

template< class Iterator >
inline boost::reverse_iterator< Iterator > make_reverse_iterator( Iterator iterator )
{
  return boost::reverse_iterator< Iterator >( iterator );
}

#else
template< typename... Args >
using multi_index_container = mira::multi_index_adapter< Args... >;
using mira::multi_index::indexed_by;
using mira::multi_index::ordered_unique;
using mira::multi_index::tag;
using mira::multi_index::member;
using mira::multi_index::composite_key;
using mira::multi_index::composite_key_compare;
using mira::multi_index::const_mem_fun;

template< typename T1, typename T2, typename T3 >
class bmic_type : public ::mira::boost_multi_index_adapter< T1, T2, T3 >
{
public:
  using mira::boost_multi_index_adapter< T1, T2, T3 >::boost_multi_index_adapter;
};

template< typename T1, typename T2, typename T3 >
class mira_type : public ::mira::multi_index_container< T1, T2, T3 >
{
public:
  using mira::multi_index_container< T1, T2, T3 >::multi_index_container;
};

template< class Iterator >
inline Iterator make_reverse_iterator( Iterator iterator )
{
  return iterator.reverse();
}

#endif

} } // crea::chain
