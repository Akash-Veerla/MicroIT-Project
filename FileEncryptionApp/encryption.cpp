#include "encryption.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/files.h>
#include <cryptopp/aes.h>
#include <cryptopp/pwdbased.h>
#include <cryptopp/modes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <iostream>
#include <fstream>
#include <regex>

using namespace CryptoPP;

const int SALT_SIZE = 16;
const int IV_SIZE = AES::BLOCKSIZE;
const int KEY_SIZE = AES::DEFAULT_KEYLENGTH;

void deriveKeyAndIV(const std::string& password, const byte* salt, byte* key, byte* iv) {
    PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
    pbkdf.DeriveKey(key, KEY_SIZE, 0, 
        (byte*)password.data(), password.size(),
        salt, SALT_SIZE, 10000);

    // Use another derivation for IV
    pbkdf.DeriveKey(iv, IV_SIZE, 0,
        (byte*)password.data(), password.size(),
        salt, SALT_SIZE, 5000);
}

bool isPasswordValid(const std::string& password) {
    if (password.length() < 8)
        return false;

    bool hasDigit = std::regex_search(password, std::regex("[0-9]"));
    bool hasLower = std::regex_search(password, std::regex("[a-z]"));
    bool hasUpper = std::regex_search(password, std::regex("[A-Z]"));
    bool hasSpecial = std::regex_search(password, std::regex("[!@#$%^&*()_+]"));

    return hasDigit && hasLower && hasUpper && hasSpecial;
}

bool encryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    try {
        AutoSeededRandomPool rng;

        byte salt[SALT_SIZE];
        rng.GenerateBlock(salt, sizeof(salt));

        byte key[KEY_SIZE];
        byte iv[IV_SIZE];
        deriveKeyAndIV(password, salt, key, iv);

        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV(key, KEY_SIZE, iv);

        // Open output stream and write salt first
        FileSink out(outputPath.c_str());

        // Write salt first
        out.Put(salt, SALT_SIZE);

        // Encrypt and write data
        FileSource fs(inputPath.c_str(), true,
            new StreamTransformationFilter(encryption, new Redirector(out))
        );

        return true;
    } catch (const Exception& e) {
        std::cerr << "Encryption failed: " << e.what() << std::endl;
        return false;
    }
}

bool decryptFile(const std::string& inputPath, const std::string& outputPath, const std::string& password) {
    try {
        // Open input file and read salt
        std::ifstream inFile(inputPath, std::ios::binary);
        if (!inFile) {
            std::cerr << "Failed to open input file.\n";
            return false;
        }

        byte salt[SALT_SIZE];
        inFile.read(reinterpret_cast<char*>(salt), SALT_SIZE); // Read salt
        if (inFile.gcount() != SALT_SIZE) {
            std::cerr << "Failed to read salt.\n";
            return false;
        }

        // Derive key and IV
        byte key[KEY_SIZE];
        byte iv[IV_SIZE];
        deriveKeyAndIV(password, salt, key, iv);

        // Prepare decryption object
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV(key, KEY_SIZE, iv);

        // Create Crypto++ pipeline using remaining stream
        FileSource fs(inFile,
            true, // pumpAll
            new StreamTransformationFilter(decryption,
                new FileSink(outputPath.c_str())
            )
        );

        return true;

    } catch (const CryptoPP::Exception& e) {
        std::cerr << "Decryption failed: " << e.what() << std::endl;
        return false;
    }
}
