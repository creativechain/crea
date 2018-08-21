#pragma once

#include <creativecoin/protocol/asset.hpp>

namespace creativecoin { namespace chain { namespace util {

using creativecoin::protocol::asset;
using creativecoin::protocol::price;

inline asset to_sbd( const price& p, const asset& creativecoin )
{
   FC_ASSERT( creativecoin.symbol == CREA_SYMBOL );
   if( p.is_null() )
      return asset( 0, CBD_SYMBOL );
   return creativecoin * p;
}

inline asset to_creativecoin( const price& p, const asset& sbd )
{
   FC_ASSERT( sbd.symbol == CBD_SYMBOL );
   if( p.is_null() )
      return asset( 0, CREA_SYMBOL );
   return sbd * p;
}

} } }
