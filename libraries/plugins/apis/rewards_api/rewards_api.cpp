#include <chainbase/chainbase.hpp>
#include <crea/chain/crea_objects.hpp>
#include <crea/chain/util/reward.hpp>
#include <crea/chain/util/uint256.hpp>
#include <crea/plugins/chain/chain_plugin.hpp>
#include <crea/plugins/rewards_api/rewards_api_plugin.hpp>
#include <crea/plugins/rewards_api/rewards_api.hpp>

namespace crea { namespace plugins { namespace rewards_api {

namespace detail {

class rewards_api_impl
{
public:
  rewards_api_impl() :
    _db( appbase::app().get_plugin< crea::plugins::chain::chain_plugin >().db() ) {}

  DECLARE_API_IMPL( (simulate_curve_payouts) );

  chain::database& _db;
};

DEFINE_API_IMPL( rewards_api_impl, simulate_curve_payouts )
{
  FC_ASSERT( false, "Not supported" );
}

} // crea::plugins::rewards_api::detail

rewards_api::rewards_api() : my( std::make_unique< detail::rewards_api_impl >() )
{
  JSON_RPC_REGISTER_API( CREA_REWARDS_API_PLUGIN_NAME );
}

rewards_api::~rewards_api() {}

DEFINE_READ_APIS( rewards_api, (simulate_curve_payouts) )

} } } // crea::plugins::rewards_api

