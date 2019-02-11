#pragma once
#include <crea/chain/database.hpp>
#include <crea/protocol/asset_symbol.hpp>

#ifdef CREA_ENABLE_SMT

namespace crea { namespace chain {

   void replenish_nai_pool( database& db );
   void remove_from_nai_pool( database &db, const asset_symbol_type& a );

} } // crea::chain

#endif
