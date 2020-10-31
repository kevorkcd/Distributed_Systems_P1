#include "bully.hpp"
#include <iostream>


using namespace std;

class Network{
    private:
        // friend class Bully;
        // static vector<Bully*> node_list;
        bool is_ID_available(int ID);

    public:
        Network();
        void new_node(int new_ID);
        void shutdown_node(int ID);
};