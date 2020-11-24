
#include <crea/chain/crea_fwd.hpp>

#include <crea/chain/index.hpp>

#include <crea/chain/block_summary_object.hpp>
#include <crea/chain/history_object.hpp>
#include <crea/chain/pending_required_action_object.hpp>
#include <crea/chain/pending_optional_action_object.hpp>
#include <crea/chain/smt_objects.hpp>
#include <crea/chain/crea_objects.hpp>
#include <crea/chain/transaction_object.hpp>
#include <crea/chain/witness_schedule.hpp>

namespace crea { namespace chain {

void initialize_core_indexes( database& db )
{
  CREA_ADD_CORE_INDEX(db, dynamic_global_property_index);
  CREA_ADD_CORE_INDEX(db, account_index);
  CREA_ADD_CORE_INDEX(db, account_metadata_index);
  CREA_ADD_CORE_INDEX(db, account_authority_index);
  CREA_ADD_CORE_INDEX(db, witness_index);
  CREA_ADD_CORE_INDEX(db, transaction_index);
  CREA_ADD_CORE_INDEX(db, block_summary_index);
  CREA_ADD_CORE_INDEX(db, witness_schedule_index);
  CREA_ADD_CORE_INDEX(db, comment_index);
  CREA_ADD_CORE_INDEX(db, comment_content_index);
  CREA_ADD_CORE_INDEX(db, comment_download_index);
  CREA_ADD_CORE_INDEX(db, download_granted_index);
  CREA_ADD_CORE_INDEX(db, comment_vote_index);
  CREA_ADD_CORE_INDEX(db, witness_vote_index);
  CREA_ADD_CORE_INDEX(db, limit_order_index);
  CREA_ADD_CORE_INDEX(db, feed_history_index);
  CREA_ADD_CORE_INDEX(db, convert_request_index);
  CREA_ADD_CORE_INDEX(db, liquidity_reward_balance_index);
  CREA_ADD_CORE_INDEX(db, operation_index);
  CREA_ADD_CORE_INDEX(db, account_history_index);
  CREA_ADD_CORE_INDEX(db, hardfork_property_index);
  CREA_ADD_CORE_INDEX(db, withdraw_vesting_route_index);
  CREA_ADD_CORE_INDEX(db, owner_authority_history_index);
  CREA_ADD_CORE_INDEX(db, account_recovery_request_index);
  CREA_ADD_CORE_INDEX(db, change_recovery_account_request_index);
  CREA_ADD_CORE_INDEX(db, escrow_index);
  CREA_ADD_CORE_INDEX(db, savings_withdraw_index);
  CREA_ADD_CORE_INDEX(db, decline_voting_rights_request_index);
  CREA_ADD_CORE_INDEX(db, reward_fund_index);
  CREA_ADD_CORE_INDEX(db, vesting_delegation_index);
  CREA_ADD_CORE_INDEX(db, vesting_delegation_expiration_index);
  CREA_ADD_CORE_INDEX(db, pending_required_action_index);
  CREA_ADD_CORE_INDEX(db, pending_optional_action_index);
#ifdef CREA_ENABLE_SMT
  CREA_ADD_CORE_INDEX(db, smt_token_index);
  CREA_ADD_CORE_INDEX(db, account_regular_balance_index);
  CREA_ADD_CORE_INDEX(db, account_rewards_balance_index);
  CREA_ADD_CORE_INDEX(db, nai_pool_index);
  CREA_ADD_CORE_INDEX(db, smt_token_emissions_index);
  CREA_ADD_CORE_INDEX(db, smt_contribution_index);
  CREA_ADD_CORE_INDEX(db, smt_ico_index);
#endif
  CREA_ADD_CORE_INDEX(db, proposal_index);
  CREA_ADD_CORE_INDEX(db, proposal_vote_index);
  CREA_ADD_CORE_INDEX(db, comment_cashout_index);
}

index_info::index_info() {}
index_info::~index_info() {}

} }
