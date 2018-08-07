#include <stdint.h>
#include <creativecoin/utilities/git_revision.hpp>

#define CREA_GIT_REVISION_SHA "9543574aa03fc53f11479ad8c93fb7cc5a758efa"
#define CREA_GIT_REVISION_UNIX_TIMESTAMP 1533638929
#define CREA_GIT_REVISION_DESCRIPTION "v0.19.4rc1-563-g9543574a"

namespace creativecoin { namespace utilities {

const char* const git_revision_sha = CREA_GIT_REVISION_SHA;
const uint32_t git_revision_unix_timestamp = CREA_GIT_REVISION_UNIX_TIMESTAMP;
const char* const git_revision_description = CREA_GIT_REVISION_DESCRIPTION;

} } // end namespace creativecoin::utilities
