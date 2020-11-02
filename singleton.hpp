#pragma once

#include "bully.hpp"

/* Commands:
 *   mknode->ID
 *   mknodes->amount
 *   shutdown->ID
 *   fail->ID
 *   boot->ID
 *   lsnodes
 *   rmnode->ID
*/

class SingletonBully {
    private:
        static SingletonBully* instance;
        static Bully* bully_access;
        SingletonBully();
    public:
        static SingletonBully* get_instance();
        void make_node(int ID);
        void make_nodes(int node_amount);
        void boot(int ID);
        void shutdown(int ID);
        void fail(int ID);
        void list_nodes();
        void remove_node(int ID);
        void help();
};

SingletonBully* SingletonBully::instance = nullptr;
Bully* SingletonBully::bully_access = nullptr;

SingletonBully* SingletonBully::get_instance() {
    if (instance == nullptr) {
        instance = new SingletonBully();
    }
    return instance;
}

SingletonBully::SingletonBully() {
    bully_access = new Bully();
}

void SingletonBully::make_node(int ID) {
    Bully* tmp = new Bully(ID);
    bully_access->node_list.push_back(tmp);
}

void SingletonBully::make_nodes(int node_amount){
    for (int i = 1; i <= node_amount; i++) {
        make_node(bully_access->get_leader()+i);  
    }
}

void SingletonBully::help() {
    cout << "Avaliable commands are: " << endl;
    cout << "-   mknode      : for making a single node." << endl;
    cout << "-   mknodes     : for making multiple nodes." << endl;
    cout << "-   boot        : booting up a node." << endl;
    cout << "-   shutdown    : for shutting down a node." << endl;
    cout << "-   fail        : to crash a node." << endl;
    cout << "-   ls          : to list all nodes." << endl;
    cout << "-   rmnode      : to remove a node from the network." << endl;
}
