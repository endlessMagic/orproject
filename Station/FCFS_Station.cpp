#include "Station/FCFS_Station.h"



// in the p98 of the literature 20
double FCFS_Station::waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths_,int partIndex, Vector4D *comma,int k){
    double T = 0.0;
    for (unsigned int j = 1; j < customer_num_; j++){
        for (unsigned int e = 0; e < entries_num_; ++e){
            if (visits_[e][j]>0) {
                T += queue_lengths_[m][e][j][partIndex];
                //lastTime//cout<<"Qlength"<<queue_lengths_[m][e][j][partIndex]<<endl;
                ///lastTime//cout<<"+T"<<queue_lengths_[j][partIndex]*serviceTimes[e][0]<<endl;
            }
        }
    }


    ///lastTime//cout<<"waitTime"<<(serviceTimes[k]+T)<<endl;
    //lastTime//cout<<"serviceTimes [k:xx;e:"<<e<<"]"<< serviceTimes[e][0]*(1+T)<<endl;
    return service_times_[e][0]*(1+T);
}

