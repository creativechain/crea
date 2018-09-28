#pragma once

#include <crea/protocol/crea_required_actions.hpp>

#include <crea/protocol/operation_util.hpp>

namespace crea { namespace protocol {

   /** NOTE: do not change the order of any actions or it will trigger a hardfork.
    */
   typedef fc::static_variant<
            required_action
         > required_automated_action;

} } // crea::protocol

CREA_DECLARE_OPERATION_TYPE( crea::protocol::required_automated_action );
FC_REFLECT_TYPENAME( crea::protocol::required_automated_action );
