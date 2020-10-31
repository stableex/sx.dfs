# **`DFS`**

> Peripheral EOSIO smart contracts for interacting with DFS

## Dependencies

- [`sx.uniswap`](https://github.com/stableex/sx.uniswap)

## Quickstart

```c++
#include <sx.dfs/dfs.hpp>
#include <sx.uniswap/uniswap.hpp>

// user input
const asset quantity = asset{10000, symbol{"EOS", 4}};
const uint64_t mid = 17; // EOS/USDT pair

// get dfs info
const auto [ reserve_in, reserve_out ] = dfs::get_reserves( mid, quantity.symbol );
const uint8_t fee = dfs::get_fee();

// calculate out price
const asset out = uniswap::get_amount_out( quantity, reserves_in, reserves_out, fee );
// => "2.6500 USDT"
```

## Table of Content

- [STATIC `get_reserves`](#static-get_reserves)
- [STATIC `get_fee`](#static-get_fee)
- [STATIC `get_rewards`](#static-get_rewards)

## STATIC `get_reserves`

Get reserves for a pair

### params

- `{uint64_t} mid` - market id
- `{symbol} sort` - sort by symbol (reserve0 will be first item in pair)
- `{name} [code="defisswapcnt"]` - code account

### returns

- `{pair<asset, asset>}` - pair of reserve assets

### example

```c++
const uint64_t mid = 17;
const symbol sort = symbol{"EOS", 4};

const auto [reserve0, reserve1] = dfs::get_reserves( mid, sort );
// reserve0 => "4585193.1234 EOS"
// reserve1 => "12568203.3533 USDT"
```

## STATIC `get_fee`

Get DFS total fee

### params

- `{name} [code="defisswapcnt"]` - code account

### returns

- `{uint8_t}` - total fee (trade + protocol)

### example

```c++
const uint8_t fee = dfs::get_fee();
// => 30
```

## STATIC `get_rewards`

Get rewards for trading

### params

- `{uint64_t} pair_id` - pair id
- `{asset} in` - input quantity
- `{asset} out` - out quantity
- `{name} [code="defisswapcnt"]` - code account

### returns

- {asset} = rewards in DFS

### example

```c++
const uint64_t pair_id = 12;
const asset in = asset{10000, {"EOS", 4}};
const asset out = asset{12345, {"USDT", 4}};

const auto rewards = dfs::get_rewards( pair_id, in, out );
// rewards => "0.123456 DFS"
```