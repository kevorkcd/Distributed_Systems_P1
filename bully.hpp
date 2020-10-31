#include<iostream>
#include<vector>

using namespace std;

enum message {ELECTION=0, OK, COORDINATOR};
enum state   {OFFLINE=0, ONLINE, IN_ELECTION, LEADER};

class Bully {
    private:
        static int message_no;
        static int curr_leader_ID;
        static vector<Bully> node_list;
        int ID;
        state st;

    public:
        Bully(int ID);
        void send_message(message msg, Bully* receiver);
        void receive(message msg);
        void set_state(state st);
        int get_ID();
        int get_leader();
};

// Temporary test method
int Bully::get_leader() {
    return curr_leader_ID;
}

int Bully::message_no = 0;
int Bully::curr_leader_ID = -1;

Bully::Bully(int ID) {
    this->st = ONLINE;
    this->ID = ID;
}

int Bully::get_ID() {
    return this->ID;
}

void Bully::set_state(state st) {
    this->st = st;
}

void Bully::send_message(message msg, Bully* receiver) {
    if (receiver->st == ONLINE) {
        message_no++;
        if (msg == COORDINATOR) {
            curr_leader_ID = this->ID;
        }
        receiver->receive(msg);
    }
    return;
}

void Bully::receive(message msg) {
    switch(msg) {
        case ELECTION:
            this->st = IN_ELECTION;
            this->send_message(OK, this);
            cout << "ID: " << this->ID << " received ELECTION" << endl;
            break;
        case OK:
            this->st = ONLINE;
            cout << "ID: " << this->ID << " received OK" << endl;
            break;
        case COORDINATOR:
            this->st = ONLINE;
            cout << "ID: " << this->ID << " received COORDINATOR" << endl;
            break;
        default:
            break;
    }
}