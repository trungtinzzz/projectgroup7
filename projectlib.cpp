#include "projectlib.h"

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

int _checkLoginForStaff(const string &username, const string &password) {
    string fileName = "staffFile/" + username + ".dat";
    ifstream inFile(fileName, ios::binary);
    StaffInfor tmpStaff;

    if (inFile.fail()) {
        inFile.close();
        return -2;
    }

    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&tmpStaff), sizeof(tmpStaff));
        if (inFile.eof()) break;
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

    return -2;
}

int _checkLoginForStudent(const string &username, const string &password, StudentInfor &studentinfor) {
    ifstream inFile;
    StudentInfor tmpStudent;
    
    string fileName = "studentfile/" + username + "_data.dat";

    inFile.open(fileName, ios::binary);
    if (inFile.fail()) {
        inFile.close();
        return -2;
    }
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&tmpStudent), sizeof(tmpStudent));
        if (inFile.eof()) break;
        if (username.compare(tmpStudent.studentID) == 0) {
            if (password.compare(tmpStudent.password) == 0) {
                studentinfor = tmpStudent;
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

    return -2;
}

bool _checkSignUp(const string &username) {
    StaffInfor tmpStaff;
    ifstream inFile;
    string fileName = "staffFile/" + username + ".dat";
    inFile.open(fileName, ios::binary);
    if (inFile.fail())
        return true;
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

bool _checkCreatedYear(int begin) {
    SchoolYear tmp;
    ifstream inFile("dataFile/schoolYear.dat", ios::binary);
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&tmp), sizeof(tmp));
        if (begin == tmp.begin) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
} 

bool _checkCreatedClass(string className) {
    string tmpClass;
    ifstream inFile("dataFile/firstYearClasses.dat", ios::binary);
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&tmpClass), sizeof(tmpClass));
        if (className.compare(tmpClass) == 0) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

bool _checkCreatedCourse(string fileName, string courseID) {
    ifstream inFile(fileName, ios::binary);
    Course tmpCourse;
    if (inFile.fail()) {
        inFile.close();
        return false;
    }
    else {
        while (!inFile.eof()) {
            inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
            if (inFile.eof()) break;
            if (courseID.compare(tmpCourse.courseID) == 0) {
                inFile.close();
                return true;
            } 
        }
    }
    inFile.close();
    return false;
}

bool _checkConflictedSession(string fileCourseName, string courseID, string fileStudentName) {
    ifstream inFile(fileCourseName, ios::binary);
    Course tmpCourse;
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
        if (tmpCourse.courseID == courseID) break;
    }
    inFile.close();

    inFile.open(fileStudentName, ios::binary);
    if (!inFile.fail()) {
        Course tmpEnrolledCourse;
        while (!inFile.eof()) {
            inFile.read(reinterpret_cast<char*>(&tmpEnrolledCourse), sizeof(tmpEnrolledCourse));
            if (tmpCourse.courseID.compare(tmpEnrolledCourse.courseID) == 0) return true;
            if (tmpCourse.day1.compare(tmpEnrolledCourse.day1) == 0 && tmpCourse.session1.compare(tmpEnrolledCourse.session1) == 0) return true;
            if (tmpCourse.day2.compare(tmpEnrolledCourse.day2) == 0 && tmpCourse.session2.compare(tmpEnrolledCourse.session2) == 0) return true;
        }
    }
    inFile.close();
    return false;
}

int _findSemesterOfCourse(string courseID) {
    SchoolYear tmpSchoolyear;
    Course tmpCourse;
    ifstream inFile("dataFile/schoolYear.dat", ios::binary);
    string fileCourseName;
    if (inFile.fail()) {
        cout << "fail\n";
        return 0;
    }
    else { 
        while(!inFile.eof()) {
            inFile.read(reinterpret_cast<char *>(&tmpSchoolyear), sizeof(tmpSchoolyear));
            if (inFile.eof()) break;
            
            for (int i = 1; i <= 3; i++) {
                fileCourseName = "courselist/" + to_string(tmpSchoolyear.begin) + to_string(tmpSchoolyear.end) + "_" + to_string(i) + ".dat";
                ifstream inCourseFile(fileCourseName, ios::binary);
                if (!inCourseFile.fail()) {
                    while (!inCourseFile.eof()) {
                        inCourseFile.read(reinterpret_cast<char *>(&tmpCourse), sizeof(tmpCourse));
                        if (inCourseFile.eof()) break;

                        if (courseID.compare(tmpCourse.courseID) == 0) {
                            inCourseFile.close();
                            return i;
                        }
                    }
                }
                inCourseFile.close();
            }
        }
    }
    inFile.close();
    return -1;
}

int numOfEnrolledCourses(string fileStudentName, int schoolyearBegin, string semester) {
    ifstream inFile(fileStudentName, ios::binary);
    Course tmpCourse;
    int count = 0;
    if (inFile.fail())
        return count;
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
        if (tmpCourse.schoolyearBegin == schoolyearBegin && tmpCourse.semester == semester[0] - '0') 
            count++;
    }

    inFile.close();
    return count;
}

void createSchoolYear() {
    cout << " ------------------------ " << endl;
    cout << "|   CREATE SCHOOL YEAR   |" << endl;
    cout << " ------------------------ " << endl;
    SchoolYear x;
    while (true) {
        cout << "Enter year begin: ";
        string tmp;
        getline(cin, tmp);
        if(tmp.size() == 4 && tmp[0] >= '0' && tmp[0] <= '9' && tmp[1] >= '0' && tmp[1] <= '9'
            && tmp[2] >= '0' && tmp[2] <= '9' && tmp[3] >= '0' && tmp[3] <= '9') {
                x.begin = atoi(tmp.c_str());
                break;
        }
        else cout << "Invalid input. Try again!\n";
    }
    x.end = x.begin + 1;
    if (_checkCreatedYear(x.begin)) {
        cout << "School year already existed!" << endl;
        return;
    }

    ofstream outFile("dataFile/schoolYear.dat", ios::app | ios::binary);
    outFile.write(reinterpret_cast<char*>(&x), sizeof(x));
    outFile.close();

    cout << "Create school year successfully!\n";
}

void createClass() {
    cout << " --------------------------- " << endl;
    cout << "|   CREATE 1ST YEAR CLASS   |" << endl;
    cout << " --------------------------- " << endl;
    string className;
    cout << "Enter class name: ";
    getline(cin, className);

    if (_checkCreatedClass(className)) {
        cout << "Class already existed!" << endl;
        return;
    }

    ofstream outFile("dataFile/firstYearClasses.dat", ios::app | ios::binary);
    outFile.write(reinterpret_cast<char*>(&className), sizeof(className));
    outFile.close();

    cout << "Create class " << className << " successfully!\n";
    string fileName = "studentlist/" + className + ".dat";
    outFile.open(fileName, ios::binary | ios::app);
    outFile.close();
}

void addStudent() {
    cout << " --------------------------- " << endl;
    cout << "|   ADD 1ST YEAR STUDENTS   |" << endl;
    cout << " --------------------------- " << endl;

    string className;
    cout << "Which class do you want to add students to?" << endl;
    getline(cin, className);

    if (!_checkCreatedClass(className)) {
        cout << "Class doesn't exist.\n";
        return;
    }
    
    string fileName = "studentlist/" + className + ".dat";
    ifstream testFile(fileName, ios::binary);
    char test;
    testFile.read(&test, 1);
    if (!testFile.eof()) {
        cout << "You have already add student to this class!!!" << endl;
        testFile.close();
        return;
    }
    testFile.close();

    ofstream outFile(fileName, ios::binary);

    string csvFileName;
    while (true) {
        cout << "Enter the file name: ";
        getline(cin, csvFileName);
        if (_checkSpace(csvFileName)) break;
        else cout << "Invalid input. Try again!\n";
    }
    
    string data[1000][7];
    string line, word;

    csvFileName = "importedStudentFile/" + csvFileName;

    fstream file(csvFileName, ios::in);
    file.ignore(1000, '\n');
    int n = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream str(line);

            int j = 0;
            while (getline(str, word, ',')) {
                data[n][j] = word;
                j++;
            }
            n++;
        }
        file.close();
    }
    else {
        cout << "Could not open the file\n";
        file.close();
        return;
    }

    for(int i = 0; i < n; i++) {
        StudentInfor tmp;
        tmp.no = data[i][0];
        tmp.studentID = data[i][1];
        tmp.fullName = data[i][2];
        tmp.gender = data[i][3];
        tmp.DoB = data[i][4];
        tmp.socialID = data[i][5];
        tmp.password = data[i][6];

        outFile.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        string fileName = "studentfile/" + tmp.studentID + "_data.dat";
        ofstream outData(fileName, ios::binary);
        outData.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        outData.close();
        
    }
    
    outFile.close();
    cout << "Add students to class " << className << " successfully!\n";
}

