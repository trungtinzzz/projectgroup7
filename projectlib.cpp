#include "projectlib.h"
#include <iostream>
#include <fstream>
using namespace std;

bool _checkSpace(string cstr) {
    for (int i = 0; i < cstr.size(); i++) {
        if (cstr[i] == ' ')
            return false;
    }

    return true;
} 

int _checkLogin(const string &username, const string &password) {
    ifstream inFile("teacherData.txt");
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
                return 1;
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
                return 0;
            }
            else {
                inFile.close();
                return 1;
            }
        }
    }
    inFile.close();
    return -1;
}

bool _checkSignUp(const string &username, string choice) {
    string tmpUsername, fileName;
    if (choice[0] == '1')
        fileName = "teacherData.txt";
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

void startMenu() {
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
}

void Login(bool &isSignUporLogined) {
    string username, password;
    int loginResult;
    
    cout << "Username: ";
    cin.ignore(1000, '\n');
    getline(cin, username);
    
    cout << "Password: ";
    getline(cin, password);

    loginResult = _checkLogin(username, password);
    if (loginResult == 0) {
        cout << "Dang nhap thanh cong" << endl;
        isSignUporLogined = true;
    }
    else if (loginResult == 1) {
        cout << "Mat khau khong dung" << endl;
    }
    else if (loginResult == -1) {
        cout << "Tai khoan khong ton tai" << endl;
    }
    
}

void SignUp(bool &isSignUporLogined) {
    string choice;
    string username, password,fullname;
    cout << "Ban la giao vu hay hoc sinh?" << endl;
    do {
        cout << "1. Giao vu\t\t";
        cout << "2. Hoc sinh" << endl;
        cout << "Lua chon: ";
        cin >> choice;
    }
    while ((choice[0] != '1' && choice[0] != '2') || choice.size() >= 2);
    
    cout << "Username: ";
    cin.ignore(1000, '\n');
    getline(cin, username);
    while (!_checkSpace(username)) {
        cout << "Khong duoc co khoang trang" << endl;
        cout << "Username: ";
        getline(cin, username);
    }
    if (_checkSignUp(username, choice)) {
        cout << "Passord: ";
        getline(cin, password);
        while (!_checkSpace(password)) {
            cout << "Khong duoc co khoang trang" << endl;
            cout << "Password: ";
            getline(cin, password);
        }
        cout << "Ten cua ban la: ";
        getline(cin, fullname);
        if (choice[0] == '1') {
            ofstream outFile("teacherData.txt", ios::app);
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
        isSignUporLogined = true;
        cout << "Dang ky thanh cong" << endl;
    }
    else {
        cout << "Tai khoan da ton tai" << endl;
    }
}
