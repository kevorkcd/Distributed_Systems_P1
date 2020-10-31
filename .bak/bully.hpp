#include <thread>
#include <vector>

//class Network;

using namespace std;

class Bully {
    protected:
        //friend class Network;

        int current_leader;
        bool online;
        bool election_in_progress;
        int ID;
        virtual void run() = 0;

    public:
        static vector<Bully*> node_list;

        bool is_online(){return online;}    // Return referenced node state
        
        // Election messages
        void election(int ID);        // Sends election message to Object
        void OK(int ID);              // Replies to election message that it's heard
        void coordinator(int ID);     // Notifies all instances of 
        int get_ID()    {return ID;}
        void shutdown() {online = false;}
        void bootup()   {online = true;}
};

void Bully::election(int ID) {
    election_in_progress = true;
    node_list[ID]->OK(this->ID);
}

void Bully::OK(int ID) {

}

void Bully::coordinator(int ID){
    //current_leader = /*leader id*/;

}