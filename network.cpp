#include "bully_network.hpp"

Network::Network() {}

void Network::new_node() {
    int max_id = node_list.size();
    Bully* temp = new Bully(max_id);
    node_list.push_back(temp);
}

void Network::shutdown_node(int ID){
    int max_index = node_list.size() - 1;

    if(ID < 0  || ID > max_index){
        cout << "Node with ID" << ID << "is not avaliable." << endl;
    }
    else{
        node_list[ID]->shutdown();
    }
}