#include "MVA/Linearizer_MVA.h"


void Linearizer_MVA::Solve(){
    //initialization
   int entry_size;
    double lengthM;
    double lengthE;
   int visitEntryNo;
    for (unsigned int k = 1; k <=K_; ++k){
        cout<<"k:"<<k<<"customerSize:"<< customer_nums_[k] << endl;
        lengthM = double(customer_nums_[k])/M_;
        for (unsigned int m = 0; m < M_; ++m){
            //cout << "m:"<< m << endl;
            entry_size = stations_[m]->entries_num_;
            visitEntryNo = 0;
            for (unsigned int e = 0; e < entry_size; ++e){
                if (stations_[m]->visits_[e][k]>0) {
                   ++visitEntryNo;
                }
            }
            //visitEntryNo = entry_size;//this line just for testing
            if (visitEntryNo == 0){
                lengthE = 0;
            } else {
                lengthE = lengthM/visitEntryNo;
                for (unsigned int e = 0; e < entry_size; ++e){

                    if (stations_[m]->visits_[e][k]>0) {
                       queue_lengths_[m][e][k][0] = lengthE;
                    } /*else {
                       queue_lengths_[m][e][k][0] = 0;
                    }*/

                    for (unsigned int j = 1; j <= K_; ++j){
                        if (k != j) {
                            queue_lengths_[m][e][k][j] = queue_lengths_[m][e][k][0];
                        } else if (customer_nums_[j] < 2){
                            queue_lengths_[m][e][k][j] = 0;
                        } else {
                            if (stations_[m]->visits_[e][k]>0) {
                               queue_lengths_[m][e][k][j] = double(customer_nums_[k]-1)/(M_*visitEntryNo);
                               //cout <<queue_lengths_[m][e][k][j] <<endl;
                            } else {
                               queue_lengths_[m][e][k][j] = 0;
                            }
                        }
                    }
                }
            }
        }
    }


    for (unsigned int I = 1; I < 3 ; ++I){
        // Step 1
        Core(customer_nums_);
        // Step 2
        for (unsigned int c = 1; c <= K_; ++c){
            --customer_nums_[c];
            signal = c;
            Core(customer_nums_);
            ++customer_nums_[c];
            signal = 0;
        }
        // Step 3

        for (unsigned int m = 0; m < M_; ++m){
            entry_size = stations_[m]->entries_num_;
            for (unsigned int e = 0; e < entry_size; ++e){
                for (unsigned int k = 1; k <=K_; ++k){

                    fractions[m][e][k] = queue_lengths_[m][e][k][0]/customer_nums_[k];
                    for (unsigned int j = 1; j <=K_; ++j){
                        if (j != k){
                            dFractions[m][e][k][j] = queue_lengths_[m][e][k][j]/customer_nums_[k] - fractions[m][e][k];
                        } else if (customer_nums_[k] < 2){
                            dFractions[m][e][k][j] = - fractions[m][e][k];
                        } else {
                            // k == j
                            dFractions[m][e][k][j] = queue_lengths_[m][e][k][j]/(customer_nums_[k]-1) - fractions[m][e][k];
                        }

                    }

                }

            }

        }
    }

    Core(customer_nums_);


    //print results
    for (unsigned int m = 0; m < M_; ++m){
        entry_size = stations_[m]->entries_num_;
        for (unsigned int e = 0; e < entry_size; ++e){
            for (unsigned int k = 1; k <=K_; ++k){
                cout<<"m:"<<m<<"e:"<<e<<"k:"<<k<<"queueLength:"<<queue_lengths_[m][e][k][0]<<endl;
            }
        }
    }
}


int Linearizer_MVA::queue_part_index(unsigned int k, vector<unsigned int>& amounts){
    if (signal == 0){
        return k;
    } else {
        return k + (customer_nums_.size()-1)*signal;
    }
}

void Linearizer_MVA::set_queue_lengths(unsigned int k, vector<int>& amounts){
    double sum;
   int entry_size;
    if (with_traffic_dependency_ == false){
        for (unsigned int m = 0; m < M_; m++){
            entry_size = stations_[m]->entries_num_;
            sum = 0;
            for (unsigned int e = 0; e < entry_size; ++e){
                newLength = throughputs_[k] * stations_[m]->visits_[e][k] * waiting_times_[m][e][k];
                //calculate the max_diff_
                lastDifference = fabs(queue_lengths_[m][e][k][signal]-newLength) ;
                if (lastDifference > max_diff_) {
                    max_diff_ = lastDifference;
                }
                //set queueLength
                queue_lengths_[m][e][k][signal] = newLength;

                //lastTime//cout<<"station:"<<m<<", classNo:"<<k<<endl;
                //lastTime//cout<<"throughput:["<< k <<"]:"<<throughputs_[k]<<endl;
                //lastTime//cout<<"e:"<<e<<"queue_lengths_:"<<queue_lengths_[m][e][k][0]<<endl;
                sum += queue_lengths_[m][e][k][signal];
                //lastTime//cout<<""<<endl;
            }
            //lastTime//cout <<"°°°°°°°°°°°°"<<sum <<endl;
        }
    } else {
        for (unsigned int m = 0; m < M_; m++){
            entry_size = stations_[m]->entries_num_;
            sum = 0;
            for (unsigned int e = 0; e < entry_size; ++e){
                newLength = (throughputs_[k]*stations_[m]->visits_[e][k] - (*rate_declines_)[m][e][k]) *
                        stations_[m]->visits_[e][k] * waiting_times_[m][e][k];
                //calculate the max_diff_
                lastDifference = fabs(queue_lengths_[m][e][k][signal]-newLength) ;
                if (lastDifference > max_diff_) {
                    max_diff_ = lastDifference;
                }
                //set queueLength
                queue_lengths_[m][e][k][signal] = newLength;

                //lastTime//cout<<"station:"<<m<<", classNo:"<<k<<endl;
                //lastTime//cout<<"throughput:["<< k <<"]:"<<throughputs_[k]<<endl;
                //lastTime//cout<<"e:"<<e<<"queue_lengths_:"<<queue_lengths_[m][e][k][0]<<endl;
                sum += queue_lengths_[m][e][k][signal];
                //lastTime//cout<<""<<endl;
            }
            //lastTime//cout <<"°°°°°°°°°°°°"<<sum <<endl;
        }
    }
}
