#include "utils.hpp"
#include <botan/block_cipher.h>
#include <botan/hex.h>
#include <botan/kdf.h>
#include <botan/pwdhash.h>
#include <botan/auto_rng.h>
#include <botan/cipher_mode.h>
#include <sstream>

using namespace std;


enum TT_Input {
	BACKSPACE = 8,
	RETURN = 32
};

const std::vector<uint8_t> IV = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
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

void clearScreen() {
	std::cout << "\033[2J\033[1;1H";
}

string decryptAndLoadFile(string password, string filename) {
	const std::vector<uint8_t> salt = { 0x4D, 0x79, 0x53, 0x61, 0x6C, 0x74, 0x56, 0x61 };
	ifstream loadedFile(filename, std::ios::binary);
	if (!loadedFile.is_open()) {
		cout << "Loading failed" << endl;
		return "";
	}
	auto pbkdf = Botan::PasswordHashFamily::create("PBKDF2(SHA-256)");
	auto kdf = pbkdf->default_params();
	std::vector<uint8_t> key(32);
	kdf->derive_key(key.data(), key.size(), password.c_str(), password.size(), salt.data(), salt.size());
	std::stringstream ss;
	ss << loadedFile.rdbuf();
	loadedFile.close();
	std::string fileContents = ss.str();
	Botan::secure_vector<uint8_t> buf(
		fileContents.begin(), fileContents.end()
	);
	auto dec = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7", Botan::Cipher_Dir::Decryption);
	dec->set_key(key);
	dec->start(IV);
	try {
		dec->finish(buf);
	}
	catch (const std::exception& e) {
		cout << "Decryption failed" << endl;
		return "";
	}
	loadedFile.close();
	string plaintextVault(buf.begin(), buf.end());
	//DEBUG ---------------------
	/*
	ofstream decryptedFile(filename + ".decrypted");
	decryptedFile << plaintextVault;
	decryptedFile.close();
	//END DEBUG -----------------
	*/
	return plaintextVault;
}

void encryptAndSaveFile(string password, string filename, string data) {
	const std::vector<uint8_t> salt = { 0x4D, 0x79, 0x53, 0x61, 0x6C, 0x74, 0x56, 0x61 };
	auto pbkdf = Botan::PasswordHashFamily::create("PBKDF2(SHA-256)");
	auto kdf = pbkdf->default_params();
	std::vector<uint8_t> key(32);
	kdf->derive_key(key.data(), key.size(), password.c_str(), password.size(), salt.data(), salt.size());
	auto enc = Botan::Cipher_Mode::create("AES-256/CBC/PKCS7", Botan::Cipher_Dir::Encryption);
	enc->set_key(key);
	enc->start(IV);
	Botan::secure_vector<uint8_t> buf(data.begin(), data.end());
	enc->finish(buf);
	ofstream outFile(filename, std::ios::binary);
	outFile.write(reinterpret_cast<const char*>(buf.data()), buf.size());
}