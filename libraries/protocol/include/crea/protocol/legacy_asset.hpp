#pragma once

#include <crea/protocol/asset.hpp>

#define CREA_SYMBOL_LEGACY_SER_1   (uint64_t(1) | (CREA_SYMBOL_U64 << 8))
#define CREA_SYMBOL_LEGACY_SER_2   (uint64_t(2) | (CREA_SYMBOL_U64 << 8))
#define CREA_SYMBOL_LEGACY_SER_3   (uint64_t(5) | (CREA_SYMBOL_U64 << 8))
#define CREA_SYMBOL_LEGACY_SER_4   (uint64_t(3) | (uint64_t('0') << 8) | (uint64_t('.') << 16) | (uint64_t('0') << 24) | (uint64_t('0') << 32) | (uint64_t('1') << 40))
#define CREA_SYMBOL_LEGACY_SER_5   (uint64_t(3) | (uint64_t('6') << 8) | (uint64_t('.') << 16) | (uint64_t('0') << 24) | (uint64_t('0') << 32) | (uint64_t('0') << 40))

namespace crea { namespace protocol {

class legacy_crea_asset_symbol_type
{
   public:
      legacy_crea_asset_symbol_type() {}

      bool is_canon()const
      {   return ( ser == CREA_SYMBOL_SER );    }

      uint64_t ser = CREA_SYMBOL_SER;
};

struct legacy_crea_asset
{
   public:
      legacy_crea_asset() {}

      template< bool force_canon >
      asset to_asset()const
      {
         if( force_canon )
         {
            FC_ASSERT( symbol.is_canon(), "Must use canonical CREA symbol serialization" );
         }
         return asset( amount, CREA_SYMBOL );
      }

      static legacy_crea_asset from_amount( share_type amount )
      {
         legacy_crea_asset leg;
         leg.amount = amount;
         return leg;
      }

      static legacy_crea_asset from_asset( const asset& a )
      {
         FC_ASSERT( a.symbol == CREA_SYMBOL );
         return from_amount( a.amount );
      }

      share_type                       amount;
      legacy_crea_asset_symbol_type   symbol;
};

} }

namespace fc { namespace raw {

template< typename Stream >
inline void pack( Stream& s, const crea::protocol::legacy_crea_asset_symbol_type& sym )
{
   switch( sym.ser )
   {
      case CREA_SYMBOL_LEGACY_SER_1:
      case CREA_SYMBOL_LEGACY_SER_2:
      case CREA_SYMBOL_LEGACY_SER_3:
      case CREA_SYMBOL_LEGACY_SER_4:
      case CREA_SYMBOL_LEGACY_SER_5:
         wlog( "pack legacy serialization ${s}", ("s", sym.ser) );
      case CREA_SYMBOL_SER:
         pack( s, sym.ser );
         break;
      default:
         FC_ASSERT( false, "Cannot serialize legacy symbol ${s}", ("s", sym.ser) );
   }
}

template< typename Stream >
inline void unpack( Stream& s, crea::protocol::legacy_crea_asset_symbol_type& sym, uint32_t depth )
{
   //  994240:        "account_creation_fee": "0.1 CREA"
   // 1021529:        "account_creation_fee": "10.0 CREA"
   // 3143833:        "account_creation_fee": "3.00000 CREA"
   // 3208405:        "account_creation_fee": "2.00000 CREA"
   // 3695672:        "account_creation_fee": "3.00 CREA"
   // 4338089:        "account_creation_fee": "0.001 0.001"
   // 4626205:        "account_creation_fee": "6.000 6.000"
   // 4632595:        "account_creation_fee": "6.000 6.000"
   depth++;
   uint64_t ser = 0;

   fc::raw::unpack( s, ser, depth );
   switch( ser )
   {
      case CREA_SYMBOL_LEGACY_SER_1:
      case CREA_SYMBOL_LEGACY_SER_2:
      case CREA_SYMBOL_LEGACY_SER_3:
      case CREA_SYMBOL_LEGACY_SER_4:
      case CREA_SYMBOL_LEGACY_SER_5:
         wlog( "unpack legacy serialization ${s}", ("s", ser) );
      case CREA_SYMBOL_SER:
         sym.ser = ser;
         break;
      default:
         FC_ASSERT( false, "Cannot deserialize legacy symbol ${s}", ("s", ser) );
   }
}

} // fc::raw

inline void to_variant( const crea::protocol::legacy_crea_asset& leg, fc::variant& v )
{
   to_variant( leg.to_asset<false>(), v );
}

inline void from_variant( const fc::variant& v, crea::protocol::legacy_crea_asset& leg )
{
   crea::protocol::asset a;
   from_variant( v, a );
   leg = crea::protocol::legacy_crea_asset::from_asset( a );
}

template<>
struct get_typename< crea::protocol::legacy_crea_asset_symbol_type >
{
   static const char* name()
   {
      return "crea::protocol::legacy_crea_asset_symbol_type";
   }
};

} // fc

FC_REFLECT( crea::protocol::legacy_crea_asset,
   (amount)
   (symbol)
   )
