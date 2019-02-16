//isConnected V2
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include "VideoShare.hpp"

using namespace std;

#define SSTR( x ) static_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()
#define NAME( x , y ) x + SSTR(y)
#define MIN  0
#define MAX  100

void populate_VideoShare(VideoShare & vs) {
    for (int i = MIN; i < MAX; i++) {
        vs.createUser(NAME("u", i));
        vs.createVideo(NAME("v", i), "");
    }
}

void create_random_friendships(VideoShare & vs) {
    srand(39);

    vector< vector <int> > isFriend(MAX, std::vector<int>(MAX, -1));

    for (int i = MIN; i < MAX * 10; i++) {
        int u1 = rand() % MAX;
        int u2 = rand() % MAX;

        if ((u1 != u2) && (isFriend[u1][u2] == -1)) {
            isFriend[u1][u2] = 0;
            isFriend[u2][u1] = 0;
            vs.addFriendship(NAME("u", u1), NAME("u", u2));
        }
    }

}

int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    vs1.createUser("u1");
    vs1.createUser("u2");
    vs1.createUser("u3");
    vs1.createUser("u4");
    vs1.createUser("u5");
    vs1.createUser("u6");
    vs1.createUser("u7");
    vs1.createUser("u8");
    vs1.createUser("u9");
    vs1.createUser("u10");
    vs1.createUser("u11");
    
    
    vs1.addFriendship("u1","u2");
    vs1.addFriendship("u1","u3");
    vs1.addFriendship("u1","u4");
    
    vs1.addFriendship("u4","u5");
    vs1.addFriendship("u4","u6");
    vs1.addFriendship("u4","u7");
    
    vs1.addFriendship("u9","u10");
    vs1.addFriendship("u9","u11");
    vs1.addFriendship("u9","u12");
    
    
    
    
    if (vs1.isConnected("u1","u1")) cout<<"u1 and u1 are connected"<<endl;
    else cout<<"u1 and u1 are not connected"<<endl;
    
    if (vs1.isConnected("u3","u11")) cout<<"u3 and u11 are connected"<<endl;
    else cout<<"u3 and u11 are not connected"<<endl;
    
    if (vs1.isConnected("u4","u3")) cout<<"u4 and u3 are connected"<<endl;
    else cout<<"u4 and u3 are not connected"<<endl;
    
    

    return 0;
}
