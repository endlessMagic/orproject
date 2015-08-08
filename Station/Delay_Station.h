#ifndef DELAY_STATION_H
#define DELAY_STATION_H

#include "Station/STATION.h"


class Delay_Station: public Station {
public:
    Delay_Station(unsigned int cN,unsigned int QSize ,unsigned int eN):Station(cN, QSize, eN){

    }


    Delay_Station(unsigned int customer_num,unsigned int Q_size ,vector< vector<double> > &service_times)
        :Station(customer_num, Q_size, service_times){

    }

    virtual ~ Delay_Station(){

    }

    virtual double waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D * comma = NULL,int k = 0);
        ///lastTime//cout<<"IS:"<<serviceTimes[e][0]<<endl;
        //return serviceTimes[e][0];

    /*
    virtual double openWait(){
        return serviceTimes[0];
    }

    virtual double alpha(int n){
        return pow( rou ,n) ;
    }

    virtual double mixedWait(){
        return serviceTimes[0];
    }*/
};


#endif // DELAY_STATION_H
