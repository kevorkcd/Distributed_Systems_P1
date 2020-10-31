#include "bully.hpp"

class BullyBase : public Bully {
    public:
        BullyBase();
        BullyBase(int ID);
        void run() override;
};

void BullyBase::run() {
    while (online) {
        if (!node_list[current_leader]->is_online()) {
            for (int i = ID+1; i < node_list.size(); i++) {
                this->election(node_list[i]->get_ID());
            }
        }
    }
    return;
}

BullyBase::BullyBase() {}

BullyBase::BullyBase(int ID) {
    online = true;
    this->ID = ID;
}