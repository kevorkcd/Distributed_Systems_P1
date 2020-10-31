#include "bully.hpp"

Bully::Bully(int ID) {
    online = true;
    this->ID = ID;
}

Bully::shutdown() {
    online = false;
}

Bully::bootup() {
    online = true;
}