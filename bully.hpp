#include<iostream>
#include<vector>

using namespace std;

enum message {ELECTION=0, OK, COORDINATOR};
enum state   {OFFLINE=0, ONLINE, IN_ELECTION, LEADER};

class Bully {
    private:
        static int message_no;
        int ID;
        state st;

        int curr_leader_ID;
    public:
        Bully(int ID);
        void send_message(message msg, Bully* receiver);
        void received(message msg);
        void set_state(state st);
        int get_ID();
};

int Bully::message_no = 0;

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
    if (this->st == ONLINE) {
        message_no++;
        switch(msg) {
            case ELECTION:
                receiver->received(msg);
                receiver->set_state(IN_ELECTION);
                receiver->send_message(OK, this);
                break;
            case OK:
                receiver->received(msg);
                receiver->set_state(ONLINE);
                break;
            case COORDINATOR:
                receiver->received(msg);
                receiver->set_state(ONLINE);
                curr_leader_ID = this->ID;
                break;
            default:
                break;
        }
    }
    return;
}

void Bully::received(message msg) {
    switch(msg) {
        case ELECTION:
            cout << "ID: " << this->ID << " received ELECTION" << endl;
            break;
        case OK:
            cout << "ID: " << this->ID << " received OK" << endl;
            break;
        case COORDINATOR:
            cout << "ID: " << this->ID << " received COORDINATOR" << endl;
            break;
        default:
            break;
    }
}