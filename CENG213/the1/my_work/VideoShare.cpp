#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "VideoShare.hpp"
#include "User.hpp"

using namespace std;

void VideoShare::printAllVideos() {
    videos.print();
}

void VideoShare::printAllUsers() {
    users.print();
}

/* TO-DO: method implementations below */

VideoShare::VideoShare(){

}

VideoShare::~VideoShare(){
    
}

void VideoShare::createUser(const string & userName, const string & name, const string & surname){
    User tmp = User(userName, name, surname);
    users.insertNode(users.getHead(), tmp);
}

void VideoShare::loadUsers(const string & fileName){
    string uName, nName, sName, line;
    int fstPos, scdPos;
    ifstream newFile;
    newFile.open(fileName.c_str());
    while(getline(newFile, line)){
        fstPos = line.find(";");
        if(fstPos==0) continue;
		uName = line.substr(0, fstPos);
        scdPos = line.find(";", fstPos+1);
        nName = line.substr(fstPos+1, scdPos-fstPos-1);
		sName = line.substr(scdPos+1);
		createUser(uName, nName, sName);
	}
}

void VideoShare::createVideo(const string & title, const string & genre){
    Video tmp = Video(title, genre);
    videos.insertNode(videos.getHead(), tmp);
}

void VideoShare::loadVideos(const string & fileName){
    string tName, gName, line;
    int fstPos;
    ifstream newFile;
    newFile.open(fileName.c_str());
    while(getline(newFile, line)){
        fstPos = line.find(";");
        if(fstPos==0) continue;
		tName = line.substr(0, fstPos);
		gName = line.substr(fstPos+1);
		createVideo(tName, gName);
	}
}

Node<User>* VideoShare::findUser(const string & userName){
    Node<User>* a = users.first();
    while(a){
        if(a->getDataPtr()->getUsername() == userName) return a;
        a = a->getNext();
    }
    return NULL;
}

void VideoShare::addFriendship(const string & userName1, const string & userName2){
    Node<User> *user1, *user2;
    user1 = findUser(userName1);
    user2 = findUser(userName2);

    if(user1 && user2){
        user1->getDataPtr()->addFriend(user2->getDataPtr());
        user2->getDataPtr()->addFriend(user1->getDataPtr());
    }
}

void VideoShare::removeFriendship(const string & userName1, const string & userName2){
    Node<User> *user1, *user2;
    user1 = findUser(userName1);
    user2 = findUser(userName2);

    if(user1 && user2){
        user1->getDataPtr()->removeFriend(user2->getDataPtr());
        user2->getDataPtr()->removeFriend(user1->getDataPtr());
    }
}

void VideoShare::updateUserStatus(const string & userName, Status newStatus){
    Node<User> *user = findUser(userName);
    user->getDataPtr()->updateStatus(newStatus);
}

Node<Video>* VideoShare::findVideo(const string & videoTitle){
    Node<Video>* a = videos.first();
    while(a){
        if(a->getDataPtr()->getTitle() == videoTitle) return a;
        a = a->getNext();
    }
    return NULL;
}

void VideoShare::subscribe(const string & userName, const string & videoTitle){
    Node<User> *userN;
    Node<Video> *videoT;
    userN = findUser(userName);
    videoT = findVideo(videoTitle);

    if(userN && userN->getDataPtr()->getStatus()==ACTIVE && videoT)
        userN->getDataPtr()->subscribe(videoT->getDataPtr());

}

void VideoShare::unSubscribe(const string & userName, const string & videoTitle){
    Node<User> *userN;
    Node<Video> *videoT;
    userN = findUser(userName);
    videoT = findVideo(videoTitle);

    if(userN && userN->getDataPtr()->getStatus()==ACTIVE && videoT)
        userN->getDataPtr()->unSubscribe(videoT->getDataPtr());
}

