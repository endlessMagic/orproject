#include "SRNSolver.h"

int in_visits(vector<Visit*> &vs,Task *task){
    for(int i = 0; i < vs.size(); ++i){
        if(vs[i]->task()== task){
            return i;
        }
    }
    return -1;
}


bool Phase::hasVisit(Task *task){
    if(connections_.size() == 0){
        return false;
    } else {
        Connection *connection;
        for(int i = 0; i < connections_.size(); ++i){
            connection = connections_[i];
            if(connection->target_->task_== task){
                return true;
            }
        }
        return false;
    }
}

string SRNSolver::task_type_name(TaskType type){
    switch(type){
    case kRefTask:{
        return "kRefTask";
    }
    case kActiveTask:{
        return "kActiveTask";
    }
    case kProcessor:{
        return "kProcessor";
    }
    default:{
        return "unKown";
    }
    }
}

void SRNSolver::PrintTaskTypes(){
    Task *task;
    for(int i = 1; i < tasks_.size(); ++i){
        task = tasks_[i];
        cout<< i <<" "<<task_type_name(task->type())<<endl;
    }
}

void SRNSolver::PrintTaskDepth(){
    Task *task;
    for(int i = 1; i < tasks_.size(); ++i){
        task = tasks_[i];
        cout<< i <<" "<<task->depth()<<endl;
    }
}

