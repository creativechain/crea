
#include <crea/plugins/rc/resource_user.hpp>

#include <crea/protocol/transaction.hpp>

namespace crea { namespace plugins { namespace rc {

using namespace crea::protocol;

account_name_type get_resource_user( const signed_transaction& tx )
{
  flat_set< account_name_type > req_active;
  flat_set< account_name_type > req_owner;
  flat_set< account_name_type > req_posting;
  vector< authority > other;

  for( const operation& op : tx.operations )
  {
    operation_get_required_authorities( op, req_active, req_owner, req_posting, other );
    for( const account_name_type& account : req_active )
      return account;
    for( const account_name_type& account : req_owner )
      return account;
    for( const account_name_type& account : req_posting )
      return account;
  }
  return account_name_type();
}

} } } // crea::plugins::rc
