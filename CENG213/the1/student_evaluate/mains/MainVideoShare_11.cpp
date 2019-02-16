/*unsubscribe V1: unsubscibe "valid" video from "valid" user*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "VideoShare.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    vs1.loadUsers("listUser10.txt");
    vs1.loadVideos("listVideo10.txt");
    
    vs1.subscribe("userName056","title061");
    vs1.subscribe("userName056","title135");
    vs1.subscribe("userName056","title384");
    
    
    vs1.unSubscribe("userName056","title135");
    
    cout<<"Subscriptions of userName056" <<endl;
    vs1.printUserSubscriptions("userName056");
    
    
    return 0;
}
