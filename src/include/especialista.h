#ifndef ESPEC
#define  ESPEC

#include <cstdlib>

enum state_esp{
    OCIOSITY=0, ACTIVE
};

class Especialista{
  public:
    /* CONSTRUCTOR*/
    Especialista(u_int8_t  id, state_esp situation,
                u_int32_t time_ociosity, u_int32_t start_ociosity);
    /* SET METHODS */
    void set_id(u_int8_t id);
    void set_situation(state_esp situation);
    void set_time_ociosity (u_int32_t time_ociosity);
    void set_start_ociosity(u_int32_t start_ociosity);
    /* GET METHODS */
    u_int8_t  get_id();
    state_esp get_situation();
    u_int32_t get_time_ociosity ();
    u_int32_t get_start_ociosity();
  private:
    u_int8_t  id;
    state_esp situation;
    u_int32_t time_ociosity;
    u_int32_t start_ociosity;
};

#endif
