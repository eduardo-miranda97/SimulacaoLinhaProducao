#ifndef ARTES
#define  ARTES

#include <cstdlib>

enum state_art{
    OCIOSITY=0, ACTIVE
};

extern u_int16_t last_id_arts;

class Artesao{
  public:
    /* CONSTRUCTOR */
    Artesao(u_int8_t id, state_art situation,
            u_int32_t time_ociosity, u_int32_t start_ociosity);
    /* SET METHODS */
    void set_id(u_int8_t id);
    void set_situation(state_art situation);
    void set_time_ociosity (u_int32_t time_ociosity);
    void set_start_ociosity(u_int32_t start_ociosity);
    /* GET METHODS */
    u_int8_t get_id();
    state_art get_situation();
    u_int32_t get_time_ociosity ();
    u_int32_t get_start_ociosity();
  private:
    u_int8_t id;               // ID do Artesão.
    state_art situation;        // Situação do Artesão.
    u_int32_t time_ociosity;    // Tempo acumulado ocioso.
    u_int32_t start_ociosity;   // Tempo de inicio de um periodo ocioso.
};

#endif
