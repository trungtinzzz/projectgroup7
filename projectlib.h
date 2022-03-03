#include <string>

struct StudentAccount {
    std::string username;
    std::string password;
    std::string fullName;
};

struct TeacherAccount {
    std::string username;
    std::string password;
    std::string fullName;
};

void startMenu();
void Login(bool &isSignUporLogined);
void SignUp(bool &isSignUporLogined);