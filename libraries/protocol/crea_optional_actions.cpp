#include <crea/protocol/validation.hpp>
#include <crea/protocol/crea_optional_actions.hpp>

namespace crea { namespace protocol {

#ifdef IS_TEST_NET
void example_optional_action::validate()const
{
   validate_account_name( account );
}
#endif

} } //crea::protocol
