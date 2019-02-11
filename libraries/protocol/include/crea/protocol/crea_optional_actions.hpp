#pragma once
#include <crea/protocol/types.hpp>
#include <crea/protocol/base.hpp>

namespace crea { namespace protocol {

#ifdef IS_TEST_NET
   struct example_optional_action : public base_operation
   {
      account_name_type account;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(account); }
   };
#endif

} } // crea::protocol

#ifdef IS_TEST_NET
FC_REFLECT( crea::protocol::example_optional_action, (account) )
#endif
