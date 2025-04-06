module;
/*
    eLyKseeR or LXR - cryptographic data archiving software
    https://github.com/eLyKseeR/elykseer-cpp
    Copyright (C) 2019-2025 Alexander Diemand

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <cstddef>
#include <algorithm>
#include <cstring>
#include <string>
#include <cassert>

#if CRYPTOLIB == OPENSSL
#include "openssl/evp.h"
#include "openssl/core_names.h"
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#endif

import lxr_key128;
import lxr_key160;
import lxr_key256;


module lxr_hmac;


#if CRYPTOLIB == OPENSSL

namespace lxr {

class HMACbase {
    public:
        explicit HMACbase(const char* digest_name) 
            : _mac{EVP_MAC_fetch(NULL, "HMAC", NULL)}, 
              _ctx{EVP_MAC_CTX_new(_mac)},
              _digest{digest_name} {
            if (!_mac) {
                throw std::runtime_error("Failed to fetch HMAC provider");
            }
            if (!_ctx) {
                EVP_MAC_free(_mac);
                throw std::runtime_error("Failed to create HMAC context");
            }
        }
    
        ~HMACbase() {
            EVP_MAC_CTX_free(_ctx);
            EVP_MAC_free(_mac);
        }
    
        void init(const unsigned char* key, size_t key_len) {
            OSSL_PARAM params[2];
            params[0] = OSSL_PARAM_construct_utf8_string(OSSL_MAC_PARAM_DIGEST, (char*)_digest.c_str(), 0);
            params[1] = OSSL_PARAM_construct_end();
            if (EVP_MAC_init(_ctx, key, key_len, params) != 1) {
                throw std::runtime_error("Failed to initialize HMAC context");
            }
        }
    
        void update(const unsigned char* data, size_t data_len) {
            size_t remaining = data_len;
            const size_t chunk_size = EVP_MAC_CTX_get_block_size(_ctx);
            
            while (remaining > 0) {
                size_t processing = std::min(chunk_size, remaining);
                if (EVP_MAC_update(_ctx, data, processing) != 1) {
                    throw std::runtime_error("Failed to update HMAC context");
                }
                data += processing;
                remaining -= processing;
            }
        }
    
        void final(unsigned char* digest, size_t digest_size, size_t* out_len) {
            if (EVP_MAC_final(_ctx, digest, out_len, digest_size) != 1) {
                throw std::runtime_error("Failed to finalize HMAC");
            }
        }
    
    private:
        EVP_MAC *_mac;
        EVP_MAC_CTX *_ctx;
        std::string _digest;
};

enum class Hash {
    MD5,
    SHA1,
    SHA256
};

template<Hash HashClass, typename KeyClass>
class HashHMAC {
public:
    static const char* DIGEST_NAME;
    static const size_t DIGEST_SIZE;
    static KeyClass compute(const unsigned char* key, size_t key_len,
                        const unsigned char* data, size_t data_len) {
        lxr::HMACbase helper{DIGEST_NAME};
        
        unsigned char digest[DIGEST_SIZE];
        size_t digest_out;
        try {
            helper.init(key, key_len);
            helper.update(data, data_len);
            helper.final(digest, DIGEST_SIZE, &digest_out);
        } catch (const std::runtime_error& e) {
            // Handle specific OpenSSL errors
            ERR_print_errors_fp(stderr);
            throw;
        }
        if (digest_out != DIGEST_SIZE) {
            throw std::runtime_error("Failed to compute HMAC");
        }
        KeyClass reskey{true};
        reskey.fromBytes(digest);
        return reskey;
    }
};

// Specializations for each hash algorithm
template<> const char* HashHMAC<Hash::MD5, Key128>::DIGEST_NAME = "md5";
template<> const size_t HashHMAC<Hash::MD5, Key128>::DIGEST_SIZE = MD5_DIGEST_LENGTH;

template<> const char* HashHMAC<Hash::SHA1, Key160>::DIGEST_NAME = "sha1";
template<> const size_t HashHMAC<Hash::SHA1, Key160>::DIGEST_SIZE = SHA_DIGEST_LENGTH;

template<> const char* HashHMAC<Hash::SHA256, Key256>::DIGEST_NAME = "sha256";
template<> const size_t HashHMAC<Hash::SHA256, Key256>::DIGEST_SIZE = SHA256_DIGEST_LENGTH;


Key128 HMAC::hmac_md5(const char k[], int klen, std::string const & msg)
{
    // return HMAC::hmac_md5(k, klen, msg.c_str(), msg.size());
    return HashHMAC<Hash::MD5, Key128>::compute((const unsigned char*)k, klen, (const unsigned char*)msg.c_str(), msg.size());
}

Key128 HMAC::hmac_md5(const char k[], int klen, const char *buffer, int blen)
{
    return HashHMAC<Hash::MD5, Key128>::compute((const unsigned char*)k, klen, (const unsigned char*)buffer, blen);
}

Key256 HMAC::hmac_sha256(const char k[], int klen, std::string const & msg)
{
    return HashHMAC<Hash::SHA256, Key256>::compute((const unsigned char*)k, klen, (const unsigned char*)msg.c_str(), msg.size());
}

Key256 HMAC::hmac_sha256(const char k[], int klen, const char *buffer, int blen)
{
    return HashHMAC<Hash::SHA256, Key256>::compute((const unsigned char*)k, klen, (const unsigned char*)buffer, blen);
}

Key160 HMAC::hmac_sha1(const char k[], int klen, std::string const & msg)
{
    return HashHMAC<Hash::SHA1, Key160>::compute((const unsigned char*)k, klen, (const unsigned char*)msg.c_str(), msg.size());
}

Key160 HMAC::hmac_sha1(const char k[], int klen, const char *buffer, int blen)
{
    return HashHMAC<Hash::SHA1, Key160>::compute((const unsigned char*)k, klen, (const unsigned char*)buffer, blen);
}

} // namespace

#endif