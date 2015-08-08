#ifndef SVNSOLVER_H
#define SVNSOLVER_H


#include <iostream>


using namespace std;

//class Measure;

// constructing priority queues
#include <iostream>       // std::cout
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <list>           // std::list
#include <deque>          // std::deque
#include <math.h>
// google style


template<typename T>
void PrintVector(vector<T> &v){
    for(int i = 0; i < v.size(); ++i){
        cout<<v[i]<<endl;
    }
}

template<typename T>
bool in_vector(vector<T> &v, T &a){
    for(int i = 0; i < v.size(); ++i){
        if(v[i] == a){
            return true;
        }
    }
    return false;
}

//template<typename T>
//T max(T a, T b){
//    return (a > b)?a:b;
//}

//template<typename T>
//T min(T a, T b){
//    return (a > b)?b:a;
//}



class Entry;
class Task;

class Connection{
public:
    Connection(Entry *target, double num):target_(target), num_(num){

    }

    Entry *target_; // id of the target entry
    double num_;
};


typedef deque<Connection*> Connections;

// p 58
enum TaskType {kRefTask, kActiveTask, kProcessor};



class Phase{
public:
    Phase(double service_time):service_time_(service_time),connections_(0){
        entry_ = NULL;
    }

    Phase(double service_time, Entry *entry):service_time_(service_time),connections_(0){
        entry_ = entry;
    }

    double service_time(){
        return service_time_;
    }

    void set_service_time(double service_time){
        service_time_ = service_time;
    }



    Connections connections_;

    void AddConnectionBack(Connection *connection){
        connections_.push_back(connection);

    }

    void AddConnectionFront(Connection *connection){
        connections_.push_front(connection);
    }

    bool hasVisit(Task *task);

    Entry *entry_;

    double wait_time(){
        return wait_time_;
    }

    void set_wait_time(double wait_time){
        wait_time_ = wait_time;
    }

private:
    double service_time_;
    double wait_time_;
};

class Task;

class Entry{
public:
    Entry(int id):id_(id),visit_(0.),index_(-1),lambda_(0.){

    }

    void AddPhase(double service_time){
        phases_.push_back(new Phase(service_time, this));

    }

    void AddPhase(Phase *phase){
        phases_.push_back(phase);
        phase->entry_ = this;
    }

    void AddConnectionFront(Connection *connection, int phase_no){
        if(phase_no >= phases_.size()){
            cout<<"phase_no is larger than size of phases by Adding a connection"<<endl;
        } else {
            phases_[phase_no]->AddConnectionFront(connection);
        }
    }

    void AddConnectionBack(Connection *connection, int phase_no){
        if(phase_no >= phases_.size()){
            cout<<"phase_no is larger than size of phases by Adding a connection"<<endl;
        } else {
            phases_[phase_no]->AddConnectionBack(connection);
        }
    }

    bool hasVisit(Task *task){
        if(phases_.size() == 0){
            return false;
        } else {
            Phase *phase;
            for(int i = 0; i < phases_.size();++i){
                phase = phases_[i];
                if(phase->hasVisit(task)==true){
                    return true;
                }
            }
            return false;
        }
    }

    double visit(){
        return visit_;
    }

    void set_visit(double visit){
        visit_ = visit;
    }

    void AddVisit(double add){
        visit_ += add;
    }

    vector<Phase*> phases_;
    Task *task_;
    int id_;
    double think_time_;

    double wait_time(){
        return wait_time_;
    }

    void set_wait_time(double wait_time){
        wait_time_ = wait_time;
    }

    int index(){
        return index_;
    }

    void set_index(int index){
        index_ = index;
    }

    double lambda(){
        return lambda_;
    }

    void set_lambda(double lambda){
        lambda_ = lambda;
    }

    void AddLambda(double add){
        lambda_ += add;
    }

    double rho(){
        return lambda() * wait_time();
    }

protected:
    double wait_time_;
    double visit_;
    int index_;
    double lambda_;
};

enum Color{kGrey, kWhite, kBlack};

class Visit{
public:
    Visit(){

    }

