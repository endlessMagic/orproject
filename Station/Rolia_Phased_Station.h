#ifndef ROLIA_PHASED_STATION_H
#define ROLIA_PHASED_STATION_H

#include "Station/Phased_Station.h"

class Rolia_Phased_Station: public Phased_Station {
public:
    Rolia_Phased_Station(unsigned int cN,unsigned int QSize ,unsigned int eN):Phased_Station(cN, QSize, eN){

    }

    Rolia_Phased_Station(unsigned int customer_num,unsigned int Q_size ,vector< vector<double> > &service_times)
        :Phased_Station(customer_num, Q_size, service_times){

    }

    virtual ~ Rolia_Phased_Station();

    virtual double waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D * comma = NULL,int k = 0);

    double getOverTakingProb();
    /*// here need to be defined
    virtual double openWait(){
        return serviceTimes[0]/(1-utilisation);
    }*/

};

#endif // ROLIA_PHASED_STATION_H
