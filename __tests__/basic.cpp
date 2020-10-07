#include <eosio/eosio.hpp>
#include <sx.dfs/dfs.hpp>

using eosio::print;
using eosio::symbol;

class [[eosio::contract]] basic : public eosio::contract {

public:
    using contract::contract;

    [[eosio::action]]
    void getreserves( const uint64_t mid, const symbol sort )
    {
        const auto [ reserveIn, reserveOut] = dfs::get_reserves( mid, sort );
        print( reserveIn );
        print( reserveOut );
    }

    [[eosio::action]]
    void getfee() {
        const uint8_t fee = dfs::get_fee();
        print( fee );
    }
};