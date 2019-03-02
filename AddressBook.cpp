#include <iomanip> //setw
#include <limits> //numeric_limits<streamsize>::max()
#include <string> //to_string(int)
#include <stdlib.h> //rand
#include <time.h> //time
#include <iostream>
using namespace std;

const int NUM_FIELDS = 4,
FIRST_NAME = 0,
LAST_NAME = 1,
TEL = 2,
ADDR = 3;


void displayMenu();
void displayAddressBook(string **AddressBook, const int user_count);
void addContact(string **AddressBook, int &user_count);
void sort(string **AddressBook, const int user_count);
void deleteContact(string **AddressBook, int &user_count);
void editContact(string **AddressBook, const int user_count);
void lowerCase(string &input);
void exitProg();
void searchMech(string **AddressBook, const int input, const int user_count, string responseString);
void searchFunc(string **AddressBook, const int user_count);

string randLastName();
string randFirstName();
string randStreetName();

int main() {
	srand(time(NULL));
	int user_count = 0;
	string **AddressBook; //first, last, tel, addr 

	int menuItem;
	bool keepGoing = true;
	do {
		displayAddressBook(AddressBook, user_count);
		displayMenu();
		cin >> menuItem;
		cin.ignore();
		switch (menuItem) {
		case 1:
			cout << "1. Add a new contact" << endl;
			addContact(AddressBook, user_count);
			break;
		case 2:
			cout << "2. Update an existing contact" << endl;
			editContact(AddressBook, user_count);
			break;
		case 3:
			cout << "3. Delete a contact" << endl;
			deleteContact(AddressBook, user_count);
			break;
		case 4:
			cout << "4. List all added contacts in sorted order" << endl;
			sort(AddressBook, user_count);
			break;
		case 5:
			cout << "5. Search for a given contact" << endl;
			searchFunc(AddressBook, user_count);
			break;
		case 6:
			cout << "6. Quit" << endl;
			keepGoing = false;
			exitProg();
			break;
		default:
			break;
		}
	} while (keepGoing);

	for (int i = 0; i < user_count; i++)
		delete[] AddressBook[i];

	return 0;
}

void displayMenu() {
	cout << "\n\n";
	cout << "1. Add a new contact" << endl;
	cout << "2. Update an existing contact" << endl;
	cout << "3. Delete a contact" << endl;
	cout << "4. List all added contacts in sorted order" << endl;
	cout << "5. Search for a given contact" << endl;
	cout << "6. Quit" << endl;
}

void displayAddressBook(string **AddressBook, const int user_count) {
	cout << "\n";
	for (int i = 0; i < user_count; i++) {
		cout << "[" << setw(3) << i << "] " << setw(12) << AddressBook[i][LAST_NAME] << ", " << setw(12) << AddressBook[i][FIRST_NAME] << "; " << AddressBook[i][TEL] << "; " << AddressBook[i][3] << " " << endl;
	}
	cout << "\n";
}

void addContact(string **AddressBook, int &user_count) {
	AddressBook[user_count] = new string[NUM_FIELDS];

	// Fake Data For Testing
	/*AddressBook[user_count][FIRST_NAME] = randFirstName();
	AddressBook[user_count][LAST_NAME] = randLastName();
	AddressBook[user_count][TEL] = "(" + to_string(rand()%(1000-100)+100) + ") " + to_string(rand()%(1000-100)+100) + "-" + to_string(rand()%(10000-1000)+1000);
	AddressBook[user_count][ADDR] = to_string(rand()%(1000-100)+100) + " " + randStreetName();*/

	cout << "First name? ";
	getline(cin, AddressBook[user_count][FIRST_NAME]);
	cout << "Last name? ";
	getline(cin, AddressBook[user_count][LAST_NAME]);
	cout << "Phone number? ";
	getline(cin, AddressBook[user_count][TEL]);
	cout << "Address? ";
	getline(cin, AddressBook[user_count][ADDR]);

	user_count++;
}


