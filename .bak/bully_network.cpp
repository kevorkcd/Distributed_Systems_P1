#include "bully_network.hpp"

Network::Network() {}

bool Network::is_ID_available(int ID) {
    for (auto& r : node_list) {
        if (ID == r->get_ID()) {
            return false;
        }
    }
    return true;
}

void Network::new_node(int new_ID) {
    if (is_ID_available(new_ID)) {
        Bully* temp = new Bully(new_ID);
        node_list.push_back(temp);
        cout << "Created a node with ID " << new_ID << endl;
    }
    else {
        cout << "ID " << new_ID << " is already taken" << endl;
    }
}

void Network::shutdown_node(int ID){
    int max_index = node_list.size() - 1;

    if(ID < 0  || ID > max_index){
        cout << "Node with ID " << ID << " is not avaliable." << endl;
    }
    else{
        node_list[ID]->shutdown();
    }
}