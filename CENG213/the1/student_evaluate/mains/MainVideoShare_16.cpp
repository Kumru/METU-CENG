/*updateUserStatus + unsubscribe*/
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
    vs1.loadVideos("listVideo10.txt");

    vs1.subscribe("userName056", "title061");
    vs1.subscribe("userName056", "title135");
    vs1.subscribe("userName056", "title384");
    
    vs1.updateUserStatus("userName056", SUSPENDED);
    
    vs1.unSubscribe("userName056", "title384");

    vs1.printUserSubscriptions("userName056"); 


    return 0;
}
