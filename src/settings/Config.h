#ifndef TYRO_CONFIG_H
#define	TYRO_CONFIG_H

#include "src/common.h"

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

