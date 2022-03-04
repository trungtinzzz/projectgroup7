#include "projectlib.h"

int main() {
    // Create school year
    DNodeSYear *schoolYear = new DNodeSYear;
    schoolYear = NULL;
    createSchoolYear(schoolYear);

    // Create 1st-year classes
    DNodeClass *newClasses = new DNodeClass;
    newClasses = NULL;
    createClass(newClasses);

    // Add new 1st-year student to 1st-year classes
    addStudent(newClasses);

    // Display list of student
    DNodeClass* pCur = newClasses;
    while (pCur != NULL) {
        cout << "Class: " << pCur->className << endl;
        DNodeStudent* pStCur = pCur->StudentList;
        while (pStCur != NULL) {
            cout << pStCur->data.No << " " << pStCur->data.StudentID << " ";
            cout << pStCur->data.FName << " " << pStCur->data.LName << " ";
            cout << pStCur->data.Gender << " " << pStCur->data.DoB << " ";
            cout << pStCur->data.SocialID << endl;
            pStCur = pStCur->pNext;
        }
        cout << endl;
        pCur = pCur->pNext;
    }

    return 0;
}
