#include "link.h"

struct MyLink2 {
    uint16_t anchor_addr;
    float range;
    float dbm;
};

struct globalLink{
  struct MyLink2 anchor81;
  struct MyLink2 anchor82;
  struct MyLink2 anchor83;
};

struct globalLink fresh_link2(struct globalLink global, uint16_t addr, float range, float dbm)
{
#ifdef SERIAL_DEBUG
    Serial.println("fresh_link");
#endif

  struct globalLink tmp = global;

    if(addr == 129){

      tmp.anchor81.range = range;
      tmp.anchor81.dbm = dbm;

    }else if(addr == 130){

      tmp.anchor82.range = range;
      tmp.anchor82.dbm = dbm;

    }else if(addr == 131){

      tmp.anchor83.range = range;
      tmp.anchor83.dbm = dbm;

    }

    return tmp;
  
}




