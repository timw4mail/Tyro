#pragma once

#include "src/common.h"

class TyroConfig {
public:
    TyroConfig();
    ~TyroConfig();
    Json::Value GetRoot();
	void LoadJson(const char json[]);
private:
    Json::Value default_root;
    Json::Reader reader;
};
