#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

// Encrypt a file using AES with a password
bool encryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password);

// Decrypt a file using AES with a password
bool decryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password);

// Validate password strength
bool isPasswordValid(const std::string& password);

#endif // ENCRYPTION_H
