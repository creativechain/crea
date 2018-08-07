#pragma once

#include <creativecoin/protocol/asset.hpp>

namespace creativecoin { namespace plugins { namespace condenser_api {

using creativecoin::protocol::asset;
using creativecoin::protocol::asset_symbol_type;
using creativecoin::protocol::share_type;

struct legacy_asset
{
   public:
      legacy_asset() {}

      asset to_asset()const
      {
         return asset( amount, symbol );
      }

      operator asset()const { return to_asset(); }

      static legacy_asset from_asset( const asset& a )
      {
         legacy_asset leg;
         leg.amount = a.amount;
         leg.symbol = a.symbol;
         return leg;
      }

      string to_string()const;
      static legacy_asset from_string( const string& from );

      share_type                       amount;
      asset_symbol_type                symbol = CREA_SYMBOL;
};

} } } // creativecoin::plugins::condenser_api

namespace fc {

   inline void to_variant( const creativecoin::plugins::condenser_api::legacy_asset& a, fc::variant& var )
   {
      var = a.to_string();
   }

   inline void from_variant( const fc::variant& var, creativecoin::plugins::condenser_api::legacy_asset& a )
   {
      a = creativecoin::plugins::condenser_api::legacy_asset::from_string( var.as_string() );
   }

} // fc

FC_REFLECT( creativecoin::plugins::condenser_api::legacy_asset,
   (amount)
   (symbol)
   )
