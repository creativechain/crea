/*
 * Copyright (c) 2016 Steemit, Inc., and contributors.
 * Copyright (c) 2018 Creativechain FDN, and contributors.
 */
#pragma once
#include <crea/protocol/hardfork.hpp>

// WARNING!
// Every symbol defined here needs to be handled appropriately in get_config.cpp
// This is checked by get_config_check.sh called from Dockerfile

#ifdef IS_TEST_NET
#define CREA_BLOCKCHAIN_VERSION              ( version(0, 21, 0) )

#define CREA_INIT_PRIVATE_KEY                (fc::ecc::private_key::regenerate(fc::sha256::hash(std::string("init_key"))))
#define CREA_INIT_PUBLIC_KEY_STR             (std::string( crea::protocol::public_key_type(CREA_INIT_PRIVATE_KEY.get_public_key()) ))
#define CREA_CHAIN_ID (fc::sha256::hash("testnet"))
#define CREA_ADDRESS_PREFIX                  "crea"

#define CREA_GENESIS_TIME                    (fc::time_point_sec(1550592000))
#define CREA_MINING_TIME                     (fc::time_point_sec(1550592000))
#define CREA_CASHOUT_WINDOW_SECONDS          (60*60) /// 1 hr
#define CREA_FIRST_WINDOW_SECONDS            (60*60)       /// 1 hour -> Reward regulation
#define CREA_CASHOUT_WINDOW_SECONDS_PRE_HF12 (CREA_CASHOUT_WINDOW_SECONDS)
#define CREA_CASHOUT_WINDOW_SECONDS_PRE_HF17 (CREA_CASHOUT_WINDOW_SECONDS)
#define CREA_SECOND_CASHOUT_WINDOW           (60*60*24*3) /// 3 days
#define CREA_MAX_CASHOUT_WINDOW_SECONDS      (60*60*24) /// 1 day
#define CREA_UPVOTE_LOCKOUT_HF7              (fc::minutes(1))
#define CREA_UPVOTE_LOCKOUT_SECONDS          (60*5)    /// 5 minutes
#define CREA_UPVOTE_LOCKOUT_HF17             (fc::minutes(5))


#define CREA_MIN_ACCOUNT_CREATION_FEE          0
#define CREA_MAX_ACCOUNT_CREATION_FEE          int64_t(1000000000)

#define CREA_OWNER_AUTH_RECOVERY_PERIOD                  fc::seconds(60)
#define CREA_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD  fc::seconds(12)
#define CREA_OWNER_UPDATE_LIMIT                          fc::seconds(0)
#define CREA_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM 1

//Supply for CREA and CBD
#define CREA_TOTAL_INIT_SUPPLY               (int64_t( 250 ) * int64_t( 1000000 ) * int64_t( 1000 ))
#define CREA_CBD_PERCENT_SUPPLY              (int64_t(5)) //5% of total supply is CBD
#define CREA_TO_CBD_SUPPLY                   (int64_t(CREA_TOTAL_INIT_SUPPLY * CREA_CBD_PERCENT_SUPPLY / 100)) //Total CREA to convert as CBD
#define CREA_INIT_PRICE                      (int64_t(23)) //CBD per 1 CREA at start
#define CREA_INIT_SUPPLY                     (int64_t(CREA_TOTAL_INIT_SUPPLY - CREA_TO_CBD_SUPPLY))
#define CREA_CBD_INIT_SUPPLY                 (CREA_TO_CBD_SUPPLY / CREA_INIT_PRICE)

/// Allows to limit number of total produced blocks.
#define TESTNET_BLOCK_LIMIT                   (3000000)

#else // IS LIVE CREA NETWORK

#define CREA_BLOCKCHAIN_VERSION              ( version(0, 20, 6) )

#define CREA_INIT_PRIVATE_KEY                (fc::ecc::private_key::regenerate(fc::sha256::hash(std::string("init_key"))))
#define CREA_INIT_PUBLIC_KEY_STR             (std::string( crea::protocol::public_key_type(CREA_INIT_PRIVATE_KEY.get_public_key()) ))
#define CREA_CHAIN_ID                        fc::sha256()
#define CREA_ADDRESS_PREFIX                  "CREA"

