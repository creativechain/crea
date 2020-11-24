#pragma once

#include <crea/chain/crea_object_types.hpp>

namespace crea
{
  namespace chain
  {
    template< uint32_t _SYMBOL >
    class tiny_asset
    {
    public:

      share_type amount;

      tiny_asset() {}
      tiny_asset( const asset& val )       { set( val ); }
      tiny_asset( asset&& val )            { set( val ); }
      asset operator=( const asset& val )  { set( val ); return *this; }
      asset operator=( asset&& val )       { set( val ); return *this; }

      asset operator+=( const asset& val ) { check( val ); amount += val.amount; return *this; }
      asset operator-=( const asset& val ) { check( val ); amount -= val.amount; return *this; }

      operator asset() const               { return to_asset(); }

      asset to_asset() const               { return asset( amount, asset_symbol_type::from_asset_num( _SYMBOL ) ); }
      
    private:

      void set( const asset& val )         { check( val ); amount = val.amount; }
      void check( const asset& val ) const { FC_ASSERT( val.symbol.asset_num == _SYMBOL ); }
    };

    using CBD_asset  = tiny_asset< CREA_ASSET_NUM_CBD >;
    using CREA_asset = tiny_asset< CREA_ASSET_NUM_CREA >;
    using VEST_asset = tiny_asset< CREA_ASSET_NUM_VESTS >;

    template< uint32_t _SYMBOL >
    bool operator==( const tiny_asset< _SYMBOL >& obj1, const tiny_asset< _SYMBOL >& obj2 ) { return obj1.to_asset() == obj2.to_asset(); }

    template< uint32_t _SYMBOL >
    bool operator!=( const tiny_asset< _SYMBOL >& obj1, const tiny_asset< _SYMBOL >& obj2 ) { return !( obj1.to_asset() == obj2.to_asset() ); }

    template< uint32_t _SYMBOL >
    asset operator-( const tiny_asset< _SYMBOL >& obj1) { return -static_cast< asset >( obj1 ); }

  }
}

FC_REFLECT( crea::chain::CBD_asset,  (amount) )
FC_REFLECT( crea::chain::CREA_asset, (amount) )
FC_REFLECT( crea::chain::VEST_asset, (amount) )