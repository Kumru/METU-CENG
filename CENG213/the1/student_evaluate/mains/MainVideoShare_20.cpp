//printFriendSubscriptions
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

void create_random_subscription(VideoShare & vs) {
    srand(39);
    vector< vector <int> > isSubscribed(MAX, std::vector<int>(MAX, -1));
    for (int i = MIN; i < MAX * 30; i++) {
        int u = rand() % MAX;
        int v = rand() % MAX;

        if ((isSubscribed[u][v] == -1)) {
            isSubscribed[u][v] = 0;
            vs.subscribe(NAME("u", u), NAME("v", v));
        }

    }
}

int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    populate_VideoShare(vs1);

    create_random_subscription(vs1);

    cout << "u0" << endl;
    vs1.printUserSubscriptions("u0");
    cout << "u1" << endl;
    vs1.printUserSubscriptions("u1");
    cout << "end" << endl;

    cout << "------After Sort-----" << endl;
    vs1.sortUserSubscriptions("u0");
    vs1.sortUserSubscriptions("u1");

    cout << "u0" << endl;
    vs1.printUserSubscriptions("u0");
    cout << "u1" << endl;
    vs1.printUserSubscriptions("u1");
    cout << "end" << endl;

    cout << "Common Subscriptions" << endl;
    vs1.printCommonSubscriptions("u0", "u1");
    return 0;
}
