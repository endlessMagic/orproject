#include "MVA/Exact_MVA.h"


void Exact_MVA::Loop(int class_id){
    if (class_id > 1) {
        for (population_[class_id] = 0;  population_[class_id] <= customer_nums_[class_id];  population_[class_id]++){
            //
            Loop(class_id-1);
        }
    } else if (class_id == 1) {
        for (population_[class_id] = 0;  population_[class_id] <= customer_nums_[class_id];  population_[class_id]++){
            //
            Step(population_);
       }
    }
}


int Exact_MVA::queue_part_index(unsigned int k, vector<int>& amounts){
   int part_index = 0;
    if (amounts[k] > 0) {
        --amounts[k];
        part_index = Station::partIndexCalc(amounts);
        ++amounts[k];
    } else {
        part_index =  Station::partIndexCalc(amounts);
    }
    return part_index;
}


void Exact_MVA::set_queue_lengths(unsigned int k, vector<int>& amounts){
   int part_index;
    // to indicate that this index is for a population_ of amounts
    part_index = Station::partIndexCalc(amounts);
    double sum;
   int entry_size;
    if ( with_traffic_dependency_ == false) {
        for (unsigned int m = 0; m < M_; m++){
            entry_size = stations_[m]->entries_num_;
            sum = 0;
            for (unsigned int e = 0; e < entry_size; ++e){
                queue_lengths_[m][e][k][part_index] = throughputs_[k]* stations_[m]->visits_[e][k]* waiting_times_[m][e][k];
                //lastTime//cout<<"station:"<<m<<", classNo:"<<k<<endl;
                //lastTime//cout<<"throughput:["<< k <<"]:"<<throughputs_[k]<<endl;
                //lastTime//cout<<"e:"<<e<<"queue_lengths_:"<<queue_lengths_[m][e][k][part_index]<<endl;
                sum += queue_lengths_[m][e][k][part_index];
                //lastTime//cout<<""<<endl;
            }
            //cout <<"°°°°°°°°°°°°"<<sum <<endl;
            if (sum < 0){
                cout<<"!!!!!!!!!"<<sum<<endl;
            }
        }
    } else {
        for (unsigned int m = 0; m < M_; m++){
            entry_size = stations_[m]->entries_num_;
            sum = 0;
            for (unsigned int e = 0; e < entry_size; ++e){
                // queue_lengths_[m][e][k][part_index] = (throughputs_[k]*stations_[m]->visits[e][k] - (*rateDeclines)[m][e][k]) *
                //        stations_[m]->visits[e][k] * waiting_times_[m][e][k];
                queue_lengths_[m][e][k][part_index] = (throughputs_[k]*stations_[m]->visits_[e][k] - (*rate_declines_)[m][e][k]) *
                                        stations_[m]->visits_[e][k] * waiting_times_[m][e][k];
                //lastTime//cout<<"station:"<<m<<", classNo:"<<k<<endl;
                //lastTime//cout<<"throughput:["<< k <<"]:"<<throughputs_[k]<<endl;
                //lastTime//cout<<"e:"<<e<<"queue_lengths_:"<<queue_lengths_[m][e][k][part_index]<<endl;
                sum += queue_lengths_[m][e][k][part_index];
                //lastTime//cout<<""<<endl;
            }
            //cout <<"°°°°°°°°°°°°"<<sum <<endl;
            if (sum < 0){
                cout<<"!!!!!!!!!"<<sum<<endl;
            }
        }
    }
}
