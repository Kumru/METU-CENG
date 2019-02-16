#include "hw3.h"

//Pair output overload.
std::ostream &operator<<(std::ostream &out, const std::pair<std::string, std::string> &p)
{
	out << p.first << " " << p.second;
	return out;
}



/*

HASH TABLE

*/


template<typename T>
HashTable<T>::HashTable(int table_size, T empty_data, T deleted_data)
{
	emptyData = empty_data;
	deletedData = deleted_data;
	
	table.resize(table_size, std::make_pair("", emptyData));
	
	tableSize = table_size;
	currentSize = 0;
}

template<typename T>
HashTable<T>::~HashTable() {}

template<typename T>
int HashTable<T>::hashFunction(std::string key, int tableSize, int i)
{
	int length = key.length();
	int newK = 0;
	for (int i = 0; i < length; i++)
		newK += (int)key[i];

	// hash function 1 -> (newK % tableSize)
	// hash function 2 -> (newK * tableSize - 1) % tableSize)

	return ((newK % tableSize) + i * ((newK * tableSize - 1) % tableSize)) % tableSize;
}

template<typename T>
int HashTable<T>::insert(const std::string &key, const T data, std::vector< std::pair<std::string, T> >& genTable, bool allowMultiple, bool updateCounter)
{
	int index = 0;
	for (int secIndex = 0; secIndex < (int)genTable.size(); secIndex++) {

		index = hashFunction(key, genTable.size(), secIndex);

		//Multiple is not allowed and key already exists.
		if (!allowMultiple && genTable[index].first == key) return 0;

		//Found an empty or deleted place.
		if (genTable[index].first == ""){
			genTable[index].first = key;
			genTable[index].second = data;

			//Not rehash.
			if(updateCounter) currentSize++;
			
			//Rehash.
			if (( (float)currentSize / genTable.size()) > MAX_LOAD_FACTOR) rehash();

			return 1;
		}
	}
	return 0;
}

template<typename T>
T HashTable<T>::remove(const std::string & key, bool updateCounter)
{
	int index = 0;
	T temp = emptyData;

	for (int secIndex = 0; secIndex < tableSize; secIndex++) {

		index = hashFunction(key, tableSize, secIndex);

		//Not found.
		if (table[index].second == emptyData) return emptyData;

		//Found the first one.
		if (table[index].first == key) {

			//Saving the password.
			temp = table[index].second;

			//Deleting.
			table[index].first = "";
			table[index].second = deletedData;
			
			//Not rehash.
			if (updateCounter) currentSize--;

			return temp;
		}
	}
	return emptyData;
}

template<typename T>
bool HashTable<T>::get(const std::string & key, T & toCheck)
{
	int index = 0;

	T lastInstance = emptyData;

	for (int secIndex = 0; secIndex < tableSize; secIndex++) {
		
		index = hashFunction(key, tableSize, secIndex);

		//Update last instance.
		if (table[index].first == key)
			lastInstance = table[index].second;

		//End searching.
		else if (table[index].second == emptyData) {

			//Information is correct.
			if (lastInstance == toCheck)
				return true;
			else
				break;
		}
	}
	return false;
}

template<typename T>
float HashTable<T>::printTable()
{
	for (int i = 0; i < tableSize; i++)
		std::cout << table[i].second << std::endl;
	
	return (float)currentSize/tableSize;
}

template<typename T>
void HashTable<T>::rehash()
{
	//Determining the new size.
	int newP = tableSize * 2;
	bool isPrime = false;

	while (!isPrime) {
		newP++;
		isPrime = true;

		//Searching for divisor.
		for (int i = 2; i*i <= newP; i++) {

			//Found.
			if (!(newP % i)) {
				isPrime = false;
				break;
			}
		}
	}

	//New holder. Resizing with emptyData.
	std::vector< std::pair<std::string, T> > tempTable;
	tempTable.resize(newP, std::make_pair("", emptyData));

	for (int i = 0; i < tableSize; i++) {

		//Don't rehash empty or deleted.
		if (table[i].first == "")
			continue;

		std::string currentKey = table[i].first;
		int index = 0;

		for (int secIndex = 0; secIndex < tableSize; secIndex++) {

			index = hashFunction(currentKey, tableSize, secIndex);

			//End of the indexes that we can get from hashFunction.
			if (table[index].second == emptyData)
				break;

			//Found an instance.
			if (table[index].first == currentKey) {

				//Inserting to the tempTable with multiple allowed and counter unchanged.
				insert(currentKey, table[index].second, tempTable, true, false);
				
				//Removing from the table with counter unchanged.
				remove(currentKey, false);
			}
		}

	}

	//Updating the old table.
	table = tempTable;
	tableSize = newP;
}



/*

ACCESS CONTROL

*/


AccessControl::AccessControl(int table1Size, int table2Size)
{
	activeUsers = HashTable<std::string>                           (table2Size, EMPTY, DELETED);
	regUsers    = HashTable< std::pair<std::string, std::string> > (table1Size, std::make_pair(EMPTY, EMPTY), std::make_pair(DELETED, DELETED));
}

AccessControl::~AccessControl(){}

int AccessControl::addUser(std::string username, std::string pass)
{
	return regUsers.insert(username, std::make_pair(username, pass), regUsers.getTable(), false);
}

int AccessControl::addUsers(std::string filePath)
{
	std::string uName, pass, line;
	int fstPos, counter = 0;

	//Open file.
	std::ifstream newFile;
	newFile.open(filePath.c_str());

	//Get line by line.
	while (std::getline(newFile, line)) {

		//Line empty.
		if (line == "")
			continue;

		fstPos = line.find(" ");

		//Username empty.
		if (fstPos == 0)
			continue;

		uName = line.substr(0, fstPos);
		pass = line.substr(fstPos + 1);

		//Add user and increase counter if successful.
		counter += addUser(uName, pass);
	}

	//Close file.
	newFile.close();

	//Return number of users added.
	return counter;
}

int AccessControl::delUser(std::string username, std::vector<std::string>& oldPasswords)
{
	bool retVal = false;
	std::pair<std::string, std::string> temp, tempEmpty;

	temp = regUsers.remove(username, true);
	tempEmpty = std::make_pair((std::string)EMPTY, (std::string)EMPTY);

	while (temp != tempEmpty) {
		//Something is removed so push its password.
		oldPasswords.push_back(temp.second);

		//We deleted at least something.
		retVal = true;

		//Try to remove again.
		temp = regUsers.remove(username, true);
	}

	return retVal;
}

int AccessControl::changePass(std::string username, std::string oldpass, std::string newpass)
{
	std::pair<std::string, std::string> temp = std::make_pair(username, oldpass);

	//Such username-pass combination is valid, so change password.
	if (regUsers.get(username, temp))
		return regUsers.insert(username, std::make_pair(username, newpass), regUsers.getTable(), true);

	return 0;
}



int AccessControl::login(std::string username, std::string pass)
{
	std::pair<std::string, std::string> temp = std::make_pair(username, pass);

	//Such username-pass combination is valid, so login.
	if (regUsers.get(username, temp))
		return activeUsers.insert(username, username, activeUsers.getTable(), false);

	return 0;
}

int AccessControl::logout(std::string username)
{
	//Logged out.
	if (activeUsers.remove(username, true) != EMPTY)
		return 1;

	return 0;
}

float AccessControl::printActiveUsers()
{
	return activeUsers.printTable();
}

float AccessControl::printPasswords()
{
	return regUsers.printTable();
}

//END.