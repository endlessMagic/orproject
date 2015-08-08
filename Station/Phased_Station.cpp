#include "Station/Phased_Station.h"


Phased_Station::~Phased_Station(){
}


void Phased_Station:: UpdateUtilisation(int index, vector<double> &throughputs){
    double uValue= 0.0;
    for (unsigned int k = 1; k < throughputs.size(); ++k){
        for (unsigned int e = 0; e < entries_num_; e++){
            utilisation_[e][k][index] = throughputs[k]* visits_[e][k]* service_times_[e][0];
            uValue += utilisation_[e][k][index];
        }
    }
    utilisation_[0][0][index]= uValue;
}

double Phased_Station:: utilisation(unsigned int index,int e,int k){
    return utilisation_[e][k][index];
}
