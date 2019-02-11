#pragma once

#include <crea/protocol/crea_optional_actions.hpp>

#include <crea/chain/evaluator.hpp>

namespace crea { namespace chain {

using namespace crea::protocol;

#ifdef IS_TEST_NET
CREA_DEFINE_ACTION_EVALUATOR( example_optional, optional_automated_action )
#endif

} } //crea::chain
