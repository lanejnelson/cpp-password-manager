#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <utility>
#include <random>

using namespace std;

enum TT_Input {

	// ASCII code of backspace is 8
	BACKSPACE = 8,
	RETURN = 32
};

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
	void save();
	string generatePassword();
};

std::string takePasswdFromUser()
{
	string ipt = "";
	char ipt_ch;
	while (true) {
		ipt_ch = _getch();

		if (ipt_ch < TT_Input::RETURN
			&& ipt_ch != TT_Input::BACKSPACE) {
			cout << endl;
			return ipt;
		}

		// Check whether the user
		// pressed backspace
		if (ipt_ch == TT_Input::BACKSPACE) {

			// Check if ipt is empty or not
			if (ipt.length() == 0)
				continue;
			else {

				// Removes last character
				ipt.pop_back();

				continue;
			}
		}
		ipt.push_back(ipt_ch);
	}
}



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
}

void Vault::save() {
	ofstream savedVault(filename);
	if (!savedVault.is_open()) {
		cout << "Some error has happened I'm not really sure what" << endl;
	}
	for (int i = 0; i < numEntries; i++) {
		savedVault << entries.at(i).first << ";" << entries.at(i).second.username << ";" << entries.at(i).second.password << endl;
	}
	cout << "Vault has been saved to " << filename << "." << endl;
}

int main(int argc, char *argv[]) {
	int encryptionKey;
	cout << "Checking for existing vault..." << endl;
	ifstream vaultFile("vault.bin");
	if (!vaultFile.is_open()) {
		cout << "Vault doesn't exist" << endl;
		cout << "Creating new vault..." << endl;
		ofstream vaultFile("vault.bin");
		vaultFile << "";
		vaultFile.close();
	}
	else {
		cout << "Vault found!" << endl;
	}
	cout << "Enter your encryption key: ";
	cin >> encryptionKey;
	Vault userVault("vault.bin", encryptionKey);
	userVault.add();
	userVault.view();
	cout << "Would you like to view another account? (y\\n): ";
	char input;
	input = _getch();
	while (input != 'n') {
		if (input == 'y') {
			userVault.view();
			cout << endl;
		}
		else {
			cout << "Please enter 'y' or 'n'." << endl;
			cout << "Would you like to view another account? (y\\n): ";
			char input;
			cin >> input;
			cout << endl;
		}
	}
	cout << "Would you like to save the vault? (y\\n): ";
	cin >> input;
	if (input == 'y') {
		userVault.save();
	}
	cout << "Exiting password manager..." << endl;
	//cout << "Hello world!" << endl;
}