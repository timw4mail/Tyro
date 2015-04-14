#ifndef TYRO_CONFIG_H
#define	TYRO_CONFIG_H

#include "../common.h"
#include "../../include/json/json.h"

typedef Json::Value JsonValue;
typedef Json::Reader JsonReader;
typedef Json::Writer JsonWriter;

class TyroConfig {
public:
    TyroConfig();
    ~TyroConfig();
    JsonValue GetRoot();
	JsonValue GetLang(string name);
private:
    JsonValue default_root;
    JsonReader reader;
};


#endif	/* TYRO_CONFIG_H */

