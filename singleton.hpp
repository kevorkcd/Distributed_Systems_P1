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
        // void remove_node(int ID);
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
}

void SingletonBully::make_nodes(int node_amount) {
    int curr_max_ID = bully_access->get_leader();
    for (int i = 1; i <= node_amount; i++) {
        make_node(curr_max_ID+i);
    }
}

void SingletonBully::boot(int ID) {
	for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) {
            bully_access->node_list[i]->st = ONLINE;
            thread alive(&bully_access->node_list[i]->run());
        }
    }
}

void SingletonBully::shutdown(int ID) {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) {
            bully_access->node_list[i]->st = OFFLINE;
        }
    }
}

void SingletonBully::fail(int ID) {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) { 
            bully_access->node_list[i]->st = FAILED;     
        }
    }
}

void SingletonBully::list_nodes() {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        Bully* curr = bully_access->node_list[i];
        cout << "ID: " << curr->ID << "\t" << curr->st_string() << endl;
    }
    cout << "Total nodes: " << bully_access->node_list.size() << endl;
}

// void SingletonBully::remove_node(int ID) {
//     for (int i = 0; i < bully_access->node_list.size(); i++) {
//         if (ID == node_list[i]) {
//             Bully* tmp = node_list[i];
//             node_list
//         }
//     }
// }

void SingletonBully::help() {
    cout << "Avaliable commands are: " << endl;
    cout << "-   mknode      : for making a single node." << endl;
    cout << "-   mknodes     : for making multiple nodes." << endl;
    cout << "-   boot        : booting up a node." << endl;
    cout << "-   shutdown    : for shutting down a node." << endl;
    cout << "-   fail        : to crash a node." << endl;
    cout << "-   ls          : to list all nodes." << endl;
    //cout << "-   rmnode      : to remove a node from the network." << endl;
}