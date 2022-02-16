declared in [Gpg](gpg.hpp.md)

```cpp

struct Gpg::pimpl {
    public:
    bool startup_gpgme();
    bool has_public_key(std::string const &);
    std::optional<std::string> encrypt_to_key(std::string const & fpr);
    std::ostream& ostream() { oss.str() = ""; return oss; }
    bool good() { return oss.good(); }

    private:
    gpgme_ctx_t ctx{nullptr};
    std::ostringstream oss;
    std::optional<gpgme_key_t> find_key(std::string const &);
};

#define OUTPUT_GPGME_ERROR(err) std::clog << "gpgme error: " << err << "/" << gpgme_strerror(err) << std::endl;

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
        while (eng) {
            std::clog << "GPGme engine: " << eng->file_name << " " << eng->version << std::endl;
            eng = eng->next;
        }
        const auto sender = gpgme_get_sender(ctx);
        if (sender) {
            std::clog << "GPGme sender: " << sender << std::endl;
        } else {
            std::clog << "GPGme no sender." << std::endl;
        }
    } catch (...) {
        std::clog << "gpgme exception!" << std::endl;
        return false;
    }

    return true;
}

std::optional<gpgme_key_t> Gpg::pimpl::find_key(std::string const &addr) {
    gpgme_error_t err;
    try {
        gpgme_key_t k{nullptr};
        // gpgme_subkey_t sk;
        err = gpgme_get_key(ctx, addr.c_str(), &k, false);
        if (err) { OUTPUT_GPGME_ERROR(err); return {}; }
        return k;
    } catch (...) {
        std::clog << "gpgme exception!" << std::endl;
    }
    return {};
}

bool Gpg::pimpl::has_public_key(std::string const &addr) {
    auto k0 = find_key(addr);
    gpgme_error_t err;
    if (k0) {
        gpgme_key_t k = k0.value();;
        gpgme_subkey_t sk;
        sk = k->subkeys;
        while (sk) {
            std::clog << "GPGme subkey: " << sk->keyid << " " << sk->fpr << std::endl;
            sk = sk->next;
        }
    } else {
        std::clog << "no key found" << std::endl;
        return false;
    }

    return true;
}

std::optional<std::string> Gpg::pimpl::encrypt_to_key(std::string const & addr) {
    gpgme_error_t err;
    err = gpgme_new(&ctx);
    if (err) { OUTPUT_GPGME_ERROR(err); return {}; }

    auto k0 = find_key(addr);
    if (! k0) { return {}; }

    std::string s = oss.str();
    const char *sc = s.c_str();
    int sl = s.size();
    gpgme_data_t plain, cipher;
    err = gpgme_data_new_from_mem(&plain, sc, sl, 0);
    if (err) { OUTPUT_GPGME_ERROR(err); return {}; }
    err = gpgme_data_new(&cipher);
    if (err) { OUTPUT_GPGME_ERROR(err); return {}; }

    gpgme_key_t k[] = { NULL, NULL };
    k[0] = k0.value();
    gpgme_set_armor(ctx, 1);
    err = gpgme_op_encrypt(ctx, k, GPGME_ENCRYPT_ALWAYS_TRUST, plain, cipher);
    if (err) { OUTPUT_GPGME_ERROR(err); return {}; }
    gpgme_encrypt_result_t result = gpgme_op_encrypt_result(ctx);
    if (result->invalid_recipients) { OUTPUT_GPGME_ERROR(err); return {}; }

    err = gpgme_data_seek(cipher, 0, SEEK_SET);
    if (err) { OUTPUT_GPGME_ERROR(err); return {}; }
    constexpr int bufsz = 1024;
    char buf[bufsz + 1];
    std::string res{""};
    int nread = 0;
    while ((nread = gpgme_data_read(cipher, buf, bufsz)) > 0) {
        res = res + std::string(buf, nread);
    }

    gpgme_data_release(plain);
    gpgme_data_release(cipher);
    gpgme_release(ctx);

    return res;
}

Gpg::Gpg()
    : _pimpl(std::make_unique<pimpl>())
{
    _pimpl->startup_gpgme();
}

Gpg::~Gpg() = default;

```