#define CREA_GENESIS_TIME                    (fc::time_point_sec(1550592000))
#define CREA_MINING_TIME                     (fc::time_point_sec(1550592000))
#define CREA_CASHOUT_WINDOW_SECONDS_PRE_HF12 (60*60*24)    /// 1 day
#define CREA_CASHOUT_WINDOW_SECONDS_PRE_HF17 (60*60*12)    /// 12 hours
#define CREA_FIRST_WINDOW_SECONDS            (60*60)       /// 1 hour -> Reward regulation
#define CREA_CASHOUT_WINDOW_SECONDS          (60*60*24*10) /// 10 days -> Rewards
#define CREA_SECOND_CASHOUT_WINDOW           (60*60*24*30) /// 30 days
#define CREA_MAX_CASHOUT_WINDOW_SECONDS      (60*60*24*14) /// 2 weeks
#define CREA_UPVOTE_LOCKOUT_HF7              (fc::minutes(1))
#define CREA_UPVOTE_LOCKOUT_SECONDS          (60*60*12)    /// 12 hours
#define CREA_UPVOTE_LOCKOUT_HF17             (fc::hours(12))

#define CREA_MIN_ACCOUNT_CREATION_FEE           1
#define CREA_MAX_ACCOUNT_CREATION_FEE           int64_t(1000000000)

#define CREA_OWNER_AUTH_RECOVERY_PERIOD                  fc::days(30)
#define CREA_ACCOUNT_RECOVERY_REQUEST_EXPIRATION_PERIOD  fc::days(1)
#define CREA_OWNER_UPDATE_LIMIT                          fc::minutes(60)
#define CREA_OWNER_AUTH_HISTORY_TRACKING_START_BLOCK_NUM 1

//Supply for CREA and CBD
#define CREA_TOTAL_INIT_SUPPLY               (int64_t( 19078301 ) * int64_t( 1000 ))
#define CREA_CBD_PERCENT_SUPPLY              (int64_t(0)) //1% of total supply is CBD
#define CREA_TO_CBD_SUPPLY                   (int64_t(CREA_TOTAL_INIT_SUPPLY * CREA_CBD_PERCENT_SUPPLY / 100)) //Total CREA to convert as CBD
#define CREA_INIT_PRICE                      (int64_t(7)) //CBD per 1 CREA at start
#define CREA_INIT_SUPPLY                     (int64_t(CREA_TOTAL_INIT_SUPPLY - CREA_TO_CBD_SUPPLY))
#define CREA_CBD_INIT_SUPPLY                 (CREA_TO_CBD_SUPPLY / CREA_INIT_PRICE)

#endif

#define VESTS_SYMBOL  (crea::protocol::asset_symbol_type::from_asset_num( CREA_ASSET_NUM_VESTS ) )
#define CREA_SYMBOL  (crea::protocol::asset_symbol_type::from_asset_num( CREA_ASSET_NUM_CREA ) )
#define CBD_SYMBOL    (crea::protocol::asset_symbol_type::from_asset_num( CREA_ASSET_NUM_CBD ) )

#define CREA_BLOCKCHAIN_HARDFORK_VERSION     ( hardfork_version( CREA_BLOCKCHAIN_VERSION ) )

#define CREA_BLOCK_INTERVAL                  3 //3 seconds
#define CREA_BLOCKS_PER_YEAR                 (365*24*60*60/CREA_BLOCK_INTERVAL)
#define CREA_BLOCKS_PER_DAY                  (24*60*60/CREA_BLOCK_INTERVAL)
#define CREA_START_VESTING_BLOCK             (CREA_BLOCKS_PER_DAY * 7)
#define CREA_START_MINER_VOTING_BLOCK        (CREA_BLOCKS_PER_DAY * 30)

#define CREA_INIT_MINER_NAME                 "initminer"
#define CREA_NUM_INIT_MINERS                 1
#define CREA_INIT_TIME                       (fc::time_point_sec());

#define CREA_MAX_WITNESSES                   25

#define CREA_MAX_VOTED_WITNESSES_HF0         24
#define CREA_MAX_MINER_WITNESSES_HF0         0
#define CREA_MAX_RUNNER_WITNESSES_HF0        1

#define CREA_MAX_VOTED_WITNESSES_HF17        24
#define CREA_MAX_MINER_WITNESSES_HF17        0
#define CREA_MAX_RUNNER_WITNESSES_HF17       1

