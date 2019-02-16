#include "hw3.h"

using namespace std;

int main()
{
  AccessControl ac(20, 3);
  cout << "User Add: Sarah " << ac.addUser("Sarah", "Sarahspass") << endl;
  cout << "User Add: Morgan " << ac.addUser("Morgan", "morganspass") << endl;
  cout << "User Add: Ellie " << ac.addUser("Ellie", "elspassword") << endl;
  cout << "User Add: Vicky " << ac.addUser("Vicky", "vickypass") << endl;
  cout << "User Add: Casey " << ac.addUser("Casey", "caseysspassword") << endl;
  cout << "User Add: Vale " << ac.addUser("Vale", "valespass") << endl;
  cout << "User Add: Devon " << ac.addUser("Devon", "devonspass") << endl;
  cout << "User Add: Chuck " << ac.addUser("Chuck", "chuckspass") << endl;
  cout << "User Add: Orion " << ac.addUser("Orion", "orionspass") << endl;
  cout << "User Add: BigMike " << ac.addUser("BigMike", "mikesPassword") << endl;
  cout << "User Add: Jeff " << ac.addUser("Jeff", "jeffsspassword") << endl;
  cout << "User Add: Lester " << ac.addUser("Lester", "Lestersspassword") << endl;

  cout << "Login: Chuck " << ac.login("Chuck", "chuckspass") << endl;
  cout << "Logout: Chuck " << ac.logout("Chuck") << endl;
  cout << "---------" << endl << "Active Users" << endl;
  ac.printActiveUsers();
  cout << endl;

  cout << "Login: Chuck " << ac.login("Chuck", "chuckspass") << endl;
  cout << "---------" << endl << "Active Users" << endl;
  ac.printActiveUsers();
  cout << endl;

  cout << "Login: Morgan " << ac.login("Morgan", "morganspass") << endl;
  cout << "Login: Sarah " << ac.login("Sarah", "Sarahspass") << endl;
  cout << "Logout: Sarah " << ac.logout("Sarah") << endl;
  cout << "---------" << endl << "Active Users" << endl;
  ac.printActiveUsers();
  cout << endl;

  cout << "Logout: Chuck " << ac.logout("Chuck") << endl;
  cout << "Logout: Chuck " << ac.logout("Chuck") << endl;
  cout << "Logout: Morgan " << ac.logout("Morgan") << endl;
  cout << "Logout: Morgan " << ac.logout("Morgan") << endl;

  cout << "Login: Sarah " << ac.login("Sarah", "Sarahspass") << endl;
  cout << "Login: Casey " << ac.login("Casey", "caseysspassword") << endl;
  cout << "Login: Vicky " << ac.login("Vicky", "vickypass") << endl;
  cout << "Login: Vale " << ac.login("Vale", "valespass") << endl;
  cout << "Login: Orion " << ac.login("Orion", "orionspass") << endl;
  cout << "Login: Jefster " << ac.login("Jefster", "None") << endl;
  cout << "Login: BigMike " << ac.login("BigMike", "mikesPassword") << endl;
  cout << "Login: Jeff " << ac.login("Jeff", "jeffsspassword") << endl;
  cout << "Login: Lester " << ac.login("Lester", "Lestersspassword") << endl;
  cout << "---------" << endl << "Active Users" << endl;
  ac.printActiveUsers();

  cout << "Logout: Sarah " << ac.logout("Sarah") << endl;
  cout << "Logout: Casey " << ac.logout("Casey") << endl;
  cout << "Logout: Sarah " << ac.logout("Sarah") << endl;
  cout << "---------" << endl << "Active Users" << endl;
  ac.printActiveUsers();
  cout << endl;

  cout << "Logout: Orion " << ac.logout("Orion") << endl;
  cout << "Logout: Jefster " << ac.logout("Jefster") << endl;
  cout << "Logout: BigMike " << ac.logout("BigMike") << endl;
  cout << "Logout: Jeff " << ac.logout("Jeff") << endl;
  cout << "Logout: Lester " << ac.logout("Lester") << endl;
  cout << "Logout: Vicky " << ac.logout("Vicky") << endl;
  cout << "Logout: Vale " << ac.logout("Vale") << endl;

  cout << "Login: BigMike " << ac.login("BigMike", "mikesPassword") << endl;
  cout << "Login: Jeff " << ac.login("Jeff", "jeffsspassword") << endl;
  cout << "Login: Lester " << ac.login("Lester", "Lestersspassword") << endl;

  cout << "Logout: BigMike " << ac.logout("BigMike") << endl;
  cout << "Logout: Jeff " << ac.logout("Jeff") << endl;
  cout << "Logout: Lester " << ac.logout("Lester") << endl;
  cout << "---------" << endl << "Active Users" << endl;
  ac.printActiveUsers();
  cout << endl;
  return 0;
}
