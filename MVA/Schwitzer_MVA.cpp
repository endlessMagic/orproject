#include "MVA/Schwitzer_MVA.h"


void Schwitzer_MVA::Core(vector<int>& amounts){
    //here is initalisation

   int runNo = 0;
   int entry_size;

    double tolerance;
   int customer_num_sum = 0;
    for (unsigned int k = 1; k < amounts.size(); ++k){
        customer_num_sum += amounts[k];
    }
    tolerance = 1.0/(4000.0+16*customer_num_sum);
    //cout << "customer_num_sum:"<< customer_num_sum<<endl;
    //cout <<"tolerance:"<< tolerance << endl;
   int index_part;
    index_part = (customer_nums_.size()-1)*signal;
    do {//!!!!!!!!!!!!111
        ++runNo;
        max_diff_ = 0;
        for (unsigned int m = 0; m < M_; ++m){
            entry_size = stations_[m]->entries_num_;
            for (unsigned int e = 0; e < entry_size; ++e){
                for (unsigned int k = 1; k <=K_; ++k){
                    //fractions[m][e][k] = queue_lengths_[m][e][k][0]/amounts[k];

                    for (unsigned int j = 1; j <=K_; ++j){
                        if (k != j){
                            queue_lengths_[m][e][k][j+index_part] = queue_lengths_[m][e][k][signal]+dFractions[m][e][k][j]*amounts[k];
                        } else if (amounts[k] < 2) {
                            queue_lengths_[m][e][k][j+index_part] = 0;
                        } else {
                            queue_lengths_[m][e][k][j+index_part] = (amounts[k]-1)*(queue_lengths_[m][e][k][signal]/amounts[k]+dFractions[m][e][k][j]);
                        }
                    }
                }
            }
        }
        Step(amounts);
        //cout <<"max:dff:"<< max_diff_ <<endl;
    } while ((max_diff_ > tolerance) and (runNo < 1000000));


    cout << "runNo"<< runNo<<endl;/* */
}

void Schwitzer_MVA::Solve(){
    // initialise queue_lengths_
   int customer_num;
    customer_num = customer_nums_.size();
    queue_lengths_.resize(M_);
   int entry_size;
    for (unsigned int m = 0; m < M_; ++m){
        entry_size = stations_[m]->entries_num_;
        queue_lengths_[m].resize(entry_size);
        for(unsigned int e = 0; e <entry_size; ++e){
            queue_lengths_[m][e].resize(customer_num);
            for (unsigned int k = 0; k < customer_num; ++k){
                //0 is for the QueueLenghs, 1 is for the exQueueLenghs
                queue_lengths_[m][e][k].resize(customer_num);
            }
        }
    }

    InitPhaseVar(customer_num);

    //initialize queueLength
    //unsigned int entry_size;
    double lengthM;
    double lengthE;
   int visit_entry_num;
    for (unsigned int k = 1; k <=K_; ++k){
        cout<<"k:"<<k<<"customerSize:"<< customer_nums_[k] << endl;
        lengthM = double(customer_nums_[k])/M_;
        for (unsigned int m = 0; m < M_; ++m){
            entry_size = stations_[m]->entries_num_;
            visit_entry_num = 0;
            for (unsigned int e = 0; e < entry_size; ++e){
                if (stations_[m]->visits_[e][k]>0) {
                   ++visit_entry_num;
                }
            }
            //visit_entry_num = entry_size;//this line just for testing
            if (visit_entry_num == 0){
                lengthE = 0;
            } else {
                lengthE = lengthM/visit_entry_num;
            }
            for (unsigned int e = 0; e < entry_size; ++e){

                if (stations_[m]->visits_[e][k]>0) {
                   queue_lengths_[m][e][k][0] = lengthE;
                   //cout <<"m:"<<m<<"e:"<<e<<"k:"<<k<<"lengthE"<<lengthE <<endl;
                } else {
                   queue_lengths_[m][e][k][0] = 0;
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

int Schwitzer_MVA::queue_part_index(unsigned int k, vector<unsigned int>& amounts){
    return K_;
}

void Schwitzer_MVA::set_queue_lengths(unsigned int k, vector<unsigned int>& amounts){
    double sum;
   int entry_size;
    if ( with_traffic_dependency_ == false) {
        for (unsigned int m = 0; m < M_; m++){
            entry_size = stations_[m]->entries_num_;
            sum = 0;
            for (unsigned int e = 0; e < entry_size; ++e){
                newLength = throughputs_[k] * stations_[m]->visits_[e][k] * waiting_times_[m][e][k];
                //calculate the max_diff_
                lastDifference = fabs(queue_lengths_[m][e][k][0]-newLength) ;
                if (lastDifference > max_diff_) {
                    max_diff_ = lastDifference;
                }
                //set queueLength
                queue_lengths_[m][e][k][0] = newLength;
                //lastTime//cout<<"station:"<<m<<", classNo:"<<k<<endl;
                //lastTime//cout<<"throughput:["<< k <<"]:"<<throughputs_[k]<<endl;
                //lastTime//cout<<"e:"<<e<<"queue_lengths_:"<<queue_lengths_[m][e][k][0]<<endl;
                sum += queue_lengths_[m][e][k][0];
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
                lastDifference = fabs(queue_lengths_[m][e][k][0]-newLength) ;
                if (lastDifference > max_diff_) {
                    max_diff_ = lastDifference;
                }
                //set queueLength
                queue_lengths_[m][e][k][0] = newLength;
                //lastTime//cout<<"station:"<<m<<", classNo:"<<k<<endl;
                //lastTime//cout<<"throughput:["<< k <<"]:"<<throughputs_[k]<<endl;
                //lastTime//cout<<"e:"<<e<<"queue_lengths_:"<<queue_lengths_[m][e][k][0]<<endl;
                sum += queue_lengths_[m][e][k][0];
                //lastTime//cout<<""<<endl;
            }
            //lastTime//cout <<"°°°°°°°°°°°°"<<sum <<endl;
        }
    }
}