void displayList() {
    string classFileName;
    ifstream inFile("dataFile/firstYearClasses.dat", ios::binary);
    ifstream studentFile;
    string studentFileName;
    StudentInfor tmpStudent;
    int count = 0;
    int studentCount;
    while(!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&classFileName), sizeof(classFileName));
        if (inFile.eof()) break;
        
        cout << "Class " << classFileName << ":" << endl;
        studentFileName = "studentlist/" + classFileName + ".dat";
        studentFile.open(studentFileName, ios::binary);
        studentCount = 0;
        if (!studentFile.fail()) {
            while(!studentFile.eof()) {
                studentFile.read(reinterpret_cast<char *>(&tmpStudent), sizeof(tmpStudent));
                if (studentFile.eof()) {
                    studentFile.close();
                    break;
                } 
                cout << "\t" << tmpStudent.no << " "; 
                cout << tmpStudent.studentID << " ";
                cout << tmpStudent.fullName << " ";
                cout << tmpStudent.gender << " ";
                cout << tmpStudent.DoB << " ";
                cout << tmpStudent.socialID << " ";
                cout << endl;
                studentCount++;
            }
        }
        if (studentCount == 0)
            cout << "\tNo students" << endl;
        studentFile.close();
        count++;
    }
    if (count == 0)
        cout << "No classes created" << endl;
    inFile.close();
}

bool _checkAlreadyCreatedYear(SchoolYearLinkedList *pHead, int begin, SchoolYearLinkedList *&theChosen) {
    SchoolYearLinkedList *pRead = pHead;
    while (pRead != nullptr) {
        if (pRead->data.begin == begin) {
            theChosen = pRead;
            return true;
        }
        pRead = pRead->pNext;
    }
    return false;
}

bool _checkCourseRegistration(CourseRegistration course) {
    if (course.end.year <= 0 || course.end.month <= 0 || course.end.date <=0) {
        return false;
    } 
    return true;
}

void _displayCourseRegistration(CourseRegistrationLinkedList *pHead) {
    cout << "Course registration sessions created:" << endl;
    int count = 0;
    CourseRegistrationLinkedList *pCur = pHead;
    while (pCur != nullptr) {
        cout << pCur->data.schoolyear << " " << pCur->data.schoolyear + 1 << endl;
        cout << "\tSemester " << pCur->data.semester << endl;
        cout << "\tEnd Date: "; 
        cout << pCur->data.end.year << "/" << pCur->data.end.month << "/" << pCur->data.end.date << endl;
        count++;
        pCur = pCur->pNext;
    }
    if (count == 0)
        cout << "There are no coure registration sessions created!!!" << endl;
}

