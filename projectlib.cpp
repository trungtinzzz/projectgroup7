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

bool _checkCreatedYear(int begin) {
    SchoolYear tmp;
    ifstream inFile("schoolYear.dat", ios::binary);
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
    ifstream inFile("firstYearClasses.dat", ios::binary);
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
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
        if (courseID.compare(tmpCourse.courseID) == 0) return true;
    }

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
    Course tmpEnrolledCourse;
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&tmpEnrolledCourse), sizeof(tmpEnrolledCourse));
        if (tmpCourse.day1.compare(tmpEnrolledCourse.day1) == 0 && tmpCourse.session1.compare(tmpEnrolledCourse.session1) == 0) return true;
        if (tmpCourse.day2.compare(tmpEnrolledCourse.day2) == 0 && tmpCourse.session2.compare(tmpEnrolledCourse.session2) == 0) return true;
    }

    return false;
}

int numOfEnrolledCourses(string fileStudentName, int schoolyearBegin, string semester) {
    ifstream inFile(fileStudentName, ios::binary);
    Course tmpCourse;
    int count = 0;
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
    cout << "Enter year begin: ";
    cin >> x.begin;
    x.end = x.begin + 1;
    if (_checkCreatedYear(x.begin)) {
        cout << "School year already existed!" << endl;
        return;
    }

    ofstream outFile("schoolYear.dat", ios::app | ios::binary);
    outFile.write(reinterpret_cast<char*>(&x), sizeof(x));
    outFile.close();

    cout << "Create school year successfully!\n";
}

void createClass() {
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

    ofstream outFile("firstYearClasses.dat", ios::app | ios::binary);
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
    cin.ignore(1000, '\n');
    getline(cin, className);

    if (!_checkCreatedClass(className)) {
        cout << "Class doesn't exist. Enter again!\n";
        return;
    }
    
    string fileName = "studentlist/" + className + ".dat";
    ofstream outFile(fileName, ios::app | ios::binary);
    string csvFileName;
    cout << "Enter the file name: ";
    cin >> csvFileName;
    
    string data[1000][7];
    string line, word;

    fstream file(csvFileName, ios::in);
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
        Student tmp;
        tmp.No = data[i][0];
        tmp.StudentID = data[i][1];
        tmp.FName = data[i][2];
        tmp.LName = data[i][3];
        tmp.Gender = data[i][4];
        tmp.DoB = data[i][5];
        tmp.SocialID = data[i][6];

        outFile.write(reinterpret_cast<char*>(&tmp), sizeof(tmp));
        
    }
    
    outFile.close();
    cout << "Add students to class " << className << " successfully!\n";
}