#define CREA_HARDFORK_REQUIRED_WITNESSES     20 // 19 of the 25 dpos witnesses (24 elected and 1 virtual time) required for hardfork. This guarantees 75% participation on all subsequent rounds.
#define CREA_MAX_TIME_UNTIL_EXPIRATION       (60*60) // seconds,  aka: 1 hour
#define CREA_MAX_MEMO_SIZE                   2048
#define CREA_MAX_PROXY_RECURSION_DEPTH       4
#define CREA_VESTING_WITHDRAW_INTERVALS_PRE_HF_16 104
#define CREA_VESTING_WITHDRAW_INTERVALS      8
#define CREA_VESTING_WITHDRAW_INTERVAL_SECONDS (60*60*24*7) /// 1 week interval
#define CREA_MAX_WITHDRAW_ROUTES             10
#define CREA_SAVINGS_WITHDRAW_TIME        	(fc::days(3))
#define CREA_SAVINGS_WITHDRAW_REQUEST_LIMIT  100
#define CREA_VOTING_FLOW_REGENERATION_SECONDS (5*60*60*24) // 5 day
#define CREA_MAX_VOTE_CHANGES                5
#define CREA_REVERSE_AUCTION_WINDOW_SECONDS_HF6 (60*30) /// 30 minutes
#define CREA_REVERSE_AUCTION_WINDOW_SECONDS_HF20 (60*15) /// 15 minutes
#define CREA_MIN_VOTE_INTERVAL_SEC           3
#define CREA_VOTE_DUST_THRESHOLD             (50000000)
//#define CREA_VOTE_DUST_THRESHOLD             (3800000)

#define CREA_MIN_ROOT_COMMENT_INTERVAL       (fc::seconds(60*5)) // 5 minutes
#define CREA_MIN_REPLY_INTERVAL              (fc::seconds(20)) // 20 seconds
#define CREA_MIN_REPLY_INTERVAL_HF20         (fc::seconds(3)) // 3 seconds
#define CREA_POST_AVERAGE_WINDOW             (60*60*24u) // 1 day
#define CREA_POST_WEIGHT_CONSTANT            (uint64_t(4*CREA_100_PERCENT) * (4*CREA_100_PERCENT))// (4*CREA_100_PERCENT) -> 2 posts per 1 days, average 1 every 12 hours

#define CREA_MAX_ACCOUNT_WITNESS_VOTES       35

#define CREA_100_PERCENT                     10000
#define CREA_1_PERCENT                       (CREA_100_PERCENT/100)
#define CREA_DEFAULT_CBD_INTEREST_RATE       (10*CREA_1_PERCENT) ///< 10% APR

#define CREA_INFLATION_RATE_START_PERCENT    (700) // Fixes block 7,000,000 to 7.00%
#define CREA_INFLATION_RATE_STOP_PERCENT     (95) // 0.95%
#define CREA_INFLATION_NARROWING_PERIOD      (300000) // Narrow 0.01% every 300k blocks
#define CREA_CONTENT_REWARD_PERCENT          (70*CREA_1_PERCENT) //75% of inflation, 7.125% inflation
#define CREA_VESTING_FUND_PERCENT            (15*CREA_1_PERCENT) //15% of inflation, 1.425% inflation

#define CREA_MINER_PAY_PERCENT               (CREA_1_PERCENT) // 1%
#define CREA_MAX_RATION_DECAY_RATE           (1000000)

#define CREA_BANDWIDTH_AVERAGE_WINDOW_SECONDS (60*60*24*7) ///< 1 week
#define CREA_BANDWIDTH_PRECISION             (uint64_t(1000000)) ///< 1 million
#define CREA_MAX_COMMENT_DEPTH_PRE_HF17      6
#define CREA_MAX_COMMENT_DEPTH               0xffff // 64k
#define CREA_SOFT_MAX_COMMENT_DEPTH          0xff // 255

#define CREA_MAX_RESERVE_RATIO               (20000)

#define CREA_CREATE_ACCOUNT_WITH_CREA_MODIFIER 1
#define CREA_CREATE_ACCOUNT_DELEGATION_RATIO    5
#define CREA_CREATE_ACCOUNT_DELEGATION_TIME     fc::days(30)

#define CREA_MINING_REWARD                   asset( 1000, CREA_SYMBOL )
#define CREA_EQUIHASH_N                      140
#define CREA_EQUIHASH_K                      6

