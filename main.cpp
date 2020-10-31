#include "bully.hpp"

int main() {
    Bully* b0 = new Bully(0);
    Bully* b1 = new Bully(1);

    b0->send_message(ELECTION, b1);

    return 0;
}