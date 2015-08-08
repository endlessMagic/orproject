#include "Station/Markov_Phased_Station.h"

// in the p144 of the literature 20
double Markov_Phased_Station::waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int part_index, Vector4D *comma,int k){
    double temp = 0.0;
    for(unsigned int j = 1; j < customer_num_; ++j){
        for (unsigned int e = 0; e < entries_num_; ++e){
            if (visits_[e][j]>0) {
                temp += (queue_lengths_[m][e][j][part_index] * (service_times_[e][0] + service_times_[e][1]));

                temp += service_times_[e][1] * (*comma)[m][e][k][part_index];

                temp += ((1- (*comma)[m][e][k][part_index]) * (service_times_[e][0] + service_times_[e][2] * service_times_[e][2]/(service_times_[e][0] * service_times_[e][1]))
                        * utilisation_[e][k][part_index]);
            }
        }
    }

    return service_times_[e][0] + temp ;
}
