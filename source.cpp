#include "projectlib.h"

int main() {
    loadFileToLinkedList();
    bool isOff = false;
    while (!isOff) {
        startMenu(isOff);
    }
    return 0;
}
