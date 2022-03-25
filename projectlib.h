#include <string>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;
struct StudentInfor {
    string username;
    string password;
    string fullName;
};

struct StaffInfor {
    string username;
    string password;
    string fullName;
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

void AddYearAtTail(DNodeSYear* &pHead, SchoolYear x);
void AddClassAtTail(DNodeClass* &pHead, string className);
void AddIn4Student(DNodeStudent* &pHead, Student x);
int AddStudentToClass(DNodeClass* &pHead, string className);

void createSchoolYear(DNodeSYear* &schoolYear);
void createClass(DNodeClass* &newClasses);
void addStudent(DNodeClass* &newClasses);

void startMenu(bool &isOff);
void loadFileToLinkedList();
