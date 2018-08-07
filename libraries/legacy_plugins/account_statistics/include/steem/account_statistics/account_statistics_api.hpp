#pragma once

#include <creativecoin/account_statistics/account_statistics_plugin.hpp>

#include <fc/api.hpp>

namespace creativecoin { namespace app {
   struct api_context;
} }

namespace creativecoin { namespace account_statistics {

namespace detail
{
   class account_statistics_api_impl;
}

class account_statistics_api
{
   public:
      account_statistics_api( const creativecoin::app::api_context& ctx );

      void on_api_startup();

   private:
      std::shared_ptr< detail::account_statistics_api_impl > _my;
};

} } // creativecoin::account_statistics

FC_API( creativecoin::account_statistics::account_statistics_api, )