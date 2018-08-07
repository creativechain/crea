#include <stdint.h>
#include <creativecoin/utilities/git_revision.hpp>

#define CREA_GIT_REVISION_SHA "ac47a3ef5a086adc82a5851587a4bd81921eea2e"
#define CREA_GIT_REVISION_UNIX_TIMESTAMP 1533640597
#define CREA_GIT_REVISION_DESCRIPTION "v0.19.4rc1-564-gac47a3ef"

namespace creativecoin { namespace utilities {

const char* const git_revision_sha = CREA_GIT_REVISION_SHA;
const uint32_t git_revision_unix_timestamp = CREA_GIT_REVISION_UNIX_TIMESTAMP;
const char* const git_revision_description = CREA_GIT_REVISION_DESCRIPTION;

} } // end namespace creativecoin::utilities
