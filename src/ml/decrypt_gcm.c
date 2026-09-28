/*
 * decrypt_gcm.c — AES-256-GCM file decryption in 4096-byte chunks
 *
 * Usage: decryptGCM <inputfile> <outputfile> <key_hex64> <nonce_hex24>
 *   key_hex64  : 64 hex characters (256-bit key)  — must match encryption
 *   nonce_hex24: 24 hex characters (96-bit nonce) — must match encryption
 *
 * Expected input format: [ciphertext bytes ...][16-byte auth tag]
 * Decryption is aborted if the auth tag does not match (tamper detection).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#define CHUNK_SIZE   4096
#define NONCE_LEN      12
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

    /* Determine ciphertext length: file size minus the trailing auth tag */
    if (fseek(fin, 0, SEEK_END) != 0) { perror("fseek"); fclose(fin); return 1; }
    long fsize = ftell(fin);
    if (fsize < GCM_TAG_LEN) {
        fprintf(stderr, "Error: input file too small to contain a GCM auth tag\n");
        fclose(fin); return 1;
    }
    rewind(fin);
    long ciphertext_len = fsize - GCM_TAG_LEN;

    FILE *fout = fopen(outfile, "ab");
    if (!fout) { perror("fopen output"); fclose(fin); return 1; }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        fprintf(stderr, "EVP_CIPHER_CTX_new failed\n");
        fclose(fin); fclose(fout); return 1;
    }

    /* 1. Select cipher */
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1) {
        fprintf(stderr, "EVP_DecryptInit_ex (cipher) failed\n");
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
    if (EVP_DecryptInit_ex(ctx, NULL, NULL, key, nonce) != 1) {
        fprintf(stderr, "EVP_DecryptInit_ex (key/nonce) failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }

    unsigned char inbuf[CHUNK_SIZE];
    unsigned char outbuf[CHUNK_SIZE];
    int outlen;
    long remaining = ciphertext_len;
    long total_in = 0, total_out = 0;

    while (remaining > 0) {
        size_t to_read = (remaining < CHUNK_SIZE) ? (size_t)remaining : CHUNK_SIZE;
        size_t inlen = fread(inbuf, 1, to_read, fin);
        if (inlen == 0) {
            if (ferror(fin)) { perror("fread"); }
            else { fprintf(stderr, "Unexpected end of file\n"); }
            EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
        }
        remaining -= (long)inlen;
        total_in  += (long)inlen;

        if (EVP_DecryptUpdate(ctx, outbuf, &outlen, inbuf, (int)inlen) != 1) {
            fprintf(stderr, "EVP_DecryptUpdate failed\n");
            ERR_print_errors_fp(stderr);
            EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
        }
        if (outlen > 0) {
            fwrite(outbuf, 1, (size_t)outlen, fout);
            total_out += outlen;
        }
    }

    /* Read the trailing 16-byte auth tag */
    unsigned char tag[GCM_TAG_LEN];
    if (fread(tag, 1, GCM_TAG_LEN, fin) != GCM_TAG_LEN) {
        fprintf(stderr, "Failed to read auth tag\n");
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }

    /* Set expected tag before calling Final */
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, GCM_TAG_LEN, tag) != 1) {
        fprintf(stderr, "EVP_CTRL_GCM_SET_TAG failed\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }

    /* Finalize — returns > 0 only if tag matches */
    if (EVP_DecryptFinal_ex(ctx, outbuf, &outlen) <= 0) {
        fprintf(stderr, "Authentication FAILED — wrong key/nonce or data corrupted\n");
        ERR_print_errors_fp(stderr);
        EVP_CIPHER_CTX_free(ctx); fclose(fin); fclose(fout); return 1;
    }
    if (outlen > 0) {
        fwrite(outbuf, 1, (size_t)outlen, fout);
        total_out += outlen;
    }

    EVP_CIPHER_CTX_free(ctx);
    fclose(fin);
    fclose(fout);

    printf("Decrypted '%s' -> '%s'  (%ld -> %ld bytes, auth tag OK)\n",
           infile, outfile, total_in, total_out);
    return 0;
}
