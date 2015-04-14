/* 
 * File:   Settings.h
 * Author: twarren
 *
 * Created on April 13, 2015, 4:18 PM
 */

#ifndef SETTINGS_H
#define	SETTINGS_H

#include "../common.h"
#include "../../include/json/json.h"

class TyroSettings {
public:
    TyroSettings();
    ~TyroSettings();
    Json::Value GetRoot();
private:
    Json::Value default_root;
    Json::Reader reader;
};


#endif	/* SETTINGS_H */