    Visit(Task *task, double num){
        task_ = task;
        num_ = num;
    }

    Task* task(){
        return task_;
    }
    void set_task(Task *task){
        task_ = task;
    }

    double num(){
        return num_;
    }
    void set_num(double num){
        num_ = num;
    }

private:
    Task *task_;
    double num_;
};

int in_visits(vector<Visit *> &vs, Task *task);

class Task{
public:
    Task(TaskType type, int id, int M)
        :type_(type),id_(id),M_(M),depth_(0), color_(kWhite),server_index_(-1){
        processor_ = NULL;
        customer_num_ = M_;
        server_visits.resize(0);
        client_visits.resize(0);
    }

    Task(TaskType type, int id, int M, double service_time, int entry_id)
        :type_(type),id_(id),M_(M),depth_(0), color_(kWhite),server_index_(-1){
        processor_ = NULL;
        customer_num_ = M_;
        Entry *entry = new Entry(entry_id);
        Phase *phase = new Phase(service_time);
        entry->AddPhase(phase);
        this->AddEntry(entry);
        server_visits.resize(0);
        client_visits.resize(0);
    }

    ~Task(){
    }

    void AddEntry(Entry *entry){
        entry->task_ = this;
        entry->set_index(entries_.size());
        entries_.push_back(entry);        
    }

    int M(){
        return M_;
    }

    TaskType type(){
        return type_;
    }

    int M_;


    vector<Entry*> entries_;
    int id_;


    Task *processor_;



    int depth(){
        return depth_;
    }

    void set_depth(int depth){
        depth_ = depth;
    }

    Color color(){
        return color_;
    }

    void set_color(Color color){
        color_ = color;
    }

    bool hasVisit(Task *task){
        if(entries_.size()==0){
            return false;
        } else {
            Entry *entry;
            for(int i = 0; i < entries_.size(); ++i){
                entry = entries_[i];
                if(entry->hasVisit(task) == true){
                    return true;
                }
            }
            return false;
        }
    }

    vector<Visit*> server_visits;
    vector<Visit*> client_visits;

    int server_index(){
        return server_index_;
    }

    void set_server_index(int server_index){
        server_index_ = server_index;
    }


    // calculated from its entries
    double lambda(){
        double res = 0.;
        Entry *entry;
        for (int i = 0; i < entries_.size(); ++i) {
            entry = entries_[i];
            res += entry->lambda();
        }
        return res;
    }


//    double think_time(){
//        return think_time_;
//    }

    /**
     * @brief think_time
     * @param customer_num
     * @return think_time under customer_num;
     */
    double think_time(){
        if(type_ == kRefTask){
            return think_time_;
        } else {
            return customer_num_*(1-rho())/lambda();
        }
    }

    void set_think_time(double think_time){
        think_time_ = think_time;
    }

    // utilisation as server, for kRefTask it is rho_ = wait_time_(first phase)/(wait_time_+think_time_)
    double rho() {
        double res = 0.;
        Entry *entry;
        for (int i = 0; i < entries_.size(); ++i) {
            entry = entries_[i];
            res += entry->rho();
        }
        return res;
    }

int customer_num(){
    return customer_num_;
}

void set_customer_num(int customer_num){
    customer_num_ = customer_num;
}


protected:
    int depth_;
    Color color_;
    TaskType type_;
    int server_index_; // the position index that this task takes as a server in a submodel(it is unique since the task only serves as server in a submodel)
    double think_time_;
    int customer_num_;
};


// ----------------------------------------------------------------------------
// Ueberliest Leerzeichen (' ' und '\t') und prueft dann, ob als
// naechstes ein '#' folgt. In dem Fall wird aller Text bis zur
// naechsten Zeile ignoriert, d.h. es wird nach dem naechsten '\n'
// gesucht. Dies geschieht, bis weder ein Leer- noch ein
// Kommentarzeichen gefunden wurde.
// ----------------------------------------------------------------------------
std::istream& SkipComment(std::istream& is);

class Submodel{
public:
    Submodel(){
        clients_.resize(0);
        servers_.resize(0);
        depth_ = 0;
    }

