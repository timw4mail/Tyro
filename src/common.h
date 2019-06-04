/**
 * A common header for global includes
 */
#pragma once

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

// Typedef some common templates
typedef map<string, int> StringConstMap;
typedef map<string, string> StringMap;
