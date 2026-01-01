#include <iostream>
#include <string>
#include <vector>

#include "../include/WorldObjects.h"

using namespace std;

WorldObjects::WorldObjects(const string& objectID, int x, int y, char g):id(objectID), pos{x, y}, glyph(g) {
    cout << "World Object Created (" << id << ")" << endl;
}

WorldObjects :: ~WorldObjects() {

}

const string& WorldObjects::gedId() const {
    return id;
}

Position WorldObjects::getPosition() const {
    return pos;
}



