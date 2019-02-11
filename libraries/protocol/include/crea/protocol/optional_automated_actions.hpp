#pragma once

#include <crea/protocol/crea_optional_actions.hpp>

#include <crea/protocol/operation_util.hpp>

namespace crea { namespace protocol {

   /** NOTE: do not change the order of any actions or it will trigger a hardfork.
    */
   typedef fc::static_variant<
#ifdef IS_TEST_NET
            example_optional_action
#endif
         > optional_automated_action;

} } // crea::protocol

CREA_DECLARE_OPERATION_TYPE( crea::protocol::optional_automated_action );

FC_TODO( "Remove ifdef when first optional automated action is added" )
#ifdef IS_TEST_NET
FC_REFLECT_TYPENAME( crea::protocol::optional_automated_action );
#endif