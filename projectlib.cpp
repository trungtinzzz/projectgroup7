#include "projectlib.h"
#include <iostream>
#include <fstream>
using namespace std;

DNodeSYear *schoolYear = nullptr;
DNodeClass *newClasses = nullptr;

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
        inFile.ignore();
        inFile >> tmpPassword;
        inFile.ignore();
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
        inFile.ignore();
        inFile >> tmpPassword;
        inFile.ignore();
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

bool _checkSignUp(const string &username) {
    string tmpUsername;
    ifstream inFile;

    inFile.open("studentData.txt");
    while (!inFile.eof()) {
        inFile >> tmpUsername;
        inFile.ignore();
        inFile.ignore(1000, '\n');
        inFile.ignore(1000, '\n');
        if (username.compare(tmpUsername) == 0) {
            inFile.close();
            return false;
        }
    }
    inFile.close();

    inFile.open("staffData.txt");
    while (!inFile.eof()) {
        inFile >> tmpUsername;
        inFile.ignore();
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

bool _checkCreatedClass(string className) {
    string tmpClass;
    ifstream inFile;

    inFile.open("firstYearClasses.txt");
    while (!inFile.eof()) {
        getline(inFile, tmpClass);
        cout << tmpClass << endl;
        if (className.compare(tmpClass) == 0) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

void AddYearAtTail(DNodeSYear* &pHead, SchoolYear x) {
    DNodeSYear* pNew = new DNodeSYear;
    DNodeSYear* pTail = pHead;

    pNew->data = x;
    pNew->pNext = NULL;

    if (pHead == NULL) {
        pNew->pPrev = NULL;
        pHead = pNew;
        return;
    }

    while (pTail->pNext != NULL) pTail = pTail->pNext;
    pTail->pNext = pNew;
    pNew->pPrev = pTail;
}

void AddClassAtTail(DNodeClass* &pHead, string className) {
    DNodeClass* pNew = new DNodeClass;
    DNodeClass* pTail = pHead;

    pNew->className = className;
    pNew->pNext = NULL;

    if (pHead == NULL) {
        pNew->pPrev = NULL;
        pHead = pNew;
        return;
    }

    while (pTail->pNext != NULL) pTail = pTail->pNext;
    pTail->pNext = pNew;
    pNew->pPrev = pTail;
}

void AddIn4Student(DNodeStudent* &pHead, Student x) {
    DNodeStudent* pNew = new DNodeStudent;
    DNodeStudent* pTail = pHead;

    pNew->data = x;
    pNew->pNext = NULL;

    if (pHead == NULL) {
        pNew->pPrev = NULL;
        pHead = pNew;
        return;
    }

    while (pTail->pNext != NULL) pTail = pTail->pNext;
    pTail->pNext = pNew;
    pNew->pPrev = pTail;
}

bool AddStudentToClass(DNodeClass* &pHead, string className) {
    DNodeClass* pCur = pHead;
    while (pCur && pCur->className.compare(className) != 0) 
        pCur = pCur->pNext;

    if (pCur == NULL) return false;

    /* Import CSV file has form:
        No, Student ID, First name, Last name, Gender, Date of Birth, Social ID
        1, 21127155, Fan, Quin, Female, 08/06/2003, 123456
        2, 21127001, John, Henry, Male, 01/01/2003, 654321
        ...
        Each of the in4's student in each line is separated by a comma
    */
    string fileName;
    cout << "Enter the file name: ";
    cin >> fileName;
    
    vector< vector<string> > content;
    vector<string> row;
    string line, word;

    fstream file(fileName, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }
    else cout << "Could not open the file\n";

    pCur->StudentList = new DNodeStudent;
    pCur->StudentList = NULL;
    for(int i = 0; i < content.size(); i++) {
        Student tmp;
        tmp.No = stoi(content[i][0], 0, 10);
        tmp.StudentID = stoi(content[i][1], 0, 10);
        tmp.FName = content[i][2];
        tmp.LName = content[i][3];
        tmp.Gender = content[i][4];
        tmp.DoB = content[i][5];
        tmp.SocialID = stoi(content[i][6], 0, 10);

        AddIn4Student(pCur->StudentList, tmp);
    }
    return true;
}


void createSchoolYear(DNodeSYear* &schoolYear) {
    cout << " ------------------------ " << endl;
    cout << "|   CREATE SCHOOL YEAR   |" << endl;
    cout << " ------------------------ " << endl;
    SchoolYear x;
    cout << "Enter year begin: ";
    cin >> x.begin;
    cout << "Enter year end: ";
    cin >> x.end;

    ofstream outFile("schoolYear.txt", ios::app);
    outFile << x.begin << " " << x.end << endl;
    outFile.close();

    AddYearAtTail(schoolYear, x);
    cout << "Create school year successfully!\n\n";
}

void createClass(DNodeClass* &newClasses) {
    cout << " --------------------------- " << endl;
    cout << "|   CREATE 1ST YEAR CLASS   |" << endl;
    cout << " --------------------------- " << endl;
    string className;
    cout << "Enter class name: \n";
    cin.ignore(1000, '\n');
    getline(cin, className);

    if (_checkCreatedClass(className)) {
        cout << "Class already existed!" << endl;
        return;
    }

    ofstream outFile("firstYearClasses.txt", ios::app);
    outFile << className << endl;
    outFile.close();

    AddClassAtTail(newClasses, className);
    cout << "Create class " << className << " successfully!\n\n";
}

void addStudent(DNodeClass* &newClasses) {
    cout << " --------------------------- " << endl;
    cout << "|   ADD 1ST YEAR STUDENTS   |" << endl;
    cout << " --------------------------- " << endl;
    cout << "Which class do you want to add students to?\n";
    cin.ignore(1000, '\n');
    string className;
    getline(cin, className);

    if (AddStudentToClass(newClasses, className) == false) 
        cout << "Class doesn't exist. Enter again!\n\n";
    else cout << "Add students to class " << className << " successfully!\n\n";
}

void displayList(DNodeClass* newClasses) {
    DNodeClass* pCur = newClasses;
    while (pCur != NULL) {
        cout << "Class: " << pCur->className << endl;
        DNodeStudent* pStCur = pCur->StudentList;
        while (pStCur != NULL) {
            cout << pStCur->data.No << " " << pStCur->data.StudentID << " ";
            cout << pStCur->data.FName << " " << pStCur->data.LName << " ";
            cout << pStCur->data.Gender << " " << pStCur->data.DoB << " ";
            cout << pStCur->data.SocialID << endl;
            pStCur = pStCur->pNext;
        }
        cout << endl;
        pCur = pCur->pNext;
    }
}

void staffMenu(bool &isOff) {
    cout << " -------------------- " << endl;
    cout << "|     STAFF MENU     |" << endl;
    cout << " -------------------- " << endl;
    string choice;
    cout << "0. Sign out\t\t";
    // Add option here
    cout << "1. Create school year\t\t";
    cout << "2. Create 1st-year class" << endl;
    cout << "3. Add new 1st-year students to 1st-year class\t\t";
    cout << "4. Display list of students\t\t";
    cout << endl;
    do {
        cout << "Your choice: ";
        cin >> choice;
    } while ((choice[0] != '0' && choice[0] != '1' && choice[0] != '2' && choice[0] != '3' && choice[0] != '4') || choice.size() >= 2);
    // Add function below this;
    if (choice[0] == '0') {
        startMenu(isOff);
    } else if (choice[0] == '1') {
        // Create school year
        createSchoolYear(schoolYear);
    } else if (choice[0] == '2') {
        // Create 1st-year classes
        createClass(newClasses);
    } else if (choice[0] == '3') {
        // Add new 1st-year student to 1st-year classes
        addStudent(newClasses);
    } else if (choice[0] == '4') {
        // Display list of student
        displayList(newClasses);
    }
}

void studentMenu(bool &isOff) {
    cout << " -------------------- " << endl;
    cout << "|    STUDENT MENU    |" << endl;
    cout << " -------------------- " << endl;
    string choice;
    cout << "0. Sign out\t\t";
    // Add option here
    cout << endl;
    do {
        cout << "Your choice: ";
        cin >> choice;
    } while (choice[0] != '0' || choice.size() >= 2);
    // Add function below this;
    if (choice[0] == '0') {
        startMenu(isOff);
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
        cout << "1. Academic staff\t\t";
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
    if (_checkSignUp(username)) {
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
            outFile << endl;
            outFile << username << endl;
            outFile << password << endl;
            outFile << fullname << static_cast<char>(8);
            outFile.close();
        } 
        else if (choice[0] == '2') {
            ofstream outFile("studentData.txt", ios::app);
            outFile << endl;
            outFile << username << endl;
            outFile << password << endl;
            outFile << fullname << static_cast<char>(8);
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