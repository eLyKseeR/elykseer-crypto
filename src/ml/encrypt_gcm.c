/*
 * encrypt_gcm.c — AES-256-GCM file encryption in 4096-byte chunks
 *
 * Usage: encryptGCM <inputfile> <outputfile> <key_hex64> <nonce_hex24>
 *   key_hex64  : 64 hex characters (256-bit key)
 *   nonce_hex24: 24 hex characters (96-bit nonce — standard for GCM)
 *
 * Output format: [ciphertext bytes ...][16-byte auth tag]
 * No padding is added; output is exactly len(input) + 16 bytes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define CHUNK_SIZE   4096
#define NONCE_LEN      12   /* 96-bit nonce, standard for GCM */
#define GCM_TAG_LEN    16

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
            "Usage: %s <inputfile> <outputfile> <key_hex64> <nonce_hex24>\n",
            argv[0]);
        return 1;
    }

    const char *infile    = argv[1];
    const char *outfile   = argv[2];
    const char *key_hex   = argv[3];
    const char *nonce_hex = argv[4];

    unsigned char key[32], nonce[NONCE_LEN];
    if (hex_to_bytes(key_hex, key, 32) != 0) {
        fprintf(stderr, "Error: key must be exactly 64 hex characters (256-bit)\n");
        return 1;
    }
    if (hex_to_bytes(nonce_hex, nonce, NONCE_LEN) != 0) {
        fprintf(stderr, "Error: nonce must be exactly 24 hex characters (96-bit)\n");
        return 1;
    }

    FILE *fin = fopen(infile, "rb");
    if (!fin) { perror("fopen input"); return 1; }

    FILE *fout = fopen(outfile, "ab");
    if (!fout) { perror("fopen output"); fclose(fin); return 1; }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "EVP_CIPHER_CTX_new failed\n");
        fclose(fin); fclose(fout); return 1;
    }

    /* 1. Select cipher */
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) {
        fprintf(stderr, "EVP_EncryptInit_ex (cipher) failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }

    /* 2. Set nonce length */
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, NONCE_LEN, NULL) != 1) {
        fprintf(stderr, "EVP_CTRL_GCM_SET_IVLEN failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }

    /* 3. Set key and nonce */
    if (EVP_EncryptInit_ex(ctx, NULL, NULL, key, nonce) != 1) {
        fprintf(stderr, "EVP_EncryptInit_ex (key/nonce) failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }

    unsigned char inbuf[CHUNK_SIZE];
    unsigned char outbuf[CHUNK_SIZE]; /* GCM: no size expansion per chunk */
    int outlen;
    size_t inlen;
    long total_in = 0, total_out = 0;

    while ((inlen = fread(inbuf, 1, CHUNK_SIZE, fin)) > 0) {
        total_in += (long)inlen;
        if (EVP_EncryptUpdate(ctx, outbuf, &outlen, inbuf, (int)inlen) != 1) {
            fprintf(stderr, "EVP_EncryptUpdate failed\n");
            ERR_print_errors_fp(stderr);
            EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
        }
        if (outlen > 0) {
            fwrite(outbuf, 1, (size_t)outlen, fout);
            total_out += outlen;
        }
    }

    if (ferror(fin)) {
        perror("fread");
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }

    /* Finalize — GCM produces no extra bytes here (no padding) */
    if (EVP_EncryptFinal_ex(ctx, outbuf, &outlen) != 1) {
        fprintf(stderr, "EVP_EncryptFinal_ex failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }
    if (outlen > 0) {
        fwrite(outbuf, 1, (size_t)outlen, fout);
        total_out += outlen;
    }

    /* Retrieve and append the 16-byte authentication tag */
    unsigned char tag[GCM_TAG_LEN];
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, GCM_TAG_LEN, tag) != 1) {
        fprintf(stderr, "EVP_CTRL_GCM_GET_TAG failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }
    fwrite(tag, 1, GCM_TAG_LEN, fout);
    total_out += GCM_TAG_LEN;

    EVP_CIPHER_CTX_free(ctx);
    fclose(fin);
    fclose(fout);

    printf("Encrypted '%s' -> '%s'  (%ld -> %ld bytes, incl. %d-byte auth tag)\n",
           infile, outfile, total_in, total_out, GCM_TAG_LEN);
    return 0;
}
