#ifndef INPUT
#define  INPUT

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "simulacao.h"
#include "especialista.h"
#include "artesao.h"
#include "vaso.h"

using namespace std;

const vector<string> split(const string& s, const char& c);

void input_args(const string& path);

void set_parameters(const vector<string>& tokens);

#endif
