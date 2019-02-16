/*Delete user V2: delete user with friends*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "VideoShare.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    vs1.loadUsers("listUser10.txt");

    vs1.addFriendship("userName200", "userName051");
    vs1.addFriendship("userName200", "userName284");
    vs1.addFriendship("userName200", "userName265");

    vs1.addFriendship("userName284", "userName265");
    vs1.addFriendship("userName284", "userName051");

    cout << "Friends of userName200" << endl;
    vs1.printFriendsOfUser("userName200");

    cout << "Friends of userName284" << endl;
    vs1.printFriendsOfUser("userName284");

    vs1.deleteUser("userName200");
    cout << "AFTER DELETE" << endl;

    cout << "Friends of userName200" << endl;
    vs1.printFriendsOfUser("userName200");

    cout << "Friends of userName284" << endl;
    vs1.printFriendsOfUser("userName284");


    return 0;
}
