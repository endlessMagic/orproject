#include "Station/PS_Station.h"

double PS_Station::waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths,int part_index, Vector4D *comma,int k){
    double T = 0.0;
    for (unsigned int j = 1; j < customer_num_; j++){
        for (unsigned int e = 0; e < entries_num_; ++e){
            if (visits_[e][j]>0) {
                T += queue_lengths[m][e][j][part_index];
                //lastTime//cout<<"Qlength"<<queue_lengths_[m][e][j][part_index]<<endl;
                ///lastTime//cout<<"+T"<<queue_lengths_[j][part_index]*serviceTimes[e][0]<<endl;
            }
        }
    }


    ///lastTime//cout<<"waitTime"<<(serviceTimes[k]+T)<<endl;
    //lastTime//cout<<"serviceTimes [k:xx;e:"<<e<<"]"<< serviceTimes[e][0]*(1+T)<<endl;
    return service_times_[e][0]*(1+T);
}