#define CREA_LIQUIDITY_TIMEOUT_SEC           (fc::seconds(60*60*24*7)) // After one week volume is set to 0
#define CREA_MIN_LIQUIDITY_REWARD_PERIOD_SEC (fc::seconds(60)) // 1 minute required on books to receive volume
#define CREA_LIQUIDITY_REWARD_PERIOD_SEC     (60*60)
#define CREA_LIQUIDITY_REWARD_BLOCKS         (CREA_LIQUIDITY_REWARD_PERIOD_SEC/CREA_BLOCK_INTERVAL)
#define CREA_MIN_LIQUIDITY_REWARD            (asset( 1000*CREA_LIQUIDITY_REWARD_BLOCKS, CREA_SYMBOL )) // Minumum reward to be paid out to liquidity providers
#define CREA_MIN_CONTENT_REWARD              CREA_MINING_REWARD
#define CREA_MIN_CURATE_REWARD               CREA_MINING_REWARD
#define CREA_MIN_PRODUCER_REWARD             CREA_MINING_REWARD
#define CREA_MIN_POW_REWARD                  CREA_MINING_REWARD

#define CREA_ACTIVE_CHALLENGE_FEE            asset( 2000, CREA_SYMBOL )
#define CREA_OWNER_CHALLENGE_FEE             asset( 30000, CREA_SYMBOL )
#define CREA_ACTIVE_CHALLENGE_COOLDOWN       fc::days(1)
#define CREA_OWNER_CHALLENGE_COOLDOWN        fc::days(1)

#define CREA_POST_REWARD_FUND_NAME           ("post")
#define CREA_COMMENT_REWARD_FUND_NAME        ("comment")
#define CREA_RECENT_RSHARES_DECAY_TIME_HF17    (fc::days(30))
#define CREA_RECENT_RSHARES_DECAY_TIME_HF19    (fc::days(15))
#define CREA_CONTENT_CONSTANT_HF0            (uint128_t(uint64_t(2000000000000ll)))
// note, if redefining these constants make sure calculate_claims doesn't overflow

// 5ccc e802 de5f
// int(expm1( log1p( 1 ) / BLOCKS_PER_YEAR ) * 2**CREA_APR_PERCENT_SHIFT_PER_BLOCK / 100000 + 0.5)
// we use 100000 here instead of 10000 because we end up creating an additional 9x for vesting
#define CREA_APR_PERCENT_MULTIPLY_PER_BLOCK          ( (uint64_t( 0x5ccc ) << 0x20) \
                                                        | (uint64_t( 0xe802 ) << 0x10) \
                                                        | (uint64_t( 0xde5f )        ) \
                                                        )
// chosen to be the maximal value such that CREA_APR_PERCENT_MULTIPLY_PER_BLOCK * 2**64 * 100000 < 2**128
#define CREA_APR_PERCENT_SHIFT_PER_BLOCK             87

#define CREA_APR_PERCENT_MULTIPLY_PER_ROUND          ( (uint64_t( 0x79cc ) << 0x20 ) \
                                                        | (uint64_t( 0xf5c7 ) << 0x10 ) \
                                                        | (uint64_t( 0x3480 )         ) \
                                                        )

#define CREA_APR_PERCENT_SHIFT_PER_ROUND             83

// We have different constants for hourly rewards
// i.e. hex(int(math.expm1( math.log1p( 1 ) / HOURS_PER_YEAR ) * 2**CREA_APR_PERCENT_SHIFT_PER_HOUR / 100000 + 0.5))
#define CREA_APR_PERCENT_MULTIPLY_PER_HOUR           ( (uint64_t( 0x6cc1 ) << 0x20) \
                                                        | (uint64_t( 0x39a1 ) << 0x10) \
                                                        | (uint64_t( 0x5cbd )        ) \
                                                        )

// chosen to be the maximal value such that CREA_APR_PERCENT_MULTIPLY_PER_HOUR * 2**64 * 100000 < 2**128
#define CREA_APR_PERCENT_SHIFT_PER_HOUR              77

// These constants add up to GRAPHENE_100_PERCENT.  Each GRAPHENE_1_PERCENT is equivalent to 1% per year APY
// *including the corresponding 9x vesting rewards*
#define CREA_CURATE_APR_PERCENT              3875
#define CREA_CONTENT_APR_PERCENT             3875
#define CREA_LIQUIDITY_APR_PERCENT            750
#define CREA_PRODUCER_APR_PERCENT             750
#define CREA_POW_APR_PERCENT                  750

#define CREA_MIN_PAYOUT_CBD                  (asset(10,CBD_SYMBOL))

