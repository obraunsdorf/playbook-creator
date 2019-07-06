/** @file pbcSingleton.h
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

#ifndef PBCSINGLETON_H
#define PBCSINGLETON_H

#include <stddef.h>

/**
 * @class PBCSingleton
 * @brief A generic class that enforces the singleton pattern for every class
 * that inherits PBCSingleton.
 *
 * A class C has to inherit PBCSingleton<C> and must have a protected
 * constructor. Then it is enforced to have only one instance in the whole
 * application. For more information on the singleton pattern see
 * https://en.wikipedia.org/wiki/Singleton_pattern
 */
template <class T>
class PBCSingleton {
 private:
    static T* _instance;
    PBCSingleton(const PBCSingleton& obj) {}

 protected:
    PBCSingleton() {}
    ~PBCSingleton() {}

 public:
    static T* getInstance() {
        if(_instance == NULL) {
            _instance = new T();
        }
        return _instance;
    }
};

template <class T> T* PBCSingleton<T>::_instance = NULL;

#endif  // PBCSINGLETON_H
