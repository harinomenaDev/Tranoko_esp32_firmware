#ifndef AES_CRYPT_H
#define AES_CRYPT_H

#include <Arduino.h>

class AESCrypt {
public:
    static String encrypt(const String &plainText, const String &key, const String &iv);
    static String decrypt(const String &cipherText, const String &key, const String &iv);
};

#endif