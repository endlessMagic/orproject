#ifndef LINEARIZER_MVA_H
#define LINEARIZER_MVA_H
#include "Schwitzer_MVA.h"

class Linearizer_MVA: public Schwitzer_MVA{
public:
    Linearizer_MVA(vector<int>& customer_nums , vector<double>& think_times, vector<Station*>& sations,
                   bool with2phases = false, bool with_traffic_dependency = false, Vector3D * rate_declines = NULL)
        :Schwitzer_MVA(customer_nums, think_times, sations, with2phases, with_traffic_dependency, rate_declines){
        // initialise queue_lengths_
       int customer_num;
       int indexSize;
        customer_num = customer_nums.size();
        indexSize = (customer_num-1)*customer_num+1;
        queue_lengths_.resize(M_);
       int entry_size;
        for (unsigned int m = 0; m < M_; ++m){
            entry_size = stations_[m]->entries_num_;
            queue_lengths_[m].resize(entry_size);
            for(unsigned int e = 0; e <entry_size; ++e){
                queue_lengths_[m][e].resize(customer_num);
                for (unsigned int k = 0; k < customer_num; ++k){
                    //0 is for the QueueLenghs, 1 is for the exQueueLenghs
                    queue_lengths_[m][e][k].resize(indexSize);
                }
            }
        }

    InitPhaseVar(indexSize);

    }

    virtual void Solve();

    virtual int queue_part_index(unsigned int k, vector<int>& amounts);
    virtual void set_queue_lengths(unsigned int k, vector<int> &amounts);

};

#endif // LINEARIZER_MVA_H
