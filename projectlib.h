#ifndef PROJLIB_H
#define PROJLIB_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
using namespace std;

struct SchoolYear {
    int begin, end;
};

struct DNodeSYear {
    SchoolYear data;
    DNodeSYear* pNext, *pPrev;
};

struct Student {
    int No, StudentID;
    string FName, LName, Gender, DoB;
    int SocialID;
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
bool AddStudentToClass(DNodeClass* &pHead, string className);

void createSchoolYear(DNodeSYear* &schoolYear);
void createClass(DNodeClass* &newClasses);
void addStudent(DNodeClass* &newClasses);

#endif