#define CREA_CBD_STOP_PERCENT_HF14           (5*CREA_1_PERCENT ) // Stop printing CBD at 5% Market Cap
#define CREA_CBD_STOP_PERCENT_HF20           (10*CREA_1_PERCENT ) // Stop printing CBD at 10% Market Cap
#define CREA_CBD_START_PERCENT_HF14          (2*CREA_1_PERCENT) // Start reducing printing of CBD at 2% Market Cap
#define CREA_CBD_START_PERCENT_HF20          (9*CREA_1_PERCENT) // Start reducing printing of CBD at 9% Market Cap

#define CREA_MIN_ACCOUNT_NAME_LENGTH          3
#define CREA_MAX_ACCOUNT_NAME_LENGTH         16

#define CREA_MIN_PERMLINK_LENGTH             0
#define CREA_MAX_PERMLINK_LENGTH             256
#define CREA_MAX_WITNESS_URL_LENGTH          2048

#define CREA_MAX_SHARE_SUPPLY                int64_t(1000000000000000ll)
#define CREA_MAX_SATOSHIS                    int64_t(4611686018427387903ll)
#define CREA_MAX_SIG_CHECK_DEPTH             2
#define CREA_MAX_SIG_CHECK_ACCOUNTS          125

#define CREA_MIN_TRANSACTION_SIZE_LIMIT      1024
#define CREA_SECONDS_PER_YEAR                (uint64_t(60*60*24*365ll))

#define CREA_CBD_INTEREST_COMPOUND_INTERVAL_SEC  (60*60*24*30)
#define CREA_MAX_TRANSACTION_SIZE            (1024*64)
#define CREA_MIN_BLOCK_SIZE_LIMIT            (CREA_MAX_TRANSACTION_SIZE)
#define CREA_MAX_BLOCK_SIZE                  (CREA_MAX_TRANSACTION_SIZE*CREA_BLOCK_INTERVAL*2000)
#define CREA_SOFT_MAX_BLOCK_SIZE             (2*1024*1024)
#define CREA_MIN_BLOCK_SIZE                  115
#define CREA_BLOCKS_PER_HOUR                 (60*60/CREA_BLOCK_INTERVAL)
#define CREA_FEED_INTERVAL_BLOCKS            (CREA_BLOCKS_PER_HOUR)
#define CREA_FEED_HISTORY_WINDOW_PRE_HF_16   (24*7) /// 7 days * 24 hours per day
#define CREA_FEED_HISTORY_WINDOW             (12*7) // 3.5 days
#define CREA_MAX_FEED_AGE_SECONDS            (60*60*24*7) // 7 days
#define CREA_MIN_FEEDS                       (CREA_MAX_WITNESSES/3) /// protects the network from conversions before price has been established
#define CREA_CONVERSION_DELAY_PRE_HF_16      (fc::days(7))
#define CREA_CONVERSION_DELAY                (fc::hours(CREA_FEED_HISTORY_WINDOW)) //3.5 day conversion

#define CREA_MIN_UNDO_HISTORY                10
#define CREA_MAX_UNDO_HISTORY                10000

#define CREA_MIN_TRANSACTION_EXPIRATION_LIMIT (CREA_BLOCK_INTERVAL * 5) // 5 transactions per block
#define CREA_BLOCKCHAIN_PRECISION            uint64_t( 1000 )

#define CREA_BLOCKCHAIN_PRECISION_DIGITS     3
#define CREA_MAX_INSTANCE_ID                 (uint64_t(-1)>>16)
/** NOTE: making this a power of 2 (say 2^15) would greatly accelerate fee calcs */
#define CREA_MAX_AUTHORITY_MEMBERSHIP        10
#define CREA_MAX_ASSET_WHITELIST_AUTHORITIES 10
#define CREA_MAX_URL_LENGTH                  127

#define CREA_IRREVERSIBLE_THRESHOLD          (75 * CREA_1_PERCENT)

#define CREA_VIRTUAL_SCHEDULE_LAP_LENGTH  ( fc::uint128(uint64_t(-1)) )
#define CREA_VIRTUAL_SCHEDULE_LAP_LENGTH2 ( fc::uint128::max_value() )

#define CREA_INITIAL_VOTE_POWER_RATE (40)
#define CREA_REDUCED_VOTE_POWER_RATE (10)