void sort(string **AddressBook, const int user_count) {
	for (int i = 1; i < user_count; ++i) {
		for (int j = 0; j < (user_count - i); ++j)
			if (AddressBook[j][LAST_NAME] > AddressBook[j + 1][LAST_NAME]) {
				std::swap(AddressBook[j][LAST_NAME], AddressBook[j + 1][LAST_NAME]);
				std::swap(AddressBook[j][FIRST_NAME], AddressBook[j + 1][FIRST_NAME]);
				std::swap(AddressBook[j][TEL], AddressBook[j + 1][TEL]);
				std::swap(AddressBook[j][ADDR], AddressBook[j + 1][ADDR]);
			}
	}
}

void deleteContact(string **AddressBook, int &user_count) {
	if (user_count == 0) return;

	int removeIndex = -1;
	cout << "Which entry would you like to remove? ";
	bool isRemoveIndexValid = false;
	do {
		cin >> removeIndex;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		isRemoveIndexValid = (removeIndex >= 0 && removeIndex < user_count);
	} while (!isRemoveIndexValid);

	AddressBook[removeIndex][FIRST_NAME] = AddressBook[user_count - 1][FIRST_NAME];
	AddressBook[removeIndex][LAST_NAME] = AddressBook[user_count - 1][LAST_NAME];
	AddressBook[removeIndex][TEL] = AddressBook[user_count - 1][TEL];
	AddressBook[removeIndex][ADDR] = AddressBook[user_count - 1][ADDR];

	AddressBook[user_count - 1][FIRST_NAME] = "";
	AddressBook[user_count - 1][LAST_NAME] = "";
	AddressBook[user_count - 1][TEL] = "";
	AddressBook[user_count - 1][ADDR] = "";

	delete[] AddressBook[user_count - 1];
	user_count--;
}

void editContact(string **AddressBook, const int user_count) {
	string target;
	int input;
	int report[10][2] = { 0 };//initializing the array to have two columns with 0,0
	int n = 0;
	string empty = "";

	cout << "\nEnter First or Last Name of the contact that you wish to edit" << endl;
	cin >> target;
	cin.ignore();

	for (int i = 0; i < user_count; i++) {
		//Trying to go through array and search for matches between input with first or last name
		string firstName = AddressBook[i][FIRST_NAME];
		string lastName = AddressBook[i][LAST_NAME];

		lowerCase(firstName);//makes all string variables lower case
		lowerCase(lastName);
		lowerCase(target);

		if (target == firstName || target == lastName) {
			report[n][0] = i;
			report[n][1] = 1;//one column has the address book index, second column is a true or false place holder
			n++;
		}
	}

	cout << "\nSelect the contact to edit.(number)" << endl;

	for (int j = 0; j < (sizeof(report) / sizeof(report[0])); j++) {//Outputing the search result, asking for user selection

		if (report[j][1] != 0) {
			cout << " " << report[j][0] + 1 << ") " << AddressBook[report[j][0]][0] << " " << AddressBook[report[j][0]][1] << endl;
		}
	}

	cin >> input;//with the input, an index is saved
	int index = input - 1;

	int part = 0;//initializing part variable

	do {//Asking the user with part of the contact does he or she wish to edit
		cout << "\nWhich part do you wish to edit?\n 1) First Name\n 2) Last Name\n 3) Phone Number \n 4) Address\n 5) Exit Edit Mode" << endl;
		cin >> input;
		cin.ignore();

		part = input - 1;

		switch (part) {
		case 0:
			cout << "\nEnter First Name" << endl;
			getline(cin, AddressBook[index][FIRST_NAME]);
			break;
		case 1:
			cout << "\nEnter Last Name" << endl;
			getline(cin, AddressBook[index][LAST_NAME]);
			break;
		case 2:
			cout << "\nEnter Phone Number" << endl;
			getline(cin, AddressBook[index][TEL]);
			break;
		case 3:
			cout << "\nEnter Address" << endl;
			getline(cin, AddressBook[index][ADDR]);
			break;
		case 4:
			cout << "\nExiting Edit Mode" << endl;
			break;
		default:
			cout << "\nNot a valid option" << endl;
			break;
		}
	} while (part < 4 || part > 4);
}

void lowerCase(string &input) {
	for (int i = 0; i < input.length(); i++) {
		input[i] = tolower(input[i]);
	}
}

