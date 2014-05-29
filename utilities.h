#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <algorithm>
using namespace std;

template<class T>
bool contains(const vector<T>& vec, const T& ele){
    return find(vec.begin(), vec.end(), ele) != vec.end();
}

//Attenzione, non può aggiungere se stesso
template<class T>
void appendTo(vector<T>& vec1, const vector<T>& vec2){
    vec1.insert(vec1.end(), vec2.begin(), vec2.end());
}

template<class T>
int indexOf(const vector<T>& vec, const T& ele){
    int idx = 0;
    for(const auto& e : vec){
        if(e == ele){
            return idx;
        }
        idx++;
    }
    return -1; //elemento non trovato
}

template<class T>
void removeAt(vector<T>& vec, int pos){
    vec.erase(vec.begin() + pos);
}


template<class T>
T takeAt(vector<T>& vec, int pos){
    auto ele = vec[pos];
    removeAt(vec, pos);
    return ele;
}



#endif // UTILITIES_H
