#pragma once

#include <crea/protocol/crea_operations.hpp>

#include <crea/chain/evaluator.hpp>

namespace crea { namespace chain {

using namespace crea::protocol;

CREA_DEFINE_EVALUATOR( account_create )
CREA_DEFINE_EVALUATOR( account_create_with_delegation )
CREA_DEFINE_EVALUATOR( account_update )
CREA_DEFINE_EVALUATOR( transfer )
CREA_DEFINE_EVALUATOR( transfer_to_vesting )
CREA_DEFINE_EVALUATOR( witness_update )
CREA_DEFINE_EVALUATOR( account_witness_vote )
CREA_DEFINE_EVALUATOR( account_witness_proxy )
CREA_DEFINE_EVALUATOR( withdraw_vesting )
CREA_DEFINE_EVALUATOR( set_withdraw_vesting_route )
CREA_DEFINE_EVALUATOR( comment )
CREA_DEFINE_EVALUATOR( comment_download )
CREA_DEFINE_EVALUATOR( comment_options )
CREA_DEFINE_EVALUATOR( delete_comment )
CREA_DEFINE_EVALUATOR( vote )
CREA_DEFINE_EVALUATOR( custom )
CREA_DEFINE_EVALUATOR( custom_json )
CREA_DEFINE_EVALUATOR( custom_binary )
CREA_DEFINE_EVALUATOR( pow )
CREA_DEFINE_EVALUATOR( pow2 )
CREA_DEFINE_EVALUATOR( feed_publish )
CREA_DEFINE_EVALUATOR( convert )
CREA_DEFINE_EVALUATOR( limit_order_create )
CREA_DEFINE_EVALUATOR( limit_order_cancel )
CREA_DEFINE_EVALUATOR( report_over_production )
CREA_DEFINE_EVALUATOR( limit_order_create2 )
CREA_DEFINE_EVALUATOR( escrow_transfer )
CREA_DEFINE_EVALUATOR( escrow_approve )
CREA_DEFINE_EVALUATOR( escrow_dispute )
CREA_DEFINE_EVALUATOR( escrow_release )
CREA_DEFINE_EVALUATOR( claim_account )
CREA_DEFINE_EVALUATOR( create_claimed_account )
CREA_DEFINE_EVALUATOR( request_account_recovery )
CREA_DEFINE_EVALUATOR( recover_account )
CREA_DEFINE_EVALUATOR( change_recovery_account )
CREA_DEFINE_EVALUATOR( transfer_to_savings )
CREA_DEFINE_EVALUATOR( transfer_from_savings )
CREA_DEFINE_EVALUATOR( cancel_transfer_from_savings )
CREA_DEFINE_EVALUATOR( decline_voting_rights )
CREA_DEFINE_EVALUATOR( reset_account )
CREA_DEFINE_EVALUATOR( set_reset_account )
CREA_DEFINE_EVALUATOR( claim_reward_balance )
#ifdef CREA_ENABLE_SMT
CREA_DEFINE_EVALUATOR( claim_reward_balance2 )
#endif
CREA_DEFINE_EVALUATOR( delegate_vesting_shares )
CREA_DEFINE_EVALUATOR( witness_set_properties )
#ifdef CREA_ENABLE_SMT
CREA_DEFINE_EVALUATOR( smt_setup )
CREA_DEFINE_EVALUATOR( smt_cap_reveal )
CREA_DEFINE_EVALUATOR( smt_refund )
CREA_DEFINE_EVALUATOR( smt_setup_emissions )
CREA_DEFINE_EVALUATOR( smt_set_setup_parameters )
CREA_DEFINE_EVALUATOR( smt_set_runtime_parameters )
CREA_DEFINE_EVALUATOR( smt_create )
#endif

} } // crea::chain
