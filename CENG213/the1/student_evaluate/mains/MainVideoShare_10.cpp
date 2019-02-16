/*subscribe V3: subscribe "invalid" video to "valid" user*/
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
    vs1.subscribe("userName056","INVALID");
    vs1.subscribe("userName056","title384");
    
    vs1.printUserSubscriptions("userName056");
    
    return 0;
}
