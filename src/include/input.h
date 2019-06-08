#ifndef INPUT
#define  INPUT

#include <fstream>
#include <string>
#include <vector>

#include "simulacao.h"

const std::vector<std::string> split(const std::string& s, const char& c);

void input_args(const std::string& path);

void set_parameters(const std::vector<std::string>& tokens);

#endif
