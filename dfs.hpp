#pragma once

#include <eosio/asset.hpp>

namespace dfs {

    using eosio::asset;
    using eosio::symbol;
    using eosio::name;

    /**
     * DFS markets
     */
    struct [[eosio::table]] markets_row {
        uint64_t            mid;
        name                contract0;
        name                contract1;
        symbol              sym0;
        symbol              sym1;
        asset               reserve0;
        asset               reserve1;
        uint64_t            liquidity_token;

        uint64_t primary_key() const { return mid; }
    };
    typedef eosio::multi_index< "markets"_n, markets_row > markets;

    // /**
    //  * DFS votes
    //  */
    // struct [[eosio::table]] poolsvotes_row {
    //     uint64_t            mid;
    //     uint64_t            rank;
    //     double_t            total_votes;

    //     uint64_t primary_key() const { return mid; }
    // };
    // typedef eosio::multi_index< "pools"_n, poolsvotes_row > pools;

    // /**
    //  * DFS mining
    //  */
    // struct [[eosio::table]] poolslots_row {
    //     uint64_t            rank;
    //     double_t            default_distount;
    //     double_t            lucky_distount;
    //     double_t            pool_weight;
    //     asset               trigger_value_max;
    //     asset               daily_max_supply;
    //     asset               daily_supply;
    //     double_t            aprs;
    //     double_t            dynamic_aprs;
    //     time_point_sec      last_update;

    //     uint64_t primary_key() const { return rank; }
    // };
    // typedef eosio::multi_index< "poolslots"_n, poolslots_row > poolslots;
    /**
     * ## STATIC `get_fee`
     *
     * Get DFS total fee
     *
     * ### returns
     *
     * - `{uint8_t}` - total fee (trade + protocol)
     *
     * ### example
     *
     * ```c++
     * const uint8_t fee = dfs::get_fee();
     * // => 30
     * ```
     */
    static uint8_t get_fee()
    {
        return 30;
    }

    /**
     * ## STATIC `get_reserves`
     *
     * Get reserves for a pair
     *
     * ### params
     *
     * - `{uint64_t} mid` - market id
     * - `{symbol} sort` - sort by symbol (reserve0 will be first item in pair)
     *
     * ### returns
     *
     * - `{pair<asset, asset>}` - pair of reserve assets
     *
     * ### example
     *
     * ```c++
     * const uint64_t mid = 17;
     * const symbol sort = symbol{"EOS", 4};
     *
     * const auto [reserve0, reserve1] = dfs::get_reserves( mid, sort );
     * // reserve0 => "4585193.1234 EOS"
     * // reserve1 => "12568203.3533 USDT"
     * ```
     */
    static std::pair<asset, asset> get_reserves( const uint64_t mid, const symbol sort )
    {
        // table
        dfs::markets _pairs( "defisswapcnt"_n, "defisswapcnt"_n.value );
        auto pairs = _pairs.get( mid, "DFSLibrary: INVALID_MID" );
        eosio::check( pairs.reserve0.symbol == sort || pairs.reserve1.symbol == sort, "sort symbol does not match" );

        return sort == pairs.reserve0.symbol ?
            std::pair<asset, asset>{ pairs.reserve0, pairs.reserve1 } :
            std::pair<asset, asset>{ pairs.reserve1, pairs.reserve0 };
    }

    /**
     * ## STATIC `get_rewards`
     *
     * Get rewards for trading
     *
     * ### params
     *
     * - `{uint64_t} pair_id` - pair id
     * - `{asset} from` - tokens we are trading from
     * - `{asset} to` - tokens we are trading to
     *
     * ### returns
     *
     * - {asset} = rewards in DFS
     *
     * ### example
     *
     * ```c++
     * const uint64_t pair_id = 12;
     * const asset from = asset{10000, {"EOS", 4}};
     * const asset to = asset{12345, {"USDT", 4}};
     *
     * const auto rewards = dfs::get_rewards( pair_id, from, to);
     * // rewards => "0.123456 DFS"
     * ```
     */
    static asset get_rewards( const uint64_t pair_id, asset from, asset to )
    {
        asset res {0, symbol{"DFS",4}};
        auto eos = from.symbol.code().to_string() == "EOS" ? from : to;
        if(eos.symbol.code().to_string() != "EOS")
            return res;     //return 0 if non-EOS pair

        // //formula seems right but result doesn't match...
        // dfs::pools _pools( "dfspoolsvote"_n, "dfspoolsvote"_n.value );
        // auto poolit = _pools.find( pair_id );
        // if(poolit==_pools.end()) return res;

        // dfs::poolslots _slots( "miningpool11"_n, "miningpool11"_n.value );
        // auto mineit = _slots.find( poolit->rank );
        // if(mineit==_slots.end()) return res;


        // float fee = eos.amount * 0.003;

        ////for OGX: res.amount = 3000 * 0.452 * 0.75 * 0.1605 * 1.479;
        //res.amount = fee * 0.452 * 0.75 * mineit->default_distount * mineit->pool_weight;

        return res;
    }
}