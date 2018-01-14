#ifndef __PERSISTENT__
#define __PERSISTENT__

#include <EEPROM.h>

class classPersistent {
    private:
        int next_address;

    public:
        classPersistent() {
            EEPROM.begin (128);
            next_address = 0;
        }

        int allocate (int size) {
            int n = next_address;
            next_address += size;
            return n;
        }

        void store (int address, void* pdata, int size) {
            unsigned char *p = (unsigned char*) pdata;
            for (int n = 0; n < size; n++) {
                EEPROM.write(address++, p[n]);
            }
            EEPROM.commit();
        }

        void fetch (int address, void* pdata, int size) {
            unsigned char *p = (unsigned char*) pdata;
            for (int n = 0; n < size; n++) {
                p[n] = EEPROM.read(address++);
            }
        }
};

extern classPersistent Persistent;

#endif
