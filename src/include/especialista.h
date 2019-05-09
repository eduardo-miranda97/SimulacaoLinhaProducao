#ifndef ESPEC
#define  ESPEC

#include <cstdlib>

enum state_esp{
    OCIOSITY=0, ACTIVE
};

class Especialista{
public:

private:
  u_int8_t  id;
  state_esp situation;
  u_int32_t time_ociosity;
  u_int32_t start_ociosity;
};

#endif
