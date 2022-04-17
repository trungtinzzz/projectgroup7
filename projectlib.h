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
using namespace std;

struct Scoreboard {
    string courseID;
    string No, StudentID, FullName;
    double TotalMark, FinalMark, MidtermMark, OtherMark;
};

struct StudentInfor {
    string username;
    string password;
    string fullName;
    string studentID;
    int numOfCourse;
    Scoreboard score[100];
    double semesterGPA[4], overallGPA;
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
    int schoolyear;
    int semester;
    Date start;
    Date end;
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

struct Student {
    string No, StudentID, FName, LName, Gender, DoB, SocialID;
};


struct StudentLinkedList {
    Student data;
    StudentLinkedList* pNext, *pPrev;
};

void createSchoolYear();
void createClass();
void addStudent();
void enrollCourse(string username);
void displayEnrolledCourses(string username);
void removeEnrolledCourse(string username);

void startMenu(bool &isOff);

#endif