void displayList() {
    string classFileName;
    ifstream inFile("firstYearClasses.dat", ios::binary);
    ifstream studentFile;
    string studentFileName;
    Student tmpStudent;
    int count = 0;
    int studentCount;
    while(!inFile.eof()) {
        inFile.read(reinterpret_cast<char *>(&classFileName), sizeof(classFileName));
        if (inFile.eof()) {
            break;
        } 
        cout << "Class " << classFileName << ":" << endl;
        studentFileName = "studentlist/" + classFileName + ".dat";
        studentFile.open(studentFileName, ios::binary);
        studentCount = 0;
        while(!studentFile.eof()) {
            studentFile.read(reinterpret_cast<char *>(&tmpStudent), sizeof(tmpStudent));
            if (studentFile.eof()) {
                studentFile.close();
                break;
            } 
            cout << "\t" << tmpStudent.No << " " << tmpStudent.StudentID << " ";
            cout << tmpStudent.FName << " " << tmpStudent.LName << " ";
            cout << tmpStudent.Gender << " " << tmpStudent.DoB << " " << tmpStudent.SocialID << " ";
            cout << endl;
            studentCount++;
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
    if (course.start.year > course.end.year || course.start.year <= 0 || course.end.year <= 0) {
        return false;
    } else if (course.start.month > course.end.month || course.start.month <=0 || course.end.month <= 0) {
        return false;
    } else if (course.start.date > course.end.date || course.start.date <=0 || course.end.date <=0) {
        return false;
    }
    return true;
}

void addCourseRegistrationSession(SchoolYear schoolyear, int semester) {
    CourseRegistration tmp;
    string tmpStartDate, tmpStartMonth, tmpStartYear, tmpEndDate, tmpEndMonth, tmpEndYear;
    bool inputGood = false;
    cout << "Add course registration session" << endl;
    do {
        cout << "Start" << endl;
        cout << "Year: ";
        cin >> tmpStartYear;
        cout << "Month: ";
        cin >> tmpStartMonth;
        cout << "Date: ";
        cin >> tmpStartDate;
        cout << "----------" << endl;
        cout << "End" << endl;
        cout << "Year: ";
        cin >> tmpEndYear;
        cout << "Month: ";
        cin >> tmpEndMonth;
        cout << "Date: ";
        cin >> tmpEndDate;
        try {
            tmp.start.year = stoi(tmpStartYear);
            tmp.start.month = stoi(tmpStartMonth);
            tmp.start.date = stoi(tmpStartDate);
            tmp.end.year = stoi(tmpEndYear);
            tmp.end.month = stoi(tmpEndMonth);
            tmp.end.date = stoi(tmpEndDate);
            if (_checkCourseRegistration(tmp))
                inputGood = true;
            else 
                cout << "Invalid input (End date must later than start date)" << endl;
        } catch (...) {
            cout << "Invalid input" << endl;
        }
    } while (!inputGood || !_checkCourseRegistration(tmp));
    tmp.schoolyear = schoolyear.begin;
    tmp.semester = semester;
    ofstream outFile("sessions.dat", ios::binary | ios::app);
    outFile.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    outFile.close();
}

void addCourse(SchoolYear schoolyear, int semester) {
    Course tmp;
    cout << "Add courses to semester" << endl;
    cout << "Course ID: ";
    cin >> tmp.courseID;
    cout << "Course Name: ";
    cin.ignore(1000, '\n');
    getline(cin, tmp.courseName);
    cout << "Teacher Name: ";
    getline(cin, tmp.teacherName);
    cout << "Number of credits: ";
    cin >> tmp.credits;
    cout << "Number of students: ";
    cin >> tmp.maxStudent;
    cin.ignore(1000, '\n');

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
                    inputGood = true;
                    break;
                case '2':
                    tmp.day2 = "TUE";
                    inputGood = true;
                    break;
                case '3':
                    tmp.day2 = "WED";
                    inputGood = true;
                    break;
                case '4':
                    tmp.day2 = "THU";
                    inputGood = true;
                    break;
                case '5':
                    tmp.day2 = "FRI";
                    inputGood = true;
                    break;
                case '6':
                    tmp.day2 = "SAT";
                    inputGood = true;
                    break;
                case '7':
                    tmp.day2 = "SUN";
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
    string fileName;
    fileName = "courselist/" + to_string(schoolyear.begin) + to_string(schoolyear.end) + "_" + to_string(semester) + ".dat";
    ofstream outFile(fileName, ios::binary | ios::app);
    outFile.write(reinterpret_cast<char *>(&tmp), sizeof(tmp));
    outFile.close();
}

void createSemester() {
    cout << "List of school years created:" << endl;
    SchoolYearLinkedList *pHead = nullptr;
    SchoolYearLinkedList *pRead = pHead;
    SchoolYearLinkedList *pNew = nullptr;
    SchoolYear tmp;
    ifstream inFile("schoolYear.dat", ios::binary);
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
            cin >> choiceStr;
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
            cin >> semChoice;
        } while (semChoice[0] != '1' && semChoice[0] != '2' && semChoice[0] != '3' || semChoice.size() >= 2); 

        bool checkEmptySem = false;
        while (!checkEmptySem) {
            if (semChoice[0] == '1') {
                if (theChosen->data.sems[0] == true) {
                    cout << "It is already created" << endl;
                    cout << "What semester do you want to create 1, 2 or 3: ";
                    cin >> semChoice;
                }
                else {
                    checkEmptySem = true;
                    theChosen->data.sems[0] = true;
                    addCourseRegistrationSession(theChosen->data, 1);
                    addCourse(theChosen->data, 1);
                }
            } else if (semChoice[0] == '2') {                
                if (theChosen->data.sems[1] == true) {
                    cout << "It is already created" << endl;
                    cout << "What semester do you want to create 1, 2 or 3: ";
                    cin >> semChoice;
                }
                else {
                    checkEmptySem = true;
                    theChosen->data.sems[1] = true;
                    addCourseRegistrationSession(theChosen->data, 2);
                    addCourse(theChosen->data, 2);
                }
            } else {
                if (theChosen->data.sems[2] == true) {
                    cout << "It is already created" << endl;
                    cout << "What semester do you want to create 1, 2 or 3: ";
                    cin >> semChoice;
                }
                else {
                    checkEmptySem = true;
                    theChosen->data.sems[2] = true;
                    addCourseRegistrationSession(theChosen->data, 3);
                    addCourse(theChosen->data, 3);
                }
            }
        }
        cout << "Semester is successfully created" << endl;
    }
    inFile.close();
    
    ofstream outFile("schoolYear.dat", ios::binary | ios::out);
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
    ifstream inFile("schoolYear.dat", ios::binary);
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
                    cout << "\t\tNo courses added" << endl;
                else
                    cout << "\t\tThis semester is not added" << endl;
            } else {
                while (!inCourseFile.eof()) {
                    inCourseFile.read(reinterpret_cast<char *>(&tmpCourse), sizeof(tmpCourse));
                    if (inCourseFile.eof()) break;
                    cout << "\t\t";
                    cout << tmpCourse.courseID << " ";
                    cout << tmpCourse.courseName << " ";
                    cout << tmpCourse.teacherName << " ";
                    cout << tmpCourse.maxStudent << " ";
                    cout << tmpCourse.credits << " ";
                    cout << tmpCourse.day1 << " ";
                    cout << tmpCourse.session1 << " ";
                    cout << tmpCourse.day2 << " ";
                    cout << tmpCourse.session2 << endl;
                }
            }
            inCourseFile.close();
        }
        count++;   
    }
    if (count == 0)
        cout << "No school years created!!!" << endl;
    inFile.close();
}

