#ifndef _PROJECTLIB_
#define _PROJECTLIB_

#include <string>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;

struct Scoreboard {
    string courseID, StudentID, FullName;
    double TotalMark, FinalMark, MidtermMark, OtherMark;
};

struct StudentGPA {
    double GPA1 = -1;
    double GPA2 = -1;
    double GPA3 = -1;
};

struct StudentInfor {
    string no, studentID, fullName, gender, DoB, socialID, password;
};

struct StudentInforLinkedList {
    StudentInforLinkedList *pPrev;
    StudentInfor data;
    StudentInforLinkedList *pNext;
};

struct StaffInfor {
    string username;
    string password;
    string fullName;
    string title;
};

struct Date {
    int year;
    int month;
    int date;
};

struct CourseRegistration {
    int schoolyear;
    int semester;
    Date end;
};

struct CourseRegistrationLinkedList {
    CourseRegistrationLinkedList *pPrev;
    CourseRegistration data;
    CourseRegistrationLinkedList *pNext;
};

struct SchoolYear {
    int begin, end;
    bool sems[3] = {false, false, false};
};

struct SchoolYearLinkedList {
    SchoolYearLinkedList *pPrev;
    SchoolYear data;
    SchoolYearLinkedList *pNext;
};

struct Course {
    int schoolyearBegin;
    int semester;
    string courseID, courseName, teacherName;
    int credits, maxStudent;
    string day1, day2;
    string session1, session2;
};

struct CourseLinkedList {
    CourseLinkedList *pPrev;
    Course data;
    CourseLinkedList *pNext;
};



void createSchoolYear();
void createClass();
void addStudent();
void enrollCourse(string username);
void displayEnrolledCourses(string username);
void removeEnrolledCourse(string username);

void startMenu(bool &isOff);

#endif