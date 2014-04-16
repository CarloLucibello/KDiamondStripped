/*
 * RNG.h
 *
 *  Created on: 11/gen/2013
 *      Author: carlo
 */

#ifndef RNG_H_
#define RNG_H_

#include <cmath>
#include <fstream>
#include <cassert>
#include <vector>

#include "../cppUtils/my_algorithm.h"

using namespace std;

#ifdef RANDOMLIB
#include <RandomLib/Random.hpp>
#include <RandomLib/NormalDistribution.hpp>

using namespace RandomLib;
#endif //RANDOMLIB

#ifdef GSLLIB
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#endif //GSLLIB

namespace cpputils{

/********
Forwarda declarations
***********************/
double logFactorial(int n);


/***
* INTERFACE
/************///

template <class RNG>
float uniform(RNG& rng){
	return rng.unif();
}

template <class RNG>
double unifReal(RNG& rng){
	return rng.unifReal();
}

template <class RNG>
int unifInt(RNG& rng, int N){
	return rng.unifInt(N);
}

template <class RNG, class T>
T unifEle(RNG& rng, const vector<T>& v){
	return v[unifInt(rng, v.size())];
}

template <class RNG>
double unifReal(RNG& rng, double a, double b){
	return a + (b-a) * rng.unifReal();
}

//BOx-Muller
template <class RNG>
double gaussian(RNG& rng){
	static int iset = 0;
	static double gset;
	double fac, rsq, v1, v2;

	if (iset == 0) {
		do {
				v1 = 2.0 * unifReal(rng) - 1.0;
				v2 = 2.0 * unifReal(rng) - 1.0;
				rsq = v1 * v1 + v2 * v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac = sqrt(-2.0 * log(rsq) / rsq);
		gset = v1 * fac;
		iset = 1;
		return v2 * fac;
	} else {
		iset = 0;
		return gset;
	}
}

template <class RNG>
int poisson(RNG& rng, double lambda){
    return (lambda < 30.0) ? poissonSmall(rng, lambda) : poissonLarge(rng, lambda);
}

template <class RNG>
int poissonSmall(RNG& rng, double lambda)
{
    // Algorithm due to Donald Knuth, 1969.
    double p = 1.0;
    double L = exp(-lambda);
	int k = 0;
	do
	{
		k++;
		p *= unifReal(rng);
	}
	while (p > L);
    return k - 1;
}

template <class RNG>
int poissonLarge(RNG& rng, double lambda)
{
	// "Rejection method PA" from "The Computer Generation of Poisson Random Variables" by A. C. Atkinson
	// Journal of the Royal Statistical Society Series C (Applied Statistics) Vol. 28, No. 1. (1979)
	// The article is on pages 29-35. The algorithm given here is on page 32.

	double c = 0.767 - 3.36/lambda;
	double beta = M_PI / sqrt(3.0*lambda);
	double alpha = beta*lambda;
	double k = log(c) - lambda - log(beta);

	for(;;)
	{
		double u = unifReal(rng);
		double x = (alpha - log((1.0 - u)/u))/beta;
		int n = (int) floor(x + 0.5);
		if (n < 0)
			continue;
		double v = unifReal(rng);
		double y = alpha - beta*x;
        double temp = 1.0 + exp(y);
		double lhs = y + log(v/(temp*temp));
		double rhs = k + n*log(lambda) - logFactorial(n);
		if (lhs <= rhs)
			return n;
	}
}

// da stackoverflow : http://stackoverflow.com/questions/1728736/c-numerical-algorithm-to-generate-numbers-from-binomial-distribution
template <class RNG>
int binomial(RNG& rng, int n, double p) {
    if (n < 1000) {
        int result = 0;
        for (int i = 0; i < n; ++i)
            if (unifReal(rng) < p) result++;
        return result;
    }
    if (n * p < 10){
        return poisson(rng, n * p);
    } else if (n * (1 - p) < 10){
        return n - poisson(rng, n * p);
    } else { //nella costruzione di Erdos-Renyi esegue questo
        int v = int(0.5 + n*p + gaussian(rng)*sqrt(n*p*(1-p)));
        if (v < 0){
            v = 0;
        } else if (v > n){
            v = n;
        }
        return v;
    }
}

template<class RNG>
int pm(RNG& rng, double p = 0.5){
	return 2 * unifInt(rng, 2) - 1;
}

template<class RNG>
bool tf(RNG& rng, double p = 0.5){
	return rng.unifReal() < p ? true : false;
}

template<class RNG>
vector<int> permutation(RNG& rng, int n){
    vector<int> vec = range(n);
    vector<int> perm;
    perm.reserve(n);
    for(int i = 0; i < n; i++){
        int ind = unifInt(rng, vec.size());
        perm.push_back(vec[ind]);
        vec[ind] = vec[n-i-1];
        vec.pop_back();
    }
    return perm;
}

int randomSeed(){
    int rdm;
    ifstream urandom("/dev/urandom", ios::in|ios::binary);
    urandom.read((char*)&rdm,4);
    urandom.close();

	return abs(rdm);
}


/********
Auxiliary functions
***********************/
double logFactorial(int n){
    if (n > 254)
    {
        double x = n + 1;
        return (x - 0.5)*log(x) - x + 0.5*log(2*M_PI) + 1.0/(12.0*x);
    }
    else
    {
        double lf[] =
        {
            0.000000000000000,
            0.000000000000000,
            0.693147180559945,
            1.791759469228055,
            3.178053830347946,
            4.787491742782046,
            6.579251212010101,
            8.525161361065415,
            10.604602902745251,
            12.801827480081469,
            15.104412573075516,
            17.502307845873887,
            19.987214495661885,
            22.552163853123421,
            25.191221182738683,
            27.899271383840894,
            30.671860106080675,
            33.505073450136891,
            36.395445208033053,
            39.339884187199495,
            42.335616460753485,
            45.380138898476908,
            48.471181351835227,
            51.606675567764377,
            54.784729398112319,
            58.003605222980518,
            61.261701761002001,
            64.557538627006323,
            67.889743137181526,
            71.257038967168000,
            74.658236348830158,
            78.092223553315307,
            81.557959456115029,
            85.054467017581516,
            88.580827542197682,
            92.136175603687079,
            95.719694542143202,
            99.330612454787428,
            102.968198614513810,
            106.631760260643450,
            110.320639714757390,
            114.034211781461690,
            117.771881399745060,
            121.533081515438640,
            125.317271149356880,
            129.123933639127240,
            132.952575035616290,
            136.802722637326350,
            140.673923648234250,
            144.565743946344900,
            148.477766951773020,
            152.409592584497350,
            156.360836303078800,
            160.331128216630930,
            164.320112263195170,
            168.327445448427650,
            172.352797139162820,
            176.395848406997370,
            180.456291417543780,
            184.533828861449510,
            188.628173423671600,
            192.739047287844900,
            196.866181672889980,
            201.009316399281570,
            205.168199482641200,
            209.342586752536820,
            213.532241494563270,
            217.736934113954250,
            221.956441819130360,
            226.190548323727570,
            230.439043565776930,
            234.701723442818260,
            238.978389561834350,
            243.268849002982730,
            247.572914096186910,
            251.890402209723190,
            256.221135550009480,
            260.564940971863220,
            264.921649798552780,
            269.291097651019810,
            273.673124285693690,
            278.067573440366120,
            282.474292687630400,
            286.893133295426990,
            291.323950094270290,
            295.766601350760600,
            300.220948647014100,
            304.686856765668720,
            309.164193580146900,
            313.652829949878990,
            318.152639620209300,
            322.663499126726210,
            327.185287703775200,
            331.717887196928470,
            336.261181979198450,
            340.815058870798960,
            345.379407062266860,
            349.954118040770250,
            354.539085519440790,
            359.134205369575340,
            363.739375555563470,
            368.354496072404690,
            372.979468885689020,
            377.614197873918670,
            382.258588773060010,
            386.912549123217560,
            391.575988217329610,
            396.248817051791490,
            400.930948278915760,
            405.622296161144900,
            410.322776526937280,
            415.032306728249580,
            419.750805599544780,
            424.478193418257090,
            429.214391866651570,
            433.959323995014870,
            438.712914186121170,
            443.475088120918940,
            448.245772745384610,
            453.024896238496130,
            457.812387981278110,
            462.608178526874890,
            467.412199571608080,
            472.224383926980520,
            477.044665492585580,
            481.872979229887900,
            486.709261136839360,
            491.553448223298010,
            496.405478487217580,
            501.265290891579240,
            506.132825342034830,
            511.008022665236070,
            515.890824587822520,
            520.781173716044240,
            525.679013515995050,
            530.584288294433580,
            535.496943180169520,
            540.416924105997740,
            545.344177791154950,
            550.278651724285620,
            555.220294146894960,
            560.169054037273100,
            565.124881094874350,
            570.087725725134190,
            575.057539024710200,
            580.034272767130800,
            585.017879388839220,
            590.008311975617860,
            595.005524249382010,
            600.009470555327430,
            605.020105849423770,
            610.037385686238740,
            615.061266207084940,
            620.091704128477430,
            625.128656730891070,
            630.172081847810200,
            635.221937855059760,
            640.278183660408100,
            645.340778693435030,
            650.409682895655240,
            655.484856710889060,
            660.566261075873510,
            665.653857411105950,
            670.747607611912710,
            675.847474039736880,
            680.953419513637530,
            686.065407301994010,
            691.183401114410800,
            696.307365093814040,
            701.437263808737160,
            706.573062245787470,
            711.714725802289990,
            716.862220279103440,
            722.015511873601330,
            727.174567172815840,
            732.339353146739310,
            737.509837141777440,
            742.685986874351220,
            747.867770424643370,
            753.055156230484160,
            758.248113081374300,
            763.446610112640200,
            768.650616799717000,
            773.860102952558460,
            779.075038710167410,
            784.295394535245690,
            789.521141208958970,
            794.752249825813460,
            799.988691788643450,
            805.230438803703120,
            810.477462875863580,
            815.729736303910160,
            820.987231675937890,
            826.249921864842800,
            831.517780023906310,
            836.790779582469900,
            842.068894241700490,
            847.352097970438420,
            852.640365001133090,
            857.933669825857460,
            863.231987192405430,
            868.535292100464630,
            873.843559797865740,
            879.156765776907600,
            884.474885770751830,
            889.797895749890240,
            895.125771918679900,
            900.458490711945270,
            905.796028791646340,
            911.138363043611210,
            916.485470574328820,
            921.837328707804890,
            927.193914982476710,
            932.555207148186240,
            937.921183163208070,
            943.291821191335660,
            948.667099599019820,
            954.046996952560450,
            959.431492015349480,
            964.820563745165940,
            970.214191291518320,
            975.612353993036210,
            981.015031374908400,
            986.422203146368590,
            991.833849198223450,
            997.249949600427840,
            1002.670484599700300,
            1008.095434617181700,
            1013.524780246136200,
            1018.958502249690200,
            1024.396581558613400,
            1029.838999269135500,
            1035.285736640801600,
            1040.736775094367400,
            1046.192096209724900,
            1051.651681723869200,
            1057.115513528895000,
            1062.583573670030100,
            1068.055844343701400,
            1073.532307895632800,
            1079.012946818975000,
            1084.497743752465600,
            1089.986681478622400,
            1095.479742921962700,
            1100.976911147256000,
            1106.478169357800900,
            1111.983500893733000,
            1117.492889230361000,
            1123.006317976526100,
            1128.523770872990800,
            1134.045231790853000,
            1139.570684729984800,
            1145.100113817496100,
            1150.633503306223700,
            1156.170837573242400,
        };
        return lf[n];
    }
}
/********
Specializations
***********************/


/**********************************************
Generators
************************/





#ifdef GSLLIB
class GSL_MarsTwist{
public:
	gsl_rng * rng;
	gsl_ran_discrete_t * discr;
	bool discrInited;

	GSL_MarsTwist(int s = -1){
		rng = gsl_rng_alloc(gsl_rng_taus);
		seed(s);
		discrInited = false;
	}

	void seed(int seed = -1){
		while(seed <= 0){
			seed = randomSeed();
		}
		gsl_rng_set(rng, seed);
	}

	~GSL_MarsTwist(){
		gsl_rng_free(rng);
		if(discrInited){
            free(discr);
		}
	}

	void initDiscrete(const vector<double>& p){
		if(discrInited){
             gsl_ran_discrete_free (discr);
		}
        discr = gsl_ran_discrete_preproc (p.size(), p.data());
        discrInited = true;
	}

	size_t discrete(){
        return gsl_ran_discrete (rng, discr);
	}

	double unifReal(){
			return gsl_rng_uniform(rng);
	}

	int unifInt(int n){
		return gsl_rng_uniform_int(rng, n);
	}

	int binomial(double p, int n){
		return gsl_ran_binomial(rng, p, n);
	}

	double gauss(){
        return gaussian(*this);
	}
};
#endif //GSLLIB



#ifdef RANDOMLIB
class MarsTwist{
public:

	double emz = 1;
	SRandom64 sr64;
	NormalDistribution<double> normdist;

	MarsTwist(int s = -1){
		seed(s);
	}

	void seed(int seed = -1){
		while(seed <= 0){
			seed = randomSeed();
		}
		sr64.Reseed(seed);
	}

	void setParamPoiss(double z){
		emz = exp(-z);
	}

	float unif(){
		return sr64.Fixed<float>();
	}

	float unif(double a, double b){
		return a + (b-a) * sr64.Fixed<float>();
	}

	double unifReal(){
			return sr64.Fixed();
	}

	double unifReal(double a, double b){
			return a + (b-a) * sr64.Fixed();
	}

	//uniform int in [0,b)
	int unifInt(int b){
//		return sr64(b);
		return sr64.Integer(b);
	}

	int poiss(){
        int k = 0;
        double p = 1;

        do{
                k += 1;
                p *= unif();
        } while(p > emz);

        return (k-1);
	}

	int pm(float p = 0.5){
		return unif() < p ? 1 : -1;
	}

	int p0(float p = 0.5){
		return unif() < p ? 1 : 0;
	}

	int m0(float p = 0.5){
		return unif() < p ? -1 : 0;
	}

	bool tf(float p = 0.5){
		return unif() < p ? true : false;
	}

	double gauss(){
		return normdist(sr64);
	}
};

#endif //RANDOMLIB

class ParRap{
public:
	unsigned int myrand;
	unsigned int ira[256];
	unsigned char ip, ip1, ip2, ip3;
	double emz;

	ParRap(int s = -1){
		seed(s);
	}

	void seed(int seed = -1){
		while(seed <= 0){
			seed = randomSeed();
		}
		myrand = seed;
		initRandom();
	}

	void initRandom() {
		int i;

		ip = 128;
		ip1 = ip - 24;
		ip2 = ip - 55;
		ip3 = ip - 61;

		for (i = ip3; i < ip; i++) {
			ira[i] = randForInit();
		}
	}

	unsigned int randForInit(void) {
		unsigned long long y;

		y = myrand * 16807LL;
		myrand = (y & 0x7fffffff) + (y >> 31);
		if (myrand & 0x80000000) {
			myrand = (myrand & 0x7fffffff) + 1;
		}
		return myrand;
	}

	void setParamPoiss(double z){
		emz = exp(-z);
	}

	//TODO forse il membro a destra del return è un double, attenzione!!
	float unif(){
		return 2.3283064365e-10 * ((ira[ip++] = ira[ip1++] + ira[ip2++]) ^ ira[ip3++]);
	}

	float unif(float a, float b){
			return a + (b-a) * unif();
	}

//#define FNORM   (2.3283064365e-10)
//#define RANDOM  ((ira[ip++] = ira[ip1++] + ira[ip2++]) ^ ira[ip3++])
//#define FRANDOM (FNORM * RANDOM)

	double unifReal(){
			return 2.3283064365e-10 * ((ira[ip++] = ira[ip1++] + ira[ip2++]) ^ ira[ip3++]);
	}

	double unifReal(double a, double b){
			return a + (b-a) * unifReal();
	}

	//uniform int in [0,b)
	int unifInt(int b){
		return (int) (b * unifReal());  //Non funziona con unif(), a volte genera b
	}

	int poiss(){
        int k = 0;
        double p = 1;
        do{
                k += 1;
                p *= unif();
        } while(p > emz);
        return (k-1);
	}

	int pm(float p = 0.5){
		return unif() < p ? 1 : -1;
	}

	int p0(float p = 0.5){
		return unif() < p ? 1 : 0;
	}

	int m0(float p = 0.5){
		return unif() < p ? -1 : 0;
	}

	bool tf(float p = 0.5){
		return unif() < p ? true : false;
	}

	//Box-Muller algorithm
	double gauss() {
		static int iset = 0;
		static double gset;
		double fac, rsq, v1, v2;

		if (iset == 0) {
			do {
					v1 = 2.0 * unifReal() - 1.0;
					v2 = 2.0 * unifReal() - 1.0;
					rsq = v1 * v1 + v2 * v2;
			} while (rsq >= 1.0 || rsq == 0.0);
			fac = sqrt(-2.0 * log(rsq) / rsq);
			gset = v1 * fac;
			iset = 1;
			return v2 * fac;
		} else {
			iset = 0;
			return gset;
		}
	}

};


#ifdef RANDOMLIB
double gaussian(MarsTwist& rng){
	return rng.gauss();
}
#endif //RANDOMLIB


#ifdef UNITTEST
#include  <gtest/gtest.h>
class ParRapTest : public ::testing::Test {
public:
	ParRap rng;

};

TEST_F(ParRapTest, pm){
	for(int i = 0; i < 1000; i++){
		int r = rng.pm();
		EXPECT_TRUE(r == 1 || r == -1);
	}
}


TEST_F(ParRapTest, poisson){
	double mean = 0;
	int samples = 1000;
	double z = 4;
	rng.setParamPoiss(z);
	for(int i = 0; i < samples; i++){
		mean += rng.poiss();


	}
	mean /= samples;
	EXPECT_TRUE(mean > z - 4*sqrt(z) /sqrt(double(samples)));
	EXPECT_TRUE(mean < z + 4*sqrt(z)/sqrt(double(samples)));
}

class MarsTwistTest : public ::testing::Test {
public:
	MarsTwist rng;

};

TEST_F(MarsTwistTest, pm){
	for(int i = 0; i < 1000; i++){
		int r = rng.pm();
		EXPECT_TRUE(r == 1 || r == -1);
	}
}


TEST_F(MarsTwistTest, poisson){
	double mean = 0;
	int samples = 1000;
	double z = 4;
	rng.setParamPoiss(z);
	for(int i = 0; i < samples; i++){
		mean += rng.poiss();


	}
	mean /= samples;
	EXPECT_TRUE(mean > z - 4*sqrt(z) /sqrt(double(samples)));
	EXPECT_TRUE(mean < z + 3*sqrt(z) /sqrt(double(samples)));
}

TEST_F(MarsTwistTest, gaussian){
	int mean = 0;
	int samples = 10000;
	for(int i = 0; i < samples; i++){
		mean += gaussian(rng);


	}
	mean /= samples;
	EXPECT_TRUE(mean > -4 / sqrt((double)samples));
	EXPECT_TRUE(mean < 4 /sqrt((double)samples));
}

TEST_F(MarsTwistTest, unifInt){
	int N = 1000;
	int samples = 100*N;
	int count0 = 0;
	int countN = 0;
	int countN1 = 0;
	for(int i = 0; i < samples; i++){
		int r = unifInt(rng, N);
		if(r == 0){
			count0 ++;
		}
		if(r == N-1){
			countN1++;
		}

        if(r == N){
			countN++;
		}
		EXPECT_TRUE(r >= 0 && r < N);
	}
	EXPECT_TRUE(count0 > 0);
	EXPECT_TRUE(countN1 > 0);
	EXPECT_TRUE(countN == 0);
}


class GSLMarsTwistTest : public ::testing::Test {
public:
	GSL_MarsTwist rng;

};

TEST_F(GSLMarsTwistTest, pm){
	for(int i = 0; i < 1000; i++){
		int r = pm(rng);
		EXPECT_TRUE(r == 1 || r == -1);
	}
}

TEST_F(GSLMarsTwistTest, binomial){
	double mean = 0;
	int samples = 1000;
	double z = 4;
	for(int i = 0; i < samples; i++){
		mean += rng.binomial(z/samples, samples);


	}
	mean /= samples;
	EXPECT_TRUE(mean > z - 3*sqrt(z) /sqrt(double(samples)));
	EXPECT_TRUE(mean < z + 3*sqrt(z) /sqrt(double(samples)));
}

TEST_F(GSLMarsTwistTest, unifInt){
	int N = 1000;
	int samples = 100*N;
	int count0 = 0;
	int countN = 0;
	for(int i = 0; i < samples; i++){
		int r = rng.unifInt(N);
		if(r == 0){
			count0 ++;
		}
		if(r == N-1){
			countN ++;
		}
		EXPECT_TRUE(r >= 0 && r < N);
	}
	EXPECT_TRUE(count0 > 0);
	EXPECT_TRUE(countN > 0);

}

TEST_F(GSLMarsTwistTest, gaussian){
	int mean = 0;
	int samples = 10000;
	for(int i = 0; i < samples; i++){
		mean += gaussian(rng);


	}
	mean /= samples;
	EXPECT_TRUE(mean > -4 / sqrt((double)samples));
	EXPECT_TRUE(mean < 4 /sqrt((double)samples));
}

TEST_F(GSLMarsTwistTest, discrete){
    int n = 5;
	vector<int> counter(n);
	vector<double> p(n);
	for(int i = 0; i < n; i++){
        counter[i] = 0;
        p[i] = 1;
	}

    rng.initDiscrete(p);
	int samples = 10000;
	for(int i = 0; i < samples; i++){
        auto r = rng.discrete();
        EXPECT_TRUE(r < n);
        EXPECT_TRUE(r >= 0);
		counter[rng.discrete()]++;
	}

	for(int i = 0; i < n; i++){
        EXPECT_TRUE(counter[i] / double(samples) > 1/double(n) - 5/sqrt((double)samples));
        EXPECT_TRUE(counter[i] / double(samples) < 1/double(n) + 5/sqrt((double)samples));
	}

}


#endif
}//end cpputils

#endif /* RNG_H_ */
