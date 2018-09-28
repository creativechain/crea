#pragma once

#include <crea/protocol/crea_optional_actions.hpp>

#include <crea/protocol/operation_util.hpp>

namespace crea { namespace protocol {

   /** NOTE: do not change the order of any actions or it will trigger a hardfork.
    */
   typedef fc::static_variant<
            optional_action
         > optional_automated_action;

} } // crea::protocol

CREA_DECLARE_OPERATION_TYPE( crea::protocol::optional_automated_action );
FC_REFLECT_TYPENAME( crea::protocol::optional_automated_action );
