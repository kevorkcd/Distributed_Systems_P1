#include "bully.hpp"
#include <iostream>
#include <vector>

using namespace std;

class Network{
    protected:
        static vector<Bully*> node_list;

    public:
        Network();
        void new_node();
        void shutdown_node(int ID);
};