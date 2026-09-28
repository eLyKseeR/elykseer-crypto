/*
 * encrypt.c — AES-256-CBC file encryption in 4096-byte chunks
 *
 * Usage: encrypt <inputfile> <outputfile> <key_hex64> <iv_hex32>
 *   key_hex64 : 64 hex characters (256-bit key)
 *   iv_hex32  : 32 hex characters (128-bit IV)
 *
 * The output file is opened in append mode; PKCS7 padding is applied
 * to the final block by OpenSSL automatically.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define CHUNK_SIZE 4096

static int hex_to_bytes(const char *hex, unsigned char *out, int expected_len)
{
    int slen = (int)strlen(hex);
    if (slen != expected_len * 2) return -1;
    for (int i = 0; i < expected_len; i++) {
        unsigned int byte;
        if (sscanf(hex + 2 * i, "%02x", &byte) != 1) return -1;
        out[i] = (unsigned char)byte;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 5) {
        fprintf(stderr,
            "Usage: %s <inputfile> <outputfile> <key_hex64> <iv_hex32>\n",
            argv[0]);
        return 1;
    }

    const char *infile   = argv[1];
    const char *outfile  = argv[2];
    const char *key_hex  = argv[3];
    const char *iv_hex   = argv[4];

    unsigned char key[32], iv[16];
    if (hex_to_bytes(key_hex, key, 32) != 0) {
        fprintf(stderr, "Error: key must be exactly 64 hex characters (256-bit)\n");
        return 1;
    }
    if (hex_to_bytes(iv_hex, iv, 16) != 0) {
        fprintf(stderr, "Error: IV must be exactly 32 hex characters (128-bit)\n");
        return 1;
    }

    FILE *fin = fopen(infile, "rb");
    if (!fin) { perror("fopen input"); return 1; }

    FILE *fout = fopen(outfile, "ab");
    if (!fout) { perror("fopen output"); fclose(fin); return 1; }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "EVP_CIPHER_CTX_new failed\n");
        fclose(fin); fclose(fout);
        return 1;
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        fprintf(stderr, "EVP_EncryptInit_ex failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx);
        fclose(fin); fclose(fout);
        return 1;
    }

    unsigned char inbuf[CHUNK_SIZE];
    /* output buffer: chunk + one extra block for CBC buffering */
    unsigned char outbuf[CHUNK_SIZE + EVP_MAX_BLOCK_LENGTH];
    int outlen;
    size_t inlen;
    long total_in = 0, total_out = 0;

    while ((inlen = fread(inbuf, 1, CHUNK_SIZE, fin)) > 0) {
        total_in += (long)inlen;
        if (EVP_EncryptUpdate(ctx, outbuf, &outlen, inbuf, (int)inlen) != 1) {
            fprintf(stderr, "EVP_EncryptUpdate failed\n");
            ERR_print_errors_fp(stderr);
            EVP_CIPHER_CTX_free(ctx);
            fclose(fin); fclose(fout);
            return 1;
        }
        if (outlen > 0) {
            fwrite(outbuf, 1, (size_t)outlen, fout);
            total_out += outlen;
        }
    }

    if (ferror(fin)) {
        perror("fread");
        EVP_CIPHER_CTX_free(ctx);
        fclose(fin); fclose(fout);
        return 1;
    }

    /* Flush remaining bytes and append PKCS7 padding block */
    if (EVP_EncryptFinal_ex(ctx, outbuf, &outlen) != 1) {
        fprintf(stderr, "EVP_EncryptFinal_ex failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx);
        fclose(fin); fclose(fout);
        return 1;
    }
    if (outlen > 0) {
        fwrite(outbuf, 1, (size_t)outlen, fout);
        total_out += outlen;
    }

    EVP_CIPHER_CTX_free(ctx);
    fclose(fin);
    fclose(fout);

    printf("Encrypted '%s' -> '%s'  (%ld -> %ld bytes)\n",
           infile, outfile, total_in, total_out);
    return 0;
}
