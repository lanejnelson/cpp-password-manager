#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <utility>
#include <sstream>
#include <random>
#include "utils.hpp"

using namespace std;

struct PassEntry {
	string url;
	string username;
	string password;
};

class Vault {
	string filename;
	int encryptionKey;

private:
	vector<pair<int, PassEntry>> entries;
	int numEntries;

public:
	Vault(string filenameIn, int encryptionKeyIn);
	void add();
	void encrypt();
	void decrypt();
	void view();
	bool save();
	bool load();
	bool remove();
	string generatePassword();
};





Vault::Vault(string filenameIn, int encryptionKeyIn) {
	filename = filenameIn;
	encryptionKey = encryptionKeyIn;
	numEntries = 0;
}


string Vault::generatePassword() {
	const string chars = "1234567890ABCDEFGHIOJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()_+-=./:;<>?";
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<size_t> distribution(0, chars.size() - 1);
	string password;
	cout << "How long would you like this password to be? ";
	int length;
	cin >> length;
	for (size_t i = 0; i < length; i++) {
		password += chars[distribution(mt)];
	}
	return password;
}

void Vault::add() {
	PassEntry newEntry;
	cout << "Adding new entry" << endl;
	cout << "Enter the url for this login: ";
	cin >> newEntry.url;
	cout << "Enter the username for this login (no spaces or commas): ";
	cin >> newEntry.username;
	cout << "Would you like to have a password generated or enter your own? (y\\n): ";
	char input;
	cin >> input;
	while (input != 'y' && input != 'n') {
		cout << "Please enter either 'y' or 'n'." << endl;
		cout << "Would you like to have a password generated or enter your own? (y\\n): ";
		cin >> input;
	}
	if (input == 'y') {
		string password = generatePassword();
		cout << "Password generated" << endl;
		cout << "Password is: " << password << endl;
		newEntry.password = password;
	}
	else if (input == 'n') {
		cout << "Enter the desired password (no commas please): ";
		newEntry.password = takePasswdFromUser();
		cout << "Password is: " << newEntry.password << endl;
	}
	entries.push_back({numEntries, newEntry});
	numEntries++;
}

void Vault::view() {
	if (numEntries == 0) {
		cout << "There are no accounts to view. Exiting to menu..." << endl;
		return;
	}
	for (int i = 0; i < numEntries; i++) {
		cout << "[" << i << "]: " << entries.at(i).second.url << endl;
	}
	cout << "Which account would you like to login to? ";
	int input;
	cin >> input;
	while ((input < 0) || (input > (numEntries - 1))) {
		cout << "Please enter a valid account." << endl;
		for (int i = 0; i < numEntries; i++) {
			cout << "[" << i << "]: " << entries.at(i).second.url << endl;
		}
		cout << "Which account would you like to login to? ";
		cin >> input;
	}
	cout << "[" << input << "]: " << entries.at(input).second.url << endl;
	cout << "Username: " << entries.at(input).second.username << endl;
	cout << "Password: " << entries.at(input).second.password << endl;
	cout << "Would you like to view another account? (y\\n): ";
	char input2;
	cin >> input2;
	while (input2 != 'n' && input2 != 'y') {
		cout << "Please enter 'y' or 'n'." << endl;
		cout << "Would you like to view another account? (y\\n): ";
		cin >> input2;
	}
	if (input2 == 'y') {
		view();
	}
	else {
		return;
	}
	/*
	while (input2 != 'n') {
		if (input2 == 'y') {
			view();
			cout << endl;
		}
		else {
			while (input2 != 'n' && input2 != 'y') {
				cout << "Please enter 'y' or 'n'." << endl;
				cout << "Would you like to view another account? (y\\n): ";
				cin >> input2;
				cout << endl;
			}
		}
	}
	*/
}

bool Vault::save() {
	ofstream savedVault(filename);
	if (!savedVault.is_open()) {
		cout << "Some error has happened I'm not really sure what" << endl;
		return false;
	}
	for (int i = 0; i < numEntries; i++) {
		savedVault << entries.at(i).first << ";" << entries.at(i).second.url << ";" << entries.at(i).second.username << ";" << entries.at(i).second.password << endl;
	}
	cout << "Vault has been saved to " << filename << "." << endl;
	savedVault.close();
	return true;
}

