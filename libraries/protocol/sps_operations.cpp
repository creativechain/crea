#include <crea/protocol/sps_operations.hpp>

#include <crea/protocol/validation.hpp>

namespace crea { namespace protocol {

void create_proposal_operation::validate()const
{
  validate_account_name( creator );
  validate_account_name( receiver );

  FC_ASSERT( end_date > start_date, "end date must be greater than start date" );

  FC_ASSERT( daily_pay.amount >= 0, "Daily pay can't be negative value" );
  FC_ASSERT( daily_pay.symbol.asset_num == CREA_ASSET_NUM_CBD, "Daily pay should be expressed in CBD");

  FC_ASSERT( !subject.empty(), "subject is required" );
  FC_ASSERT( subject.size() <= CREA_PROPOSAL_SUBJECT_MAX_LENGTH, "Subject is too long");
  FC_ASSERT( fc::is_utf8( subject ), "Subject is not valid UTF8" );

  FC_ASSERT( !permlink.empty(), "permlink is required" );
  validate_permlink(permlink);
}

void update_proposal_operation::validate()const
{
  validate_account_name( creator );

  FC_ASSERT( proposal_id >= 0, "The proposal id can't be negative" );
  FC_ASSERT( daily_pay.amount >= 0, "Daily pay can't be negative value" );
  FC_ASSERT( daily_pay.symbol.asset_num == CREA_ASSET_NUM_CBD, "Daily pay should be expressed in CBD");

  FC_ASSERT( !subject.empty(), "subject is required" );
  FC_ASSERT( subject.size() <= CREA_PROPOSAL_SUBJECT_MAX_LENGTH, "Subject is too long");
  FC_ASSERT( fc::is_utf8( subject ), "Subject is not valid UTF8" );

  FC_ASSERT( !permlink.empty(), "permlink is required" );
  validate_permlink(permlink);
}

void update_proposal_votes_operation::validate()const
{
  validate_account_name( voter );
  FC_ASSERT(proposal_ids.empty() == false);
  FC_ASSERT(proposal_ids.size() <= CREA_PROPOSAL_MAX_IDS_NUMBER);
}

void remove_proposal_operation::validate() const
{
  validate_account_name(proposal_owner);
  FC_ASSERT(proposal_ids.empty() == false);
  FC_ASSERT(proposal_ids.size() <= CREA_PROPOSAL_MAX_IDS_NUMBER);
}

} } //crea::protocol
