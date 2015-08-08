#ifndef PS_STATION_H
#define PS_STATION_H

#include "STATION.h"


class PS_Station: public Station {
public:
    PS_Station(unsigned int cN,unsigned int QSize ,unsigned int eN):Station(cN, QSize, eN){

    }

    PS_Station(unsigned int customer_num,unsigned int Q_size ,vector< vector<double> > &service_times)
        :Station(customer_num, Q_size, service_times){

    }

    virtual ~ PS_Station(){

    }

    virtual double waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D * comma = NULL,int k = 0);
    /*
    virtual double waitTimeCalc(vector<unsigned int>& population, int k);

    virtual double openWait(){
        return serviceTimes[0]/(1-utilisation);
    }*/
};








#endif // PS_STATION_H
