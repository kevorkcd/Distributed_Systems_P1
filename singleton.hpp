#pragma once

#include "bully.hpp"

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
        void shutdown_node(int ID);
        void shutdown_nodes();
        void fail(int ID);
        void list_nodes();
        void join_threads();
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

// Makes N nodes with ID's beginning from the node with highest ID
void SingletonBully::make_nodes(int node_amount) {
    int max_ID = 0;
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        int curr_ID = bully_access->node_list[i]->ID;
        if (curr_ID > max_ID) {
            max_ID = curr_ID;
        }
    }
    for (int i = 1; i <= node_amount; i++) {
        make_node(max_ID+i);
    }
}

void SingletonBully::boot_node(int ID) {
	for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) {
            if (bully_access->node_list[i]->st < TIMEOUT) {
                bully_access->node_list[i]->responsive.unlock();
                bully_access->node_list[i]->st = BOOTING;
                bully_access->node_list[i]->alive = new thread(&Bully::run, bully_access->node_list[i]);
            }
            else {
                cout << "Node with ID " << bully_access->node_list[i]->ID << " is already in state '" << bully_access->node_list[i]->st_string() << "'." << endl;
            }
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

void SingletonBully::shutdown_node(int ID) {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) {
            if (bully_access->node_list[i]->st > OFFLINE) {
                bully_access->node_list[i]->st = OFFLINE;
                bully_access->node_list[i]->responsive.lock();
                bully_access->node_list[i]->m_election.unlock();
            }
            else {
                cout << "Node with ID " << bully_access->node_list[i]->ID << " is already in state '" << bully_access->node_list[i]->st_string() << "'." << endl;
            }
        }
    }
}

void SingletonBully::shutdown_nodes() {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (bully_access->node_list[i]->st > OFFLINE) {
            bully_access->node_list[i]->st = OFFLINE;
            bully_access->node_list[i]->responsive.lock();
            bully_access->node_list[i]->m_election.unlock();
        }
        else {
            cout << "Node with ID " << bully_access->node_list[i]->ID << " is already in state '" << bully_access->node_list[i]->st_string() << "'." << endl;
        }
    }
}

void SingletonBully::fail(int ID) {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        if (ID == bully_access->node_list[i]->ID) {
            if (bully_access->node_list[i]->st > OFFLINE) {
                bully_access->node_list[i]->st = FAILED;
                bully_access->node_list[i]->responsive.lock();
                bully_access->node_list[i]->m_election.unlock();
            }
            else {
                cout << "Node with ID " << bully_access->node_list[i]->ID << " is already in state '" << bully_access->node_list[i]->st_string() << "'." << endl;
            }
        }
    }
}

void SingletonBully::list_nodes() {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        Bully* curr = bully_access->node_list[i];
        if (bully_access->node_list[i]->st >= TIMEOUT) {
            cout << "ID: " << curr->ID << "\t" << "   "<< curr->st_string() << endl;
        }
        else {
            cout << "ID: " << curr->ID << "\t" << " X " << curr->st_string() << endl;
        }
    }
    cout << "Total nodes:\t" << bully_access->node_list.size() << endl;
    cout << "Total mesages:\t" << bully_access->message_no << endl;
}

void SingletonBully::join_threads() {
    for (int i = 0; i < bully_access->node_list.size(); i++) {
        bully_access->node_list[i]->alive->join();
    }
}

// Prints all avaliable commands and their use case.
void SingletonBully::help() {
    cout << "Avaliable commands are: " << endl;
    cout << "-   mknode      : make a single node." << endl;
    cout << "-   mknodes     : make multiple nodes." << endl;
    cout << "-   btnode      : boot up a node." << endl;
    cout << "-   btnodes     : boot up all offline nodes." << endl;
    cout << "-   kill        : shut down a node." << endl;
    cout << "-   crash       : fail a node." << endl;
    cout << "-   ls          : list all nodes." << endl;
}