/*subscribe V2: subscribe "valid" video to "invalid" user*/
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
    
    vs1.subscribe("INVALID_USER","title061");
    
    cout<<"No error occured"<<endl;
    
    return 0;
}
