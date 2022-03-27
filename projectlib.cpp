#include "projectlib.h"
#include <iostream>
#include <fstream>

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
    ifstream inFile("staffData.dat", ios::binary);
    StaffInfor tmpStaff;
    StudentInfor tmpStudent;
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&tmpStaff), sizeof(tmpStaff));
        if (username.compare(tmpStaff.username) == 0) {
            if (password.compare(tmpStaff.password) == 0) {
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
    inFile.open("studentData.dat", ios::binary);
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&tmpStudent), sizeof(tmpStudent));
        if (username.compare(tmpStudent.username) == 0) {
            if (password.compare(tmpStudent.password) == 0) {
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
    StaffInfor tmpStaff;
    StudentInfor tmpStudent;
    ifstream inFile;

    inFile.open("studentData.dat", ios::binary);
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&tmpStudent), sizeof(tmpStudent));
        if (username.compare(tmpStudent.username) == 0) {
            inFile.close();
            return false;
        }
    }
    inFile.close();

    inFile.open("staffData.dat", ios::binary);
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&tmpStaff), sizeof(tmpStaff));
        if (username.compare(tmpStaff.username) == 0) {
            inFile.close();
            return false;
        }
    }
    inFile.close();

    return true;
}

bool _checkCreatedYear(int begin, int end) {
    int tmpBegin, tmpEnd;
    ifstream inFile("schoolYear.txt");
    while (!inFile.eof()) {
        inFile >> tmpBegin >> tmpEnd;
        if (begin == tmpBegin && end == tmpEnd) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

bool _checkCreatedClass(string className) {
    string tmpClass;
    ifstream inFile("firstYearClasses.txt");
    while (!inFile.eof()) {
        inFile >> tmpClass;
        inFile.ignore();
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

int AddStudentToClass(DNodeClass* &pHead, string className) {
    DNodeClass* pCur = pHead;
    while (pCur && pCur->className.compare(className) != 0) 
        pCur = pCur->pNext;

    if (pCur == NULL) return 0;

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
    else {
        cout << "Could not open the file\n";
        return -1;
    }

    ofstream outFile("studentList.txt", ios::app);
    outFile << className << endl;

    pCur->StudentList = new DNodeStudent;
    pCur->StudentList = NULL;
    for(int i = 0; i < content.size(); i++) {
        Student tmp;
        tmp.No = stoi(content[i][0], 0, 10);
        tmp.StudentID = content[i][1];
        tmp.FName = content[i][2];
        tmp.LName = content[i][3];
        tmp.Gender = content[i][4];
        tmp.DoB = content[i][5];
        tmp.SocialID = content[i][6];

        outFile << tmp.No << " " << tmp.StudentID << " " << tmp.FName << " "; 
        outFile << tmp.LName << " " << tmp.Gender << " " << tmp.DoB << " " << tmp.SocialID << endl; 
        AddIn4Student(pCur->StudentList, tmp);
    }
    outFile << -1 << endl;
    
    outFile.close();
    return 1;
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

    if (_checkCreatedYear(x.begin, x.end)) {
        cout << "School year already existed!" << endl;
        return;
    }

    ofstream outFile("schoolYear.txt", ios::app);
    outFile << x.begin << " " << x.end << endl;
    outFile.close();

    AddYearAtTail(schoolYear, x);
    cout << "Create school year successfully!\n";
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
    cout << "Create class " << className << " successfully!\n";
}

void addStudent(DNodeClass* &newClasses) {
    cout << " --------------------------- " << endl;
    cout << "|   ADD 1ST YEAR STUDENTS   |" << endl;
    cout << " --------------------------- " << endl;
    cout << "Which class do you want to add students to?\n";
    cin.ignore(1000, '\n');
    string className;
    getline(cin, className);

    int tmp = AddStudentToClass(newClasses, className);
    if (tmp == 0) 
        cout << "Class doesn't exist. Enter again!\n";
    else if (tmp == 1) 
        cout << "Add students to class " << className << " successfully!\n";
}

/* void displayList(DNodeClass* newClasses) {
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
} */

void displayList() {
    ifstream inFile;
    inFile.open("studentList.txt");
    while (!inFile.eof()) {
        string className;
        getline(inFile, className);
        if (className.empty()) break;
        cout << "Class " << className << ": " << endl;

        DNodeClass* pCur = newClasses;
        while (pCur && pCur->className.compare(className) != 0)
            pCur = pCur->pNext;
        
        DNodeStudent* pNum = pCur->StudentList;
        string inforStudent;
        while (pNum != NULL) {
            getline(inFile, inforStudent);
            cout << inforStudent << endl;
            pNum = pNum->pNext;
        }
        int endClass;
        inFile >> endClass;
        inFile.ignore();
    }
    inFile.close();
}

void loadFileToLinkedList() {
    // load file school year:
    ifstream inFile("schoolYear.txt");
    SchoolYear tmp;
    while (inFile >> tmp.begin && inFile >> tmp.end)
        AddYearAtTail(schoolYear, tmp);
    inFile.close();

    // load file 1st-year classes:
    inFile.open("firstYearClasses.txt");
    string tmpClass;
    while (!inFile.eof()) {
        inFile >> tmpClass;
        inFile.ignore();
        AddClassAtTail(newClasses, tmpClass);
    }
    inFile.close();

    // load file student list:
    inFile.open("studentList.txt");
    while (!inFile.eof()) {
        getline(inFile, tmpClass);
        if (tmpClass.empty()) {
            inFile.close();
            break;
        }

        DNodeClass* pCur = newClasses;
        while (pCur && pCur->className.compare(tmpClass) != 0) 
            pCur = pCur->pNext;
        if (pCur == NULL) {
            inFile.close();
            break;
        }
    
        Student tmp;
        while (!inFile.eof()) {
            inFile >> tmp.No;
            if (tmp.No == -1) { // end class
                inFile.ignore();
                break;
            }
            inFile >> tmp.StudentID;
            inFile >> tmp.FName;
            inFile >> tmp.LName;
            inFile >> tmp.Gender;
            inFile >> tmp.DoB;
            inFile >> tmp.SocialID;
            AddIn4Student(pCur->StudentList, tmp);

            inFile.ignore();
        }
    }
    inFile.close();
}
void createSemester() {
    cout << " --------------------------- " << endl;
    cout << "|   CREATE NEW SEMESTER   |" << endl;
    cout << " --------------------------- " << endl;
    Semester s;
    cout << "Enter the semester(1,2 or 3): ";
    cin >> s.semester;
    cout << "Enter start date: ";
    cin >> s.startDate;
    cout << "Enter end date : ";
    cin >> s.endDate;
    cout << "Choose school year: ";
}

void createCourse() {
    cout << " --------------------------- " << endl;
    cout << "|   CREATE NEW COURSE   |" << endl;
    cout << " --------------------------- " << endl;
    Course c;
    cout << "Enter the start date of the course: ";
    cin >> c.startDate;
    cout << "Enter the end date of the course: ";
    cin >> c.endDate;
}
void addCourse() {
    ifstream in;
    in.open("course.txt");
    
    cout << " --------------------------- " << endl;
    cout << "|   ADD COURSE TO SEMESTER   |" << endl;
    cout << " --------------------------- " << endl;
    Course c;
    cout << "Course ID: ";
    getline(in,c.courseID);
    cout << c.courseID << endl;
    cout << "Course name: ";
    getline(in,c.courseName);
    cout << c.courseName << endl;
    cout << "Teacher name: ";
    getline(in,c.teacherName);
    cout << c.teacherName << endl;
    cout << "Credits: ";
    getline(in,c.credits);
    cout << c.credits << endl;
    cout << "First day of the week: ";
    getline(in,c.day1);
    cout << c.day1 << endl;
    cout << "Session: ";
    getline(in,c.session1);
    cout << c.session1 << endl;
    cout << "Second day of the week: ";
    getline(in,c.day2);
    cout << c.day2 << endl;
    cout << "Session: ";
    getline(in,c.session2);
    cout << c.session2 << endl;
    cout << "Maximum number of students in a course: " << 50 << endl;
    
    in.close();
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
    cout << "5. Create a semester" << endl;
    cout << "6. Create a course registration session\t\t";
    cout << "7. Add a course to the semester\t\t";
    cout << endl;
    do {
        cout << "Your choice: ";
        cin >> choice;
    } while ((choice[0] != '0' && choice[0] != '1' && choice[0] != '2' && choice[0] != '3' && choice[0] != '4' && choice[0] != '5' && choice[0] != '6' && choice[0] != '7') || choice.size() >= 2);
    // Add function below this;
    if (choice[0] == '0') {
        startMenu(isOff);
        return;
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
        displayList();
    } else if (choice[0] == '5') {

    } else if (choice[0] == '6') {

    } else if (choice[0] == '7') {

    }
    staffMenu(isOff);
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
        return;
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
            StaffInfor staff;
            staff.username = username;
            staff.password = password;
            staff.fullName = fullname;
            ofstream outFile("staffData.dat", ios::binary | ios::app);
            outFile.write(reinterpret_cast<char *>(&staff), sizeof(staff));
            outFile.close();
        } 
        else if (choice[0] == '2') {
            StudentInfor student;
            student.username = username;
            student.password = password;
            student.fullName = fullname;
            ofstream outFile("studentData.dat", ios::app);
            outFile.write(reinterpret_cast<char *>(&student), sizeof(student));
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