#ifndef RUN_H
#define RUN_H
#include "MovieManager.h"

#include <iostream>

// Declaration of the run function
void run(std::istream &input, std::ostream &output);
void addFunction(std::istringstream &input, MovieManager &manager);
void recommendFunction(std::istringstream &input, MovieManager &manager, std::ostream &output);
void helpFunction(std::ostream &output);

#endif