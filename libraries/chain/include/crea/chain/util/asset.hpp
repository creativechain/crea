#pragma once

#include <crea/protocol/asset.hpp>
#include <crea/protocol/asset_symbol.hpp>

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

inline int64_t asset_precision( const crea::protocol::asset_symbol_type& symbol )
            {
               static int64_t table[] = {
                       1, 10, 100, 1000, 10000,
                       100000, 1000000, 10000000, 100000000ll,
                       1000000000ll, 10000000000ll,
                       100000000000ll, 1000000000000ll,
                       10000000000000ll, 100000000000000ll
               };
               uint8_t d = symbol.decimals();
               return table[ d ];
            }

inline uint32_t asset_string_to_num(string& str_symbol, uint8_t decimals) {
   if (str_symbol == "CREA") {
      FC_ASSERT(decimals == CREA_PRECISION_CREA, "Incorrect decimal places");
      return CREA_ASSET_NUM_CREA;
   } else if (str_symbol == "CBD") {
      FC_ASSERT(decimals == CREA_PRECISION_CBD, "Incorrect decimal places");
      return CREA_ASSET_NUM_CBD;
   } else if (str_symbol == "VESTS") {
      FC_ASSERT(decimals == CREA_PRECISION_VESTS, "Incorrect decimal places");
      return  CREA_ASSET_NUM_VESTS;
   }

   FC_ASSERT( false, "Cannot parse asset symbol" );
}

inline asset asset_from_string( const string& from )
{
   try
   {
      string s = fc::trim( from );
      auto space_pos = s.find( " " );
      auto dot_pos = s.find( "." );

      FC_ASSERT( space_pos != std::string::npos );

      asset result;

      string str_symbol = s.substr( space_pos + 1 );

      if( dot_pos != std::string::npos )
      {
         FC_ASSERT( space_pos > dot_pos );

         auto intpart = s.substr( 0, dot_pos );
         auto fractpart = "1" + s.substr( dot_pos + 1, space_pos - dot_pos - 1 );
         uint8_t decimals = uint8_t( fractpart.size() - 1 );

         result.symbol.asset_num = asset_string_to_num(str_symbol, decimals);

         int64_t prec = asset_precision( result.symbol );

         result.amount = fc::to_int64( intpart );
         result.amount.value *= prec;
         result.amount.value += fc::to_int64( fractpart );
         result.amount.value -= prec;
      }
      else
      {
         auto intpart = s.substr( 0, space_pos );
         result.amount = fc::to_int64( intpart );
         result.symbol.asset_num = asset_string_to_num( str_symbol, 0 );
      }
      return result;
   }
   FC_CAPTURE_AND_RETHROW( (from) )
}

} } }
