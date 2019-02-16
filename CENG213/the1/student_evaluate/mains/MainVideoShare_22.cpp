//isConnected V1
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include "VideoShare.hpp"

using namespace std;




int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    vs1.createUser("u1");
    vs1.createUser("u2");
    vs1.createUser("u3");
    vs1.createUser("u4");
    vs1.createUser("u5");
    vs1.createUser("u6");
    vs1.createUser("u7");
    
    vs1.addFriendship("u1","u2");
    vs1.addFriendship("u1","u3");
    vs1.addFriendship("u1","u4");
    vs1.addFriendship("u4","u5");
    
    vs1.addFriendship("u7","u6");
    
    
    if (vs1.isConnected("u4","u1")) cout<<"u4 and u1 are connected"<<endl;
    else cout<<"u4 and u1 are not connected"<<endl;
    
    if (vs1.isConnected("u1","u7")) cout<<"u1 and u7 are connected"<<endl;
    else cout<<"u1 and u7 are not connected"<<endl;

    return 0;
}
