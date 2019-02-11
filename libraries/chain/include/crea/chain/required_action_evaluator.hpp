#pragma once

#include <crea/protocol/crea_required_actions.hpp>

#include <crea/chain/evaluator.hpp>

namespace crea { namespace chain {

using namespace crea::protocol;

#ifdef IS_TEST_NET
CREA_DEFINE_ACTION_EVALUATOR( example_required, required_automated_action )
#endif

} } //crea::chain
