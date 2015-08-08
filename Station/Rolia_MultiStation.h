#ifndef ROLIA_MULTISTATION_H
#define ROLIA_MULTISTATION_H

#include "STATION.h"


class Rolia_MultiStation: public Station {
public:
    Rolia_MultiStation(unsigned int cN,unsigned int QSize ,unsigned int eN,int a):Station(cN, QSize, eN),amount(a){

    }

    Rolia_MultiStation(unsigned int customer_num,unsigned int Q_size ,vector< vector<double> > &service_times, int a)
        :Station(customer_num, Q_size, service_times), amount(a){

    }

    virtual ~ Rolia_MultiStation(){

    }

    //  the amount of servers in this station
   int amount;

    virtual double waitTimeCalc(unsigned int m, int e, Vector4D & queue_lengths, int partIndex, Vector4D * comma = NULL, int k = 0);
    /*
    virtual double waitTimeCalc(vector<unsigned int>& population, int k);
    double pb(vector<unsigned int>& population, int k);

    virtual double openWait(){
        return serviceTimes[0]/(1-utilisation);//
    }*/
};










#endif // ROLIA_MULTISTATION_H