void SRNSolver::Read(string &def_file){
    Task *task;
    ifstream in_file;
    in_file.open("./define/"+def_file);
    if(in_file.is_open()){
        string temp_string;
        while(!in_file.eof()){

            in_file >> temp_string;
            if( !temp_string.compare("RunTime")){
                in_file>>T_;
                continue;
            }

            if( temp_string[0] == '#'){
                in_file.ignore(std::numeric_limits<int>::max(),'\n');
                continue;
            }

            if( !temp_string.compare("Task")){
                int type, id, M, processor_id;
                double think_time;
                list<pair<int, int> > task_to_processor;
                in_file.seekg(in_file.tellg()+1);
                in_file.ignore(std::numeric_limits<int>::max(),'\n');
                in_file >> temp_string;
                char first_char = temp_string[0];
                list<Task*> task_list;
                while (first_char != '#'){
                    in_file.seekg(in_file.tellg() - temp_string.length());
                    in_file>>type;
                    switch((TaskType)type){
                    case kRefTask:{// now only ref_task has think time, 1006
                        in_file>>id>>M>>processor_id>>think_time;
                        task = new Task((TaskType)type, id, M);
                        task->set_think_time(think_time);
                        task_list.push_back(task);
                        task_to_processor.push_back(pair<int, int> (id, processor_id));
                        break;
                    }
                    case kActiveTask:{
                        in_file>>id>>M>>processor_id;
                        task = new Task((TaskType)type, id, M);
                        task_list.push_back(task);
                        task_to_processor.push_back(pair<int, int> (id, processor_id));
                        break;
                    }
                    case kProcessor:{
                        in_file>>id>>M;
                        task = new Task((TaskType)type, id, M);
                        task_list.push_back(task);
                        break;
                    }
                    default:
                        break;
                    }
                    in_file>>temp_string;
                    first_char = temp_string[0];
                }
                tasks_.resize(task_list.size()+1);
                for(list<Task*>::iterator it = task_list.begin(); it != task_list.end(); ++it){
                    tasks_[(*it)->id_] = (*it);
                }
                int ii = 0;
                for(list<pair<int, int> >::iterator it = task_to_processor.begin(); it != task_to_processor.end(); ++it){

                    tasks_[(*it).first]->processor_ = tasks_[(*it).second];
                    ++ii;
                }
                continue;
            }

            if( !temp_string.compare("Entry")){
                int id;
                double service_time, think_time;
                Entry *entry;
                in_file.seekg(in_file.tellg()+1);
                in_file.ignore(std::numeric_limits<int>::max(),'\n');
                in_file >> temp_string;
                char first_char = temp_string[0];
                int count = 0;
                while (first_char != '#'){
                    in_file.seekg(in_file.tellg() - temp_string.length());
                    in_file>>id>>think_time;
                    entry = new Entry(id);
                    entries_.push_back(entry);
                    SkipComment(in_file);
                    count = 0;
                    while (in_file.peek()!='\n'){
                        ++count;
                        if(count > 100){
                            cout<<"more than 100 phases!"<<endl;
                            break;
                        }
                        in_file>>service_time;
                        entry->AddPhase(service_time);
                        SkipComment(in_file);
                    }
                    in_file>>temp_string;
                    first_char = temp_string[0];
                }
            }

            if( !temp_string.compare("Connections")){
                int id, phase_no, target_id;
                Connection *connection;
                double num;
                in_file.seekg(in_file.tellg()+1);
                in_file.ignore(std::numeric_limits<int>::max(),'\n');
                in_file >> temp_string;
                char first_char = temp_string[0];
                string last_string = "";
                while (first_char != '#'){
                    if(temp_string != last_string){
                        id = stoi(temp_string);
                    }
                    last_string = temp_string;
                    in_file>>phase_no>>target_id>> num;
                    connection = new Connection(entries_[target_id], num);
                    entries_[id]->AddConnectionBack(connection, phase_no-1);// phase start with 1 in model and with 0 in codes
                    in_file>>temp_string;
                    first_char = temp_string[0];
                }
            }

            if( !temp_string.compare("TaskEntry")){
                int entry_id, task_id;
                in_file.seekg(in_file.tellg()+1);
                in_file.ignore(std::numeric_limits<int>::max(),'\n');
                in_file >> temp_string;
                char first_char = temp_string[0];
                while (first_char != '#'){
                    in_file.seekg(in_file.tellg() - temp_string.length());
                    in_file>>entry_id>>task_id;
                    tasks_[task_id]->AddEntry(entries_[entry_id]);
                    in_file>>temp_string;
                    first_char = temp_string[0];
                }
            }


        }
        in_file.close();

    }
    // set index_ for entries
    Entry *entry;
    for(int i = 1; i < tasks_.size(); ++i){
        task = tasks_[i];
        for(int j = 0; j < task->entries_.size(); ++j){
            entry = task->entries_[j];
            entry->set_index(j);
        }
    }


    cout<<"Totally "<< tasks_.size()-1<<" tasks have been read."<<endl;
    cout<<"Totally "<< entries_.size()-1<<" entries have been read."<<endl;
    PrintTaskTypes();
}


void SRNSolver::DFSVist(Task *task){
  if (task->color() == kGrey){
      cout<<"task_id"<<task->id_<<endl;
      printf ("Error DFSVisit!");
      exit (EXIT_FAILURE);
  } else if (task->color() == kWhite){
    ++depth_;
    task->set_color(kGrey);
    task->set_depth(depth_);

    Entry *entry;
    Phase *phase;
    Connection *connection;
    for(int i = 0; i < task->entries_.size(); ++i){
        entry = task->entries_[i];
        for(int j = 0; j < entry->phases_.size(); ++j){
            phase = entry->phases_[j];
            for(int k = 0; k < phase->connections_.size(); ++k){
                connection = phase->connections_[k];
                DFSVist(connection->target_->task_);
            }
        }
    }
    task->set_color(kBlack);
    -- depth_;
  }
}

void SRNSolver::TopoSort(){
    Task *task;
    for(int i = 1; i < tasks_.size(); ++i){
        task = tasks_[i];
        if(task->color() == kWhite){
            DFSVist(task);
        }
    }

    for(int i = 1; i < tasks_.size(); ++i){
        task = tasks_[i];
        if(task->depth() > depth_){
            depth_ = task->depth();
        }
    }

 }

