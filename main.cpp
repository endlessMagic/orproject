#include "SRNSolver.h"


int main(int argc, char* argv[])
{
    if ( argc == 2) {
            // get the name "controlFile.par"
            std::string contro_file_name;
            contro_file_name = argv[1];
            ifstream control_file;
            control_file.open(contro_file_name);
            if (control_file.is_open()){
                string def_file;
                //def_file = "model";
                //def_file = "example";
                control_file>>def_file;
                SRNSolver SRN_solver(def_file);
                //sim.run(5000000);
                control_file.close();
            }

        } else {
            cout<< "Warning. There is no parameter file!" <<endl;
        }
    return 0;
}


