/**
 * A common header for global includes
 */
#ifndef TYRO_COMMON_H
#define TYRO_COMMON_H

// C++ Standard Lib includes
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

using namespace std;

// JSON 
#include <json/json.h>

typedef Json::Value JsonValue;
typedef Json::Reader JsonReader;
typedef Json::Writer JsonWriter;

#endif	// TYRO_COMMON_H