/**
 * @brief SRNSolver::ThinkTimeTransform
 * p66
 * The
 * think time for a task is handled by the underlying MVA solver as the think time for a chain.
 * Think times for individual entries are handled by rst creating a delay center, then making
 * requests to the delay center.
 */
void SRNSolver::ThinkTimeTransform(){
    Entry *entry;
    for(int i = 1; i < entries_.size(); ++i){
        entry = entries_[i];
        if(entry->think_time_ != 0){
            Task *delay_center = new Task(kProcessor,tasks_.size(),1,entry->think_time_,entries_.size());
            tasks_.push_back(delay_center);
            entries_.push_back(delay_center->entries_[0]);
            Connection *connection = new Connection(delay_center->entries_[0],1);
            entry->AddConnectionFront(connection, 0);
        }
    }
}


void SRNSolver::ProcessorTransform(Phase *phase){
    Task *processor = phase->entry_->task_->processor_;
    double visit_num = 1;

    for(int i = 0; i < phase->connections_.size(); ++i){
        visit_num += phase->connections_[i]->num_;
    }
    //cout<<phase->entry_->task_->id_<<" "<<phase->entry_->id_<<" "<<visit_num<<endl;

    Entry *entry = new Entry(entries_.size());
    entries_.push_back(entry);
    processor->AddEntry(entry);

    Phase *new_phase = new Phase(phase->service_time()/visit_num);
    Connection *connection = new Connection(entry, visit_num);
    phase->AddConnectionFront(connection);
    entry->AddPhase(new_phase);
    phase->set_service_time(0.);
}

/**
 * @brief SRNSolver::CreateSubmodels
 * create submodels and intialise wait time for each tasks
 */
void SRNSolver::CreateSubmodels(){
    Submodel *submodel;
    submodels_.resize(depth_); // first sub_modle set als null
    for(int i = 1; i < submodels_.size(); ++i){
        submodels_[i] =new Submodel(i);
//        submodel = submodels_[i];
//        submodel->set_depth(i);
    }
    Task *task;
    Entry *entry;
    Entry *target_entry;
    Phase *phase;
    Connection *connection;
    for(int i = 1; i < tasks_.size(); ++i){
        task = tasks_[i];
        // add servers to submodel
        if(task->depth() > 1){
            submodel = submodels_[task->depth()-1];
            submodel->servers_.push_back(task);
        }        

        // add clients to submodel
        for(int j = 0; j < task->entries_.size(); ++j){
            entry = task->entries_[j];
            for(int m = 0; m < entry->phases_.size(); ++m){
                phase = entry->phases_[m];
                for(int n = 0; n < phase->connections_.size(); ++n){
                    connection = phase->connections_[n];
                    target_entry = connection->target_;
                    if( target_entry->task_->depth() > 1 ){
                        submodel = submodels_[target_entry->task_->depth()-1];
                        if(!submodel->is_client(task)){
                            submodel->clients_.push_back(task);
                        }
                    }
                }
            }

        }
    }

    // set the server_index_ for each server
    for(int s = 1; s < depth_; ++s){
        submodel = submodels_[s];
        for(int i = 0; i < submodel->servers_.size(); ++i){
            task = submodel->servers_[i];
            task->set_server_index(i);
        }
    }
}

void SRNSolver::PrintSubModelInfo(Submodel *submodel){
    Task *task;
    cout<<submodel->depth()<< "th Submodel's clients:"<<endl;
    for(int i = 0; i < submodel->clients_.size(); ++i){
        task = submodel->clients_[i];
        cout<<"id "<< task->id_ << " depth "<< task->depth() <<  endl;
        PrintTaskServiceTime(task);
    }
    cout<<submodel->depth()<< "th Submodel's servers:"<<endl;
    for(int i = 0; i < submodel->servers_.size(); ++i){
        task = submodel->servers_[i];
        cout<<"id "<< task->id_ << " depth "<< task->depth() <<  endl;
        PrintTaskServiceTime(task);
    }
}


