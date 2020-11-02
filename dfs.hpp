#pragma once

#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>

namespace dfs {

    using eosio::asset;
    using eosio::symbol;
    using eosio::name;
    using eosio::current_time_point;

    const name id = "dfs"_n;
    const name exchange = "defisswapcnt"_n;

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
        double_t            price0_last;

        uint64_t primary_key() const { return mid; }
    };
    typedef eosio::multi_index< "markets"_n, markets_row > markets;

    /**
     * DFS eggs
     */
    struct [[eosio::table]] eggargs_row {
        uint64_t            mid;
        uint64_t            time_gap;
        double_t            lucky_discount;
        asset               trigger_value_max;

        uint64_t primary_key() const { return mid; }
    };
    typedef eosio::multi_index< "eggargs"_n, eggargs_row > eggargs;

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
     * Get total fee
     *
     * ### params
     *
     * - `{name} [code="defisswapcnt"]` - code account
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
    static uint8_t get_fee( const name code = dfs::exchange )
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
     * - `{name} [code="defisswapcnt"]` - code account
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
    static std::pair<asset, asset> get_reserves( const uint64_t mid, const symbol sort, const name code = "defisswapcnt"_n )
    {
        // table
        dfs::markets _pairs( code, code.value );
        auto pairs = _pairs.get( mid, "DFSLibrary: INVALID_MID" );
        //eosio::check( pairs.reserve0.symbol == sort || pairs.reserve1.symbol == sort, "DFSLibrary: sort symbol "+sort.code().to_string()+" for pair "+to_string(mid)+" does not match reserves: "+pairs.reserve0.symbol.code().to_string()+","+pairs.reserve1.symbol.code().to_string());
        eosio::check( pairs.reserve0.symbol == sort || pairs.reserve1.symbol == sort, "DFSLibrary: sort symbol doesn't match");
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
     * - `{asset} in` - input quantity
     * - `{asset} out` - out quantity
     * - `{name} [code="defisswapcnt"]` - code account
     *
     * ### returns
     *
     * - {asset} = rewards in DFS
     *
     * ### example
     *
     * ```c++
     * const uint64_t pair_id = 12;
     * const asset in = asset{10000, {"EOS", 4}};
     * const asset out = asset{12345, {"USDT", 4}};
     *
     * const auto rewards = dfs::get_rewards( pair_id, in, out );
     * // rewards => "0.123456 DFS"
     * ```
     */
    static asset get_rewards( const uint64_t pair_id, asset in, asset out, const name code = "defisswapcnt"_n )
    {
        asset res {0, symbol{"DFS",4}};
        if (in.symbol != symbol{"EOS",4}) std::swap(in, out);
        if (in.symbol != symbol{"EOS",4}) return res;     //return 0 if non-EOS pair

        //calculate only every 5 minutes. Hack: return early on non-5 minutes.
        auto now = (eosio::current_time_point().sec_since_epoch() - 1604081400);
        if (now % 300) return res;      //lucky egg times

        // //formula seems right but result doesn't match...
        // dfs::pools _pools( "dfspoolsvote"_n, "dfspoolsvote"_n.value );
        // auto poolit = _pools.find( pair_id );
        // if(poolit==_pools.end()) return res;

        // dfs::poolslots _slots( "miningpool11"_n, "miningpool11"_n.value );
        // auto mineit = _slots.find( poolit->rank );
        // if(mineit==_slots.end()) return res;

        dfs::eggargs _eggargs ("miningpool11"_n, "miningpool11"_n.value );
        auto rowit = _eggargs.find(pair_id);
        if(rowit == _eggargs.end()) return res;

        if(now % (rowit->time_gap * 60) || in > rowit->trigger_value_max) return res;

        dfs::markets _pairs( code, code.value );
        auto dfsrate = _pairs.get( 39, "DFSLibrary: Bad EOS/DFS market id" ).price0_last;

        //non-working formula: https://github.com/defis-net/defis-network#mining-defis-network-mining-pools
        float fee = in.amount * get_fee() / 10000;
        res.amount = fee * dfsrate * rowit->lucky_discount * 0.8;

        return res;
    }
}
