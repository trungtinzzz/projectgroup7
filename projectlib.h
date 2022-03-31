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

struct Student {
    string No, StudentID, FName, LName, Gender, DoB, SocialID;
};



struct StudentLinkedList {
    Student data;
    StudentLinkedList* pNext, *pPrev;
};

struct Course {
    int startDate, endDate;
    string courseID, courseName, teacherName, credits, day1, day2, session1, session2;
};

void AddYearAtTail(SchoolYearLinkedList* &pHead, SchoolYear x);
void AddIn4Student(StudentLinkedList* &pHead, Student x);
//int AddStudentToClass(ClassLinkedList* &pHead, string className);

void createSchoolYear();
void createClass();
void addStudent();

void startMenu(bool &isOff);

void DeallocateSchoolYear(SchoolYearLinkedList* &pHead);
void DeallocateStudent(StudentLinkedList* &pHead);

#endif