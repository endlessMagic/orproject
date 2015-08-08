#ifndef PHASED_STATION_H
#define PHASED_STATION_H

#include "Station/STATION.h"


class Phased_Station: public Station {
public:
    /*
    bool isCalulated(unsigned int index){
        return calcSigs(index);
    }*/
    // 3 dimensianlised vector
    Vector3D utilisation_;

    static vector<bool> calcSigs;

    Phased_Station(unsigned int cN,unsigned int QSize ,unsigned int eN):Station(cN, QSize, eN){
        utilisation_ = queue_lengths_;
    }

    Phased_Station(unsigned int customer_num,unsigned int Q_size ,vector< vector<double> > &service_times)
        :Station(customer_num, Q_size, service_times){

    }

    static void initCalcSigs(unsigned int Qsize){
        calcSigs.resize(Qsize, false);
    }

    virtual ~Phased_Station();

    virtual double waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D * comma = NULL,int k = 0) = 0 ;

    virtual void UpdateUtilisation(int index, vector<double> &throughputs);

    // return the desired value from utilisation
    virtual double utilisation(unsigned int index,int e = 0,int k = 0);

    /* here need to be defined
    virtual double openWait(){
        return serviceTimes[0]/(1-utilisation);
    }*/

};
/*
class Phased_Station{
public:
    Phased_Station(unsigned int cN,unsigned int QSize ,unsigned int eN){

    }
};*/


#endif // PHASED_STATION_H
