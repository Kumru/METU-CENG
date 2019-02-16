#ifndef _HW3_H
#define _HW3_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <fstream>

#define MAX_LOAD_FACTOR 0.65
#define EMPTY "EMPTY"
#define DELETED "DELETED"


template <typename T>
class HashTable
{
private:
	int tableSize;
	int currentSize;

	T emptyData;
	T deletedData;

	std::vector< std::pair<std::string, T> > table;

	int hashFunction(std::string key, int tableSize, int i);

public:
	//Default constructor to instantiate in the AccessControls privates.
	HashTable() {};
	//Constructor. Empty and deleted datas will ease our job on check statements.
	HashTable(int table_size, T empty_data, T deleted_data);
	//Destructor.
	~HashTable();

	//Gain access to table.
	std::vector< std::pair<std::string, T> > & getTable() { return table; };

	//On rehash we will call it with a different table so there is a table argument. Also giving it some default arguments to make the function generic. AddUser doesn't allows multiples but ChangeUser does. Rehash does not update counter.
	int insert(const std::string &key, const T data, std::vector< std::pair<std::string, T> >& genTable, bool allowMultiple = false, bool updateCounter = true);

	//Remove returns the data not to lose oldPasswords. Default argument is for while rehashing we delete the element but counter stays the same.
	T remove(const std::string &key, bool updateCounter = true);

	//Check if the item toCheck is the last instance of that data in the table. i.e. should not login with old passwords.
	bool get(const std::string &key, T &toCheck);

	//Print function that returns load factor.
	float printTable();

	//Enlarge the table to the p where p is a prime and p>tableSize*2.
	void rehash();
};


class AccessControl
{
public:
	AccessControl(int table1Size, int table2Size);
	~AccessControl();

	int addUser(std::string username, std::string pass);
	int addUsers(std::string filePath);
	int delUser(std::string username, std::vector<std::string>& oldPasswords);
	int changePass(std::string username, std::string oldpass, std::string newpass);

	int login(std::string username, std::string pass);
	int logout(std::string username);

	float printActiveUsers();
	float printPasswords();
private:
	//<<your hash class>> activeUsers;	// hash(username) -> username
	HashTable<std::string> activeUsers;

	//<<your hash class>> regUsers;  		// hash(username) -> username,password
	HashTable< std::pair<std::string, std::string> > regUsers;
};

#endif