    Submodel(int depth){
        clients_.resize(0);
        servers_.resize(0);
        depth_ = depth;
    }

    vector<Task*> clients_, servers_;

    int depth(){
        return depth_;
    }

    void set_depth(int depth){
        depth_ = depth;
    }

    bool is_client(Task *task){
        for(int i = 0; i < clients_.size(); ++i){
            if( task == clients_[i]){
                return true;
            }
        }
        return false;
    }

    bool is_server(Task *task){
        for(int i = 0; i < servers_.size(); ++i){
            if( task == servers_[i]){
                return true;
            }
        }
        return false;
    }

private:
    int depth_;
};


#include <fstream>
#include <limits>
#include <unordered_map>
#include "MVA/Exact_MVA.h"
#include "MVA/Linearizer_MVA.h"
#include "MVA/Schwitzer_MVA.h"

#include "Station/Delay_Station.h"
#include "Station/FCFS_Station.h"
#include "Station/PS_Station.h"
#include "Station/Rolia_MultiStation.h"

class SRNSolver{
public:
    SRNSolver(string def_file): finished_(false),depth_(0){
        entries_.resize(1);
        tasks_.resize(1);
        Read(def_file);
        ModelTransform();
        Layerize();


    }

    void Layerize(){
        TopoSort();
        //PrintTaskDepth(); // topo is right
        CreateSubmodels();
        InitSubModels();
        //PrintAllSubModelsInfo(); // is right
        CreateMVASubmodels();
    }
    /**
     * @brief CreateSubmodels
     *
     */
    void CreateSubmodels();

    void PrintSubModelInfo(Submodel *submodel);

    void PrintAllSubModelsInfo(){
        for(int i = 1; i < submodels_.size(); ++i){
            PrintSubModelInfo(submodels_[i]);
        }
    }

    void PrintTaskServiceTime(Task *task){
        cout<<"Task "<<task->id_<<endl;
        for(int i = 0; i < task->entries_.size(); ++i){
            cout<<" Entry "<<i+1<<"  wait_t "<<task->entries_[i]->wait_time() <<endl;
            PrintEntryServiceTime(task->entries_[i]);
        }
    }

