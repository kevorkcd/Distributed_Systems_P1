#include "bully_base.cpp"

int main(){

    //vector<Bully*> network;

    BullyBase* network = new BullyBase();

    for(int i = 1; i < 5; i++){
        BullyBase* temp = new BullyBase(i);
        network->node_list->push_back(temp);
    }
    
    network->node_list[4]->shutdown();

    return 0;
}