void VideoShare::deleteUser(const string & userName){
    Node<User> *userN = findUser(userName);
    if(!userN) return;
    Node<User*> *a = userN->getDataPtr()->getFriends()->first();
    while(a){
        removeFriendship(userName, a->getData()->getUsername());
        a = a->getNext();
    }
    Node<User> *prev = users.getHead();
    while(!(prev->getNext()->getData() == userN->getData())){
        prev = prev->getNext();
    }
    users.deleteNode(prev);
    userN->getDataPtr()->~User();
}

void VideoShare::printUserSubscriptions(const string & userName){
    Node<User> *userN = findUser(userName);
    if(userN) userN->getDataPtr()->printSubscriptions();
}

void VideoShare::printFriendsOfUser(const string & userName){
    Node<User> *userN = findUser(userName);
    if(userN) userN->getDataPtr()->printFriends();
}

void VideoShare::sortUserSubscriptions(const string & userName){
    Node<User> *userN;
    userN = findUser(userName);

    bubbleSortLinkedListbyTitle(userN->getDataPtr()->getSubscriptions(), userN->getDataPtr()->getSubscriptions()->getLength());

}

void VideoShare::bubbleSortLinkedListbyTitle(LinkedList<Video*> *subs, size_t length){
    Node<Video*> *right, *left;
    if(length < 2) return;
    left = subs->first();
    right = left->getNext();
    for(int i = 0; i < length; i++){
        for(int j = 0; j < length-1; j++){
            if( left->getData()->getTitle() > right->getData()->getTitle() ){
                subs->swap(j, j+1);
            }
            left = left->getNext();
            right = right->getNext();
        }
        left = subs->first();
        right = left->getNext();
    }
}

void VideoShare::printCommonSubscriptions(const string & userName1, const string & userName2){

    Node<Video*> *pos;
    pos = findUser(userName1)->getDataPtr()->getSubscriptions()->first();

    while(pos){
        if(findUser(userName2)->getDataPtr()->getSubscriptions()->findNode(pos->getData())) cout << *(pos->getData()) << endl;
        pos = pos->getNext();
    }
}

void VideoShare::printFriendSubscriptions(const string & userName){
    LinkedList<Video*> distincts;
    Node<User*> *userFH;
    Node<Video*> *currFrSubs;
    userFH = findUser(userName)->getDataPtr()->getFriends()->getHead();
    while(userFH->getNext()){
        currFrSubs = userFH->getNext()->getData()->getSubscriptions()->getHead();
        while(currFrSubs->getNext()){
            if(!distincts.findNode(currFrSubs->getNext()->getData())){
                distincts.insertNode(distincts.getHead(), currFrSubs->getNext()->getData());
            }
            currFrSubs = currFrSubs->getNext();
        }
        userFH = userFH->getNext();
    }
    bubbleSortLinkedListbyTitle(&distincts, distincts.getLength());
    Node<Video*> *tmp = distincts.first();
    while(tmp){
        cout << *(tmp->getData()) << endl;
        tmp = tmp->getNext();
    }
}

bool VideoShare::isConnected(const string & userName1, const string & userName2){
    
    LinkedList<User*> traversed;
    return isConnectedRecursive(&traversed, userName1, userName2);
}

bool VideoShare::isTraversed(LinkedList<User*> *traversed, const string & userName){
    Node<User*> *temp = traversed->first();
    while(temp){
        if( temp->getData()->getUsername() == userName ) return true;
        temp = temp->getNext();
    }
    return false;
}

bool VideoShare::isConnectedRecursive(LinkedList<User*> *traversed, const string & userName1, const string & userName2){
    if( userName1 == userName2 ) return true;
    Node<User> *user1 = findUser(userName1);
    Node<User> *user2 = findUser(userName2);

    traversed->insertNode(traversed->getHead(), user1->getDataPtr());
    Node<User*> *u1F = user1->getDataPtr()->getFriends()->first();

    while(u1F){
        if( !isTraversed(traversed, u1F->getData()->getUsername()) ){
            return false || isConnectedRecursive(traversed, u1F->getData()->getUsername(), userName2);
        }
        u1F = u1F->getNext();
    }
    return false;
}
