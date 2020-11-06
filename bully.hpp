#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

enum message {ELECTION=0, OK, COORDINATOR};
enum state   {FAILED=0, OFFLINE, TIMEOUT, ONLINE, BOOTING, IN_ELECTION, LEADER};

class LeaderInfo {
    private:
        friend class Bully;
        friend class SingletonBully;
        bool found;
        int ID;
        int index;
        
        LeaderInfo() { found = false; }
};

class Bully {
    private:
        friend class SingletonBully;

        static int message_no;
        static vector<Bully*> node_list;
        static LeaderInfo* leader;

        int ID;
        state st;
        thread* alive;
        mutex m;

        Bully() {};

    public:
        void construct_leader_info();
        Bully(int ID);
        void run();
        void send_message(message msg, Bully* receiver);
        void receive(message msg, Bully* sender);
        void raise_election();
        void fail();
        string st_string();
};

// Initializing the static member attributes
int Bully::message_no = 0;
LeaderInfo* Bully::leader = nullptr;
vector<Bully*> Bully::node_list;

// Leader construction
void Bully::construct_leader_info() {
    leader = new LeaderInfo();
}

// Constructor
Bully::Bully(int ID) {
    this->m.lock();
    this->st = OFFLINE;
    this->m.unlock();

    bool is_ID_avalailable = true;
    for (int i = 0; i < node_list.size(); i++) {    // Is ID avaliable
        if (ID == node_list[i]->ID) {
            is_ID_avalailable = false;
        }
    }
    if (is_ID_avalailable) {                        // ID is avaliable, add this node to list
        this->m.lock();
        this->ID = ID;
        this->m.unlock();

        node_list.push_back(this);
    }
    else {                                          // ID is not avaliable, destruct the current object
        cout << "ID " << ID << " not available" << endl;
        delete this;
    }
}

// Run function to be called on thread
void Bully::run() {
    srand(time(0));
    while (!(this->st <= OFFLINE)) {
        if (this->st == TIMEOUT) {            
            chrono::milliseconds interval(5000);
            this_thread::sleep_for(interval);
            this->st = ONLINE;
        }
        chrono::milliseconds interval((rand() % 3000) + 1001);
        this_thread::sleep_for(interval);
        // If a leader exists which isn't offline
        if (this->st == BOOTING || !leader->found || node_list[leader->index]->st <= OFFLINE) {
            if (!(this->st <= OFFLINE)) {
                this->m.lock();
                this->st = ONLINE;
                this->m.unlock();
                cout << this->ID << " initiate election." << endl;
                this->raise_election();   
            }
        }
    }
}

// This is the gate for the node to send messages to other nodes
void Bully::send_message(message msg, Bully* receiver) {
    if (this->st >= ONLINE) {   // If online
        this->m.lock();
        message_no++;
        this->m.unlock();

        receiver->receive(msg, this);
    }
    else {
        // cout << "NOTICE " << this->ID << " is " << this->st_string() << endl;
    }
}

// Here the node receives a msg and does something according to the message
void Bully::receive(message msg, Bully* sender) {
    if (this->st >= TIMEOUT) {
        switch(msg) {
            case ELECTION:                      //The message is Election
                cout << sender->ID << " -> " << this->ID << " ELECTION" << endl;
                this->send_message(OK, sender); //send OK to sender
                // NOTE BUG
                // Not raising election as leader is bad, as coordinator messages
                // won't be send to newly booted nodes.
                if (this->st >= ONLINE) {
                    this->raise_election();
                }
                break;
            case OK:                            //the message is OK
                cout << sender->ID << " -> " << this->ID << " OK" << endl;
                this->m.lock();
                if (!(this->st <= OFFLINE)) {   //set state as online
                    this->st = TIMEOUT;
                }
                this->m.unlock();
                break;
            case COORDINATOR:                   //The message is COORDINATOR
                cout << sender->ID << " -> " << this->ID << " COORDINATOR" << endl;
                this->m.lock();
                if (!(this->st <= OFFLINE)) {
                    this->st = TIMEOUT;
                }
                this->m.unlock();
                break;
            default:
                break;
        }
    }
}

void Bully::raise_election() {
    cout << this->ID << " taking over election." << endl;

    this->m.lock();
    this->st = IN_ELECTION;
    this->m.unlock();

    for (int i = 0; i < node_list.size() && this->st >= ONLINE; i++) {
        if (node_list[i]->ID > this->ID) {
            this->send_message(ELECTION, node_list[i]);
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));    // Sleep for 500ms - 0.5 seconds
    if (this->st == IN_ELECTION) {
        this->m.lock();
        this->st = LEADER;
        leader->found = true;
        leader->ID = this->ID;
        for (int i = 0; i < node_list.size(); i++){
            if (this->ID == node_list[i]->ID) {
                leader->index = i;
            }
        }
        this->m.unlock();

        for (int i = 0; i < node_list.size(); i++) {
            if (!(node_list[i]->ID == this->ID)) {
                this->send_message(COORDINATOR, node_list[i]);
            }
        }
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
        case BOOTING:
            return "BOOTING";
            break;
        case TIMEOUT:
            return "TIMEOUT";
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