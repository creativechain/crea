#include <crea/account_statistics/account_statistics_api.hpp>

namespace crea { namespace account_statistics {

namespace detail
{
   class account_statistics_api_impl
   {
      public:
         account_statistics_api_impl( crea::app::application& app )
            :_app( app ) {}

         crea::app::application& _app;
   };
} // detail

account_statistics_api::account_statistics_api( const crea::app::api_context& ctx )
{
   _my= std::make_shared< detail::account_statistics_api_impl >( ctx.app );
}

void account_statistics_api::on_api_startup() {}

} } // crea::account_statistics