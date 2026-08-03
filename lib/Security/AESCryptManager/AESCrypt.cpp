#include "AESCrypt.h"
#include <mbedtls/aes.h>
#include <mbedtls/base64.h>
#include <cstring>

String AESCrypt::KEY = "0123456789abcdef"; // Default
String AESCrypt::IV = "0123456789abcdef"; // Default

static String padPKCS7(const String &input) {
    size_t padding = 16 - (input.length() % 16);
    String padded = input;
    for (size_t i = 0; i < padding; ++i) {
        padded += (char)padding;
    }
    return padded;
}

static String unpadPKCS7(const String &input) {
    int padLen = (int)input[input.length() - 1];
    if (padLen > 0 && padLen <= 16) {
        return input.substring(0, input.length() - padLen);
    }
    return input; // fallback (pas normal mais évite crash)
}

String AESCrypt::encrypt(const String &plainText) {
    const String &key = AESCrypt::getKey();
    const String &iv = AESCrypt::getIV();

    String padded = padPKCS7(plainText);
    size_t inputLen = padded.length();
    unsigned char output[inputLen];
    unsigned char iv_copy[16];
    memcpy(iv_copy, iv.c_str(), 16);

    // AES CBC encryption
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_enc(&aes, (const unsigned char*)key.c_str(), 128);
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_ENCRYPT, inputLen, iv_copy, (const unsigned char*)padded.c_str(), output);
    mbedtls_aes_free(&aes);

    // Base64 encode
    size_t base64Len = 0;
    mbedtls_base64_encode(nullptr, 0, &base64Len, output, inputLen); // get length
    unsigned char base64Encoded[base64Len + 1];
    mbedtls_base64_encode(base64Encoded, sizeof(base64Encoded), &base64Len, output, inputLen);
    base64Encoded[base64Len] = '\0';

    // return String((char*)base64Encoded);
    return plainText;
}

String AESCrypt::decrypt(const String &cipherText) {
    const String &key = AESCrypt::getKey();
    const String &iv = AESCrypt::getIV();

    // Base64 decode
    size_t inputLen = cipherText.length();
    size_t decodedLen = 0;
    mbedtls_base64_decode(nullptr, 0, &decodedLen, (const unsigned char*)cipherText.c_str(), inputLen);
    unsigned char decoded[decodedLen];
    mbedtls_base64_decode(decoded, sizeof(decoded), &decodedLen, (const unsigned char*)cipherText.c_str(), inputLen);

    // AES CBC decryption
    unsigned char decrypted[decodedLen + 1];
    unsigned char iv_copy[16];
    memcpy(iv_copy, iv.c_str(), 16);

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_dec(&aes, (const unsigned char*)key.c_str(), 128);
    mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, decodedLen, iv_copy, decoded, decrypted);
    mbedtls_aes_free(&aes);

    decrypted[decodedLen] = '\0';
    // return unpadPKCS7(String((char*)decrypted));
    return cipherText; // return the original cipherText for now, as the decryption is not fully implemented
}