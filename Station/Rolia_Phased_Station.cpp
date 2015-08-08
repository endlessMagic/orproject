#include "Station/Rolia_Phased_Station.h"

double Rolia_Phased_Station::getOverTakingProb(){

}


double Rolia_Phased_Station::waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths,int part_index, Vector4D *comma,int k){
    double temp = 0.0;
    for(unsigned int j = 1; j < customer_num_; ++j){
        for (unsigned int e = 0; e < entries_num_; ++e){
            if (visits_[e][j]>0) {
                temp += (queue_lengths[m][e][j][part_index]* (service_times_[e][0] + service_times_[e][1]));
            }
        }
    }

    return service_times_[e][0] + temp + ((*comma)[m][e][k][part_index] ) * (service_times_[e][1]);
}
