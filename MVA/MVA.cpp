#include "MVA.h"



void MVA::Init(){
    // for open and closed classes
    waiting_times_.resize(M_);
    //R.resize(M);
   int entry_size;
    for (unsigned int m = 0; m < M_; ++m){
        entry_size = stations_[m]->entries_num_;
        waiting_times_[m].resize(entry_size);
        //R[m].resize(entry_size);
        for(unsigned int e = 0; e <entry_size; ++e){
            waiting_times_[m][e].resize(customer_nums_.size());
            //R[m][e].resize(customerSizes.size());
        }
    }
    throughputs_.resize(customer_nums_.size(),0);
    Station::initIndexHelper(customer_nums_);
}


// population as input
/// checked 1206, but the visits_ should be taken with caucious
void MVA::Step(vector<int>& amounts){
   int part_index = 0;
   int entries_num = 0;
    /// test part begin 11.06
    //lastTime//cout<<endl<<"PopuSize:------------------------------->";
    for (unsigned int i= 0; i < amounts.size();++i){
        //lastTime//cout << i << ":"<< amounts[i]<< " ; ";
    }
    //lastTime//cout<<endl;
    /// test part end

    for (unsigned int m = 0; m < stations_.size(); m++){
        entries_num = stations_[m]->entries_num_;
        for (unsigned int k = 1; k <= K_; ++k){
            for (unsigned int e = 0; e < entries_num; ++e){
                //lastTime//cout<<"++++m:"<<m<<"k:"<<k<<"e:"<<e<<"visits:"<<stations_[m]->visits_[e][k]<<endl;
                waiting_times_[m][e][k] = stations_[m]->waitTimeCalc(m, e, queue_lengths_, queue_part_index(k, amounts), &overtaking_prob_, k);
                //lastTime//cout<<"m:"<<m<<"e:"<<e<<"k:"<<k<<"waiting_times_:"<< waiting_times_[m][e][k] <<endl;
            }

        }
    }
    //lastTime//cout<<"finish"<<endl;
    double denominator;

    for (unsigned int k = 1; k <= K_; k++){

        if (amounts[k] != 0) {
            denominator = 0;
            for (unsigned int m = 0; m < M_; m++){
                entries_num = stations_[m]->entries_num_;
                for (unsigned int e = 0; e < entries_num; ++e){
                    denominator += stations_[m]->visits_[e][k]*waiting_times_[m][e][k];
                    //lastTime//cout<<"waiting_times_(m,e,k)["<< m<< ","<<e<< "," <<k<<"]:"<<waiting_times_[m][e][k]<<endl;
                }
            }
            ///lastTime//cout<<"amounts["<< k <<"]:"<<amounts[k]<<endl;
            throughputs_[k] = amounts[k]/(think_times_[k]+denominator);
            //lastTime//cout<<"denominator"<<(thinkTimes[k]+denominator)<<endl;
            //lastTime//cout<<"throughputs_["<< k <<"]:"<<throughputs_[k]<<endl;


            set_queue_lengths(k, amounts);

        } else {
            throughputs_[k] = 0;
        }
    }


    for (unsigned int m= 0; m < M_; m++){
        //calculation the utilisation
        part_index = Station::partIndexCalc(amounts);
        stations_[m]->UpdateUtilisation(part_index, throughputs_);
    }

    if (with2phases_ == true){
        UpdateOvertakingProb(amounts);
    }

}

void MVA::InitPhaseVar(unsigned int Qsize){
    if (with2phases_ == true){
        overtaking_prob_ = queue_lengths_;

        Rk_.resize(K_+1);
        for(unsigned int k = 1; k <= K_; ++k){
            Rk_[k].resize(Qsize);
        }

        Rmk_.resize(M_);
        for(unsigned int m = 0; m < M_; ++m){
            Rmk_[m].resize(K_+1);
            for(unsigned int k = 1; k <= K_; ++k){
                Rmk_[m][k].resize(Qsize);
            }
        }
    }
}


void MVA::UpdateOvertakingProb(vector<int>& amounts){
   int entry_size;
   int index;
    //calculate the R_{mk}
    for(unsigned int k = 1; k <= K_; ++k){
        index = queue_part_index(k, amounts);
            for(unsigned int m = 0; m < M_; ++m){
            entry_size = stations_[m]->entries_num_;
            for(unsigned int e = 0; e < entry_size; ++e){
                Rmk_[m][k][index] += stations_[m]->visits_[e][k]* waiting_times_[m][e][k];
            }
        }
    }

    //calculate the R_{k}
    for(unsigned int k = 1; k <= K_; ++k){
        index = queue_part_index(k, amounts);
        for(unsigned int m = 0; m < M_; ++m){
            Rk_[k][index] += Rmk_[m][k][index];
        }
    }

    //calculate the comma
    double temp;
    for(unsigned int k = 1; k <= K_; ++k){
        index = queue_part_index(k, amounts);
        for(unsigned int m = 0; m < M_; ++m){
            entry_size = stations_[m]->entries_num_;
            for(unsigned int e = 0; e < entry_size; ++e){
                temp = stations_[m]->visits_[e][k] * stations_[m]->service_times_[e][1];
                overtaking_prob_[m][e][k][index] = temp/
                        (temp + think_times_[k] + Rk_[k][index] - Rmk_[m][k][index]);
            }
        }
    }
}
