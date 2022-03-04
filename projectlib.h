#include <string>

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

void startMenu(bool &isOff);
