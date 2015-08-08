#ifndef FCFS_STATION_H
#define FCFS_STATION_H

#include "STATION.h"


class FCFS_Station: public Station {
public:
    FCFS_Station(unsigned int cN,unsigned int QSize ,unsigned int eN):Station(cN, QSize, eN){

    }

    FCFS_Station(unsigned int customer_num,unsigned int Q_size ,vector< vector<double> > &service_times)
        :Station(customer_num, Q_size, service_times){

    }

    virtual ~ FCFS_Station(){

    }

    virtual double waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D * comma = NULL,int k = 0);

    /*
    virtual double openWait(){
        return serviceTimes[0]/(1-utilisation);
    }*/


};


#endif // FCFS_STATION_H
