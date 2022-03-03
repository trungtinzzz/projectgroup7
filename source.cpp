#include <iostream>
#include <fstream>
#include "projectlib.h"
using namespace std;

int main() {
    string choice;
    cout << " -------------------- " << endl;
    cout << "|   GROUP 7 MOODLE   |" << endl;
    cout << " -------------------- " << endl;
    bool isSignUporLogined = false;
    while (!isSignUporLogined) {
        cout << "1. Dang ky\t\t";
        cout << "2. Dang nhap" << endl;
        do {
            cout << "Lua chon: ";
            cin >> choice;
        }
        while ((choice[0] != '1' && choice[0] != '2') || choice.size() >= 2);

        if (choice[0] == '1') {
            SignUp(isSignUporLogined);
        }
        else {
            Login(isSignUporLogined);
        }
    }
    return 0;
}