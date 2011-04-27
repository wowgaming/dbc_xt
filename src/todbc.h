#pragma once
#include "stdafx.h"
#include "common.h"
#include "database.h"

class todbc
{
public:
    todbc(string fileName);
    ~todbc();

    bool convert();

private:
    std::ofstream csvOutput;
    std::string fName;
};

