declared in [Gpg](gpg.hpp.md)

```cpp

struct Gpg::pimpl {
    public:
    bool startup_gpgme();
    bool has_key(std::string const &fpr, bool private_key = false);
    bool has_public_key(std::string const &fpr) { return has_key(fpr, false); };
    bool has_private_key(std::string const &fpr) { return has_key(fpr, true); };
    std::optional<std::string> encrypt_to_key(std::string const & fpr, std::string const & );
    std::optional<std::string> encrypt_to_key(std::string const & fpr);
    std::ostream& ostream() { oss.reset(new std::ostringstream()); return *oss; }
    std::istream& istream() { return *iss; }
    bool decrypt_from_file(std::string const & fpath);

    private:
    gpgme_ctx_t ctx{nullptr};
    std::unique_ptr<std::ostringstream> oss;
    std::unique_ptr<std::istringstream> iss;
    std::optional<gpgme_key_t> find_key(std::string const &fpr, bool private_key = false);
};

// define the following to get debugging output about the GPG engine and the keys found
#undef DEBUG_OUTPUT

#ifdef DEBUG_OUTPUT
#define OUTPUT_GPGME_ERROR(err) std::clog << "gpgme error: " << err << "/" << gpgme_strerror(err) << std::endl;
#else
#define OUTPUT_GPGME_ERROR(err)
#endif

bool Gpg::pimpl::startup_gpgme() {
    gpgme_error_t err;
    try {
        gpgme_check_version(NULL);
        setlocale(LC_ALL, "");
        gpgme_set_locale(NULL, LC_CTYPE, setlocale(LC_CTYPE, NULL));
    #ifndef _WIN32
        gpgme_set_locale(NULL, LC_MESSAGES, setlocale(LC_MESSAGES, NULL));
    #endif
        err = gpgme_engine_check_version(GPGME_PROTOCOL_OpenPGP);
        if (err) { OUTPUT_GPGME_ERROR(err); return false; }
        err = gpgme_new (&ctx);
        if (err) { OUTPUT_GPGME_ERROR(err); return false; }
        gpgme_engine_info_t eng = gpgme_ctx_get_engine_info (ctx);
#ifdef DEBUG_OUTPUT
        while (eng) {
            std::clog << "GPGme engine: " << eng->file_name << " " << eng->version << std::endl;
            eng = eng->next;
        }
#endif
        const auto sender = gpgme_get_sender(ctx);
#ifdef DEBUG_OUTPUT
        if (sender) {
            std::clog << "GPGme sender: " << sender << std::endl;
        } else {
            std::clog << "GPGme no sender." << std::endl;
        }
#endif
    } catch (...) {
        std::clog << "gpgme exception!" << std::endl;
        return false;
    }

    return true;
}

std::optional<gpgme_key_t> Gpg::pimpl::find_key(std::string const &addr, bool private_key) {
    gpgme_error_t err;
    try {
        gpgme_key_t k{nullptr};
        err = gpgme_get_key(ctx, addr.c_str(), &k, private_key);
        if (err) { OUTPUT_GPGME_ERROR(err); return {}; }
        return k;
    } catch (...) {
        std::clog << "gpgme exception!" << std::endl;
    }
    return {};
}

bool Gpg::pimpl::has_key(std::string const &addr, bool private_key) {
    auto k0 = find_key(addr, private_key);
    gpgme_error_t err;
    if (k0) {
#ifdef DEBUG_OUTPUT
        gpgme_key_t k = k0.value();;
        gpgme_subkey_t sk;
        sk = k->subkeys;
        while (sk) {
            std::clog << "GPGme subkey: " << sk->keyid << " " << sk->fpr << std::endl;
            sk = sk->next;
        }
#endif
    } else {
#ifdef DEBUG_OUTPUT
        std::clog << "no key found" << std::endl;
#endif
        return false;
    }

    return true;
}

std::optional<std::string> Gpg::pimpl::encrypt_to_key(std::string const & addr) {
    return encrypt_to_key(addr, oss->str());
}
std::optional<std::string> Gpg::pimpl::encrypt_to_key(std::string const & addr, std::string const & s) {
    gpgme_error_t err;
    err = gpgme_new(&ctx);
    if (err) { OUTPUT_GPGME_ERROR(err); return {}; }

    auto k0 = find_key(addr);
    if (! k0) { gpgme_release(ctx); return {}; }

    const char *sc = s.c_str();
    int sl = s.size();
    gpgme_data_t plain, cipher;
    err = gpgme_data_new_from_mem(&plain, sc, sl, 0);
    if (err) { OUTPUT_GPGME_ERROR(err); gpgme_release(ctx); return {}; }
    err = gpgme_data_new(&cipher);
    if (err) { OUTPUT_GPGME_ERROR(err); gpgme_data_release(plain); gpgme_release(ctx); return {}; }

    auto cleanup = [&](void) {
        gpgme_data_release(plain);
        gpgme_data_release(cipher);
        gpgme_release(ctx); };
    gpgme_key_t k[] = { NULL, NULL };
    k[0] = k0.value();
    gpgme_set_armor(ctx, 1);
    err = gpgme_op_encrypt(ctx, k, GPGME_ENCRYPT_ALWAYS_TRUST, plain, cipher);
    if (err) {
        OUTPUT_GPGME_ERROR(err);
        cleanup();
        return {};
    }
    gpgme_encrypt_result_t result = gpgme_op_encrypt_result(ctx);
    if (result->invalid_recipients) {
        OUTPUT_GPGME_ERROR(err);
        cleanup();
        return {};
    }

    err = gpgme_data_seek(cipher, 0, SEEK_SET);
    if (err) { OUTPUT_GPGME_ERROR(err); return {}; }
    size_t mlen;
    const char *mptr = gpgme_data_release_and_get_mem(cipher, &mlen);
    gpgme_data_release(plain);
    gpgme_release(ctx);
    return std::string(mptr, mlen);
}

bool Gpg::pimpl::decrypt_from_file(std::string const &p_fp)
{
    std::filesystem::path _fp{p_fp};
    if (! std::filesystem::exists(_fp)) {
        return false;
    }

    gpgme_error_t err;
    gpgme_data_t plain, cipher;
    err = gpgme_data_new_from_file(&cipher, _fp.native().c_str(), 1);
    if (err != GPG_ERR_NO_ERROR) { OUTPUT_GPGME_ERROR(err); return false; }
    err = gpgme_data_new(&plain);
    if (err != GPG_ERR_NO_ERROR) { OUTPUT_GPGME_ERROR(err); gpgme_data_release(cipher); return false; }
    auto cleanup_data = [&](void) {
        gpgme_data_release(plain);
        gpgme_data_release(cipher);
    };

    err = gpgme_new(&ctx);
    if (err != GPG_ERR_NO_ERROR) { OUTPUT_GPGME_ERROR(err); cleanup_data(); return false; }

    err = gpgme_op_decrypt(ctx, cipher, plain);
    if (err != GPG_ERR_NO_ERROR) { OUTPUT_GPGME_ERROR(err); cleanup_data(); gpgme_release(ctx); return false; }

    gpgme_data_release(cipher);
    size_t mlen;
    const char *mptr = gpgme_data_release_and_get_mem(plain, &mlen);
    iss.reset(new std::istringstream(std::string(mptr, mlen)));
    return true;
}

Gpg::Gpg()
    : _pimpl(std::make_unique<pimpl>())
{
    _pimpl->startup_gpgme();
}

Gpg::~Gpg() = default;

```