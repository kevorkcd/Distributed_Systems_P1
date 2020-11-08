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
        mutex responsive;
        //mutex m_election;
        static mutex m_election;
        static mutex m_election_perm;
        int largest_election_ID;

        Bully() {};
        void _raise_election();

    public:
        void construct_leader_info();
        Bully(int ID);
        void run();
        void send_message(message msg, Bully* receiver, Bully* leader);
        void receive(message msg, Bully* sender, Bully* leader);
        //void take_over_election();
        void initiate_election();
        string st_string();
};

// Initializing the static member attributes
int Bully::message_no = 0;
LeaderInfo* Bully::leader = nullptr;
vector<Bully*> Bully::node_list;
mutex Bully::m_election;
mutex Bully::m_election_perm;

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
    for (int i = 0; i < node_list.size(); i++) {    // Checks for ID availability
        if (ID == node_list[i]->ID) {
            is_ID_avalailable = false;
        }
    }
    if (is_ID_avalailable && ID > -1) {             // ID is available, add this node to list
        this->m.lock();
        this->ID = ID;
        this->m.unlock();

        node_list.push_back(this);
    }
    else {                                          // ID is not availiable, destruct the current object
        cout << "ID " << ID << " not available" << endl;
        delete this;
    }
}

// Run function to be called on thread
void Bully::run() {
    srand(time(0));
    while (!(this->st <= OFFLINE)) {            // Keep looping as long as state is "online"
        if (this->st == TIMEOUT) {              // If state is timeout, sleep for 0.2s 
            chrono::milliseconds interval(200);
            this_thread::sleep_for(interval);
            this->st = ONLINE;
        }
        chrono::milliseconds interval((rand() % 300) + 101);    // Simulate random amount of work before checking for leader availability
        this_thread::sleep_for(interval);
        // Is the node booting? Has a leader not been found? Is the elected leader offline and we're not in an election already? Then initiate one.
        if ((this->st == BOOTING || !leader->found || node_list[leader->index]->st <= OFFLINE) && this->st != IN_ELECTION) {
            /*if (this->st == BOOTING) {
                cout << this->ID << " BOOTING" << endl;
            }*/
            
            if (!(this->st <= OFFLINE)) {
                this->m_election_perm.unlock();
                this->m.lock();
                this->st = ONLINE;      // Why is this here?
                this->m.unlock();
                this->initiate_election();   
            }
        }
    }
}

// This is the gate for the node to send messages to other nodes
void Bully::send_message(message msg, Bully* receiver, Bully* leader) {
    if (receiver->responsive.try_lock()) {
        receiver->responsive.unlock();
        //while (this->st == TIMEOUT) {}

        if (this->st >= ONLINE) {   // Only increase message number and send message if this node is "online"
            this->m.lock();
            message_no++;
            this->m.unlock();

            if (msg == COORDINATOR) {
                receiver->receive(msg, this, leader);   // Reciever in this case will be the leader Bully instance
            }
            /*else if (msg == ELECTION){
                receiver->receive(msg, this);
            }*/
            else {
                receiver->receive(msg, this, leader);
            }
        }
    }
}

// Here the node receives a msg and does something according to the message
void Bully::receive(message msg, Bully* sender, Bully* leader) {
    if (this->responsive.try_lock()) {
        this->responsive.unlock();
        if (this->st >= TIMEOUT) {                  // Have to be "online" to recieve a message
            switch(msg) {
                case ELECTION:                      // If message is Election
                    cout << sender->ID << " -> " << this->ID << " ELECTION" << endl;
                    //this->send_message(OK, sender); // Send OK to sender
                    this->m.lock();
                    message_no++;
                    this->m.unlock();
                    sender->receive(OK, this, leader);
                    break;
                case OK:                            // If message is OK
                    cout << sender->ID << " -> " << this->ID << " OK" << endl;
                    this->m.lock();
                    if (!(this->st <= OFFLINE)) {   // If state is online
                        this->st = ONLINE;
                    }
                    if (sender->ID > this->largest_election_ID) {
                        cout << "Largest election ID yet is " << sender->ID << endl;
                        this->largest_election_ID = sender->ID;
                    }

                    this->m.unlock();
                    break;
                case COORDINATOR:                   // If message is COORDINATOR
                    cout << sender->ID << " -> " << this->ID << " COORDINATOR, " << leader->ID << endl;
                    this->m.lock();

                    if (!(this->st <= OFFLINE)) {
                        if (/*sender->ID == this->ID*/ leader->ID == this->ID) {    // Sender in this case will be Leader instance sent from possibly non-leader
                            this->st = LEADER;
                        }
                        else {
                            this->st = TIMEOUT;
                        }
                    }
                    this->m.unlock();
                    this->m_election.unlock();
                    break;
                default:
                    break;
            }
        }
    }
}

void Bully::initiate_election() {
    if (this->m_election.try_lock()) {
        cout << this->ID << " initiating election." << endl;
        _raise_election();
    }
}

/*void Bully::take_over_election() {
    if (this->m_election_perm.try_lock()) {
        this->m_election_perm.unlock();
        cout << this->ID << " taking over election." << endl;
        _raise_election();
    }
}*/

void Bully::_raise_election() {
    
    this->m.lock();             // Set own state to ELECTION before continuing, otherwise wait till it's possible.
    this->st = IN_ELECTION;     
    this->m.unlock();
    this->largest_election_ID = this->ID;

    for (int i = 0; i < node_list.size(); i++) {                    
        if (this->st >= ONLINE && this->m_election_perm.try_lock()) {       // If node has election permission, send message ELECTION to all nodes with larger ID's
            this->m_election_perm.unlock();
            if (this->ID < node_list[i]->ID) {
                this->send_message(ELECTION, node_list[i], this);
            }
            
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(250));            // Waiting  0.25s for for replies
    if (this->st == IN_ELECTION) {                                          // If still in election state, that means no one has replied OK, since that would change node state to ONLINE; self assign LEADER
        this->m_election_perm.lock();
        this->m.lock();
        this->st = LEADER;
        cout << this->ID << " has elected itself as leader." << endl;
        leader->found = true;       
        leader->ID = this->ID;      // Updating leader role information to current node
        for (int i = 0; i < node_list.size(); i++){             // Find its own index and put that in the LeaderInfo "leader"
            if (this->ID == node_list[i]->ID) {
                leader->index = i;
            }
        }
        this->m.unlock();
        this->m_election.unlock();
        for (int i = 0; i < node_list.size(); i++) {            // Update all nodes about who's the new leader
            if (node_list[i]->ID != this->ID) {
                this->send_message(COORDINATOR, node_list[i], this);
            }
        }
    }
    else {  // If the state has changed from "IN_ELECTION", that means that larger nodes has been found.
        
        // Largest ID is found, now get the index and broadcast it to the network.
        for (int i = 0; i < node_list.size(); i++){             // Find the index of said node and put that in the LeaderInfo "leader" index
            if (this->largest_election_ID == node_list[i]->ID) {
                leader->index = i;
                leader->found = true;
                leader->ID = node_list[i]->ID;
            }
        }
        this->m_election.unlock();
        cout << this->ID << " has elected " << leader->ID << " as leader." << endl;
        for (int i = 0; i < node_list.size(); i++) {            // Update all nodes about who's the new leader
            if (this->ID != node_list[i]->ID) {
                this->send_message(COORDINATOR, node_list[i], node_list[leader->index]);
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