void addCourseRegistrationSession(SchoolYear schoolyear, int semester) {
    CourseRegistration tmp;
    string tmpStartDate, tmpStartMonth, tmpStartYear, tmpEndDate, tmpEndMonth, tmpEndYear;
    bool inputGood = false;
    cout << "Add course registration session" << endl;
    do {
        cout << "End" << endl;
        cout << "Year: ";
        getline(cin, tmpEndYear);
        cout << "Month: ";
        getline(cin, tmpEndMonth);
        cout << "Date: ";
        getline(cin, tmpEndDate);
        try {
            tmp.end.year = atoi(tmpEndYear.c_str());
            tmp.end.month = atoi(tmpEndMonth.c_str());
            tmp.end.date = atoi(tmpEndDate.c_str());
            if (_checkCourseRegistration(tmp))
                inputGood = true;
            else 
                cout << "Invalid input" << endl;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!inputGood || !_checkCourseRegistration(tmp));
    tmp.schoolyear = schoolyear.begin;
    tmp.semester = semester;
    ofstream outFile("dataFile/sessions.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    outFile.close();
}

void addCourse(SchoolYear schoolyear, int semester) {
    Course tmp;
    cout << "Add courses to semester" << endl;
    cout << "Course ID: ";
    getline(cin, tmp.courseID);
    string fileName = "courselist/" + to_string(schoolyear.begin) + to_string(schoolyear.end) + "_" + to_string(semester) + ".dat";
    if (_checkCreatedCourse(fileName, tmp.courseID)) {
        cout << "This course is already added" << endl;
        return;
    }
    cout << "Course Name: ";
    getline(cin, tmp.courseName);
    cout << "Teacher Name: ";
    getline(cin, tmp.teacherName);
    bool isCheck;
    do {
        isCheck = false;
        cout << "Number of credits: ";
        string tmpCredit;
        getline(cin, tmpCredit);
        try {
            tmp.credits = stoi(tmpCredit);
            isCheck = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isCheck);
    
    do {
        isCheck = false;
        cout << "Number of students: ";
        string tmpStudent;
        getline(cin, tmpStudent);
        try {
            tmp.maxStudent = stoi(tmpStudent);
            isCheck = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isCheck);
    
    bool inputGood = false;
    while (!inputGood) {
        cout << "Day 1 (1. MON 2.TUE 3.WED 4.THU 5.FRI 6.SAT 7.SUN): ";
        string dayChoice;
        getline(cin, dayChoice);
        if (dayChoice.size() >= 2) {
            cout << "Invalid input, input again" << endl;
        } else {
            switch(dayChoice[0]) {
                case '1':
                    tmp.day1 = "MON";
                    inputGood = true;
                    break;
                case '2':
                    tmp.day1 = "TUE";
                    inputGood = true;
                    break;
                case '3':
                    tmp.day1 = "WED";
                    inputGood = true;
                    break;
                case '4':
                    tmp.day1 = "THU";
                    inputGood = true;
                    break;
                case '5':
                    tmp.day1 = "FRI";
                    inputGood = true;
                    break;
                case '6':
                    tmp.day1 = "SAT";
                    inputGood = true;
                    break;
                case '7':
                    tmp.day1 = "SUN";
                    inputGood = true;
                    break;
                default:
                    cout << "Invalid input, input again" << endl;
                    break;
            }
        }
    }
    
    inputGood = false;
    while (!inputGood) {
        cout << "Session I 1. S1(7:30) 2. S2(9:30) 3. S3(13:30) 4. S4(15:30): ";
        string dayChoice;
        getline(cin,dayChoice);
        if (dayChoice.size() >= 2) {
            cout << "Invalid input, input again" << endl;
        } else {
            switch(dayChoice[0]) {
                case '1':
                    tmp.session1 = "7:30";
                    inputGood = true;
                    break;
                case '2':
                    tmp.session1 = "9:30";
                    inputGood = true;
                    break;
                case '3':
                    tmp.session1 = "13:30";
                    inputGood = true;
                    break;
                case '4':
                    tmp.session1 = "15:30";
                    inputGood = true;
                    break;
                default:
                    cout << "Invalid input, input again" << endl;
                    break;
            }
        }
    }

    inputGood = false;
    while (!inputGood) {
        cout << "Day 2 (1. MON 2.TUE 3.WED 4.THU 5.FRI 6.SAT 7.SUN): ";
        string dayChoice;
        getline(cin, dayChoice);
        if (dayChoice.size() >= 2) {
            cout << "Invalid input, input again" << endl;
        } else {
            switch(dayChoice[0]) {
                case '1':
                    tmp.day2 = "MON";
                    break;
                case '2':
                    tmp.day2 = "TUE";
                    break;
                case '3':
                    tmp.day2 = "WED";
                    break;
                case '4':
                    tmp.day2 = "THU";
                    break;
                case '5':
                    tmp.day2 = "FRI";
                    break;
                case '6':
                    tmp.day2 = "SAT";
                    break;
                case '7':
                    tmp.day2 = "SUN";
                    break;
                default:
                    cout << "Invalid input, input again" << endl;
                    break;
            }
        }
        if (tmp.day2.compare(tmp.day1) == 0) {
            cout << "Your day 2 must different from day 1" << endl;
        } else {
            inputGood = true;
        }
    }
    
    inputGood = false;
    while (!inputGood) {
        cout << "Session II 1. S1(7:30) 2. S2(9:30) 3. S3(13:30) 4. S4(15:30): ";
        string dayChoice;
        getline(cin, dayChoice);
        if (dayChoice.size() >= 2) {
            cout << "Invalid input, input again" << endl;
        } else {
            switch(dayChoice[0]) {
                case '1':
                    tmp.session2 = "7:30";
                    inputGood = true;
                    break;
                case '2':
                    tmp.session2 = "9:30";
                    inputGood = true;
                    break;
                case '3':
                    tmp.session2 = "13:30";
                    inputGood = true;
                    break;
                case '4':
                    tmp.session2 = "15:30";
                    inputGood = true;
                    break;
                default:
                    cout << "Invalid input, input again" << endl;
                    break;
            }
        }
    }
    tmp.schoolyearBegin = schoolyear.begin;
    tmp.semester = semester;
    
    fileName = "courselist/" + to_string(schoolyear.begin) + to_string(schoolyear.end) + "_" + to_string(semester) + ".dat";
    ofstream outFile(fileName, ios::binary | ios::app);
    outFile.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    outFile.close();

    fileName = "attendedCourse/" + tmp.courseID + ".dat";
    outFile.open(fileName, ios::binary);
    outFile.close();
}

void createSemester() {
    cout << "List of school years created:" << endl;
    SchoolYearLinkedList *pHead = nullptr;
    SchoolYearLinkedList *pRead = pHead;
    SchoolYearLinkedList *pNew = nullptr;
    SchoolYear tmp;
    ifstream inFile("dataFile/schoolYear.dat", ios::binary);
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        if (inFile.eof())
            break;
        if (pHead == nullptr) {
            pHead = new SchoolYearLinkedList;
            pHead->pPrev = nullptr;
            pHead->data = tmp;
            pHead->pNext = nullptr;
            pRead = pHead;
        } else {
            pNew = new SchoolYearLinkedList;
            pNew->data = tmp;
            pNew->pPrev = pRead;
            pNew->pNext = nullptr;
            pRead->pNext = pNew;
            pRead = pNew;
        }
    }
    
    if (pHead == nullptr) {
        cout << "There are no school years created" << endl;
        return;
    } 

    pRead = pHead;
    while (pRead != nullptr) {
        cout << pRead->data.begin << " " << pRead->data.end << endl;
        pRead = pRead->pNext;
    }

    string choiceStr;
    int choice;
    bool inputGood = false;
    while(!inputGood) {
        try {
            cout << "Choose your school years (Type begin year only): ";
            getline(cin, choiceStr);
            choice = stoi(choiceStr);
            inputGood = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    }

    SchoolYearLinkedList *theChosen;
    
    if (!_checkAlreadyCreatedYear(pHead, choice, theChosen)) {
        cout << "That school year is not created" << endl;
        return;
    } else if (theChosen->data.sems[0] == true && theChosen->data.sems[1] == true && theChosen->data.sems[2] == true) {
        cout << "All semesters of this school year are created" << endl;
        return;
    } else {
        cout << "What semester do you want to create 1, 2 or 3: ";
        string semChoice;
        do {
            getline(cin, semChoice);
        } while (semChoice[0] != '1' && semChoice[0] != '2' && semChoice[0] != '3' || semChoice.size() >= 2); 

        bool checkEmptySem = false;
        while (!checkEmptySem) {
            if (semChoice[0] == '1') {
                if (theChosen->data.sems[0] == true) {
                    cout << "It is already created" << endl;
                    cout << "What semester do you want to create 1, 2 or 3: ";
                    getline(cin, semChoice);
                }
                else {
                    checkEmptySem = true;
                    theChosen->data.sems[0] = true;
                    addCourseRegistrationSession(theChosen->data, 1);
                }
            } else if (semChoice[0] == '2') {                
                if (theChosen->data.sems[1] == true) {
                    cout << "It is already created" << endl;
                    cout << "What semester do you want to create 1, 2 or 3: ";
                    getline(cin, semChoice);
                }
                else {
                    checkEmptySem = true;
                    theChosen->data.sems[1] = true;
                    addCourseRegistrationSession(theChosen->data, 2);
                }
            } else {
                if (theChosen->data.sems[2] == true) {
                    cout << "It is already created" << endl;
                    cout << "What semester do you want to create 1, 2 or 3: ";
                    getline(cin, semChoice);
                }
                else {
                    checkEmptySem = true;
                    theChosen->data.sems[2] = true;
                    addCourseRegistrationSession(theChosen->data, 3);
                }
            }
        }
        cout << "Semester is successfully created" << endl;
    }
    inFile.close();
    
    ofstream outFile("dataFile/schoolYear.dat", ios::binary | ios::out);
    pRead = pHead;
    while (pRead != nullptr) {
        outFile.write(reinterpret_cast<char *>(&pRead->data), sizeof(pRead->data));
        pRead = pRead->pNext;
    }

    outFile.close();

    while (pHead != nullptr) {
        pRead = pHead;
        pHead = pHead->pNext;
        delete pRead;
    }
}

void displayCourses() {
    SchoolYear tmpSchoolyear;
    Course tmpCourse;
    ifstream inFile("dataFile/schoolYear.dat", ios::binary);
    int count = 0;
    string fileCourseName;
    cout << "List of courses" << endl;
    while(!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&tmpSchoolyear), sizeof(tmpSchoolyear));
        if (inFile.eof()) break;
        cout << "School year: ";
        cout << tmpSchoolyear.begin << " " << tmpSchoolyear.end << endl;
        for (int i = 1; i <= 3; i++) {
            fileCourseName = "courselist/" + to_string(tmpSchoolyear.begin) + to_string(tmpSchoolyear.end) + "_" + to_string(i) + ".dat";
            ifstream inCourseFile(fileCourseName, ios::binary);
            cout << "\t";
            cout << "Semester " << i << endl;
            if (inCourseFile.fail()) {
                if (tmpSchoolyear.sems[i - 1] == true)
                    cout << "\tNo courses added" << endl;
                else
                    cout << "\tThis semester is not added" << endl;
            } else {
                int subCount = 0;
                cout << "\t";
                cout << setw(12) << left << "Course ID";
                cout << " | ";
                cout << setw(25) << left << "Course Name";
                cout << " | ";
                cout << setw(20) << left << "Teacher Name";
                cout << " | ";
                cout << setw(20) << left << "Number of students";
                cout << " | ";
                cout << setw(20) << left << "Number of credits";
                cout << " | ";
                cout << setw(5) << left << "Day 1";
                cout << " | ";
                cout << setw(10) << left << "Session I";
                cout << " | ";
                cout << setw(5) << left << "Day 2";
                cout << " | ";
                cout << "Session II" << endl;
                while (true) {
                    inCourseFile.read(reinterpret_cast<char *>(&tmpCourse), sizeof(tmpCourse));
                    if (inCourseFile.eof()) break;
                    subCount++;
                    cout << "\t";
                    cout << setw(12) << left << tmpCourse.courseID;
                    cout << " | ";
                    cout << setw(25) << left << tmpCourse.courseName;
                    cout << " | ";
                    cout << setw(20) << left << tmpCourse.teacherName;
                    cout << " | ";
                    cout << setw(20) << left << tmpCourse.maxStudent;
                    cout << " | ";
                    cout << setw(20) << left << tmpCourse.credits;
                    cout << " | ";
                    cout << setw(5) << left << tmpCourse.day1;
                    cout << " | ";
                    cout << setw(10) << left << tmpCourse.session1;
                    cout << " | ";
                    cout << setw(5) << left << tmpCourse.day2;
                    cout << " | ";
                    cout << tmpCourse.session2 << endl;
                }
                if (subCount == 0)
                    cout << "\tNo courses added" << endl;
            }
            inCourseFile.close();
        }
        count++;   
    }
    if (count == 0)
        cout << "No school years created!!!" << endl;
    inFile.close();
}

void displayStudentOfCourse() {
    displayCourses();
    cout << "Enter course ID: ";
    string courseID;
    getline(cin, courseID);
    string fileName = "attendedCourse/" + courseID + ".dat"; 
    ifstream inFile(fileName, ios::binary);
    if (inFile.fail()) {
        cout << "The course is not created" << endl;
    } else {
        StudentInfor studentinfor;
        int tmpCount = 0;
        while (!inFile.eof()) {
            inFile.read(reinterpret_cast<char *>(&studentinfor), sizeof(studentinfor));
            if (inFile.eof()) break;
            cout << studentinfor.fullName << endl;
            tmpCount++;
        }
        if (tmpCount == 0)
            cout << "No student attend this course" << endl;
    }
    inFile.close();
}

void enrollCourse(StudentInfor studentinfor) {
    
    displayCourses();

    int schoolyearBegin;
    while (true) {
        cout << "What school year are you studying? (Type begin year only) ";
        string tmp;
        getline(cin, tmp);
        if(tmp.size() == 4 && tmp[0] >= '0' && tmp[0] <= '9' && tmp[1] >= '0' && tmp[1] <= '9'
            && tmp[2] >= '0' && tmp[2] <= '9' && tmp[3] >= '0' && tmp[3] <= '9') {
                schoolyearBegin = atoi(tmp.c_str());
                break;
        }
        else cout << "Invalid input. Try again!\n";
    }
    
    string semester;
    do {
        cout << "What semester are you studying? (1, 2, 3) ";
        getline(cin, semester);
    } while ((semester[0] != '1' && semester[0] != '2' && semester[0] != '3') || semester.size() >= 2);

    ifstream inFile("dataFile/sessions.dat", ios::binary);
    CourseRegistration tmp;
    int tmpCount = 0;
    while (true) {
        inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        if (inFile.eof()) break;
        if (tmp.schoolyear == schoolyearBegin && to_string(tmp.semester).compare(semester) == 0)
            tmpCount++;
    }

    if (tmpCount == 0) {
        cout << "The registration sessions of this semester is over!!!" << endl;
        return;
    }

    string fileStudentName = "studentcourses/" + studentinfor.studentID + "_" + to_string(schoolyearBegin) + to_string(schoolyearBegin + 1) + "_" + semester + ".dat";

    if (numOfEnrolledCourses(fileStudentName, schoolyearBegin, semester) == 5) {
        cout << "You have enrolled in 5 courses in this semester. You can't enroll in anymore!" << endl;
        return;
    }

    string schoolyear = to_string(schoolyearBegin) + to_string(schoolyearBegin + 1); 
    string fileCourseName;
    fileCourseName = "courselist/" + to_string(schoolyearBegin) + to_string(schoolyearBegin + 1) + "_" + semester + ".dat";

    string courseID;
    
    cout << "What course ID do you want to enroll in? ";
    getline(cin, courseID);
    if (!_checkCreatedCourse(fileCourseName, courseID)) {
        cout << "This course doesn't exist!" << endl;
        return;
    } 
    
    if (!_checkConflictedSession(fileCourseName, courseID, fileStudentName)) {
        ifstream inFile(fileCourseName, ios::binary);
        Course tmpCourse;
        if (!inFile.fail()) {
            while (!inFile.eof()) {
                inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
                if (courseID.compare(tmpCourse.courseID) == 0) break;
            }
        }
        inFile.close();

        ofstream outFile(fileStudentName, ios::binary | ios:: app);
        outFile.write(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
        outFile.close();

        string attendedCourse;
        attendedCourse = "attendedCourse/" + tmpCourse.courseID + ".dat";
        outFile.open(attendedCourse, ios::binary | ios::app);
        outFile.write(reinterpret_cast<char *>(&studentinfor), sizeof(studentinfor));
        outFile.close();

        cout << "Enroll in successfully!" << endl;
    }
    else cout << "You can't enroll in this course because of 2 sessions are conflicted." << endl;
}

void displayEnrolledCourses(string username) {
    string schoolyear, semester;
    cout << "Enter schoolyear: ";
    getline(cin, schoolyear);
    cout << "Enter semester: ";
    getline(cin, semester);
    cout << "List of enrolled courses: " << endl;
    string fileStudentName = "studentcourses/" + username + "_" + schoolyear + to_string(stoi(schoolyear) + 1) + "_" + semester + ".dat";

    ifstream inFile(fileStudentName, ios::binary);
    Course tmpCourse;

    if (inFile.fail()) {
        cout << "No data found!!!" << endl;
    } else {
        cout << "\t";
        cout << setw(12) << left << "Course ID";
        cout << " | ";
        cout << setw(25) << left << "Course Name";
        cout << " | ";
        cout << setw(20) << left << "Teacher Name";
        cout << " | ";
        cout << setw(20) << left << "Number of students";
        cout << " | ";
        cout << setw(20) << left << "Number of credits";
        cout << " | ";
        cout << setw(5) << left << "Day 1";
        cout << " | ";
        cout << setw(10) << left << "Session I";
        cout << " | ";
        cout << setw(5) << left << "Day 2";
        cout << " | ";
        cout << "Session II" << endl;
        while (!inFile.eof()) {
            inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
            if (inFile.eof()) break;
            cout << "\t";
            cout << setw(12) << left << tmpCourse.courseID;
            cout << " | ";
            cout << setw(25) << left << tmpCourse.courseName;
            cout << " | ";
            cout << setw(20) << left << tmpCourse.teacherName;
            cout << " | ";
            cout << setw(20) << left << tmpCourse.maxStudent;
            cout << " | ";
            cout << setw(20) << left << tmpCourse.credits;
            cout << " | ";
            cout << setw(5) << left << tmpCourse.day1;
            cout << " | ";
            cout << setw(10) << left << tmpCourse.session1;
            cout << " | ";
            cout << setw(5) << left << tmpCourse.day2;
            cout << " | ";
            cout << tmpCourse.session2 << endl;
        }
    }
    inFile.close();
}

void removeEnrolledCourse(string username) {
    string schoolyear, semester;
    int schoolYear;
    bool inputGood;
    do {
        inputGood = false;
        cout << "Enter school year (begin year only): ";
        getline(cin, schoolyear);
        cout << "Enter semester: ";
        getline(cin, semester);
        try {
            schoolYear = stoi(schoolyear);
            stoi(semester);
            inputGood = true;
        } catch (...) {
            cout << "Invalid input! Try again" << endl;
        }
    } while (!inputGood);
    cout << "Enter course ID: ";
    string courseID;
    getline(cin, courseID);
    string fileName1 = "studentcourses/" + username + "_" + to_string(schoolYear) + to_string(schoolYear + 1) + "_" + semester + ".dat";
    ifstream inFile1(fileName1, ios::binary);
    if (!inFile1.fail()) {
        CourseLinkedList *pHead = nullptr;
        CourseLinkedList *pCur = nullptr;
        Course tmpCourse;
        int count = 0;
        while (true) {
            inFile1.read(reinterpret_cast<char *>(&tmpCourse), sizeof(tmpCourse));
            if (inFile1.eof()) break;
            if (tmpCourse.courseID.compare(courseID) != 0) {
                if (pHead == nullptr) {
                    pHead = new CourseLinkedList;
                    pHead->data = tmpCourse;
                    pHead->pPrev = nullptr;
                    pHead->pNext = nullptr;
                    pCur = pHead;
                } else {
                    pCur->pNext = new CourseLinkedList;
                    pCur->pNext->pPrev = pCur;
                    pCur = pCur->pNext;
                    pCur->data = tmpCourse;
                    pCur->pNext = nullptr;
                }
            } else {
                count++;
            }
        }
        if (count != 0) {
            StudentInforLinkedList *pHead1 = nullptr;
            StudentInforLinkedList *pCur1 = nullptr;
            StudentInfor tmpStudent;
            string fileName2 = "attendedCourse/" + courseID + ".dat";
            ifstream inFile2(fileName2, ios::binary);
            while(true) {
                inFile2.read(reinterpret_cast<char *>(&tmpStudent), sizeof(tmpStudent));
                if (inFile2.eof()) break;
                if (tmpStudent.studentID.compare(username) != 0) {
                    if (pHead1 == nullptr) {
                        pHead1 = new StudentInforLinkedList;
                        pHead1->pPrev = nullptr;
                        pHead1->data = tmpStudent;
                        pHead1->pNext = nullptr;
                        pCur1 = pHead1;
                    } else {
                        pCur1->pNext = new StudentInforLinkedList;
                        pCur1->pNext->pPrev = pCur1;
                        pCur1 = pCur1->pNext;
                        pCur1->data = tmpStudent;
                        pCur1->pNext = nullptr;
                    }
                }
            } 
            inFile2.close();
            pCur1 = pHead1;
            ofstream outFile2(fileName2, ios::binary);
            while (pCur1 != nullptr) {
                outFile2.write(reinterpret_cast<char *>(&pCur1->data), sizeof(pCur1->data));
                pCur1 = pCur1->pNext;
            }
            outFile2.close();
            while (pHead1 != nullptr) {
                pCur1 = pHead1;
                pHead1 = pHead1->pNext;
                delete pCur1;
            }
            cout << "Delete course successfully!!!" << endl;
        } else {
            cout << "No course found!!!" << endl;
        }
        ofstream outFile1(fileName1, ios::binary);
        pCur = pHead;
        while (pCur != nullptr) {
            outFile1.write(reinterpret_cast<char *>(&pCur->data), sizeof(pCur->data));
            pCur = pCur->pNext;
        }
        while (pHead != nullptr) {
            pCur = pHead;
            pHead = pHead->pNext;
            delete pCur;
        }
    } else {
        cout << "No data found!!!" << endl;
    }
    inFile1.close();
}

void exportToCsv() {
    cout << "Course Name: ";
    string courseName;
    getline(cin, courseName);
    string fileName = "attendedCourse/" + courseName + ".dat";
    ifstream inFile(fileName, ios::binary);
    if (inFile.fail()) {
        cout << "No students attend this course" << endl;
        inFile.close();
        return;
    } else {
        string exportFile = "exportedFile/" + courseName + ".csv";
        ofstream outFile(exportFile);
        outFile << "StudentID,FullName,Total Mark,Final Mark,Midterm Mark,Other Mark" << endl;
        StudentInfor tmp;
        while (true) {
            inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            if (inFile.eof()) break;
            outFile << tmp.studentID << ",";
            outFile << tmp.fullName << ",";
            outFile << 0 << ",";
            outFile << 0 << ",";
            outFile << 0 << ",";
            outFile << 0 << endl;
        }
        outFile.close();
    }
    inFile.close();
}

void updateStudentGPA(const string &courseID, const string &schoolyear, const string &semester) {
    string fileName = "attendedCourse/" + courseID + ".dat";
    ifstream inFile(fileName, ios::binary);
    if (!inFile.fail()) {
        while (true) {
            StudentInfor tmpStudent;
            inFile.read(reinterpret_cast<char *>(&tmpStudent), sizeof(tmpStudent));
            if (inFile.eof()) break;
            string fileName1 = "studentcourses/" + tmpStudent.studentID + "_" + schoolyear + to_string(stoi(schoolyear) + 1) + "_" + semester + ".dat";
            ifstream inFile1(fileName1, ios::binary);
            double total = 0;
            int num = 0;
            if (!inFile1.fail()) {
                Course tmpCourse;
                while (true) {
                    inFile1.read(reinterpret_cast<char *>(&tmpCourse), sizeof(tmpCourse));
                    if (inFile1.eof()) break;
                    string fileName2 = "attendedCourse/" + tmpCourse.courseID + "_scoreboard.dat";
                    ifstream inFile2(fileName2, ios::binary);
                    if (!inFile2.fail()) {
                        Scoreboard tmpScoreboard;
                        while (true) {
                            inFile2.read(reinterpret_cast<char *>(&tmpScoreboard), sizeof(tmpScoreboard));
                            if(inFile2.eof()) break;
                            if (tmpScoreboard.StudentID.compare(tmpStudent.studentID) == 0) {
                                num++;
                                total += tmpScoreboard.TotalMark;
                            }
                        }
                    }
                    inFile2.close();
                }
            }
            inFile1.close();
            double gpa = total/num;
            string fileGPAName = "GPA/" + tmpStudent.studentID + "_" + schoolyear + to_string(stoi(schoolyear) + 1) + "_" + semester + ".dat";
            ifstream inFileGPA(fileGPAName, ios::binary);
            StudentGPA tmpGPA;
            if (inFileGPA.fail()) {
                tmpGPA.GPA1 = -1;
                tmpGPA.GPA2 = -1;
                tmpGPA.GPA3 = -1;
            } else {
                inFileGPA.read(reinterpret_cast<char *>(&tmpGPA), sizeof(tmpGPA));
            }
            if (semester == "1") {
                tmpGPA.GPA1 = gpa;
            } else if (semester == "2") {
                tmpGPA.GPA2 = gpa;
            } else if (semester == "3") {
                tmpGPA.GPA3 = gpa;
            }
            inFileGPA.close();
            ofstream outFileGPA(fileGPAName, ios::binary);
            outFileGPA.write(reinterpret_cast<char *>(&tmpGPA), sizeof(tmpGPA));
        }
    } 
    inFile.close();
}

void importScoreboard() { // consists of update the result of the students
    string courseID;
    string schoolyear;
    string semester;
    bool inputGood;
    do {
        inputGood = false;
        cout << "Enter school year (begin year only): ";
        getline(cin, schoolyear);
        cout << "Enter semester: ";
        getline(cin, semester);
        cout << "Course ID: ";
        getline(cin, courseID);
        try {
            stoi(schoolyear);
            stoi(semester);
            inputGood = true;
        }  catch (...) {
            cout << "Invalid input! Try again." << endl;
        }
    } while(!inputGood);

    string fileName = "courselist/" + schoolyear + to_string(stoi(schoolyear) + 1) + "_" + semester + ".dat";
    if (!_checkCreatedCourse(fileName, courseID)) {
        cout << "No data found!" << endl;
        return;
    }

    string csvFileName;
    while (true) {
        cout << "Enter the file name: ";
        getline(cin, csvFileName);
        if (_checkSpace(csvFileName)) break;
        else cout << "Invalid input. Try again!\n";
    }

    string data[1000][6];
    string line, word;

    csvFileName = "scoreboardCsv/" + csvFileName;

    fstream file(csvFileName, ios::in);
    file.ignore(1000, '\n');
    int n = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            stringstream str(line);

            int j = 0;
            while (getline(str, word, ',')) {
                data[n][j] = word;
                j++;
            }
            n++;
        }
        file.close();
    }
    else {
        cout << "Could not open the file\n";
        file.close();
        return;
    }

    string fileAttendedCourse = "attendedCourse/" + courseID + "_scoreboard.dat";
    ofstream outFile(fileAttendedCourse, ios::binary);


    for(int i = 0; i < n; i++) {
        Scoreboard tmp;
        tmp.courseID = courseID;
        tmp.StudentID = data[i][0];
        tmp.FullName = data[i][1];
        tmp.TotalMark = atof(data[i][2].c_str());
        tmp.FinalMark = atof(data[i][3].c_str());
        tmp.MidtermMark = atof(data[i][4].c_str());
        tmp.OtherMark = atof(data[i][5].c_str());

        outFile.write(reinterpret_cast<char*>(&tmp), sizeof(tmp));
    }
    
    cout << "Import the scoreboard successfully!" << endl;

    outFile.close();
    updateStudentGPA(courseID, schoolyear, semester);

}

void updateCourseInfor() {
    displayCourses();
    string schoolYear;
    int tmpBegin;
    bool isGood;
    do {
        cout << "Enter school year (begin year only): ";
        getline(cin, schoolYear);
        isGood = false;
        try {
            tmpBegin = stoi(schoolYear);
            isGood = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isGood);
    
    string semester;
    int tmpSem;
    do {
        cout << "Enter semester (1, 2 or 3): ";
        getline(cin, semester);
        isGood = false;
        try {
            tmpSem = stoi(semester);
            isGood = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isGood);
    string fileName = "courselist/" + schoolYear + to_string(tmpBegin + 1) + "_" + semester + ".dat";
    ifstream inFile(fileName, ios::binary);

    if (inFile.fail()) {
        cout << "The school year or semester is not created!!!" << endl;
        inFile.close();
    } else {
        Course tmp;
        CourseLinkedList *pHead = nullptr;
        CourseLinkedList *pCur = nullptr;
        while (!inFile.eof()) {
            inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            if (inFile.eof()) break;
            if (pHead == nullptr) {
                pHead = new CourseLinkedList;
                pHead->pPrev = nullptr;
                pHead->data = tmp;
                pHead->pNext = nullptr;
                pCur = pHead;
            } else {
                pCur->pNext = new CourseLinkedList;
                pCur->pNext->pPrev = pCur;
                pCur = pCur->pNext;
                pCur->data = tmp;
                pCur->pNext = nullptr;
            }
        }
        inFile.close();
        string courseID;
        cout << "Enter course ID: ";
        getline(cin, courseID);
        pCur = pHead;
        Course modifiedCourse;
        while (pCur != nullptr) {
            if ((pCur->data.courseID).compare(courseID) == 0) {
                cout << "Course Name: ";
                getline(cin, modifiedCourse.courseName);
                cout << "Teacher Name: ";
                getline(cin, modifiedCourse.teacherName);
                bool isCheck;
                do {
                    isCheck = false;
                    cout << "Number of credits: ";
                    string tmpCredit;
                    getline(cin, tmpCredit);
                    try {
                        modifiedCourse.credits = stoi(tmpCredit);
                        isCheck = true;
                    } catch (...) {
                        cout << "Invalid input" << endl;
                    }
                } while (!isCheck);

                do {
                    isCheck = false;
                    cout << "Number of students: ";
                    string tmpNumOfStudent;
                    getline(cin, tmpNumOfStudent);
                    try {
                        modifiedCourse.maxStudent = stoi(tmpNumOfStudent);
                        isCheck = true;
                    } catch (...) {
                        cout << "Invalid input" << endl;
                    }
                } while (!isCheck); 

                isCheck = false;
                while (!isCheck) {
                    cout << "Day 1 (1. MON 2.TUE 3.WED 4.THU 5.FRI 6.SAT 7.SUN): ";
                    string dayChoice;
                    getline(cin, dayChoice);
                    if (dayChoice.size() >= 2) {
                        cout << "Invalid input, input again" << endl;
                    } else {
                        switch(dayChoice[0]) {
                            case '1':
                                modifiedCourse.day1 = "MON";
                                isCheck = true;
                                break;
                            case '2':
                                modifiedCourse.day1 = "TUE";
                                isCheck = true;
                                break;
                            case '3':
                                modifiedCourse.day1 = "WED";
                                isCheck = true;
                                break;
                            case '4':
                                modifiedCourse.day1 = "THU";
                                isCheck = true;
                                break;
                            case '5':
                                modifiedCourse.day1 = "FRI";
                                isCheck = true;
                                break;
                            case '6':
                                modifiedCourse.day1 = "SAT";
                                isCheck = true;
                                break;
                            case '7':
                                modifiedCourse.day1 = "SUN";
                                isCheck = true;
                                break;
                            default:
                                cout << "Invalid input, input again" << endl;
                                break;
                        }
                    }
                }
                
                isCheck = false;
                while (!isCheck) {
                    cout << "Session I 1. S1(7:30) 2. S2(9:30) 3. S3(13:30) 4. S4(15:30): ";
                    string dayChoice;
                    getline(cin,dayChoice);
                    if (dayChoice.size() >= 2) {
                        cout << "Invalid input, input again" << endl;
                    } else {
                        switch(dayChoice[0]) {
                            case '1':
                                modifiedCourse.session1 = "7:30";
                                isCheck = true;
                                break;
                            case '2':
                                modifiedCourse.session1 = "9:30";
                                isCheck = true;
                                break;
                            case '3':
                                modifiedCourse.session1 = "13:30";
                                isCheck = true;
                                break;
                            case '4':
                                modifiedCourse.session1 = "15:30";
                                isCheck = true;
                                break;
                            default:
                                cout << "Invalid input, input again" << endl;
                                break;
                        }
                    }
                }
            
                isCheck = false;
                while (!isCheck) {
                    cout << "Day 2 (1. MON 2.TUE 3.WED 4.THU 5.FRI 6.SAT 7.SUN): ";
                    string dayChoice;
                    getline(cin, dayChoice);
                    if (dayChoice.size() >= 2) {
                        cout << "Invalid input, input again" << endl;
                    } else {
                        switch(dayChoice[0]) {
                            case '1':
                                modifiedCourse.day2 = "MON";
                                break;
                            case '2':
                                modifiedCourse.day2 = "TUE";
                                break;
                            case '3':
                                modifiedCourse.day2 = "WED";
                                break;
                            case '4':
                                modifiedCourse.day2 = "THU";
                                break;
                            case '5':
                                modifiedCourse.day2 = "FRI";
                                break;
                            case '6':
                                modifiedCourse.day2 = "SAT";
                                break;
                            case '7':
                                modifiedCourse.day2 = "SUN";
                                break;
                            default:
                                cout << "Invalid input, input again" << endl;
                                break;
                        }
                    }

                    if (modifiedCourse.day2.compare(modifiedCourse.day1) == 0) {
                        cout << "The day 2 must different from day 1" << endl;
                    } else {
                        isCheck = true;
                    }
                }
            
                isCheck = false;
                while (!isCheck) {
                    cout << "Session II 1. S1(7:30) 2. S2(9:30) 3. S3(13:30) 4. S4(15:30): ";
                    string dayChoice;
                    getline(cin,dayChoice);
                    if (dayChoice.size() >= 2) {
                        cout << "Invalid input, input again" << endl;
                    } else {
                        switch(dayChoice[0]) {
                            case '1':
                                modifiedCourse.session2 = "7:30";
                                isCheck = true;
                                break;
                            case '2':
                                modifiedCourse.session2 = "9:30";
                                isCheck = true;
                                break;
                            case '3':
                                modifiedCourse.session2 = "13:30";
                                isCheck = true;
                                break;
                            case '4':
                                modifiedCourse.session2 = "15:30";
                                isCheck = true;
                                break;
                            default:
                                cout << "Invalid input, input again" << endl;
                                break;
                        }
                    }
                }
                break;
            }
            pCur = pCur->pNext;
        }

        if (pCur == nullptr) {
            cout << "No course found!!!" << endl; 
        } else {
            modifiedCourse.courseID = courseID;
            pCur->data = modifiedCourse;
        }

        pCur = pHead;
        ofstream outFile(fileName, ios::binary);
        while (pCur != nullptr) {
            outFile.write(reinterpret_cast<char *>(&pCur->data), sizeof(pCur->data));
            pCur = pCur->pNext;
        }
        outFile.close();
        
        pCur = pHead;
        while (pHead != nullptr) {
            pCur = pHead;
            pHead = pHead->pNext;
            delete pCur;
        }
    }
}

void removeCourse() {
    displayCourses();
    string schoolYear;
    int tmpBegin;
    bool isGood;
    do {
        cout << "Enter school year (begin year only): ";
        getline(cin, schoolYear);
        isGood = false;
        try {
            tmpBegin = stoi(schoolYear);
            isGood = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isGood);
    
    string semester;
    int tmpSem;
    do {
        cout << "Enter semester (1, 2 or 3): ";
        getline(cin, semester);
        isGood = false;
        try {
            tmpSem = stoi(semester);
            isGood = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isGood);
    string fileName = "courselist/" + schoolYear + to_string(tmpBegin + 1) + "_" + semester + ".dat";
    ifstream inFile(fileName, ios::binary);

    if (inFile.fail()) {
        cout << "No data found!!!" << endl;
    } else {
        Course tmp;
        CourseLinkedList *pHead = nullptr;
        CourseLinkedList *pCur = nullptr;
        while (!inFile.eof()) {
            inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
            if (inFile.eof()) break;
            if (pHead == nullptr) {
                pHead = new CourseLinkedList;
                pHead->pPrev = nullptr;
                pHead->data = tmp;
                pHead->pNext = nullptr;
                pCur = pHead;
            } else {
                pCur->pNext = new CourseLinkedList;
                pCur->pNext->pPrev = pCur;
                pCur = pCur->pNext;
                pCur->data = tmp;
                pCur->pNext = nullptr;
            }
        }
        inFile.close();
        string courseID;
        cout << "Enter course ID: ";
        getline(cin, courseID);
        pCur = pHead;
        int countDel = 0;
        while (pCur != nullptr) {
            if ((pCur->data.courseID).compare(courseID) == 0) {
                if (pCur == pHead) {
                    pHead = pHead->pNext;
                    delete pCur;
                    pCur = pHead;
                    if (pCur != nullptr)
                        pCur->pPrev = nullptr;
                    countDel++;
                } else {
                    pCur->pPrev->pNext = pCur->pNext;
                    if (pCur->pNext != nullptr) {
                        pCur->pNext->pPrev = pCur->pPrev;
                    }
                    CourseLinkedList *pDel = pCur;
                    pCur = pCur->pNext;
                    delete pDel;
                    countDel++;
                }
            }
            if (pCur != nullptr)
                pCur = pCur->pNext;
        }

        ofstream outFile(fileName, ios::binary);
        pCur = pHead;
        while (pCur != nullptr) {
            outFile.write(reinterpret_cast<char *>(&pCur->data), sizeof(pCur->data));
            pCur = pCur->pNext;
        }
        outFile.close();

        if (countDel == 0) {
            cout << "No course found!!!" << endl;
        } else {
            cout << "Delete course successfully!!!" << endl;
        }
            
        pCur = pHead;
        while (pHead != nullptr) {
            pCur = pHead;
            pHead = pHead->pNext;
            delete pCur;
        }
    }
}

void displayScoreboardOfCourse() {
    string courseID;
    int semester;
    
    while (true) {
        cout << "What course ID of the scoreboard do you want to view? ";
        getline(cin, courseID);

        semester = _findSemesterOfCourse(courseID);
        if (semester == -1) {
            cout << "This course doesn't exist" << endl;
            return;
        }
        
        else break;
    }

    string fileName = "attendedCourse/" + courseID + "_scoreboard.dat"; 
    ifstream inFile(fileName, ios::binary);
    if (inFile.fail()) {
        cout << "Scoreboard of this course doesn't exist!" << endl;
    } 
    else {
        Scoreboard score;
        while (!inFile.eof()) {
            inFile.read(reinterpret_cast<char *>(&score), sizeof(score));
            if (inFile.eof()) break;
            cout << " " << score.StudentID << " " << score.FullName << " " 
                 << score.TotalMark << " " << score.FinalMark << " "
                 << score.MidtermMark << " " << score.OtherMark << endl;
        }
    }
    inFile.close();
}

void displayScoreboardOfClass() {
    string className;
    cout << "What class of the scoreboard do you want to view? ";
    getline(cin, className);
    string schoolyear, semester;
    cout << "Enter school year (begin year only): ";
    getline(cin, schoolyear);
    cout << "Enter semester: ";
    getline(cin, semester);

    string fileClassName = "studentlist/" + className + ".dat";
    ifstream inClassFile(fileClassName, ios::binary);

    if (inClassFile.fail()) {
        cout << "No data!" << endl;
        inClassFile.close();
        return;
    }

    StudentInfor student;
    while (!inClassFile.eof()) {
        inClassFile.read(reinterpret_cast<char*>(&student), sizeof(student));
        if (inClassFile.eof()) break;

        string fileName = "studentcourses/" + student.studentID + "_" + schoolyear + to_string(stoi(schoolyear) + 1) + "_" + semester + ".dat";
        ifstream inStudentFile(fileName, ios::binary);
        if (!inStudentFile.fail()) {
            cout << student.studentID << " " << student.fullName << endl;
            Course tmpCourse;
            while (true) {
                inStudentFile.read(reinterpret_cast<char *>(&tmpCourse), sizeof(tmpCourse));
                if (inStudentFile.eof()) break;
                cout << "\t" << tmpCourse.courseID << " ";
                cout << tmpCourse.courseName << " ";
                string fileName1 = "attendedCourse/" + tmpCourse.courseID + "_scoreboard.dat";
                ifstream inFile(fileName1, ios::binary);
                if (!inFile.fail()) {
                    Scoreboard tmpScoreboard;
                    while (true) {
                        inFile.read(reinterpret_cast<char *>(&tmpScoreboard), sizeof(tmpScoreboard));
                        if (inFile.eof()) break;
                        if (tmpScoreboard.StudentID.compare(student.studentID)) {
                            cout << "Final Mark: " <<  tmpScoreboard.FinalMark;
                        }
                    }
                }
                cout << endl;
                inFile.close();
            }
        }
        inStudentFile.close();
        string fileName2 = "GPA/" + student.studentID + "_" + schoolyear + to_string(stoi(schoolyear) + 1) + "_" + semester + ".dat";
        ifstream inFile2(fileName2, ios::binary);
        if (!inFile2.fail()) {
            StudentGPA tmpGPA;
            cout << "GPA: ";
            inFile2.read(reinterpret_cast<char *>(&tmpGPA), sizeof(tmpGPA));
            if (semester == "1") {
                if (tmpGPA.GPA1 != -1)
                    cout << tmpGPA.GPA1;
            } else if (semester == "2") {
                if (tmpGPA.GPA2 != -1)
                    cout << tmpGPA.GPA2;
            } else if (semester == "3") {
                if (tmpGPA.GPA3 != -1)
                    cout << tmpGPA.GPA3;
            }
        }
        cout << endl;
        inFile2.close();
    }
    inClassFile.close();
}

void addCourseAtBegin() {
    displayCourses();
    SchoolYear schoolyear;
    int semester;
    bool isChecked;
    do {
        isChecked = false;
        cout << "Enter school year (begin year only): ";
        string tmpBeg;
        getline(cin, tmpBeg);
        try {
            schoolyear.begin = stoi(tmpBeg);
            isChecked = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isChecked);

    do {
        isChecked = false;
        cout << "Enter semester: ";
        string tmpSem;
        getline(cin, tmpSem);
        try {
            semester = stoi(tmpSem);
            isChecked = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isChecked);
    ifstream inFile("dataFile/schoolYear.dat", ios::binary);
    SchoolYear tmp;
    bool isFound = false;
    while(1) {
        inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        if (inFile.eof()) break;
        if (tmp.begin == schoolyear.begin) {
            schoolyear = tmp;
            isFound = true;
            break;
        }
    }
    inFile.close();
    if (isFound == false) {
        cout << "No school year founded!!!" << endl;
    } else if (schoolyear.sems[semester - 1] == false) {
        cout << "The semester is not added!!!" << endl;
    } else {
        addCourse(schoolyear, semester);
    }
}

void addCourseRegistrationSessionAtBegin() {
    ifstream sampleFile("dataFile/schoolYear.dat", ios::binary);
    SchoolYear tmpSchoolYear;
    int count = 0;
    while (1) {
        sampleFile.read(reinterpret_cast<char *>(&tmpSchoolYear), sizeof(tmpSchoolYear));
        if (sampleFile.eof()) break;
        cout << tmpSchoolYear.begin << " " << tmpSchoolYear.end << endl;
        count++;
    }
    sampleFile.close();
    if (count == 0) {
        cout << "There are no school years created!!!" << endl;
        return;
    }
    SchoolYear schoolyear;
    int semester;
    bool isChecked;
    do {
        isChecked = false;
        cout << "Enter school year: ";
        string tmpBeg;
        getline(cin, tmpBeg);
        try {
            schoolyear.begin = stoi(tmpBeg);
            isChecked = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isChecked);

    do {
        isChecked = false;
        cout << "Enter semester: ";
        string tmpSem;
        getline(cin, tmpSem);
        try {
            semester = stoi(tmpSem);
            isChecked = true;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!isChecked);
    ifstream inFile("dataFile/schoolYear.dat", ios::binary);
    SchoolYear tmp;
    bool isFound = false;
    while(1) {
        inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        if (inFile.eof()) break;
        if (tmp.begin == schoolyear.begin) {
            schoolyear = tmp;
            isFound = true;
            break;
        }
    }
    inFile.close();
    if (isFound == false) {
        cout << "No school year founded!!!" << endl;
    } else if (schoolyear.sems[semester - 1] == false) {
        cout << "The semester is not added!!!" << endl;
    } else {
        addCourseRegistrationSession(schoolyear, semester);
    }
}

void changePasswordForStaff(const string &username) {
    StaffInfor tmpStaff;
    string fileName = "staffFile/" + username + ".dat";
    ifstream inFile(fileName, ios::binary);
    inFile.read(reinterpret_cast<char *>(&tmpStaff), sizeof(tmpStaff));
    inFile.close();
    cout << "New password: ";
    string newPassword;
    getline(cin, newPassword);
    tmpStaff.password = newPassword;
    ofstream outFile(fileName, ios::binary);
    outFile.write(reinterpret_cast<char *>(&tmpStaff), sizeof(tmpStaff));
    outFile.close();
    cout << "Change password successfully!!!" << endl;
}

void viewProfile(const string &username) {
    StaffInfor tmpStaff;
    string fileName = "staffFile/" + username + ".dat";
    ifstream inFile(fileName, ios::binary);
    inFile.read(reinterpret_cast<char *>(&tmpStaff), sizeof(tmpStaff));
    inFile.close();
    cout << "Name: " << tmpStaff.fullName << endl;
    cout << "Title: " << tmpStaff.title << endl;
}

void staffMenu(bool &isOff, const string &username) {
    cout << " -------------------- " << endl;
    cout << "|     STAFF MENU     |" << endl;
    cout << " -------------------- " << endl;

    string choice;
    cout << "0. Sign out" << endl;
    // Add option here
    cout << "1. Create school year" << endl;
    cout << "2. Create 1st-year class" << endl;
    cout << "3. Add new 1st-year students to 1st-year class" << endl;
    cout << "4. Display list of students in exist classes" << endl;
    cout << "5. Create a semester" << endl;
    cout << "6. View list of courses" << endl;
    cout << "7. Add course to exist semester" << endl;
    cout << "8. Update courses information" << endl;
    cout << "9. Remove a course" << endl;
    cout << "10. View list of student in a course" << endl;
    cout << "11. Export a list of students in a course to a CSV file" << endl;
    cout << "12. Import a scoreboard of a course" << endl; 
    cout << "13. View the scoreboard of a course" << endl;
    cout << "14. View the scoreboard of a class" << endl;
    cout << "15. Add new course registration sessions" << endl;
    cout << "16. View your profile" << endl;
    cout << "17. Change password" << endl;
    
    do {
        cout << "Your choice: ";
        getline(cin, choice);
    } while ((choice[0] != '0' && choice[0] != '1' && choice[0] != '2' && choice[0] != '3' && choice[0] != '4' && choice[0] != '5' 
                && choice != "6" && choice != "7" && choice != "8" && choice != "9" && choice != "10" && choice != "11" 
                && choice != "12" && choice != "13" && choice != "14" && choice != "15" && choice != "16" && 
                choice != "17") || !_checkSpace(choice));
    // Add function below this;
    if (choice[0] == '0') {
        startMenu(isOff);
        return;
    } else if (choice == "1") {
        // Create school year
        createSchoolYear();
    } else if (choice == "2") {
        // Create 1st-year classes
        createClass();
    } else if (choice == "3") {
        // Add new 1st-year student to 1st-year classes
        addStudent();
    } else if (choice == "4") {
        // Display list of student
        displayList();
    } else if (choice == "5") {
        // Create semester
        createSemester();
    } else if (choice == "6") {
        // Display courses
        displayCourses();
    } else if (choice == "7") {
        // Add new course
        addCourseAtBegin();
    } else if (choice == "8") {
        // Update courses infor
        updateCourseInfor();
    } else if (choice == "9") {
        // Remove course
        removeCourse();
    } else if (choice == "10") {
        displayStudentOfCourse();
    } else if (choice == "11") {
        exportToCsv();
    } else if (choice == "12") {
        importScoreboard();
    } else if (choice == "13") {
        displayScoreboardOfCourse();
    } else if (choice == "14") {
        displayScoreboardOfClass();
    } else if (choice == "15") {
        addCourseRegistrationSessionAtBegin();
    } else if (choice == "16") {
        viewProfile(username);
    } else if (choice == "17") {
        changePasswordForStaff(username);
    }

    staffMenu(isOff, username);
}

bool _checkCourseRegistrationAvailable() {
    int year = time(0)/31556926 + 1970;
    int month = (time(0) % 31556926) / 2629743 + 1;
    int date = ((time(0) % 31556926) % 2629743) / 86400 + 1;
    cout << "Current date (yyyy/mm/dd): ";
    cout << year << "/" << month << "/" << date << endl;
    ifstream inFile("dataFile/sessions.dat", ios::binary);
    CourseRegistration tmp;
    CourseRegistrationLinkedList *pHead = nullptr;
    CourseRegistrationLinkedList *pCur = pHead;
    while (1) {
        inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
        if (inFile.eof()) break;
        if (pHead == nullptr) {
            pHead = new CourseRegistrationLinkedList;
            pHead->pPrev = nullptr;
            pHead->data = tmp;
            pHead->pNext = nullptr;
            pCur = pHead;
        } else {
            pCur->pNext = new CourseRegistrationLinkedList;
            pCur->pNext->pPrev = pCur;
            pCur = pCur->pNext;
            pCur->data = tmp;
            pCur->pNext = nullptr;
        }
    }
    inFile.close();
    pCur = pHead;

    while (pCur != nullptr) {
        if (pCur->data.end.year < year) {
            if(pCur == pHead) {
                pHead = pHead->pNext;
                delete pCur;
                pCur = pHead;
                if (pCur != nullptr)
                    pCur->pPrev = nullptr;
            } else {
                pCur->pPrev->pNext = pCur->pNext;
                if (pCur->pNext != nullptr) {
                    pCur->pNext->pPrev = pCur->pPrev;
                }
                CourseRegistrationLinkedList *pDel = pCur;
                pCur = pCur->pNext;
                delete pDel;
            }
        } else if (pCur->data.end.year == year){
            if (pCur->data.end.month < month) {
                if(pCur == pHead) {
                    pHead = pHead->pNext;
                    delete pCur;
                    pCur = pHead;
                    if (pCur != nullptr)
                        pCur->pPrev = nullptr;
                } else {
                    pCur->pPrev->pNext = pCur->pNext;
                    if (pCur->pNext != nullptr) {
                        pCur->pNext->pPrev = pCur->pPrev;
                    }
                    CourseRegistrationLinkedList *pDel = pCur;
                    pCur = pCur->pNext;
                    delete pDel;
                }
            } else if (pCur->data.end.month == month){
                if (pCur->data.end.date < date) {
                    if(pCur == pHead) {
                        pHead = pHead->pNext;
                        delete pCur;
                        pCur = pHead;
                        if (pCur != nullptr)
                            pCur->pPrev = nullptr;
                    } else {
                        pCur->pPrev->pNext = pCur->pNext;
                        if (pCur->pNext != nullptr) {
                            pCur->pNext->pPrev = pCur->pPrev;
                        }
                        CourseRegistrationLinkedList *pDel = pCur;
                        pCur = pCur->pNext;
                        delete pDel;
                    }
                }
            }
        }
        if (pCur != nullptr)
            pCur = pCur->pNext;
    }

    _displayCourseRegistration(pHead);
    bool res;
    if (pHead == nullptr) {
        res = false;
    } else {
        res = true;
    }
    ofstream outFile("data/sessions.dat", ios::binary);
    pCur = pHead;
    while (pCur != nullptr) {
        outFile.write(reinterpret_cast<char *>(&pCur->data), sizeof(pCur->data));
        pCur = pCur->pNext;
    }
    outFile.close();
    while (pHead != nullptr) {
        pCur = pHead;
        pHead = pHead->pNext;
        delete pCur;
    }

    return res;
}

void viewOwnScoreBoard(const string &studentID) {
    string schoolyear, semester;
    cout << "Enter schoolyear (begin year only): ";
    getline(cin, schoolyear);
    cout << "Enter semester: ";
    getline(cin, semester);
    string fileName = "studentcourses/" + studentID + "_" + schoolyear + to_string(stoi(schoolyear) + 1) + "_" + semester + ".dat";
    ifstream inFile(fileName, ios::binary);
    if (!inFile.fail()) {
        cout << setw(10) << left << "Course ID";
        cout << setw(12) << left << "Total Mark";
        cout << setw(12) << left << "Final Mark";
        cout << setw(15) << left << "Midterm Mark";
        cout << setw(14) << left << "Other Mark" << endl;
        Course tmpCourse;
        while (true) {
            inFile.read(reinterpret_cast<char *>(&tmpCourse), sizeof(tmpCourse));
            if (inFile.eof()) break;
            string fileName1 = "attendedCourse/" + tmpCourse.courseID + "_scoreboard.dat";
            ifstream inFile1(fileName1, ios::binary);
            if (!inFile1.fail()) {
                Scoreboard tmpScoreboard;
                while (true) {
                    inFile1.read(reinterpret_cast<char *>(&tmpScoreboard), sizeof(tmpScoreboard));
                    if (inFile1.eof()) break;
                    if (tmpScoreboard.StudentID.compare(studentID) == 0) {
                        cout << setw(10) << left << tmpCourse.courseID << " ";
                        cout << setw(12) << left << tmpScoreboard.TotalMark << " ";
                        cout << setw(12) << left << tmpScoreboard.FinalMark << " ";
                        cout << setw(15) << left << tmpScoreboard.MidtermMark << " ";
                        cout << setw(14) << left << tmpScoreboard.OtherMark << " " << endl;
                    }
                }
            }
            inFile1.close();
        }
    } else {
        cout << "No data found!!!" << endl;
    }
    inFile.close();
}   

void changePassword(StudentInfor &studentinfor) {
    string fileName = "studentfile/" + studentinfor.studentID + "_data.dat";
    StudentInfor tmp;
    ifstream inFile(fileName, ios::binary);
    inFile.read(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    inFile.close();
    string newPassword;
    cout << "New password: ";
    getline(cin, newPassword);
    tmp.password = newPassword;
    ofstream outFile(fileName, ios::binary);
    outFile.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    outFile.close();
    cout << "Change password successfully!!!" << endl;
}

void studentMenu(bool &isOff, StudentInfor studentinfor) {
    cout << " -------------------- " << endl;
    cout << "|    STUDENT MENU    |" << endl;
    cout << " -------------------- " << endl;
    string choice;
    if (_checkCourseRegistrationAvailable()) {
        cout << "0. Sign out" << endl;
        cout << "1. Change password" << endl;
        cout << "2. Enroll in a course" << endl;
        cout << "3. Remove a course from enrolled list" << endl;
        cout << "4. View the list of enrolled courses" << endl;
        cout << "5. View a list of course available" << endl;
        cout << "6. View scoreboard" << endl;
        // Add option here
        do {
            cout << "Your choice: ";
            getline(cin, choice);
        } while ((choice[0] != '0' && choice[0] != '1' && choice[0] != '2' && choice[0] != '3' && choice[0] != '4' && choice[0] != '5' && 
        choice[0] != '6') || choice.size() >= 2);
        // Add function below this;
        if (choice[0] == '0') {
            startMenu(isOff);
            return;
        } else if (choice[0] == '1') {
            changePassword(studentinfor);
        } else if (choice[0] == '2') {
            // Enroll in a course
            enrollCourse(studentinfor);
        } else if (choice[0] == '3') {
            // Remove a course from enrolled list
            removeEnrolledCourse(studentinfor.studentID);
        } else if (choice[0] == '4') {
            // View the list of enrolled courses
            displayEnrolledCourses(studentinfor.studentID);
        } else if (choice[0] == '5') {
            displayCourses();
        }  else if (choice[0] == '6') {
            viewOwnScoreBoard(studentinfor.studentID);
        }

        studentMenu(isOff, studentinfor);
    } else {
        cout << "0. Sign out" << endl;
        cout << "1. View the list of enrolled courses" << endl;
        cout << "2. Change password" << endl;
        cout << "3. View scoreboard" << endl;
        // Add option here
        do {
            cout << "Your choice: ";
            getline(cin, choice);
        } while ((choice[0] != '0' && choice[0] != '1' && choice[0] != '2' && choice[0] != '3') || choice.size() >= 2);
        // Add function below this;
        if (choice[0] == '0') {
            startMenu(isOff);
            return;
        } else if (choice[0] == '1') {
            // View the list of enrolled courses
            displayEnrolledCourses(studentinfor.studentID);
        } else if (choice[0] == '2') {
            changePassword(studentinfor);
        } else if (choice[0] == '3') {
            viewOwnScoreBoard(studentinfor.studentID);
        }
        studentMenu(isOff, studentinfor);
    }
}

void _login(bool &isOff) {
    _printGroupLogo();
    StudentInfor studentinfor;
    string username, password;
    int loginResult;
    
    string choice;
    do {
        cout << "0. Quit\t\t";
        cout << "1. Staff\t\t";
        cout << "2. Student" << endl;
        cout << "Your choice: ";
        getline(cin, choice);
    } while((choice[0] != '0' && choice[0] != '1' && choice[0] != '2') || choice.size() >= 2);
    
    if (choice[0] == '0')
        return;
    else if (choice[0] == '1') {
        cout << "Username: ";
        getline(cin, username);
        
        cout << "Password: ";
        getline(cin, password);

        loginResult = _checkLoginForStaff(username, password);
        if (loginResult == 0) {
            cout << "Login successfully" << endl;
            staffMenu(isOff, username);
        }
        else if (loginResult == -1) {
            cout << "Incorrect password" << endl;
        }
        else if (loginResult == -2) {
            cout << "Account does not exist" << endl;
        }
    } else {
        cout << "Student ID: ";
        getline(cin, username);
        
        cout << "Password: ";
        getline(cin, password);

        loginResult = _checkLoginForStudent(username, password, studentinfor);
        if (loginResult == 0) {
            cout << "Login successfully" << endl;
            studentMenu(isOff, studentinfor);
        }
        else if (loginResult == -1) {
            cout << "Incorrect password" << endl;
        }
        else if (loginResult == -2) {
            cout << "Account does not exist" << endl;
        }
    }

    
}

void _signUp() {
    string choice;
    string username, password,fullname, title;
    
    cout << "Username: ";
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
        cout << "Your title: ";
        getline(cin, title);
        StaffInfor staff;
        staff.username = username;
        staff.password = password;
        staff.fullName = fullname;
        staff.title = title;
        string fileName = "staffFile/" + username + ".dat";
        ofstream outFile(fileName, ios::binary);
        outFile.write(reinterpret_cast<char *>(&staff), sizeof(staff));
        outFile.close();
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
    cout << "1. Sign up for staff\t\t";
    cout << "2. Log in\t\t" << endl;
    do {
        cout << "Your choice: ";
        getline(cin, choice);
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
