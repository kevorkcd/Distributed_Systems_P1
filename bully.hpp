#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <thread>
#include <chrono>
// #include <synchapi.h>

using namespace std;

enum message {ELECTION=0, OK, COORDINATOR};
enum state   {FAILED=0, OFFLINE, ONLINE, IN_ELECTION, LEADER};

class Bully {
    private:
        static int message_no;
        static int curr_leader_ID;
        static vector<Bully*> node_list;
        int ID;
        state st;

        friend class SingletonBully;
        Bully() {};

    public:
        Bully(int ID);
        void run();
        void send_message(message msg, Bully* receiver);
        void receive(message msg, Bully* sender);
        void set_state(state st);
        void fail();
        string st_string();
        int get_ID();
        int get_leader();
};

// Initiating the static member attributes
int Bully::message_no = 0;
int Bully::curr_leader_ID = -1;
vector<Bully*> Bully::node_list;

//Bully::Bully() {}

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
    while (true) {
        if (curr_leader_ID == -1 || node_list[curr_leader_ID]->st <= OFFLINE) {
            // sleep 2sec
            // this_thread::sleep_for(2s);
            // 
            // Sleep(2000);
            this->st = IN_ELECTION;
            for (int i = 0; i < node_list.size(); i++) {
                if (node_list[i]->ID > this->ID) {
                    this->send_message(ELECTION, node_list[i]);
                }
            }
        }
        cout << this->ID << ": Started sleeping" << endl;
        unsigned int seconds = 2;
        // usleep(microseconds);
        // this_thread::sleep_for(chrono::milliseconds(seconds*1000));
        cout << this->ID << ": Waking up" << endl;
    }
}

// This is the gate for the node to send messages to other nodes
void Bully::send_message(message msg, Bully* receiver) {
    if (this->st >= ONLINE) {           // If online
        if (msg == COORDINATOR) {
            curr_leader_ID = this->ID;
        }

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
                cout << "ID: " << this->ID << " received ELECTION" << endl;
                this->st = IN_ELECTION;         //sets the state of the reciever to IN_ELECTION
                this->send_message(OK, sender); //send OK to sender
                for (int i = 0; i < node_list.size(); i++) {
                    if (node_list[i]->ID > this->ID) {
                        this->send_message(ELECTION, node_list[i]);
                    }
                }
                break;
            case OK:                            //the message is OK
                cout << "ID: " << this->ID << " received OK" << endl;  
                break;
            case COORDINATOR:                   //The message is COORDINATOR
                cout << "ID: " << this->ID << " received COORDINATOR" << endl;
                this->st = ONLINE;              //
                break;
            default:
                break;
        }
    }
}

void Bully::set_state(state st) {
    this->st = st;
}

int Bully::get_ID() {
    if (this->st >= ONLINE) {
        return this->ID;
    }
    else {
        cout << "Node is " << this->st_string() << endl;
        return -1;
    }
}

// Temporary test method
int Bully::get_leader() {
    if (this->st >= ONLINE) {
        return curr_leader_ID;
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