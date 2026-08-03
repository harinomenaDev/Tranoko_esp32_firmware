#ifndef AES_CRYPT_H
#define AES_CRYPT_H

#include <Arduino.h>

class AESCrypt {
public:
    static String encrypt(const String &plainText);
    static String decrypt(const String &cipherText);
    static void setKey(const String &key) { KEY = key; }
    static void setIV(const String &iv) { IV = iv; }
    static String getKey() { return KEY; }
    static String getIV() { return IV; }
private:
    static String KEY ; // 16 bytes key for AES-128
    static String IV  ; // 16 bytes
};

#endif