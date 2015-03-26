//
//  JSON.h
//  XJSON
//
//  Created by minjie on 3/26/15.
//  Copyright (c) 2015 minjie. All rights reserved.
//

#ifndef XJSON_JSON_h
#define XJSON_JSON_h

#include <vector>
#include <string>
#include <map>
#include <stdlib.h>

// For Linux Compile
#ifdef __GUNC__
    #include <cstring>
    #include <cstdlib>
#endif


// make sure a string has at least n characters
static inline bool hasLength(const wchar_t *data, int n) {
    // empty pointer
    if (data == 0) return false;
    
    const wchar_t *tmp = data;
    while (n --) {
        if (*(tmp++) == 0) return false;
    }
    return true;
}

// XJSON Type
class JSONValue;
typedef std::vector<JSONValue*> JSONArray;
typedef std::map<std::wstring, JSONValue*> JSONObject;

#include "JSONValue.h"

class JSON {
    friend class JSONValue;
    
public:
    static JSONValue* Parse(const char *data);
    static JSONValue* Parse(const wchar_t *data);
    static std::wstring Stringify(const JSONValue *value);

protected:
    static bool SkipWhitespace(const wchar_t  **data);
    static bool ExtractString(const wchar_t **data, std::wstring &str);
    static double ParseInt(const wchar_t **data);
    static double ParseDecimal(const wchar_t **data);
    
private:
    JSON();

};

#endif
