#pragma once

#include <string>

namespace fc {
class variant;
}

namespace crea { namespace plugins { namespace block_data_export {

class exportable_block_data
{
  public:
    exportable_block_data();
    virtual ~exportable_block_data();

    virtual void to_variant( fc::variant& v )const = 0;
};

} } }

namespace fc {

inline void to_variant( const crea::plugins::block_data_export::exportable_block_data& ebd, fc::variant& v )
{
  ebd.to_variant( v );
}

}
