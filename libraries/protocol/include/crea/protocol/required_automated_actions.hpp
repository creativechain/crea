#pragma once

#include <crea/protocol/crea_required_actions.hpp>

#include <crea/protocol/operation_util.hpp>

namespace crea { namespace protocol {

   /** NOTE: do not change the order of any actions or it will trigger a hardfork.
    */
   typedef fc::static_variant<
#ifdef IS_TEST_NET
            example_required_action
#endif
         > required_automated_action;

} } // crea::protocol

CREA_DECLARE_OPERATION_TYPE( crea::protocol::required_automated_action );

FC_TODO( "Remove ifdef when first required automated action is added" )
#ifdef IS_TEST_NET
FC_REFLECT_TYPENAME( crea::protocol::required_automated_action );
#endif