void exitProg() {
	cout << "\nExiting Program Now" << endl;
	exit(0);
}

string randLastName() {
	string lastNames[20] = {
		"Cooper",
		"Ingram",
		"Chapman",
		"Pacheco",
		"Pittman",
		"Orr",
		"Horton",
		"Payne",
		"Cooke",
		"Espinoza",
		"Arnold",
		"Mccoy",
		"Boyle",
		"Vaughan",
		"Clayton",
		"Washington",
		"Bennett",
		"Mooney",
		"Valencia",
		"Blankenship"
	};
	int randomIndex = rand() % 20;
	return lastNames[randomIndex];
}
string randFirstName() {
	string firstNames[20] = {
		"Giada",
		"Darius",
		"Trevor",
		"Angela",
		"Tanya",
		"Josiah",
		"Carolyn",
		"Mariah",
		"Sanai",
		"Catalina",
		"Logan",
		"Baylee",
		"Abdiel",
		"Gage",
		"Salma",
		"Nora",
		"Tristian",
		"Maddison",
		"Haven",
		"Annie"
	};
	int randomIndex = rand() % 20;
	return firstNames[randomIndex];
}
string randStreetName() {
	string streetNames[20] = {
		"Church Street",
		"Union Street",
		"Lake Street",
		"Holly Court",
		"9th Street",
		"Fieldstone Drive",
		"Oxford Court",
		"Mechanic Street",
		"Highland Avenue",
		"Linda Lane",
		"Prospect Avenue",
		"Cobblestone Court",
		"Valley Road",
		"North Avenue",
		"Primrose Lane",
		"Oak Street",
		"Maiden Lane",
		"Hudson Street",
		"Hillcrest Drive",
		"Elm Street"
	};
	int randomIndex = rand() % 20;
	return streetNames[randomIndex];
}

void searchFunc(string **AddressBook, const int user_count) {
	int input = 0;
	string firstName;
	string lastName;

	do {
		cout << "\nHow would like perform the search?\n1) First Name\n2) Last Name\n3) Exit Search Mode" << endl;
		cin >> input;
		int n = input - 1;

		switch (n) {
		case 0:
			cout << "\nEnter First Name, Please\n";
			cin >> firstName;
			searchMech(AddressBook, n, user_count, firstName);
			break;

		case 1:
			cout << "\nEnter Last Name, Please\n";
			cin >> lastName;
			searchMech(AddressBook, n, user_count, lastName);
			break;

		case 2:
			break;

		default:
			cout << "\nInvalid option." << endl;
		}

	} while (input < 3 || input>3);
}

void searchMech(string **AddressBook, const int input, const int user_count, string responseString) {
	int matches = 0;

	switch (input) {
	case 0:
		cout << endl;
		for (int i = 0; i < user_count; i++) {
			string firstName = AddressBook[i][input];
			lowerCase(responseString);
			lowerCase(firstName);

			if (responseString == firstName) {
				cout << i << ") " << AddressBook[i][0] << " " << AddressBook[i][1] << ", " << AddressBook[i][2] << ", " << AddressBook[i][3] << endl;
				matches++;
			}
		}

		if (matches == 1) {
			cout << "\nThere are a total of " << matches << " match." << endl;
		}
		else if (matches < 1) {
			cout << "\nThere are a total of " << matches << " matches." << endl;
		}
		else {
			cout << "\nThere are no matches" << endl;
		}

		break;

	case 1:
		cout << endl;
		for (int i = 0; i < user_count; i++) {
			string lastName = AddressBook[i][input];
			lowerCase(responseString);
			lowerCase(lastName);

			if (responseString == lastName) {
				cout << i << ") " << AddressBook[i][0] << " " << AddressBook[i][1] << ", " << AddressBook[i][2] << ", " << AddressBook[i][3] << endl;
			}
		}

		if (matches == 1) {
			cout << "\nThere are a total of " << matches << " match." << endl;
		}
		else if (matches < 1) {
			cout << "\nThere are a total of " << matches << " matches." << endl;
		}

		break;
	}

}
