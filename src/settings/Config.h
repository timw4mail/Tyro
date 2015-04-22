#ifndef TYRO_CONFIG_H
#define	TYRO_CONFIG_H

#include "../common.h"
#include <json/json.h>

typedef Json::Value JsonValue;
typedef Json::Reader JsonReader;
typedef Json::Writer JsonWriter;

class TyroConfig {
public:
    TyroConfig();
    ~TyroConfig();
    JsonValue GetRoot();
	void LoadJson(const char json[]);
private:
    JsonValue default_root;
    JsonReader reader;
};


#endif	/* TYRO_CONFIG_H */