#define CREA_MAX_LIMIT_ORDER_EXPIRATION     (60*60*24*28) // 28 days
#define CREA_DELEGATION_RETURN_PERIOD_HF0   (CREA_CASHOUT_WINDOW_SECONDS)
#define CREA_DELEGATION_RETURN_PERIOD_HF20  (CREA_VOTING_FLOW_REGENERATION_SECONDS)

#define CREA_RD_MIN_DECAY_BITS               6
#define CREA_RD_MAX_DECAY_BITS              32
#define CREA_RD_DECAY_DENOM_SHIFT           36
#define CREA_RD_MAX_POOL_BITS               64
#define CREA_RD_MAX_BUDGET_1                ((uint64_t(1) << (CREA_RD_MAX_POOL_BITS + CREA_RD_MIN_DECAY_BITS - CREA_RD_DECAY_DENOM_SHIFT))-1)
#define CREA_RD_MAX_BUDGET_2                ((uint64_t(1) << (64-CREA_RD_DECAY_DENOM_SHIFT))-1)
#define CREA_RD_MAX_BUDGET_3                (uint64_t( std::numeric_limits<int32_t>::max() ))
#define CREA_RD_MAX_BUDGET                  (int32_t( std::min( { CREA_RD_MAX_BUDGET_1, CREA_RD_MAX_BUDGET_2, CREA_RD_MAX_BUDGET_3 } )) )
#define CREA_RD_MIN_DECAY                   (uint32_t(1) << CREA_RD_MIN_DECAY_BITS)
#define CREA_RD_MIN_BUDGET                  1
#define CREA_RD_MAX_DECAY                   (uint32_t(0xFFFFFFFF))

#define CREA_ACCOUNT_SUBSIDY_PRECISION      (CREA_100_PERCENT)

// We want the global subsidy to run out first in normal (Poisson)
// conditions, so we boost the per-witness subsidy a little.
#define CREA_WITNESS_SUBSIDY_BUDGET_PERCENT (125 * CREA_1_PERCENT)

// Since witness decay only procs once per round, multiplying the decay
// constant by the number of witnesses means the per-witness pools have
// the same effective decay rate in real-time terms.
#define CREA_WITNESS_SUBSIDY_DECAY_PERCENT  (CREA_MAX_WITNESSES * CREA_100_PERCENT)

// 347321 corresponds to a 5-day halflife
#define CREA_DEFAULT_ACCOUNT_SUBSIDY_DECAY  (347321)
// Default rate is 0.5 accounts per block
#define CREA_DEFAULT_ACCOUNT_SUBSIDY_BUDGET (797)
#define CREA_DECAY_BACKSTOP_PERCENT         (90 * CREA_1_PERCENT)

/**
 *  Reserved Account IDs with special meaning
 */
///@{
/// Represents the current witnesses
#define CREA_MINER_ACCOUNT                   "miners"
/// Represents the canonical account with NO authority (nobody can access funds in null account)
#define CREA_NULL_ACCOUNT                    "null"
/// Represents the canonical account with WILDCARD authority (anybody can access funds in temp account)
#define CREA_TEMP_ACCOUNT                    "temp"
/// Represents the canonical account for specifying you will vote for directly (as opposed to a proxy)
#define CREA_PROXY_TO_SELF_ACCOUNT           ""
/// Represents the canonical root post parent account
#define CREA_ROOT_POST_PARENT                (account_name_type())
///@}

#ifdef CREA_ENABLE_SMT

#define SMT_MAX_VOTABLE_ASSETS 2
#define SMT_VESTING_WITHDRAW_INTERVAL_SECONDS   (60*60*24*7) /// 1 week per interval
#define SMT_UPVOTE_LOCKOUT                      (60*60*12)   /// 12 hours
#define SMT_EMISSION_MIN_INTERVAL_SECONDS       (60*60*6)    /// 6 hours
#define SMT_EMIT_INDEFINITELY                   (std::numeric_limits<uint32_t>::max())
#define SMT_MAX_NOMINAL_VOTES_PER_DAY           (1000)
#define SMT_MAX_VOTES_PER_REGENERATION          ((SMT_MAX_NOMINAL_VOTES_PER_DAY * SMT_VESTING_WITHDRAW_INTERVAL_SECONDS) / 86400)
#define SMT_DEFAULT_VOTES_PER_REGEN_PERIOD      (50)
#define SMT_DEFAULT_PERCENT_CURATION_REWARDS    (25 * CREA_1_PERCENT)

#endif /// CREA_ENABLE_SMT

