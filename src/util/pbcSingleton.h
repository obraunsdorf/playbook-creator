#ifndef PBCSINGLETON_H
#define PBCSINGLETON_H

#include <stddef.h>
#include <assert.h>

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
