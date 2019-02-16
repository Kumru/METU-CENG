#include "hw3.h"

using namespace std;

int main()
{
  AccessControl ac(3, 3);

  cout << "User Add: Sarah " << ac.addUser("Sarah", "Sarahspass") << endl;
  cout << "User Add: Morgan " << ac.addUser("Morgan", "morganspass") << endl;
  cout << "User Add: Ellie " << ac.addUser("Ellie", "elspassword") << endl;
  cout << "User Add: Vicky " << ac.addUser("Vicky", "vickypass") << endl;
  cout << "Change Pass: Sarah " << ac.changePass("Sarah", "wrong", "Sarahsnewpass") << endl;
  cout << "Change Pass: Sarah " << ac.changePass("Sarah", "Sarahspass", "Sarahsnewpass") << endl;
  cout << "User Add: Casey " << ac.addUser("Casey", "caseysspassword") << endl;
  cout << "User Add: Vale " << ac.addUser("Vale", "valespass") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac.printPasswords();
  cout << endl;

  cout << "User Add: Devon " << ac.addUser("Devon", "devonspass") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac.printPasswords();
  cout << endl;

  cout << "Change Pass: Sarah " << ac.changePass("Sarah", "Sarahsnewpass", "Sarahsnewerpass") << endl;
  cout << "User Add: Chuck " << ac.addUser("Chuck", "chuckspass") << endl;
  cout << "User Add: Orion " << ac.addUser("Orion", "orionspass") << endl;
  cout << "Change Pass: Vale " << ac.changePass("Vale", "valespass", "valesnewpass") << endl;
  cout << "Change Pass: Sarah " << ac.changePass("Sarah", "Sarahsnewerpass", "Sarahsnewestpass") << endl;
  cout << "User Add: BigMike " << ac.addUser("BigMike", "mikesPassword") << endl;
  cout << "User Add: Jeff " << ac.addUser("Jeff", "jeffsspassword") << endl;
  cout << "Change Pass: Vale " << ac.changePass("Vale", "valesnewpass", "valesnewerpass") << endl;
  cout << "User Add: Lester " << ac.addUser("Lester", "Lestersspassword") << endl;
  cout << "User Add: Beckman " << ac.addUser("Beckman", "Generalsspassword") << endl;
  cout << "User Add: Bryce " << ac.addUser("Bryce", "brycespassword") << endl;
  cout << "User Add: Emmet " << ac.addUser("Emmet", "hothothothot") << endl;
  cout << "User Add: Tang " << ac.addUser("Tang", "harrysspassword") << endl;
  cout << "User Add: Alex " << ac.addUser("Alex", "alexsspassword") << endl;
  cout << "Change Pass: Vale " << ac.changePass("Vale", "valesnewerpass", "valesnewestpass") << endl;
  cout << "User Add: AnnaWu " << ac.addUser("AnnaWu", "AnnaWusspassword") << endl;
  cout << "Change Pass: Tang " << ac.changePass("Tang", "harrysspassword", "harryssnewpassword") << endl;
  cout << "User Add: Roark " << ac.addUser("Roark", "Tedsspassword") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac.printPasswords();
  cout << endl;
  return 0;
}