std::istream& SkipComment(std::istream& is)
{
  while (1)
    { /* Solange arbeiten, bis weder ein Leerzeichen, noch ein
         Kommentar folgt. */
      switch (is.peek())
        {
        case ' ':  /* Naechstes Zeichen ist ein Leerzeichen. */
        case '\t':
        case '\r':
          {
#ifdef _DEBUG_
            cerr << "Eating up whitespace.\n";
#endif
            char read = is.peek();
            while (read == ' ' || read == '\t' || read == '\r')
              { /* Solange lesen, bis kein Leerzeichen mehr folgt. */
                is.get();
                read = is.peek();
              }

            break;
          }
        case '#': /* Naechstes Zeichen ist ein Kommentarzeichen. */
#ifdef _DEBUG_
          cerr << "Skipping comment...\n";
#endif
          while (is.get() != '\n'); /* Solange lesen, bis ein
                                       Zeilenende gefunden wurde. */

          break;

        default:
          /* Etwas anderes ist gefunden. */
          return is;
        }
    }
}

int SRNSolver::CallClientsCosNum(Task *server){
    int depth = server->depth();
    if(depth == 1){
        return 0;
    } else {
        int num = 0;
        Submodel *submodel = submodels_[depth-1];
        Task *client;
        for(int i = 0; i < submodel->clients_.size(); ++i){
            client = submodel->clients_[i];
            if(client->hasVisit(server)){
                num += client->customer_num_;
            }
        }
        return num;
    }
}

int SRNSolver::CustomerSize(Task *task){
    int depth = task->depth();
    if (depth == 1){
        return task->M_;
    } else {
        if(task->M_==1){
            return 1;
        } else {
           task->customer_num_ = min ((task->M()), CallClientsCosNum(task));
           return task->customer_num_;
        }
    }
}

double SRNSolver::server_service_time(Phase *phase){
    if(phase->entry_->task_->type()==kProcessor){
        return phase->service_time();
    } else {
        double wait_time = 0.;
        Connection *conection;
        for(int i = 0; i < phase->connections_.size();++i){
            conection = phase->connections_[i];
            wait_time += (conection->target_->wait_time()*conection->num_);
        }
        return wait_time;
    }
}

double SRNSolver::client_service_time(Phase *phase, int submodel_id){
    double wait_time = 0.;
    Connection *conection;
    int depth;
    ++submodel_id;
    for(int i = 0; i < phase->connections_.size();++i){
        conection = phase->connections_[i];
        depth = conection->target_->task_->depth();
        if(depth != (submodel_id)){ // server is not in the same submodel of the client
            wait_time += (conection->target_->wait_time()*conection->num_);
        }
    }
    return wait_time;
}

