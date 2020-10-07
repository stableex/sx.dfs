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
}