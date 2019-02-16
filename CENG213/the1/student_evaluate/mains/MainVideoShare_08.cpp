/*subscribe V1: subscribe "valid" video to "valid" user*/
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
    
    vs1.subscribe("userName341","title061");
    vs1.subscribe("userName341","title051");
    vs1.subscribe("userName341","title353");
    
    cout<<"Subscriptions of userName056" <<endl;
    vs1.printUserSubscriptions("userName056");
    
    cout<<"Subscriptions of userName341" <<endl;
    vs1.printUserSubscriptions("userName341");
    
    return 0;
}
