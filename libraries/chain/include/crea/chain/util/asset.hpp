#pragma once

#include <crea/protocol/asset.hpp>

namespace crea { namespace chain { namespace util {

using crea::protocol::asset;
using crea::protocol::price;

inline asset to_cbd( const price& p, const asset& crea )
{
   FC_ASSERT( crea.symbol == CREA_SYMBOL );
   if( p.is_null() )
      return asset( 0, CBD_SYMBOL );
   return crea * p;
}

inline asset to_crea( const price& p, const asset& cbd )
{
   FC_ASSERT( cbd.symbol == CBD_SYMBOL );
   if( p.is_null() )
      return asset( 0, CREA_SYMBOL );
   return cbd * p;
}

} } }
