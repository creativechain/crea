#include <crea/chain/util/hf23_helper.hpp>

namespace crea { namespace chain {

void hf23_helper::gather_balance( hf23_items& source, const std::string& name, const asset& balance, const asset& cbd_balance )
{
  source.emplace( hf23_item{ name, balance, cbd_balance } );
}

} } // namespace crea::chain
