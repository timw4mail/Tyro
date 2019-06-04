#pragma once

#include "src/common.h"

class TyroConfig {
public:
    TyroConfig();
    ~TyroConfig();
    JsonValue GetRoot();
	void LoadJson(const char json[]);
private:
    JsonValue default_root;
    Json::Reader reader;
};
