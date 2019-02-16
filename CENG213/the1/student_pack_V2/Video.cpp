#include "Video.hpp"

ostream& operator<<(ostream& out, const Video & video) {
    out << "title:" << video.title << ",genre:" << video.genre << endl;
    return out;
}

/* TO-DO: method implementations below */
 

