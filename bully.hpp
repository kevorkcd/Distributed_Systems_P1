#include <thread>

using namespace std;

class Bully {
    private:
        void run();

    protected:
        static vector<Bully*> node_list;
        int current_leader;
        bool online;
        bool election_in_progress;
        int ID;

    public:
        Bully(int ID);

        bool is_online(){return online;}    // Return referenced node state

        // Election messages
        virtual void election();        // Sends election message to Object
        virtual void OK();              // Replies to election message that it's heard
        virtual void coordinator();     // Notifies all instances of 
        int get_ID()    {return ID;}
        void shutdown() {online = false;}
        void bootup()   {online = true;}
};