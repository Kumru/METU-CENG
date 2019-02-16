/*Load Videos*/
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "VideoShare.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    VideoShare vs1 = VideoShare();
    vs1.loadVideos("listVideo1.txt");
    vs1.printAllVideos();
    return 0;
}
