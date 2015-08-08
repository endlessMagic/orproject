#include "STATION.h"

vector<unsigned int> Station::index_helper_;


unsigned int getQsize(vector<int> & cs){
    int Qsize=1;
    for (unsigned int i = 1; i < cs.size(); i++){
        Qsize *=  (cs[i]+1);
    }
    return Qsize;
}

int Station:: partIndexCalc(vector<int> &population){
    int index = 0;
    for (unsigned int i = 1; i < population.size(); i++){
        index += population[i]* Station::index_helper_[i];
    }
    ///lastTime//cout<<"the index"<<index<<endl;
    return index;


}


void Station:: UpdateUtilisation(int index, vector<double> &throughputs){
    double uValue= 0.0;
    for (unsigned int k = 1; k < throughputs.size(); ++k){
        for (unsigned int e = 0; e < entries_num_; e++){
           uValue +=  throughputs[k]* visits_[e][k]* service_times_[e][0];
        }
    }
    utilisation_[index] = uValue;
}

double Station:: utilisation(unsigned int index,int e,int k){
    return utilisation_[index];
}


/*
double Station::mixedWait(){
    double L;
    for (int k = 1; k < serviceTimes.size(); k ++){
        L += queue_lengths_[(Station::index_helper_).back()*k];
    }
    return openWait()*(1 + L);
}*/




/*
int inVector(vector<unsigned int>& lt, int a){
    bool find = 0, position = 0;
    for (vector<unsigned int>::iterator it = lt.begin(); (it != lt.end()) && (find == 0); it++){
        position ++;
        if (*it == a)
            find = position;
    }
    return find;
}*/