void SRNSolver::InitWaitTimes(){
    Submodel *submodel;
    Task *task;
    double wait_time;
    Entry *entry;
    Phase *phase;
    Connection *connection;
    // step 1
    for(int d = depth_-1; d > 0; --d){
        submodel = submodels_[d];
        for(int s = 0; s < submodel->servers_.size(); ++s){
            task = submodel->servers_[s];
            // update wait time for entry
            for(int i = 0; i < task->entries_.size(); ++i){
                entry = task->entries_[i];
                // update wait time for phase
                for(int j = 0; j < entry->phases_.size(); ++j){
                    phase = entry->phases_[j];
                    if(phase->connections_.size() == 0){// which means this phase belongs to a processor
                        phase->set_wait_time(phase->service_time());
                    } else { // not of a processor
                        wait_time = 0.;
                        for(int k = 0; k < phase->connections_.size(); ++k){
                            connection = phase->connections_[k];
                            wait_time += (connection->target_->wait_time()*connection->num_);
                        }
                        phase->set_wait_time(wait_time);
                    }
                }
                /// update wait time for entry
                wait_time = 0.;
                for(int j = 0; j < entry->phases_.size(); ++j){
                    phase = entry->phases_[j];
                    wait_time += phase->wait_time();
                }
                entry->set_wait_time(wait_time);
            }
        }
    }
    //step 2
    submodel = submodels_[1];
    for(int s = 0; s < submodel->clients_.size(); ++s){
        task = submodel->clients_[s];
        // update wait time for entry
        for(int i = 0; i < task->entries_.size(); ++i){
            entry = task->entries_[i];
            // update wait time for phase
            for(int j = 0; j < entry->phases_.size(); ++j){
                phase = entry->phases_[j];
                if(phase->connections_.size() == 0){// which means this phase belongs to a processor
                    phase->set_wait_time(phase->service_time());
                } else { // not of a processor
                    wait_time = 0.;
                    for(int k = 0; k < phase->connections_.size(); ++k){
                        connection = phase->connections_[k];
                        wait_time += (connection->target_->wait_time()*connection->num_);
                    }
                    phase->set_wait_time(wait_time);
                }
            }
            /// update wait time for entry
            wait_time = 0.;
            for(int j = 0; j < entry->phases_.size(); ++j){
                phase = entry->phases_[j];
                wait_time += phase->wait_time();
            }
            entry->set_wait_time(wait_time);
        }
    }
}


// new version
/**
 * @brief SRNSolver::InitVisits
 * ps. I have written to the auther, but he did not reply to me.
 * Now I think visits should be based on lambda:
 *   v_{mek} = sum_{i\in \varepsilon(k)} \alpha_{i} y_{i,e}, where
 *   \alpha_{i} = lambda_{i}/lambda_{k} = v_{kik}
 */
void SRNSolver::InitVisits(){
    visits_.resize(depth_); // index starts from 1
    int size;
    Submodel *submodel;
    Task *task;
    int chain_num;
    // set values for visit in entries
    Entry *entry;
    Phase *phase;
    double task_lambda;
    Connection *connection;
    for(int s = 1; s < depth_; ++s){
        submodel = submodels_[s];
        size = submodel->clients_.size()+submodel->servers_.size();
        visits_[s].resize(size);
        chain_num = submodel->clients_.size();
        // initialise visits for customers
        for (int i = 0; i < chain_num; ++i) {
            task = submodel->clients_[i];
            visits_[s][i].resize(task->entries_.size());
            task_lambda = task->lambda();
            for(int j = 0; j < task->entries_.size(); ++j){
                visits_[s][i][j].resize(chain_num+1); // first position for open network
                visits_[s][i][j][i+1] = task->entries_[j]->lambda()/task_lambda;
            }
        }
        // intialise visits for servers
        /// resize
        for (int i = chain_num; i < size; ++i) {
            task = submodel->servers_[i-chain_num];
            visits_[s][i].resize(task->entries_.size());
            for(int j = 0; j < task->entries_.size(); ++j){
                visits_[s][i][j].resize(chain_num+1); // first position for open network
            }
        }
        /// set values
        for(int i = 0; i < submodel->clients_.size();  ++i){
            task = submodel->clients_[i];
            /// set the visit for servers
            for(int j = 0; j < task->entries_.size(); ++j){
                entry = task->entries_[j];
                for(int k = 0; k < entry->phases_.size(); ++k){
                    phase = entry->phases_[k];
                    for(int m = 0; m < phase->connections_.size(); ++m){
                        connection = phase->connections_[m];
                        //.mn./ set value incrementals for visits of servers
                        if(connection->target_->task_->depth()==(s+1)){
                            visits_[s][connection->target_->task_->server_index()+chain_num]
                                    [connection->target_->index()][i+1] += connection->num_ *
                                    visits_[s][i][entry->index()][i+1];
                        }
                    }
                }
            }
        }
    }
}

