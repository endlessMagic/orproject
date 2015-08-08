#include "Station/Rolia_MultiStation.h"



//s.170. Rolia_Multi_Server
double Rolia_MultiStation::waitTimeCalc(unsigned int m,int e, Vector4D & queue_lengths,int part_index, Vector4D *comma,int k){
    double T = 0.0;
    for (unsigned int j = 1; j < customer_num_; j++){
        for (unsigned int e = 0; e < entries_num_; ++e){
            if (visits_[e][j]>0) {
                T += queue_lengths[m][e][j][part_index]*service_times_[e][0];
                //lastTime//cout<<"Qlength"<<queue_lengths_[m][e][j][part_index]<<endl;
                ///lastTime//cout<<"+T"<<queue_lengths_[j][part_index]*serviceTimes[e][0]<<endl;
            }
        }
    }
    ///lastTime//cout<<"hahahahahahahahahahahahahahahahahahahahahahahahahahahahahahahahah"<<endl;
    //s.170. Rolia_Multi_Server
    T*= (pow(utilisation_[part_index]/amount,amount)/amount);


    ///lastTime//cout<<"waitTime"<<(serviceTimes[k]+T)<<endl;
    //lastTime//cout<<"serviceTimes [k:xxx;e:"<<e<<"]"<< serviceTimes[e][0]+T<<endl;

    return service_times_[e][0]+T;
}

/*


double Rolia_MultiStation::pb(vector<unsigned int>& population, int k){
    double rou = 0.0;
    int sig = 0;
    if (population[k] > 0) {
        population[k]--;
        sig = 1;
    }
    for (int j = 1; j < serviceTimes.size(); j++){
        rou += queue_lengths_[part_indexCalc(population)* j]/serviceTimes[j]/serviceTimes[j];
    }
    if(sig == 1){
        population[k]++;
    }
    rou /= (serverAmount*serverAmount);\
        return pow(rou, serverAmount)/serverAmount;
}*/

