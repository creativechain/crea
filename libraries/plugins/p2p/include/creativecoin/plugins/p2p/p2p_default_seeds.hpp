#pragma once

#include <vector>

namespace creativecoin{ namespace plugins { namespace p2p {

#ifdef IS_TEST_NET
const std::vector< std::string > default_seeds;
#else
const std::vector< std::string > default_seeds = {

};
#endif

} } } // creativecoin::plugins::p2p
