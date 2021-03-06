#ifndef _RAND_H
#define _RAND_H

#include <fstream>
#include <iostream>

class LinCongRNG {
public:
    typedef unsigned long long UInt8;

    LinCongRNG(int seed = -1) : state(seed){
        setSeed(seed);
    }

    UInt8 unifInt(){
        update();
        return state;
    }

    UInt8 unifInt(int n){
        update();
        return state % n;
    }

    double unifReal(){
        update();
        return state / dmod; //dmod è un double
    }

    void setSeed(int seed){
        if(seed <= 0){
            state = randomSeed();
        } else {
            state = seed;
        }
    }

private:
    void update(){
        state = (mult * state + add) % mod;
    }

    UInt8 randomSeed(){
        int rdm;
        std::ifstream urandom("/dev/urandom", std::ios::in | std::ios::binary);
        urandom.read((char*)&rdm, 4);
        urandom.close();

        return abs(rdm) % mod;
    }

    UInt8 state;

    static const UInt8 mult = 6364136223846793005ULL;
    static const UInt8 add = UInt8(1442695040888963407ULL);
    static const UInt8 mod = 9223372036854775807;
    static constexpr double dmod = 9223372036854775807.;
};

#endif// _RAND_H




