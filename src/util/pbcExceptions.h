#ifndef PBCEXCEPTIONS_H
#define PBCEXCEPTIONS_H

#include <exception>
#include <string>

class PBCException : public std::exception {
 private:
    std::string _msg;
 public:
    explicit PBCException(const std::string& msg = "") : _msg(msg) {}
    const char* what() const noexcept {
        std::string msg = "Error in Playbook Creator: " + _msg;
        return msg.c_str();
    }
};

class PBCRuleBreakException : public PBCException {
 private:
    std::string _msg;
 public:
    explicit PBCRuleBreakException(const std::string& msg = "") : _msg(msg) {}
    const char* what() const noexcept {
        std::string msg = "You are breaking football rules: " + _msg;
        return msg.c_str();
    }
};

class PBCStorageException : public PBCException {
 private:
    std::string _msg;
 public:
    explicit PBCStorageException(const std::string& msg = "") : _msg(msg) {}
    const char* what() const noexcept {
        std::string msg = "Error while executing storage operation: " + _msg;
        return msg.c_str();
    }
};

class PBCDecryptionException : public PBCException {
 private:
    std::string _msg;
 public:
    explicit PBCDecryptionException(const std::string& msg = "") : _msg(msg) {}
    const char* what() const noexcept {
        std::string msg = "Error while decrypting file: " + _msg;
        return msg.c_str();
    }
};

#endif  // PBCEXCEPTIONS_H
