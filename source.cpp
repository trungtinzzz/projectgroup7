#include <iostream>
#include <fstream>
#include "projectlib.h"
using namespace std;

int main() {
    char choice;
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
        while (choice != '1' && choice != '2');

        if (choice == '1') {
            SignUp(isSignUporLogined);
        }
        else {
            Login(isSignUporLogined);
        }
    }
    return 0;
}