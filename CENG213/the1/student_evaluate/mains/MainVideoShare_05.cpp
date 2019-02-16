/*Add friendship V2: add "invalid" friendship*/
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
    vs1.addFriendship("userName284", "BLA BLA BLA");
    
    cout<<"Friends of userName284"<<endl;
    vs1.printFriendsOfUser("userName284");

    cout<<"Friends of userName051"<<endl;
    vs1.printFriendsOfUser("userName051");
    
    return 0;
}
