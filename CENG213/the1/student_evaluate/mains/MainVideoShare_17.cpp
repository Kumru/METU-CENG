/*Delete user V1: delete user with no friendship*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "VideoShare.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    vs1.loadUsers("listUser10.txt");
    
    vs1.printAllUsers(); 
   
    vs1.deleteUser("userName200");
    vs1.deleteUser("userName051");
    vs1.deleteUser("userName284");
    vs1.deleteUser("userName265");
    vs1.deleteUser("userName450");
    
    cout<<"After Delete Operation"<<endl;
    vs1.printAllUsers();
    
    
    return 0;
}
