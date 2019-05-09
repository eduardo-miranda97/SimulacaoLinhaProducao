#ifndef ARTES
#define  ARTES

#include <cstdlib>

enum state_art{
    OCIOSITY=0, ACTIVE
};

class Artesao{
public:

private:
  u_int8_t  id;
  state_art situation;
  u_int32_t time_ociosity;
  u_int32_t start_ociosity;
};

#endif
