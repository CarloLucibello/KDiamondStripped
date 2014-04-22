/*
 * observable.h
 *
 *  Created on: 30/gen/2013
 *      Author: carlo
 */

#ifndef __OBSERVABLE_H_
#define __OBSERVABLE_H_

#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


class Observable{ //analisi degli errori
public:
	double v1;
	double v2;
	double t;
	double vlast;

public:
	Observable() : v1(0), v2(0), t(0), vlast(0){}

	Observable(double v) : v1(v), v2(v*v), t(1), vlast(v) {}

    int times(){
    	return (int) t;
    }

    void reset(){
    	v1 = 0;
    	v2 = 0;
    	t = 0;
    	vlast = 0;
    }

	double mean() const{
		if (t <= 0)
			return 0; //here one could throw!
		else
			return v1 / t;
	}

	double squareMean() const{
		if (t <= 0)
			return 0; //here one could throw!
		else
			return v2 / t;
	}

	double variance() const{
		return squareMean() - pow(mean(), 2);
	}

    double stdDev() const{
		return sqrt(variance());
	}

    double error() const{
    	double  var = variance();

		if (var <= 0 || t <= 1)
			return 0; //here one could throw!
		else
			return sqrt(var / (t - 1));
    }

    double last(){
    	return vlast;
    }

    Observable shifted(double val){
    	Observable obs = *this;
    	obs.v1 += t*val;
    	obs.vlast += val;

    	return obs;
    }

    void shift(double val){
     	v1 += t*val;
     	vlast += val;
     }

 	Observable& operator+=(double val){
 		v1 += val;
 		v2 += val*val;
 		vlast = val;
 		t++;

 		return *this;
 	}

 	Observable& operator+=(const Observable&  val){
 		v1 += val.v1;
 		v2 += val.v2;
 		t += val.t;

 		return *this;
 	}

 	Observable& operator*=(double val){
 		v1 *= val;
 		v2 *= val*val;

 		return *this;
 	}

 	Observable& operator/=(double val){

 		return (*this)*= 1 / val;
 	}

};

inline ostream& operator<<(ostream& stream, const  Observable& obs){
    double mean = obs.mean();
    double error = obs.error();
//    if(mean < 1e-16 && error  < 1e-16){
//        mean = 0;
//        error = 0;
//    }
//	int p = floor(-log10(obs.error() - floor(obs.error()))) + 2;
	stream << setprecision(10) << mean <<" "
		   << setprecision(4) << error << setprecision(6);

	return stream;
}

inline const Observable operator+(const  Observable& obs1,  double val){
	Observable obs = obs1;
	obs += val;

	return obs;
}

inline const Observable operator+(double val, const  Observable& obs1){
	return operator+(obs1, val);
}

inline const Observable operator+(const  Observable& obs1,  const  Observable& obs2){
	Observable obs = obs1;
	obs += obs2;

	return obs;
}

inline const Observable operator*(const  Observable& obs1, double val){
	Observable obs = obs1;
	obs *= val;

	return obs;
}

inline const Observable operator*(double val, const  Observable& obs1){
	return operator*(obs1, val);
}

inline const Observable operator/(const  Observable& obs1, double val){
	Observable obs = obs1;
	obs /= val;

	return obs;
}

inline const Observable operator/(double val, const  Observable& obs1){
	return operator/(obs1, val);
}

inline bool operator<(const  Observable& obs1,  const  Observable& obs2){
	return obs1.mean() < obs2.mean();
}

inline bool operator>(const  Observable& obs1,  const  Observable& obs2){
	return obs1.mean() > obs2.mean();
}


#endif /* __OBSERVABLE_H_ */