void enrollCourse(string username) {
    string fileStudentName = "studentfile/" + username + ".dat";
    
    displayCourses();

    int schoolyearBegin;
    cout << "What school year are you studying? (Type begin year only) ";
    cin >> schoolyearBegin;
    
    string semester;
    do {
        cout << "What semester are you studying? (1, 2, 3) ";
        cin >> semester;
    } while ((semester[0] != '1' && semester[0] != '2' && semester[0] != '3') || semester.size() >= 2);

    if (numOfEnrolledCourses(fileStudentName, schoolyearBegin, semester) == 5) {
        cout << "You have enrolled in 5 courses in this semester. You can't enroll in anymore!" << endl;
        return;
    }

    string schoolyear = to_string(schoolyearBegin) + to_string(schoolyearBegin + 1); 
    string fileCourseName;
    fileCourseName = "courselist/" + schoolyear + "_" + semester + ".dat";

    string courseID;
    do {
        cout << "What course ID do you want to enroll in? ";
        cin >> courseID;
        if (_checkCreatedCourse(fileCourseName, courseID)) break;
        else cout << "This course doesn't exist!" << endl;
    } while (true);

    if (!_checkConflictedSession(fileCourseName, courseID, fileStudentName)) {
        ifstream inFile(fileCourseName, ios::binary);
        Course tmpCourse;
        while (!inFile.eof()) {
            inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
            if (courseID.compare(tmpCourse.courseID) == 0) break;
        }
        inFile.close();

        ofstream outFile(fileStudentName, ios::binary | ios:: app);
        outFile.write(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
        outFile.close();

        cout << "Enroll in successfully!" << endl;
    }
    else cout << "You can't enroll in this course because of 2 sessions are conflicted." << endl;
}

void displayEnrolledCourses(string username) {
    cout << "List of enrolled courses: " << endl;
    string fileStudentName = "studentfile/" + username + ".dat";

    ifstream inFile(fileStudentName, ios::binary);
    Course tmpCourse;
    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
        if (inFile.eof()) break;
        cout << "\t";
        cout << tmpCourse.courseID << " ";
        cout << tmpCourse.courseName << " ";
        cout << tmpCourse.teacherName << " ";
        cout << tmpCourse.maxStudent << " ";
        cout << tmpCourse.credits << " ";
        cout << tmpCourse.day1 << " ";
        cout << tmpCourse.session1 << " ";
        cout << tmpCourse.day2 << " ";
        cout << tmpCourse.session2 << endl;
    }
}

