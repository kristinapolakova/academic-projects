#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

#include "recipes.h"

class IRecipeParser
{
public:
    virtual void parse(std::string filename, std::vector<Recipe> &recipes) = 0;
    virtual ~IRecipeParser() {}
};

class TextRecipeParser : public IRecipeParser
{
public:
    void parse(std::string filename, std::vector<Recipe> &recipes) override;
};

#endif // PARSER_H