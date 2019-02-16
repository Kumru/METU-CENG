#include "hw3.h"

using namespace std;

int main()
{
  AccessControl ac(1, 1);
  cout << ac.addUsers("input.txt") << endl;
  ac.printPasswords();
  cout << endl;
  return 0;
}