    void PrintEntryServiceTime(Entry *entry){
        for(int i = 0; i < entry->phases_.size(); ++i){
            cout<<"  Phase "<<i+1<<endl;
            cout<<"   "<<entry->phases_[i]->service_time()<<"  "<<entry->phases_[i]->wait_time()<<endl;
        }
    }

//#define DEBUG_SERVICE_TIME_INIT
    void CreateMVASubmodels(){
        MVASubmodels.resize(depth_); // first sub_modle set as null
        Submodel *submodel;
        MVA *mva;
        for(int s = 1; s < submodels_.size(); ++s){
            submodel = submodels_[s];
            int clients_num = submodel->clients_.size();
            int servers_num = submodel->servers_.size();
            Task *task;
            /**
             * @brief MVA
             * @param customer_nums K+1
             * @param think_times K+1
             * @param sations M
             * @param with2phases
             * @param with_traffic_dependency
             * @param rate_declines
             */
            cout<<"sub"<<s<<endl;
            // calc Qsize
            int Qsize=1;
            for(int i = 1; i < customer_nums[s].size(); ++i){
                Qsize *= (customer_nums[s][i]+1);
            }
//            // think_times
//            vector<double> think_times(clients_num+1);
//            for(int i = 1; i < think_times.size();++i){
//                task = submodel->clients_[i-1];
//                think_times[i] = task->think_time();
//            }

            //stations
            vector<Station*> stations(clients_num+servers_num);
            vector< vector<double> > service_times;
            Entry *entry;
            /// clients become delay stations
            for(int i = 0; i < clients_num; ++i){
                task = submodel->clients_[i];
                //// service time for stations as clients
                service_times.resize(task->entries_.size());
                for(int j = 0; j < task->entries_.size();++j){
                    entry = task->entries_[j];
                    service_times[j].resize(entry->phases_.size());
                    for(int k = 0; k < entry->phases_.size(); ++k){
                        service_times[j][k] = client_service_time(entry->phases_[k],s);
                        //cout<<service_times[j][k] <<endl;
                    }
                }
                /// new station
                stations[i] = new Delay_Station(clients_num, Qsize, service_times);
#ifdef DEBUG_SERVICE_TIME_INIT // right
                cout<<"clients:"<<endl;
                for(int j = 0; j < service_times.size(); ++j){
                    cout<<" entry "<<j+1<<endl;
                    for(int k = 0; k < service_times[j].size(); ++k){
                        //cout<<"  phase "<<k+1<<endl;
                        //cout<<"  "<<service_times[j][k]<<endl;
                        cout<<"  "<<service_times[j][k];
                    }
                    cout<<endl;
                }
#endif
            }

            /// servers become queue stations
            for(int i = clients_num; i < stations.size();++i){
                task = submodel->servers_[i-clients_num];
                //// service time for stations as clients
                service_times.resize(task->entries_.size());
                for(int j = 0; j < task->entries_.size();++j){
                    entry = task->entries_[j];
                    service_times[j].resize(entry->phases_.size());
                    for(int k = 0; k < entry->phases_.size(); ++k){
                        service_times[j][k] = server_service_time(entry->phases_[k]);
                    }
                }
                /// new station
                stations[i] = new FCFS_Station(clients_num, Qsize, service_times);
#ifdef DEBUG_SERVICE_TIME_INIT
                cout<<"server:"<<endl;
                for(int j = 0; j < service_times.size(); ++j){
                    cout<<" entry "<<j+1<<endl;
                    for(int k = 0; k < service_times[j].size(); ++k){
                        //cout<<"  phase "<<k+1<<endl;
                        //cout<<"  "<<service_times[j][k]<<endl;
                        cout<<"  "<<service_times[j][k];
                    }
                    cout<<endl;
                }
#endif
            }


            // inital MVA

            MVASubmodels[s] = new Linearizer_MVA(customer_nums[s], think_times_[s], stations);
            mva = MVASubmodels[s];
            mva->Init();
            //mva->Solve();
        }

    }

    void Solve(){

    }

    void InitSubModels(){
        InitWaitTimes();
        InitLambdas();
        InitVisits();
        InitCustomerSizes();
        InitThinkTimes();
    }


    vector< vector< vector< vector<double> > > > visits_;



    void InitVisits();


    vector< vector<int> > customer_nums;

    /**
     * @brief InitCustomerSizes
     * customer_nums are intialised with the way described in p80. Index starts from 1
     */
    void InitCustomerSizes(){
        customer_nums.resize(depth_);
        Submodel *submodel;
        int sub_size;
        for (int s = 1; s < depth_; ++s) {
            submodel = submodels_[s];
            sub_size = submodel->clients_.size()+1;
            customer_nums[s].resize(sub_size);
            for(int i = 1; i < sub_size; ++i){
                customer_nums[s][i] = CustomerSize( submodel->clients_[i-1]);// check is right
            }
        }
    }

    vector< vector<double> > think_times_;
    /**
     * @brief InitThinkTime
     * Initialise think time for each chain (corresponded to each client task); index starts from 1
     *   from the top submodel to the bottom submodel;
     *   If the task is a kRefTask, think time can be directly read;
     *   Otherwise, think time is calculated with forumlars in p80
     */
    void InitThinkTimes(){
        think_times_.resize(depth_);
        Submodel *submodel;
        Task *task;
        for (int s = 1; s < depth_; ++s) {
            submodel = submodels_[s];
            think_times_.resize(submodel->clients_.size()+1);
            for (int i = 0; i < submodel->clients_.size(); ++i) {
                task = submodel->clients_[i];
                if (task->type() == kRefTask) {
                    think_times_[s][i+1] = task->think_time();
                } else {
                    think_times_[s][i+1] = task->customer_num()* (1-task->rho())/task->lambda();
                }
            }

        }
    }

