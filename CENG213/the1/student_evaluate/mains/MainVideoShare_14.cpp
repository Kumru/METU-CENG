/*updateUserStatus*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "VideoShare.hpp"
#include "User.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    vs1.loadUsers("listUser10.txt");
    
    vs1.printAllUsers();
    
    vs1.updateUserStatus("userName200",SUSPENDED);
    vs1.updateUserStatus("userName284",SUSPENDED);
    
    cout<< "Status of userName200 and userName284 should be SUSPENDED"<<endl;
    
    vs1.printAllUsers();
    
    return 0;
}
