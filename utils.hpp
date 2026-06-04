#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <conio.h>
#include <string>

std::string takePasswdFromUser();

void clearScreen();

std::string decryptAndLoadFile(std::string encryptionKey, std::string filename);

void encryptAndSaveFile(std::string password, std::string filename, std::string data);