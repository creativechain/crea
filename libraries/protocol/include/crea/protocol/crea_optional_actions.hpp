#pragma once
#include <crea/protocol/base.hpp>
#include <crea/protocol/block_header.hpp>
#include <crea/protocol/asset.hpp>
#include <crea/protocol/validation.hpp>
#include <crea/protocol/legacy_asset.hpp>

#include <fc/crypto/equihash.hpp>

namespace crea { namespace protocol {

   struct optional_action : public base_operation
   {
      account_name_type account;

      void validate()const;
      void get_required_active_authorities( flat_set<account_name_type>& a )const{ a.insert(account); }
   };

} } // crea::protocol

FC_REFLECT( crea::protocol::optional_action, (account) )
