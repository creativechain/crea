#pragma once

#include <vector>

namespace crea{ namespace plugins { namespace p2p {

#ifdef IS_TEST_NET
const std::vector< std::string > default_seeds;
#else
const std::vector< std::string > default_seeds = {
};
#endif

} } } // steem::plugins::p2p
