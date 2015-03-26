//
//  JSONValue.h
//  XJSON
//
//  Created by minjie on 3/26/15.
//  Copyright (c) 2015 minjie. All rights reserved.
//

#ifndef XJSON_JSONValue_h
#define XJSON_JSONValue_h

#include <vector>
#include <string>

#include "JSON.h"

class JSON;

class JSONValue {
    friend class JSON;
    
public:
    JSONValue();
    JSONValue(const wchar_t *value);
    JSONValue(const std::wstring &value);
    JSONValue(const bool value);
    JSONValue(const double value);
    JSONValue(const JSONArray &value);
    JSONValue(const JSONObject &value);
    
    ~JSONValue();
    
    bool isNull() const;
    bool isString() const;
    bool isBool() const;
    bool isNumber() const;
    bool isArray() const;
    bool isObject() const;
    
    const std::wstring &asString() const;
    bool asBool() const;
    double asNumber() const;
    const JSONArray &asArray() const;
    const JSONObject &asObject() const;
    
    std::size_t numOfChildren() const;
    bool hasChild(std::size_t index) const;
    JSONValue *child(std::size_t index);
    bool hasChild(const wchar_t *name) const;
    JSONValue *child(const wchar_t *name);
    std::vector<std::wstring> objectKeys() const;
    
    std::wstring Stringify(bool const prettyindent = true);
    
protected:
    static JSONValue *Parse(const wchar_t **data);
    
private:
    
    
};

#endif
