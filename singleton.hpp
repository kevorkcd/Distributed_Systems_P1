#pragma once

#include "bully.hpp"
//#include "bully_op.hpp"

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
        void boot_node(int ID);
        void boot_nodes();
        void shutdown(int ID);
        void fail(int ID);
        void list_nodes();
        void join_threads();
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
    bully_access->construct_leader_info();
}

void SingletonBully::make_node(int ID) {
    Bully* tmp = new Bully(ID);
}

void SingletonBully::make_nodes(int node_amount) {
    // int curr_max_ID = bully_access->leader->ID;
    // BUG
    // The max ID does not have to be the Leader's Id.
    // Node 7 can be Leader while node 8 is OFFLINE or FAILED
    int max_ID = 0;
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        int curr_ID = bully_access->node_list[i]->ID;
        if (curr_ID > max_ID) {
            max_ID = curr_ID;
        }
    }
    // bool exists[max_ID+1] = { false };
    // for (int i = 0; i < bully_access->node_list.size(); i++) {
    //     exists[bully_access->node_list[i]->ID] = true;
    // }
    for (int i = 1; i <= node_amount; i++) {
        make_node(max_ID+i);
    }
}

void SingletonBully::boot_node(int ID) {
	for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) {
            bully_access->node_list[i]->responsive.unlock();
            bully_access->node_list[i]->st = BOOTING;
            bully_access->node_list[i]->alive = new thread(&Bully::run, bully_access->node_list[i]);
        }
    }
}

void SingletonBully::boot_nodes() {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (bully_access->node_list[i]->st == OFFLINE) {
            bully_access->node_list[i]->responsive.unlock();
            bully_access->node_list[i]->st = BOOTING;
            bully_access->node_list[i]->alive = new thread(&Bully::run, bully_access->node_list[i]);
        }
    }
}

void SingletonBully::shutdown(int ID) {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) {
            bully_access->node_list[i]->st = OFFLINE;
            bully_access->node_list[i]->responsive.lock();
        }
    }
}

void SingletonBully::fail(int ID) {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) {
            bully_access->node_list[i]->st = FAILED;
            bully_access->node_list[i]->responsive.unlock();
        }
    }
}

void SingletonBully::list_nodes() {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        Bully* curr = bully_access->node_list[i];
        cout << "ID: " << curr->ID << "\t" << curr->st_string() << endl;
    }
    cout << "Total nodes:\t" << bully_access->node_list.size() << endl;
    cout << "Total mesages:\t" << bully_access->message_no << endl;
}

void SingletonBully::join_threads() {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        bully_access->node_list[i]->alive->join();
    }
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
    cout << "-   mknode      : make a single node." << endl;
    cout << "-   mknodes     : make multiple nodes." << endl;
    cout << "-   btnode      : boot up a node." << endl;
    cout << "-   btnodes     : boot up all offline nodes." << endl;
    cout << "-   shutdown    : shut down a node." << endl;
    cout << "-   fail        : crash a node." << endl;
    cout << "-   ls          : list all nodes." << endl;
    //cout << "-   rmnode      : to remove a node from the network." << endl;
}