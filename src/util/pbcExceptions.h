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
