/*sortUserSubscriptions V1: small set*/
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
    
    vs1.subscribe("userName392", "title311");
    vs1.subscribe("userName392", "title061");
    vs1.subscribe("userName392", "title135");
    vs1.subscribe("userName392", "title269");
    vs1.subscribe("userName392", "title343");
    vs1.subscribe("userName392", "title276");
    vs1.subscribe("userName392", "title384");
    vs1.subscribe("userName392", "title109");
    vs1.subscribe("userName392", "title051");
    vs1.subscribe("userName392", "title353");

    cout << "Subscriptions of userName392" << endl;
    vs1.printUserSubscriptions("userName392");

    vs1.subscribe("userName265", "title384");
    vs1.subscribe("userName265", "title109");
    vs1.subscribe("userName265", "title051");
    vs1.subscribe("userName265", "title353");

    cout << "Subscriptions of userName265" <<endl;
    vs1.printUserSubscriptions("userName265");

    vs1.sortUserSubscriptions("userName392");

    cout << "After Sort Operation" << endl<<endl;

    cout << "Subscriptions of userName392" << endl;
    vs1.printUserSubscriptions("userName392");

    cout << "Subscriptions of userName265" << endl;
    vs1.printUserSubscriptions("userName265");

    return 0;
}