    /**
     * @brief InitLambdas
     *   calc lambda: the visits rates for entries are calculated as follows:
     *          like calc the visits: (lambda for tasks will be automatically calculated)
     *           i. lambda_i = 1/(think_time + wait_time) , for kRefTask i
     *           ii. lambda_j = \sum_{i \in clients} lamda_{i} visit_num_{i,j}, for other taks
     *   ps: calc think_time in every step in MVA (calc (rho is automatically updated when lambda and wait_time are updated):
     */
    void InitLambdas(){
        Submodel *submodel;
        Task *task;
        // set values for lambda in entries (lambda for tasks will be automatically calculated)
        Entry *entry;
        Phase *phase;
        Connection *connection;
        for(int s = 1; s < depth_; ++s){
            submodel = submodels_[s];
            for(int i = 0; i < submodel->clients_.size();  ++i){
                task = submodel->clients_[i];
                /// set the lambda_i for referecnce client i to 1/(think_time + wait_time)
                if(task->type() == kRefTask){ // kRefTask has only one entry
                    task->entries_[0]->set_lambda(
                                1/(task->think_time() + task->entries_[0]->wait_time())
                                );
                }
                /// set the lambda_j for server j
                for(int j = 0; j < task->entries_.size(); ++j){
                    entry = task->entries_[j];
                    for(int k = 0; k < entry->phases_.size(); ++k){
                        phase = entry->phases_[k];
                        for(int m = 0; m < phase->connections_.size(); ++m){
                            connection = phase->connections_[m];
                            if(connection->target_->task_->depth()==(s+1)){
                                connection->target_->AddLambda(
                                            connection->num_ * entry->lambda()
                                            );
                            }
                        }
                    }
                }
            }
        }

    }

    //initialize wait_time for each task and  these:     visit_list_server.resize(0);    visit_list_client.resize(0);
    /**
     * @brief InitWaitTimes
     * It is done as follows:
     *  step 1. start from the lowest submodel, update the wait time for all servers of the submodel
     *     (since it starts from the lowest submodel, all tasks visited by a present server must
     *      located in deeper submodel and their wait time have been updated. Besides, in this way,
     *     all servers will only be updated once.)
     *  step 2. update the the wait time for clients (they must be ref. task) in the oberst submodul.
     */
    void InitWaitTimes();




    double client_service_time(Phase *phase, int submodel_id);

    double server_service_time(Phase *phase);


    /**
     * @brief findCustomerSize
     * @param task
     * @return the customer size cs when the task acts as a client
     * // task with infinite servers still needs to be handled
     *
     * let d be the depth of the task
     * if d = 1, then task is a reference task, cs = the customer size determined by the reference task
     * if d > 1, then task must be a server of a submodel of depth d-1, cs can be calculated based on the
     * cusoter sizes of clients in the submodel
     */
    int CustomerSize(Task *task);

    /**
     * @brief CallClientsNum
     * @param client
     * @return the sum of customers number of clients call this server in the submodel of the server
     */
    int CallClientsCosNum(Task *server);


    vector<MVA*> MVASubmodels;

    void ModelTransform(){
        ProcessorTransform();// check right
        ThinkTimeTransform();
    }



    vector<Entry*> entries_;
    vector<Task*> tasks_;
    vector<int> ref_entries_;
    vector<Submodel*> submodels_; // index starts from 1, size depth


    int T_ = 100000;
    int n_;

    bool finished_;


    void ProcessorTransform(){
        for(int i = 1; i < tasks_.size(); ++i){
            if(tasks_[i]->type() != kProcessor){
                ProcessorTransform(tasks_[i]);
            }
        }
    }

    void ProcessorTransform(Task *task){
        for(int i = 0; i < task->entries_.size(); ++i){
            ProcessorTransform(task->entries_[i]);
        }
    }

    void ProcessorTransform(Entry *entry){
        for(int i = 0; i < entry->phases_.size(); ++i){
            ProcessorTransform(entry->phases_[i]);
        }
    }

    void ProcessorTransform(Phase *phase);

    void ThinkTimeTransform();

    int depth_= 0;



    void TopoSort();


    void DFSVist(Task *task);

    void Read(string &def_file);

    void PrintTaskTypes();
    void PrintTaskDepth();

    string task_type_name(TaskType type);

};










#endif // SVNSOLVER_H
