#include "projectlib.h"

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
    cout << "Do you want to create a school year?\n";
    cout << "1. Yes\n2. No\n";
    int choice;
    do {
        cout << "Your choice: ";
        cin >> choice;
    } while (choice != 1 && choice != 2);
    
    if (choice == 2) return;

    cout << "\n  CREATE SCHOOL YEAR\n";
    SchoolYear x;
    cout << "Enter year begin: ";
    cin >> x.begin;
    cout << "Enter year end: ";
    cin >> x.end;

    AddYearAtTail(schoolYear, x);
    cout << "Create school year successfully!\n\n";
}

void createClass(DNodeClass* &newClasses) {
    int choice;
    do {
        cout << "Do you want to create 1st-year class?\n";
        cout << "1. Yes\n2. No\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 1) {
            string className;
            cout << "Enter class name: \n";
            cin.ignore(1000, '\n');
            getline(cin, className);

            AddClassAtTail(newClasses, className);
            cout << "Create class " << className << " successfully!\n\n";
        } else cout << endl;
    } while (choice != 2);
}

void addStudent(DNodeClass* &newClasses) {
    int choice;
    do {
        cout << "Do you want to add new students to 1st-year class?\n";
        cout << "1. Yes\n2. No\n";
        cout << "Your choice: ";
        cin >> choice;
        if (choice == 1) {
            cout << "Which class do you want to add students to?\n";
            cin.ignore(1000, '\n');
            string className;
            getline(cin, className);

            if (AddStudentToClass(newClasses, className) == false) 
                cout << "Class doesn't exist. Enter again!\n\n";
            else cout << "Add students to class " << className << " successfully!\n\n";
        } else cout << endl;
    } while (choice != 2);
}