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

        Bully() {};
        void _raise_election();

    public:
        void construct_leader_info();
        Bully(int ID);
        void run();
        void send_message(message msg, Bully* receiver);
        void receive(message msg, Bully* sender);
        void take_over_election();
        void initiate_election();
        void fail();
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
    if (is_ID_avalailable) {                        // ID is available, add this node to list
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
            if (!(this->st <= OFFLINE)) {
                this->m_election_perm.unlock();
                this->m.lock();
                this->st = ONLINE;
                this->m.unlock();
                this->initiate_election();   
            }
        }
    }
}

// This is the gate for the node to send messages to other nodes
void Bully::send_message(message msg, Bully* receiver) {
    if (receiver->responsive.try_lock()) {
        receiver->responsive.unlock();
        while (this->st == TIMEOUT) {}

        if (this->st >= ONLINE) {   // Only increase message number and send message if this node is "online"
            this->m.lock();
            message_no++;
            this->m.unlock();

            receiver->receive(msg, this);
        }
    }
}

// Here the node receives a msg and does something according to the message
void Bully::receive(message msg, Bully* sender) {
    if (this->responsive.try_lock()) {
        this->responsive.unlock();
        if (this->st >= TIMEOUT) {                  // Have to be "online" to recieve a message
            switch(msg) {
                case ELECTION:                      // If message is Election
                    cout << sender->ID << " -> " << this->ID << " ELECTION" << endl;
                    this->send_message(OK, sender); // Send OK to sender
                    if (this->st >= ONLINE) {       // Only take over election if node is online (this check could be removed as it's already done earlier?)
                        this->take_over_election();
                    }
                    break;
                case OK:                            // If message is OK
                    cout << sender->ID << " -> " << this->ID << " OK" << endl;
                    this->m.lock();
                    if (!(this->st <= OFFLINE)) {   // Set state as online
                        this->st = TIMEOUT;
                    }
                    this->m.unlock();
                    break;
                case COORDINATOR:                   // If message is COORDINATOR
                    cout << sender->ID << " -> " << this->ID << " COORDINATOR" << endl;
                    this->m.lock();
                    if (!(this->st <= OFFLINE)) {
                        this->st = TIMEOUT;
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
        cout << this->ID << " initiate election." << endl;
        _raise_election();
    }
}

void Bully::take_over_election() {
    if (this->m_election_perm.try_lock()) {
        this->m_election_perm.unlock();
        cout << this->ID << " taking over election." << endl;
        _raise_election();
    }
}

void Bully::_raise_election() {
    
    this->m.lock();             // Set own state to ELECTION before continuing, otherwise wait till it's possible.
    this->st = IN_ELECTION;     
    this->m.unlock();

    for (int i = 0; i < node_list.size(); i++) {                            // Why don't we check for ONLINE state and election permission here, and instead do it in the next line?
        if (this->st >= ONLINE && this->m_election_perm.try_lock()) {       // If node has election permission, send message ELECTION to all nodes with larger ID's
            this->m_election_perm.unlock();
            if (node_list[i]->ID > this->ID) {
                this->send_message(ELECTION, node_list[i]);
            }
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(250));            // Waiting  0.25s for for replies
    if (this->st == IN_ELECTION) {                                          // If still in election state, that means no one has replied OK, since that would change node state to ONLINE. - then self assign LEADER
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

        for (int i = 0; i < node_list.size(); i++) {            // Update all nodes about who's the new leader
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