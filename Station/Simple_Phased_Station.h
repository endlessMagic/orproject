#ifndef SIMPLE_PHASED_STATION_H
#define SIMPLE_PHASED_STATION_H

#include "Station/Rolia_Phased_Station.h"


class Simple_Phased_Station: public Rolia_Phased_Station {
public:
    Simple_Phased_Station(unsigned int cN,unsigned int QSize ,unsigned int eN):Rolia_Phased_Station(cN, QSize, eN){

    }

    Simple_Phased_Station(unsigned int customer_num,unsigned int Q_size ,vector< vector<double> > &service_times)
        :Rolia_Phased_Station(customer_num, Q_size, service_times){

    }

    virtual ~ Simple_Phased_Station();

    virtual double waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D * comma = NULL,int k = 0);

    double getOverTakingProb();
    /*// here need to be defined
    virtual double openWait(){
        return serviceTimes[0]/(1-utilisation);
    }*/

};


#endif // SIMPLE_PHASED_STATION_H
