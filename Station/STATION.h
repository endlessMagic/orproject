#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cassert>
#include <list>
#include <map>
#include <math.h>
//#include "NODE.h"

using namespace::std;

typedef vector<vector<double> > Vector2D;
typedef vector<vector<vector<double> > > Vector3D;
typedef vector<vector<vector<vector<double> > > > Vector4D;

template<typename T>
T getVectorSum(vector<T> & v){
    T sum = 0;
   int size = v.size();
    for (unsigned int i = 0; i < size; ++i){
        sum += v[i];
    }

    return sum;
}

template<typename T>
bool isInList(T t, list<T> & l){
    bool find = false;

    for(typename list<T>::iterator it = l.begin(); it != l.end(); ++it){
        if ( (*it) == t){
            find == true;
            break;
        }
    }

    return find;
}

template<typename T>
void printVector(vector<T> & v){
   int size = v.size();
    for (unsigned int i = 0; i < size; ++i){
        cout<<"nr. "<<i<<";value "<<v[i]<<endl;
    }
}



// return the position if a is found in lt, otherwise return 0. Position is indexed from 1
//int inVector(vector<int>& lt, int a);

// define the type of stations
enum{
 PS = 0,
 FCFS = 1,
 DELAY = 2,
 ROLIA = 3
};

unsigned int getQsize(vector<int> & cs);

class Station{
public:
    //intialisation
    //@param
    // a: amountv is to intialise the vectro visits with the size K+1
    // cN: how many kinds of costomers(with open class)
    // QSize is the size of queue_lengths_, it should be (N_1+1)*(N_2+1)..(N_(K)+1)// !!!!!!!!!!!!now without*(K+1)
    // eN: the amount of entries
    /// visits, queue_lengths_ and service_times_ need to be initialised further
    Station(unsigned int customer_num,unsigned int Q_size ,unsigned int entries_num)
        : customer_num_(customer_num),entries_num_(entries_num)
    {//utilisation(0.0), serverAmount(1){
        queue_lengths_.resize(entries_num);
        for (unsigned int e = 0; e < entries_num; ++e){
            queue_lengths_[e].resize(customer_num_);
            for (unsigned int k = 0; k < customer_num_; ++k){
                queue_lengths_[e][k].resize(Q_size);
            }
        }
        visits_.resize(entries_num);
        service_times_.resize(entries_num);
        utilisation_.resize(Q_size);
    }


    Station(unsigned int customer_num,unsigned int Q_size ,vector< vector<double> > &service_times)
        : customer_num_(customer_num),entries_num_(service_times.size())
    {//utilisation(0.0), serverAmount(1){
        queue_lengths_.resize(entries_num_);
        for (unsigned int e = 0; e < entries_num_; ++e){
            queue_lengths_[e].resize(customer_num_);
            for (unsigned int k = 0; k < customer_num_; ++k){
                queue_lengths_[e][k].resize(Q_size);
            }
        }
        visits_.resize(entries_num_);
        service_times_ = service_times;
        utilisation_.resize(Q_size);
    }



    // as inputs


    //visits size  E*(K+1)
    Vector2D visits_;
    //servicTimes E * PhaseNo
    Vector2D service_times_;


    // how many kinds of costomers
    // it equals K+1
   int customer_num_;
    // how many entries has this station
   int entries_num_;

   double think_time = 0.;

    // to reference
    // the size of queue_lengths_ in this Station is Qsize , it should be N_1+1,..,(N_1+1)*(N_2+1)..(N_(K)+1)*(K+1)
    // there are totally M queues
    Vector3D queue_lengths_;
    // utilisation
    // the size of utilisation in this Station is Qsize/(K+1) , it should be N_1+1,..,(N_1+1)*(N_2+1)..(N_(K)+1)
    vector<double> utilisation_;
    // return the desired value from utilisation
    virtual double utilisation(unsigned int index,int e = 0,int k = 0);

    // waiting time calculation
    virtual double waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D * comma = NULL,int k = 0) = 0;

    //utilisation update
    //index: identifies which element should be updated
    virtual void UpdateUtilisation(int index, vector<double> &throughputs);

    // helper variable to calculate index of variables related to the population, after initialisation is the index_helper_ with value: (1, 1, N_1+1,..,(N_1+1)*(N_2+1)..(N_(K-1)+1))
    //with the size equal the number of kinds of customerSizes, K+1
    static vector<int> index_helper_;
    //to calculation partIndex
    //
    static int partIndexCalc(vector<int>& population);
    // initialize the index_helper_
    // @param customerSizes costomers is of size of  K+1
    static void initIndexHelper(vector<int>& customerSizes ){
        index_helper_.resize( customerSizes.size() , 1);
        for (unsigned int i = 2; i < customerSizes.size(); i++){
            index_helper_[i] = index_helper_[i-1]*(customerSizes[i-1]+1);
        }
    }

    virtual ~ Station(){

    }


protected:


private:

    //other variables
    //double rou;


    /*
    virtual double openWait() = 0;
    virtual double mixedWait();

    virtual double alpha(int n){
        return 1/ pow(rou, (n+1)) ;
    }*/


};




#endif // STATION_H
