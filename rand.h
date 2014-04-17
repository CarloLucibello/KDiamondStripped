//
//  mt64.h
//  Corr_Int
//
//  Created by jacopo rocchi on 29/11/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

// queste sono le funzioni del generatore di mersenne twister 

typedef double tipo;
typedef unsigned long long UInt8;

void init_genrand64(unsigned long long seed);
void init_by_array64(unsigned long long init_key[], unsigned long long key_length);
unsigned long long genrand64_int64(void);
long long genrand64_int63(void);
double genrand64_real1(void);
double genrand64_real2(void);
double genrand64_real3(void);
tipo Mersennetwister();

// queste sono le funzioni del generatore alle congruenze lineari 

UInt8 IntRand(UInt8 rand);




