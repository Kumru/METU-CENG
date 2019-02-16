/*Add friendship V1: add valid friendship*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "VideoShare.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    vs1.loadUsers("listUser10.txt");
    
    vs1.addFriendship("userName284", "userName051");
    vs1.addFriendship("userName284", "userName127");
    
    cout<<"Friends of userName284"<<endl;
    vs1.printFriendsOfUser("userName284");

    cout<<"Friends of userName127"<<endl;
    vs1.printFriendsOfUser("userName127");
    
    return 0;
}
