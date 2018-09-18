#pragma once

#include <crea/protocol/asset.hpp>

namespace crea { namespace plugins { namespace condenser_api {

using crea::protocol::asset;
using crea::protocol::asset_symbol_type;
using crea::protocol::share_type;

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

} } } // crea::plugins::condenser_api

namespace fc {

   inline void to_variant( const crea::plugins::condenser_api::legacy_asset& a, fc::variant& var )
   {
      var = a.to_string();
   }

   inline void from_variant( const fc::variant& var, crea::plugins::condenser_api::legacy_asset& a )
   {
      a = crea::plugins::condenser_api::legacy_asset::from_string( var.as_string() );
   }

} // fc

FC_REFLECT( crea::plugins::condenser_api::legacy_asset,
   (amount)
   (symbol)
   )
