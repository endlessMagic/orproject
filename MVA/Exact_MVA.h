#ifndef EXACT_MVA_H
#define EXACT_MVA_H
#include "MVA.h"



class Exact_MVA: public MVA{
public:
    //initalisation
    Exact_MVA(vector<int>& customer_nums , vector<double>& think_times, vector<Station*>& sations,
               bool with2phases = false, bool with_traffic_dependency = false, Vector3D * rate_declines = NULL)
            :MVA(customer_nums, think_times, sations, with2phases, with_traffic_dependency, rate_declines){
       int Q_size;
        Q_size = getQsize(customer_nums);
        queue_lengths_.resize(M_);
       int entry_size;
       int customer_num;
        customer_num = customer_nums.size();
        for (unsigned int m = 0; m < M_; ++m){
            entry_size = stations_[m]->entries_num_;
            queue_lengths_[m].resize(entry_size);
            for(unsigned int e = 0; e <entry_size; ++e){
                queue_lengths_[m][e].resize(customer_num);
                for (unsigned int k = 0; k < customer_num; ++k){
                    queue_lengths_[m][e][k].resize(Q_size);
                }
            }
        }
        
        InitPhaseVar(Q_size);

        population_.resize(customer_nums.size(), 0);
    }

    // methods
    // method to solve a MVA problem without open network
    /// checked 1206
    virtual void Solve(){
        Loop(K_);
    }

    // used in the solve method as the main process
    void Loop(int class_id);

    virtual int queue_part_index(unsigned int k, vector<int> &amounts);
    virtual void set_queue_lengths(unsigned int k, vector<int> &amounts);
    //void init();

    // size K+1, describing the present amount of customerSizes in each chain of the network
    vector<int> population_;
};






#endif // EXACT_MVA_H
