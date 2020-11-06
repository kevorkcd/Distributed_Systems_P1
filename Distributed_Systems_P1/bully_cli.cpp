#include "bully_cli.hpp"

/**
 * TO-DO:
 * 1. Maybe extract from comparing IDs in Bully::receive() into Bully::compare()
 * 2. Implement threads OR make event based to detect leader down
 * 3. ls prints amount of nodes in all states
 * 4. Detect failure within failed node
 * 
 * TO-DO: Optional
 * 1. Implement nodes on threads
 * 2. Bully.run() function for thread
 */

int main(){
    
    BullyCLI* UI = new BullyCLI();
    UI->run();

    return 0;
}