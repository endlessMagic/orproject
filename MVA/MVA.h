#ifndef MVA_H
#define MVA_H

#include <math.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include "Station/STATION.h"

using namespace::std;

enum {
    kExact = 0,
    kLinearizer = 1,
    kSchwitzer = 2
};



class MVA{
public:
    /**
     * @brief MVA
     * @param customer_nums K+1
     * @param think_times K+1
     * @param sations M
     * @param with2phases
     * @param with_traffic_dependency
     * @param rate_declines
     */
    MVA(vector<int>& customer_nums , vector<double>& think_times, vector<Station*>& sations,
        bool with2phases = false, bool with_traffic_dependency = false, Vector3D * rate_declines = NULL):
        customer_nums_(customer_nums), think_times_(think_times), stations_(sations),
        K_(customer_nums.size()-1),M_(sations.size()), with2phases_(with2phases), with_traffic_dependency_(with_traffic_dependency){
        if (with_traffic_dependency_ == true){
            rate_declines_ = rate_declines;
        }
        Init();
    }



    ~MVA(){

        for(unsigned int i =0; i < stations_.size(); i++){
            stations_[i] = NULL;
            //delete stations[i];
        }
    }
    
    // for traffic dependency
    bool  with_traffic_dependency_ ;
    Vector3D * rate_declines_;

    // methods
    // virtual method to solve a MVA problem without open network
    virtual void Solve() = 0;
    // method to solve a MVA problem with mixed network(closed and open)
    void SolveMixedNet();

    // size K+1
    vector<double> throughputs_;
    // to show if 2 phased method is used
    bool with2phases_;
    // to store the overtaking probilites
    Vector4D overtaking_prob_; // comma in old version

    //
    virtual int queue_part_index(unsigned int k, vector<int>& amounts) = 0;
    virtual void set_queue_lengths(unsigned int k, vector<int>& amounts) = 0;



    // inputs
    // size K+1, each int element stands for the amount of customerSizes in each chain
    vector<int>  customer_nums_;
    // size (K+1, temporary), think time is the time that the server do nothing
    vector<double> think_times_;
    // size M, a station contains information about its mean service times (for each chain?), visits to other stations and waiting time calculation
    vector<Station*> stations_;

    //outputs, K is the number of closed classes
    // waiting_times_ is a vector of waiting time with size M*E0*(K+1), waiting time is the sum of a customer's service time and its time spent in the waiting line
    Vector3D waiting_times_;
    Vector4D queue_lengths_;
    // used to save interim results
    Vector2D Rk_;
    Vector3D Rmk_;

    //initilise the variables related to phase method
    void InitPhaseVar(unsigned int Qsize);

    // helper variables

    // helper methods
    // save results, used in solve() and solveMixedNet()
    //void save(vector<Node> & nodes, vector<int> & clients, vector<int>& servers);


    // used in loop
    void Step(vector<int> &amounts);
    // used in the constructor, resize the output variables(throughputs_, waiting_times_) and the variable population
    void Init();
    // updating the overtaking probilities
    void UpdateOvertakingProb(vector<int>& amounts);

    // K is the number of closed classes
   int K_;
    // M is the number of the stations
   int M_;





protected:





private:
};






#endif // MVA_H
