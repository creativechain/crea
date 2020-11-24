#pragma once

#include <crea/protocol/asset.hpp>

#include<string>

namespace crea { namespace chain {

using crea::protocol::asset;

class hf23_helper
{
  public:

    struct hf23_item
    {
      std::string name;

      asset       balance;
      asset       cbd_balance;
    };

    struct cmp_hf23_item
    {
      bool operator()( const hf23_item& a, const hf23_item& b ) const
      {
        return std::strcmp( a.name.c_str(), b.name.c_str() ) < 0;
      }
    };

    using hf23_items = std::set< hf23_item, cmp_hf23_item >;

  public:

    static void gather_balance( hf23_items& source, const std::string& name, const asset& balance, const asset& cbd_balance );
};

} } // namespace crea::chain
