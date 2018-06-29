/** @file pbcExceptions.h
    This file is part of Playbook Creator.

    Playbook Creator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Playbook Creator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Playbook Creator.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2015 Oliver Braunsdorf

    @author Oliver Braunsdorf
*/

#ifndef PBCEXCEPTIONS_H
#define PBCEXCEPTIONS_H

#include <stdexcept>
#include <string>

/**
 * @class PBCException
 * @brief The basic exception for the Playbook Creator application
 */
class PBCException : public std::runtime_error {
 private:
    std::string _msg;

 protected:
    std::string getStackTrace() const {
        return "not possible in windows";
    }

 public:
    explicit PBCException(const std::string& msg = "") :
        std::runtime_error("Error in Playbook Creator: " + msg),
        _msg("Error in Playbook Creator: " + msg) {}
    const char* what() const noexcept {
        return _msg.c_str();
    }
};

/**
 * @class PBCUnexpectedError
 * @brief An exception that is thrown when an unexpected code branch
 * was taken, e.g. due to a programming mistake
 */
class PBCUnexpectedError : public PBCException {
 public:
    explicit PBCUnexpectedError(const std::string& msg = "") :
        PBCException("An unexpected error occured: "
                     + msg + "\n" + getStackTrace()) {}
};

/**
 * @class PBCRuleBreakException
 * @brief An exception that is thrown when some user action violates football
 * rules
 */
class PBCRuleBreakException : public PBCException {
 public:
    explicit PBCRuleBreakException(const std::string& msg = "") :
        PBCException("You are breaking football rules: " + msg) {}
};

/**
 * @class PBCStorageException
 * @brief An exception that is thrown when something went wrong with the storage
 * component.
 */
class PBCStorageException : public PBCException {
 public:
    explicit PBCStorageException(const std::string& msg = "") :
        PBCException("Error while executing storage operation: " + msg) {}
};

/**
 * @class PBCDecryptionException
 * @brief An exception that is thrown when something went wrong with the decryption.
 */
class PBCDecryptionException : public PBCException {
 public:
    explicit PBCDecryptionException(const std::string& msg = "") :
        PBCException("Error while decrypting file: " + msg) {}
};

#endif  // PBCEXCEPTIONS_H
