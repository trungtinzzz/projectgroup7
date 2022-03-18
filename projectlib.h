#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;
struct StudentInfor {
    std::string username;
    std::string password;
    std::string fullName;
};

struct StaffInfor {
    std::string username;
    std::string password;
    std::string fullName;
};
struct SchoolYear {
    int begin, end;
};

struct DNodeSYear {
    SchoolYear data;
    DNodeSYear* pNext, *pPrev;
};

struct Student {
    int No;
    string StudentID, FName, LName, Gender, DoB, SocialID;
};

struct DNodeStudent {
        Student data;
        DNodeStudent* pNext, *pPrev;
};
struct DNodeClass {
    string className;
    DNodeStudent *StudentList;
    DNodeClass* pNext, *pPrev;
};

struct Semester {
    int semester;
    int startDate, endDate;
};

struct Course {
    int startDate, endDate;
    string courseID, courseName, teacherName, credits, day1, day2, session1, session2;
};

void AddYearAtTail(DNodeSYear* &pHead, SchoolYear x);
void AddClassAtTail(DNodeClass* &pHead, string className);
void AddIn4Student(DNodeStudent* &pHead, Student x);
int AddStudentToClass(DNodeClass* &pHead, string className);

void createSchoolYear(DNodeSYear* &schoolYear);
void createClass(DNodeClass* &newClasses);
void addStudent(DNodeClass* &newClasses);

void startMenu(bool &isOff);
void loadFileToLinkedList();
