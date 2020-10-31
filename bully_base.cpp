#include "bully.hpp"

class BullyBase : Bully {
    public:
        
};

void Bully::run() {
    while (online) {
        if (!node_list[current_leader].is_online()) {
            for (int i = ID+1; i < node_list.size(); i++) {
                this->election(node_list[i]);
            }
        }
    }
    return;
}

Bully::Bully(int ID) {
    online = true;
    this->ID = ID;
}

void Bully::election(int ID){
    election_in_progress = true;
    this->OK(node_list[this->ID]);
}

void Bully::OK(int ID) {

}

void Bully::coordinator(){
    
}