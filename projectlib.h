#ifndef _PROJECTLIB_
#define _PROJECTLIB_

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

struct Date {
    int year;
    int month;
    int date;
};

struct CourseRegistration {
    Date start;
    Date end;
};

struct Semester {

};

struct SchoolYear {
    int begin, end;
    bool sems[3] = {false, false, false};
    Semester semesters[3];
};

struct SchoolYearLinkedList {
    SchoolYearLinkedList *pPrev;
    SchoolYear data;
    SchoolYearLinkedList *pNext;
};

struct DNodeSYear {
    SchoolYear data;
    DNodeSYear* pNext, *pPrev;
};

struct Student {
    string No, StudentID, FName, LName, Gender, DoB, SocialID;
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

#endif