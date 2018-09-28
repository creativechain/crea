
#pragma once

#include <crea/schema/abstract_schema.hpp>
#include <crea/schema/schema_impl.hpp>

#include <crea/protocol/types.hpp>

namespace crea { namespace schema { namespace detail {

//////////////////////////////////////////////
// account_name_type                        //
//////////////////////////////////////////////

struct schema_account_name_type_impl
   : public abstract_schema
{
   CREA_SCHEMA_CLASS_BODY( schema_account_name_type_impl )
};

}

template<>
struct schema_reflect< crea::protocol::account_name_type >
{
   typedef detail::schema_account_name_type_impl           schema_impl_type;
};

} }

namespace fc {

template<>
struct get_typename< crea::protocol::account_name_type >
{
   static const char* name()
   {
      return "crea::protocol::account_name_type";
   }
};

}
