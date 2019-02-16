/*Remove friendship V2: remove "invalid" friendship*/
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
    
    vs1.removeFriendship("userName284", "BLA BLA BLA");
    
    cout<<"After removal of 284-X friendship"<<endl;
    
    cout<<"Friends of userName284"<<endl;
    vs1.printFriendsOfUser("userName284");

    cout<<"Friends of userName127"<<endl;
    vs1.printFriendsOfUser("userName127");
    
    
    
    return 0;
}
