#include "projectlib.h"
#include <iostream>
#include <fstream>
using namespace std;

void _printGroupLogo() {
    cout << " -------------------- " << endl;
    cout << "|   GROUP 7 MOODLE   |" << endl;
    cout << " -------------------- " << endl;
}

bool _checkSpace(const string &cstr) {
    for (int i = 0; i < cstr.size(); i++) {
        if (cstr[i] == ' ')
            return false;
    }

    return true;
} 

int _checkLogin(const string &username, const string &password) {
    ifstream inFile("staffData.txt");
    string tmpUsername, tmpPassword;
    while (!inFile.eof()) {
        inFile >> tmpUsername;
        inFile >> tmpPassword;
        inFile.ignore(1000, '\n');
        if (username.compare(tmpUsername) == 0) {
            if (password.compare(tmpPassword) == 0) {
                inFile.close();
                return 0;
            }
            else {
                inFile.close();
                return -1;
            }
        }
    }
    inFile.close();
    inFile.open("studentData.txt");
    while (!inFile.eof()) {
        inFile >> tmpUsername;
        inFile >> tmpPassword;
        inFile.ignore(1000, '\n');
        if (username.compare(tmpUsername) == 0) {
            if (password.compare(tmpPassword) == 0) {
                inFile.close();
                return 1;
            }
            else {
                inFile.close();
                return -1;
            }
        }
    }
    inFile.close();
    return -2;
}

bool _checkSignUp(const string &username, const string &choice) {
    string tmpUsername, fileName;
    if (choice[0] == '1')
        fileName = "staffData.txt";
    else
        fileName = "studentData.txt";
    ifstream inFile(fileName);
    while (!inFile.eof()) {
        inFile >> tmpUsername;
        inFile.ignore(1000, '\n');
        inFile.ignore(1000, '\n');
        if (username.compare(tmpUsername) == 0) {
            inFile.close();
            return false;
        }
    }
    inFile.close();
    return true;
}

void staffMenu(bool &isOff) {
    cout << " -------------------- " << endl;
    cout << "|     STAFF MENU     |" << endl;
    cout << " -------------------- " << endl;
    string choice;
    cout << "0. Quit\t\t";
    // Add option here
    cout << endl;
    do {
        cout << "Your choice: ";
        cin >> choice;
    } while (choice[0] != '0' || choice.size() >= 2);
    // Add function below this;
    if (choice[0] == '0') {
        isOff = true;
    }
}

void studentMenu(bool &isOff) {
    cout << " -------------------- " << endl;
    cout << "|    STUDENT MENU    |" << endl;
    cout << " -------------------- " << endl;
    string choice;
    cout << "0. Quit\t\t";
    // Add option here
    cout << endl;
    do {
        cout << "Your choice: ";
        cin >> choice;
    } while (choice[0] != '0' || choice.size() >= 2);
    // Add function below this;
    if (choice[0] == '0') {
        isOff = true;
    }
}

void _login(bool &isOff) {
    string username, password;
    int loginResult;
    
    cout << "Username: ";
    cin.ignore(1000, '\n');
    getline(cin, username);
    
    cout << "Password: ";
    getline(cin, password);

    loginResult = _checkLogin(username, password);
    if (loginResult == 0) {
        cout << "Login successfully" << endl;
        staffMenu(isOff);
    }
    else if (loginResult == 1) {
        cout << "Login successfully" << endl;
        studentMenu(isOff);
    }
    else if (loginResult == -1) {
        cout << "Incorrect password" << endl;
    }
    else if (loginResult == -2) {
        cout << "Account does not exist" << endl;
    }
}

void _signUp() {
    string choice;
    string username, password,fullname;
    cout << "Are you academic staff or student?" << endl;
    do {
        cout << "1. Academic\t\t";
        cout << "2. Student" << endl;
        cout << "Your choice: ";
        cin >> choice;
    }
    while ((choice[0] != '1' && choice[0] != '2') || choice.size() >= 2);
    
    cout << "Username: ";
    cin.ignore(1000, '\n');
    getline(cin, username);
    while (!_checkSpace(username)) {
        cout << "Space is not accepted" << endl;
        cout << "Username: ";
        getline(cin, username);
    }
    if (_checkSignUp(username, choice)) {
        cout << "Passord: ";
        getline(cin, password);
        while (!_checkSpace(password)) {
            cout << "Space is not accepted" << endl;
            cout << "Password: ";
            getline(cin, password);
        }
        cout << "Your name: ";
        getline(cin, fullname);
        if (choice[0] == '1') {
            ofstream outFile("staffData.txt", ios::app);
            outFile << username << endl;
            outFile << password << endl;
            outFile << fullname << endl;
            outFile.close();
        } 
        else if (choice[0] == '2') {
            ofstream outFile("studentData.txt", ios::app);
            outFile << username << endl;
            outFile << password << endl;
            outFile << fullname << endl;
            outFile.close();
        }
        cout << "Sign up successfully" << endl;
    }
    else {
        cout << "Account already existed" << endl;
    }
}

void startMenu(bool &isOff) {
    string choice;
    _printGroupLogo();
    
    cout << "0. Quit\t\t";
    cout << "1. Sign up\t\t";
    cout << "2. Log in\t\t" << endl;
    do {
        cout << "Your choice: ";
        cin >> choice;
    }
    while ((choice[0] != '1' && choice[0] != '2' && choice[0] != '0') || choice.size() >= 2);

    if (choice[0] == '1') {
        _signUp();
    }
    else if (choice[0] == '2') {
        _login(isOff);
    }
    else {
        isOff = true;
    }

}