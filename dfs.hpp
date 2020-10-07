#pragma once

#include <eosio/singleton.hpp>
#include <eosio/asset.hpp>

namespace dfs {

    using eosio::asset;
    using eosio::symbol;
    using eosio::name;
    using eosio::singleton;
    using eosio::multi_index;
    using eosio::time_point_sec;

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
    //  * Defibox config
    //  */
    // struct [[eosio::table]] config_row {
    //     uint8_t             status = 0;
    //     uint64_t            pair_id = 663;
    //     uint8_t             trade_fee = 20;
    //     uint8_t             protocol_fee = 10;
    //     name                fee_account = "fees.defi"_n;
    // };
    // typedef eosio::singleton< "config"_n, config_row > config;

    // /**
    //  * Defibox stakes
    //  */
    // struct [[eosio::table("stakes")]] stakes_row {
    //     name            owner;
    //     uint64_t        staked;
    //     uint64_t        refunding;
    //     time_point_sec  release_time;

    //     uint64_t primary_key() const { return owner.value; }
    // };
    // typedef eosio::multi_index< "stakes"_n, stakes_row > stakes;

    // /**
    //  * Defibox stat
    //  */
    // struct [[eosio::table("stat")]] stat_row {
    //     uint64_t            locked;
    //     uint64_t            staked;
    //     uint64_t            refunding;
    // };
    // typedef eosio::singleton< "stat"_n, stat_row > stat;

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
        // dfs::config _config( CONTRACT, CONTRACT.value );
        // dfs::config_row config = _config.get_or_default();
        // return config.trade_fee + config.protocol_fee;
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
     * const uint64_t mid = 12;
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
}