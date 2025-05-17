#include "encryption.h"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <filesystem>

namespace fs = std::filesystem;

// Hides password input on terminal
std::string getHiddenPassword() {
    std::string password;
    termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    std::getline(std::cin, password);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    std::cout << std::endl;

    return password;
}

// Extracts extension from filename (e.g. .txt)
std::string getFileExtension(const std::string& path) {
    return fs::path(path).extension().string();
}

// Extracts parent directory from file path
std::string getDirectory(const std::string& path) {
    return fs::path(path).parent_path().string();
}

int main() {
    std::string choice;

    while (true) {
        std::cout << "\n========== File Encryption App ==========\n";
        std::cout << "1. Encrypt File\n";
        std::cout << "2. Decrypt File\n";
        std::cout << "3. Quit\n";
        std::cout << "=========================================\n";
        std::cout << "Enter your choice: ";
        std::getline(std::cin, choice);

        switch (choice[0]) {
            case '1': { // Encrypt
                std::string input, password;
                std::cout << "\nEnter input file path: ";
                std::getline(std::cin, input);

                std::cout << "\nPassword Requirements:\n";
                std::cout << "- At least 8 characters\n";
                std::cout << "- At least one digit (0-9)\n";
                std::cout << "- At least one lowercase letter (a-z)\n";
                std::cout << "- At least one uppercase letter (A-Z)\n";
                std::cout << "- At least one special character (!@#$%^&*()_+)\n";
                std::cout << "\nEnter password: ";
                password = getHiddenPassword();

                if (!isPasswordValid(password)) {
                    std::cerr << "Invalid password. Try again.\n";
                    continue;
                }

                std::string dir = getDirectory(input);
                std::string ext = getFileExtension(input);
                std::string output = dir + "/encrypted_file" + ext;

                if (encryptFile(input, output, password))
                    std::cout << "✅ File encrypted successfully at: " << output << "\n";
                else
                    std::cerr << "❌ Encryption failed.\n";
                break;
            }
            case '2': { // Decrypt
                std::string input, password;
                std::cout << "\nEnter input file path: ";
                std::getline(std::cin, input);

                std::cout << "\nPassword Requirements:\n";
                std::cout << "- At least 8 characters\n";
                std::cout << "- At least one digit (0-9)\n";
                std::cout << "- At least one lowercase letter (a-z)\n";
                std::cout << "- At least one uppercase letter (A-Z)\n";
                std::cout << "- At least one special character (!@#$%^&*()_+)\n";
                std::cout << "\nEnter password: ";
                password = getHiddenPassword();

                if (!isPasswordValid(password)) {
                    std::cerr << "Invalid password. Try again.\n";
                    continue;
                }

                std::string dir = getDirectory(input);
                std::cout << "Enter output file name (with extension): ";
                std::string decryptFileName;
                std::getline(std::cin, decryptFileName);
                std::string decryptOutput = dir + "/" + decryptFileName;

                if (decryptFile(input, decryptOutput, password))
                    std::cout << "✅ File decrypted successfully at: " << decryptOutput << "\n";
                else
                    std::cerr << "❌ Decryption failed.\n";
                break;
            }
            case '3': { // Quit
                std::cout << "Exiting program. Goodbye!\n";
                return 0;
            }
            default: { // Invalid option
                std::cout << "Invalid option. Please choose 1, 2, or 3.\n";
                break;
            }
        }
    }

    return 0;
}
