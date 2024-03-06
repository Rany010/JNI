#ifndef CPPINTERFACE_H
#define CPPINTERFACE_H
#include <iostream>
#include <list>

#include "cppInterface_global.h"

struct BodyIndex{
    bool sex;
    float height;
    double wight;
};

struct Params{
    int id;
    std::string name;
    std::list<BodyIndex> body;
};

class CPPINTERFACE_EXPORT CppInterface
{
public:
    CppInterface();
};

#endif // CPPINTERFACE_H
