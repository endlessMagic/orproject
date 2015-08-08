#ifndef SCHWITZER_MVA_H
#define SCHWITZER_MVA_H
#include "MVA.h"



class Schwitzer_MVA: public MVA{
public:
    Schwitzer_MVA(vector<int>& cs , vector<double>& ts, vector<Station*>& ss, bool ph = false, bool td = false, Vector3D * rd = NULL):MVA(cs, ts, ss, ph, td, rd),signal(0){
        max_diff_ = 0.0;
       int entrySize;
       int customer_num;
        customer_num = cs.size();
        // initialise fractions
        fractions.resize(M_);
        for (unsigned int m = 0; m < M_; ++m){
            entrySize = stations_[m]->entries_num_;
            fractions[m].resize(entrySize);
            for(unsigned int e = 0; e <entrySize; ++e){
                fractions[m][e].resize(customer_num);
            }
        }

        // initialise dFractions
        dFractions.resize(M_);
        for (unsigned int m = 0; m < M_; ++m){
            entrySize = stations_[m]->entries_num_;
            dFractions[m].resize(entrySize);
            for(unsigned int e = 0; e <entrySize; ++e){
                dFractions[m][e].resize(customer_num);
                for (unsigned int k = 0; k < customer_num; ++k){
                    //0 is for the QueueLenghs, 1 is for the exQueueLenghs
                    dFractions[m][e][k].resize(customer_num);
                }
            }
        }

        //  queue_lengths_ is initlaised in solve


    }

    virtual void Solve();


    // helper variables
    Vector4D dFractions;
    Vector3D fractions;

    double max_diff_;
    double lastDifference;
    double newLength;

    virtual int queue_part_index(unsigned int k, vector<int>& amounts);
    virtual void set_queue_lengths(unsigned int k, vector<int> &amounts);

    // helper methods
    void Core(vector<int> &amounts);

    // used to tell queue_part_index and set_queue_lengths which index to be determined
    // signal means which kind of customer has amount reduced by 1
   int signal;
private:    //void init();
};





#endif // SCHWITZER_MVA_H
