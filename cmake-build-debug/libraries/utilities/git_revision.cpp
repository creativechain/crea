#include <stdint.h>
#include <creativecoin/utilities/git_revision.hpp>

#define CREA_GIT_REVISION_SHA "306e63b771295558e9ce3a1da476e708b5903c27"
#define CREA_GIT_REVISION_UNIX_TIMESTAMP 1530651451
#define CREA_GIT_REVISION_DESCRIPTION "v0.19.4rc1-561-g306e63b7"

namespace creativecoin { namespace utilities {

const char* const git_revision_sha = CREA_GIT_REVISION_SHA;
const uint32_t git_revision_unix_timestamp = CREA_GIT_REVISION_UNIX_TIMESTAMP;
const char* const git_revision_description = CREA_GIT_REVISION_DESCRIPTION;

} } // end namespace creativecoin::utilities