bool Vault::load() {
	ifstream vaultFile(filename);
	if (!vaultFile.is_open()) {
		cout << "Loading failed try again later." << endl;
		return false;;
	}
	string entry;
	while (getline(vaultFile, entry)) {
		int id;
		string url;
		string username;
		string password;
		stringstream ss(entry);
		string readId;
		getline(ss, readId, ';');
		id = stoi(readId);
		getline(ss, url, ';');
		getline(ss, username, ';');
		getline(ss, password);
		//cout << id << ", " << url << ", " << username << ", " << password << endl;
		PassEntry currEntry = { url, username, password };
		entries.push_back({ id, currEntry });
		++numEntries;
	}
	vaultFile.close();
	return true;
}

bool Vault::remove() {
	if (numEntries == 0) {
		cout << "There are no accounts to remove" << endl;
		return false;
	}
	else {
		for (int i = 0; i < numEntries; i++) {
			cout << "[" << i << "]: " << entries.at(i).second.url << endl;
		}
		cout << "Which account would you like to remove? " << endl;
		int choice;
		cin >> choice;
		while ((choice < 0) || (choice > (numEntries - 1))) {
			cout << "Please enter a valid account." << endl;
			for (int i = 0; i < numEntries; i++) {
				cout << "[" << i << "]: " << entries.at(i).second.url << endl;
			}
			cout << "Which account would you like to login to? ";
			cin >> choice;
		}
		cout << "Are you sure you want to remove account " << choice << "? (y\\n) ";
		char confirm;
		cin >> confirm;
		if (confirm == 'y') {
			cout << "Removing account " << choice << "..." << endl;
			entries.erase(entries.begin() + choice);
			--numEntries;
		}
		else if (confirm == 'n') {
			cout << "Exiting to menu" << endl;
		}
		else {
			while (confirm != 'y' && confirm != 'n') {
				cout << "Please enter 'y' or 'n'." << endl;
				remove();
			}
		}
	}
}

void printMenu(int stage) {
	if (stage == 0) {
		cout << "[0] Load Vault File" << endl;
		cout << "[1] Create New Vault" << endl;
		cout << "[2] Exit Application" << endl;
		cout << "Choice: ";
	}
	else if (stage == 1) {
		cout << "[0] View Accounts" << endl;
		cout << "[1] Add New Account" << endl;
		cout << "[2] Remove An Account" << endl;
		cout << "[3] Save Vault" << endl;
		cout << "[4] Exit Application" << endl;
		cout << "Choice: ";
	}
}


int main(int argc, char* argv[]) {
	int choice = -1;
	int encryptionKey;
	string filename;
	Vault* userVault = nullptr;
	printMenu(0);
	cin >> choice;
	if (choice == 0) {
		cout << "Enter your encryption key: ";
		cin >> encryptionKey;
		cout << "Enter the filename of your vault: ";
		cin >> filename;
		userVault = new Vault(filename, encryptionKey);
		userVault->load();
	}
	else if (choice == 1) {
		cout << "Enter your desired encryption key: ";
		cin >> encryptionKey;
		cout << "Enter the desired filename of your new vault: ";
		cin >> filename;
		userVault = new Vault(filename, encryptionKey);
	}
	else if (choice == 2) {
		cout << "Exiting..." << endl;
		return 0;
	}
	while (choice != 0 && choice != 1 && choice != 2) {
		cout << "Please enter a valid option." << endl;
		cout << "Choice: ";
		cin >> choice;
	}
	while (true) {
		printMenu(1);
		cin >> choice;
		if (choice == 0) {
			userVault->view();
		}
		else if (choice == 1) {
			userVault->add();
		}
		else if (choice == 2) {
			userVault->remove();
		}
		else if (choice == 3) {
			userVault->save();
		}
		else if (choice == 4) {
			break;
		}
		while (choice != 0 && choice != 1 && choice != 2 && choice != 3 && choice != 4) {
			cout << "Please enter a valid option." << endl;
			cout << "Choice: ";
			cin >> choice;
		}
	}
	//clearScreen();
	//userVault->add();
	//userVault->view();
	/*
	cout << "Would you like to save the vault? (y\\n): ";
	char input;
	cin >> input;
	if (input == 'y') {
		userVault->save();
	}
	*/
	cout << "Exiting password manager..." << endl;
	delete userVault;
	return 0;
	//cout << "Hello world!" << endl;
}