//void SRNSolver::InitVisits(){
//    visits_.resize(depth_); // index starts from 1
//    int size;
//    Submodel *submodel;
//    Task *task;
//    int chain_num;
//    // set values for visit in entries
//    Entry *entry;
//    Phase *phase;
//    Connection *connection;
//    for(int s = 1; s < depth_; ++s){
//        submodel = submodels_[s];
//        for(int i = 0; i < submodel->clients_.size();  ++i){
//            task = submodel->clients_[i];
//            /// set the visit for referecnce clients to 1
//            if(task->type() == kRefTask){ // kRefTask has only one entry
//                task->entries_[0]->set_visit(1);
//            }
//            /// set the visit for servers
//            for(int j = 0; j < task->entries_.size(); ++j){
//                entry = task->entries_[j];
//                for(int k = 0; k < entry->phases_.size(); ++k){
//                    phase = entry->phases_[k];
//                    for(int m = 0; m < phase->connections_.size(); ++m){
//                        connection = phase->connections_[m];
//                        if(connection->target_->task_->depth()==(s+1)){
//                            connection->target_->AddVisit(connection->num_ * entry->visit());
//                        }
//                    }
//                }
//            }
//        }
//    }
//    // resize visits_ and set values for visits_
//    for(int s = 1; s < depth_; ++s){
//        submodel = submodels_[s];
//        size = submodel->clients_.size()+submodel->servers_.size();
//        visits_[s].resize(size);
//        chain_num = submodel->clients_.size();
//        /// set values for visits_ of kRefTasks
//        for(int i = 0; i < chain_num; ++i){
//            task = submodel->clients_[i];
//            visits_[s][i].resize(task->entries_.size());
//            for(int j = 0; j < task->entries_.size(); ++j){
//                visits_[s][i][j].resize(chain_num+1);// index starts from 1
//                entry = task->entries_[j];
//                visits_[s][i][j][i+1] = entry->visit();
//            }
//        }

//        for(int i = chain_num; i < size; ++i){
//            task = submodel->servers_[i-chain_num];
//            visits_[s][i].resize(task->entries_.size());
//            for(int j = 0; j < task->entries_.size(); ++j){
//                visits_[s][i][j].resize(chain_num+1);// index starts from 1
//            }
//        }

//        for(int i = 0; i < chain_num; ++i){
//            task = submodel->clients_[i];
//            //visits_[s][i].resize(task->entries_.size());
//            for(int j = 0; j < task->entries_.size(); ++j){
//                //visits_[s][i][j].resize(chain_num+1);// index starts from 1
//                entry = task->entries_[j];
//                //visits_[s][i][j][i+1] = entry->visit();
//                for(int k = 0; k < entry->phases_.size(); ++k){
//                    phase = entry->phases_[k];
//                    for(int m = 0; m < phase->connections_.size(); ++m){
//                        connection = phase->connections_[m];
//                        if(connection->target_->task_->depth()==(s+1)){
//                            visits_[s][connection->target_->task_->server_index()+chain_num]
//                                    [connection->target_->index()][i+1] += (connection->num_*entry->visit());
//                        }
//                    }
//                }
//            }
//        }
//    }
//#ifdef DEBUG_VISITS
//    for(int s = 1; s < depth_; s++){
//        submodel = submodels_[s];
//        cout<<"sub"<<s<<"XXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<endl;
//        for(int i = 0; i < visits_[s].size(); ++i){
//            chain_num = submodel->clients_.size();
//            if(i >= chain_num) {
//                task = submodel->servers_[i - chain_num];
//            } else {
//                task = submodel->clients_[i];
//            }
//            cout<<"T "<<task->id_<<endl;
//            for(int j = 0; j < visits_[s][i].size(); ++j){
//                cout<<"entry ";
//                for(int k = 1; k < visits_[s][i][j].size(); ++k){
//                    cout<<" "<<visits_[s][i][j][k];
//                }
//                cout<<endl;
//            }
//        }
//    }
//#endif
//}
