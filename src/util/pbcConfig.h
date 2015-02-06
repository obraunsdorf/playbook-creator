#ifndef PBCCONFIG_H
#define PBCCONFIG_H

#include <stddef.h>

class PBCConfig {
private:
    static PBCConfig* instance = NULL;

protected:
    PBCConfig();

public:
    static PBCConfig* getInstance() {
        if(instance == NULL) {
            instance = new PBCConfig();
        }
        return instance;
    }

};

#endif // PBCCONFIG_H