void removeEnrolledCourse(string username) {
    string fileStudentName = "studentfile/" + username + ".dat";
    
    displayEnrolledCourses(username);
    
    ifstream inFile(fileStudentName, ios::binary);
    Course tmpCourse;
    string courseID;
    do {
        cout << "What course ID do you want to remove? ";
        cin >> courseID;
        if (_checkCreatedCourse(fileStudentName, courseID)) break;
        else cout << "You haven't enrolled in this course yet!" << endl;
    } while (true);

    string tmpFileName = "studentfile/tmpfile.dat";
    ofstream outFile(tmpFileName, ios::binary | ios::app);

    while (!inFile.eof()) {
        inFile.read(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
        if (inFile.eof()) break;

        if (courseID.compare(tmpCourse.courseID) == 0) continue;

        outFile.write(reinterpret_cast<char*>(&tmpCourse), sizeof(tmpCourse));
    }

    int removeStudentFile = remove(fileStudentName.c_str());
    int renameTmpFile = rename(tmpFileName.c_str(), fileStudentName.c_str());

    cout << "Remove this course successfully!" << endl;
    inFile.close();
    outFile.close();
}

void staffMenu(bool &isOff) {
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

    do {
        cout << "Your choice: ";
        cin >> choice;
    } while ((choice[0] != '0' && choice[0] != '1' && choice[0] != '2' && choice[0] != '3' && choice[0] != '4' && choice[0] != '5' && choice[0] != '6' && choice[0] != '7' && choice[0] != '8') || choice.size() >= 2);
    // Add function below this;
    if (choice[0] == '0') {
        startMenu(isOff);
        return;
    } else if (choice[0] == '1') {
        // Create school year
        createSchoolYear();
    } else if (choice[0] == '2') {
        // Create 1st-year classes
        createClass();
    } else if (choice[0] == '3') {
        // Add new 1st-year student to 1st-year classes
        addStudent();
    } else if (choice[0] == '4') {
        // Display list of student
        displayList();
    } else if (choice[0] == '5') {
        // Create semester
        createSemester();
    } else if (choice[0] == '6') {
        // Display courses
        displayCourses();
    } else if (choice[0] == '7') {

    } else if (choice[0] == '8') {
    
    }

    staffMenu(isOff);
}

void studentMenu(bool &isOff, string username) {
    cout << " -------------------- " << endl;
    cout << "|    STUDENT MENU    |" << endl;
    cout << " -------------------- " << endl;
    string choice;
    cout << "0. Sign out" << endl;
    cout << "1. Enroll in a course" << endl;
    cout << "2. View the list of enrolled courses" << endl;
    cout << "3. Remove a course from enrolled list" << endl;
    // Add option here
    cout << endl;
    do {
        cout << "Your choice: ";
        cin >> choice;
    } while ((choice[0] != '0' && choice[0] != '1' && choice[0] != '2' && choice[0] != '3') || choice.size() >= 2);
    // Add function below this;
    if (choice[0] == '0') {
        startMenu(isOff);
        return;
    } else if (choice[0] == '1') {
        // Enroll in a course
        enrollCourse(username);
    } else if (choice[0] == '2') {
        // View the list of enrolled courses
        displayEnrolledCourses(username);
    } else if (choice[0] == '3') {
        // Remove a course from enrolled list
        removeEnrolledCourse(username);
    }

    studentMenu(isOff, username);
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
        studentMenu(isOff, username);
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

            // Create personal student file:
            string fileName = "studentfile/" + username + ".dat";
            outFile.open(fileName, ios::binary | ios::app);
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
