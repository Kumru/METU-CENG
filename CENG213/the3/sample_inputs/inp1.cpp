#include "hw3.h"

using namespace std;

int main()
{
  AccessControl ac1(1, 1);
  cout << "User Add: Sarah " << ac1.addUser("Sarah", "pass123") << endl;
  cout << "User Add: Sarah " << ac1.addUser("Sarah", "pass123") << endl;
  cout << "User Add: Sarah " << ac1.addUser("Sarah", "pass1234") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac1.printPasswords();
  cout << endl;

  cout << "User Add: Morgan " << ac1.addUser("Morgan", "morP4ssw0rd") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac1.printPasswords();
  cout << endl;

  cout << "User Add: BigMike " << ac1.addUser("BigMike", "mikesPassword") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac1.printPasswords();
  cout << endl;

  cout << "User Add: Jeff " << ac1.addUser("Jeff", "jeffsspassword") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac1.printPasswords();
  cout << endl;

  cout << "User Add: Lester " << ac1.addUser("Lester", "Lestersspassword") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac1.printPasswords();
  cout << endl;


  AccessControl ac2(4, 4);
  cout << "User Add: Sarah " << ac2.addUser("Sarah", "Sarahspass") << endl;
  cout << "User Add: Sarah " << ac2.addUser("Sarah", "pass1234") << endl;
  cout << "User Add: Morgan " << ac2.addUser("Morgan", "morganspass") << endl;
  cout << "User Add: Ellie " << ac2.addUser("Ellie", "elspassword") << endl;
  cout << "User Add: Vicky " << ac2.addUser("Vicky", "vickypass") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac2.printPasswords();
  cout << endl;

  cout << "User Add: Casey " << ac2.addUser("Casey", "caseysspassword") << endl;
  cout << "User Add: Casey " << ac2.addUser("Casey", "caseyssp4ssw0rd") << endl;
  cout << "User Add: Vale " << ac2.addUser("Vale", "valespass") << endl;
  cout << "User Add: Devon " << ac2.addUser("Devon", "devonspass") << endl;
  cout << "User Add: Chuck " << ac2.addUser("Chuck", "chuckspass") << endl;
  cout << "User Add: Orion " << ac2.addUser("Orion", "orionspass") << endl;
  cout << "User Add: Chuck " << ac2.addUser("Chuck", "chuckspass") << endl;
  cout << "User Add: Chuck " << ac2.addUser("Chuck", "chuckspass") << endl;
  cout << "User Add: Devon " << ac2.addUser("Devon", "devonspass") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac2.printPasswords();
  cout << endl;

  AccessControl ac3(3, 1);
  cout << "User Add: Sarah " << ac3.addUser("Sarah", "Sarahspass") << endl;
  cout << "User Add: Sarah " << ac3.addUser("Sarah", "sarspass") << endl;
  cout << "---------" << endl << "Passwords" << endl;
  ac3.printPasswords();
  cout << endl;
  return 0;
}
