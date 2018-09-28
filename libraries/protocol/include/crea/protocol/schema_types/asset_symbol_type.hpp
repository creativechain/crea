
#pragma once

#include <crea/schema/abstract_schema.hpp>
#include <crea/schema/schema_impl.hpp>

#include <crea/protocol/asset_symbol.hpp>

namespace crea { namespace schema { namespace detail {

//////////////////////////////////////////////
// asset_symbol_type                        //
//////////////////////////////////////////////

struct schema_asset_symbol_type_impl
   : public abstract_schema
{
   CREA_SCHEMA_CLASS_BODY( schema_asset_symbol_type_impl )
};

}

template<>
struct schema_reflect< crea::protocol::asset_symbol_type >
{
   typedef detail::schema_asset_symbol_type_impl           schema_impl_type;
};

} }
