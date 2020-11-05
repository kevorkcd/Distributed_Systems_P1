#pragma once

#include <iostream>
#include <vector>
#include <string>
//#include <unistd.h>
#include <thread>
#include <chrono>
// #include <synchapi.h>

using namespace std;

enum message {ELECTION=0, OK, COORDINATOR};
enum state   {FAILED=0, OFFLINE, TIMEOUT, ONLINE, IN_ELECTION, LEADER};

class LeaderInfo {
    private:
        friend class Bully;
        bool found;
        int ID;
        int index;
        
        LeaderInfo() { found = false; }
};

class Bully {
    private:
        static int message_no;
        static vector<Bully*> node_list;

        static LeaderInfo* leader;

        /*typedef struct leader_info {
            bool found;
            int ID;
            int index;
        };
        static leader_info leader;*/

        int ID;
        state st;
        thread* alive;

        friend class SingletonBully;
        Bully() {};

    public:
        void construct_leader_info();
        Bully(int ID);
        void run();
        void send_message(message msg, Bully* receiver);
        void receive(message msg, Bully* sender);
        void raise_election();
        // void set_state(state st);
        void fail();
        string st_string();
        // int get_ID();
        int get_leader();
};

// Initializing the static member attributes
int Bully::message_no = 0;
LeaderInfo* Bully::leader = nullptr;
// struct leader_info Bully::leader;
// Bully::leader_info Bully::leader;
vector<Bully*> Bully::node_list;

//Bully::Bully() {}
void Bully::construct_leader_info() {
    leader = new LeaderInfo();
}

// Constructor
Bully::Bully(int ID) {
    this->st = OFFLINE;
    bool is_ID_avalailable = true;
    for (int i = 0; i < node_list.size(); i++) {    // Is ID avaliable
        if (ID == node_list[i]->ID) {
            is_ID_avalailable = false;
        }
    }
    if (is_ID_avalailable) {                        // ID is avaliable, add this node to list
        this->ID = ID;
        node_list.push_back(this);
    }
    else {                                          // ID is not avaliable, destruct the current object
        cout << "ID " << ID << " not available" << endl;
        delete this;
    }
}

void Bully::run() {
    srand(time(0));
    while (true) {
        if (!leader->found || node_list[leader->ID]->st <= OFFLINE) {
            this->raise_election();
        }
        std::chrono::milliseconds interval((rand() % 3000) + 1001);
        std::this_thread::sleep_for(interval);
    }
}

// This is the gate for the node to send messages to other nodes
void Bully::send_message(message msg, Bully* receiver) {
    if (this->st >= ONLINE) {           // If online
        // if (msg == COORDINATOR) {
        //     curr_leader_ID = this->ID;
        // }
        message_no++;
        receiver->receive(msg, this);
    }
    else {
        cout << "Node " << ID << " is " << this->st_string() << endl;
    }
}

// Here the node receives a msg and does something according to the message
void Bully::receive(message msg, Bully* sender) {
    if (this->st >= ONLINE) {
        switch(msg) {
            case ELECTION:                      //The message is Election
                cout << "ID: " << this->ID << " received ELECTION from ID: " << sender->ID << endl;
                this->st = IN_ELECTION;         //sets the state of the reciever to IN_ELECTION
                this->send_message(OK, sender); //send OK to sender
                this->raise_election();
                break;
            case OK:                            //the message is OK
                cout << "ID: " << this->ID << " received OK from ID: " << sender->ID << endl;
                this->st = TIMEOUT;              //set state as online
                //this->largest_alive = false;
                break;
            case COORDINATOR:                   //The message is COORDINATOR
                cout << "ID: " << this->ID << " received COORDINATOR from ID: " << sender->ID << endl;
                this->st = ONLINE;              //
                break;
            default:
                break;
        }
    }
}

void Bully::raise_election() {
    cout << "ID: " << this->ID << " raising election" << endl;
    this->st = IN_ELECTION;
    //this->largest_alive = true;
    for (int i = 0; i < node_list.size(); i++) {
        if (node_list[i]->ID > this->ID) {
            this->send_message(ELECTION, node_list[i]);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));    // Sleep for 500ms - 0.5 seconds
    //
    if (this->st == IN_ELECTION) {
        this->st = LEADER;
        leader->found = true;
        leader->ID = this->ID;
        for (int i = 0; i < node_list.size(); i++) {
            if (!(node_list[i]->ID == this->ID)) {
                this->send_message(COORDINATOR, node_list[i]);
            }
        }
    }
    /*
    if(this->largest_alive == true){
        for (int i = 0; i < node_list.size(); i++) {
            if (node_list[i]->ID > this->ID) {
                this->send_message(COORDINATOR, node_list[i]);
            }
        }
    }*/
}

// void Bully::set_state(state st) {
//     this->st = st;
// }

// int Bully::get_ID() {
//     if (this->st >= ONLINE) {
//         return this->ID;
//     }
//     else {
//         cout << "Node is " << this->st_string() << endl;
//         return -1;
//     }
// }

// Temporary test method
int Bully::get_leader() {
    if (this->st >= ONLINE) {
        return leader->ID;
    }
    else {
        cout << "Node is " << this->st_string() << endl;
        return -1;
    }
}

string Bully::st_string() {
    switch(this->st) {
        case FAILED:
            return "FAILED";
            break;
        case OFFLINE:
            return "OFFLINE";
            break;
        case ONLINE:
            return "ONLINE";
            break;
        case IN_ELECTION:
            return "IN_ELECTION";
            break;
        case LEADER:
            return "LEADER";
            break;
        default:
            return "";
            break;
    }
}