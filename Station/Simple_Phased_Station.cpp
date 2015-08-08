#include "Station/Simple_Phased_Station.h"

double Simple_Phased_Station::waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D *comma,int k){
    double temp = 0.0;
    for(unsigned int j = 1; j < customer_num_; ++j){
        for (unsigned int e = 0; e < entries_num_; ++e){
            if (visits_[e][j]>0) {
                temp += (queue_lengths_[m][e][j][partIndex] * (service_times_[e][0] + service_times_[e][1]));

                temp += service_times_[e][1] * (*comma)[m][e][k][partIndex];

                temp += ((1- (*comma)[m][e][k][partIndex]) * (service_times_[e][0] + service_times_[e][2] * service_times_[e][2]/(service_times_[e][0] * service_times_[e][1]))
                        * utilisation_[e][k][partIndex]);
            }
        }
    }

    return service_times_[e][0] + temp ;
}


