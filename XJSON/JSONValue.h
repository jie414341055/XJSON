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

enum JSONType { JSONType_null, JSONType_string, JSONType_bool, JSONType_number, JSONType_array, JSONType_object };

class JSONValue {
    friend class JSON;
    
public:
    JSONValue();
    JSONValue(const wchar_t *value);
    JSONValue(const std::wstring &value);
    JSONValue(bool value);
    JSONValue(double value);
    JSONValue(const JSONArray &value);
    JSONValue(const JSONObject &value);
    
    ~JSONValue();
    
    bool isNull() const;
    bool isString() const;
    bool isBool() const;
    bool isNumber() const;
    bool isArray() const;
    bool isObject() const;
    
    const std::wstring &getString() const;
    bool getBool() const;
    double getNumber() const;
    const JSONArray &getArray() const;
    const JSONObject &getObject() const;
    
    std::size_t numOfChildren() const;
    bool hasChild(std::size_t index) const;
    JSONValue *getChild(std::size_t index);
    bool hasChild(const wchar_t *name) const;
    JSONValue *getChild(const wchar_t *name);
    std::vector<std::wstring> objectKeys() const;
    
    std::wstring Stringify(bool const prettyindent = true);
    
protected:
    static JSONValue *Parse(const wchar_t **data);
    
private:
    JSONType type;
    std::wstring string_value;
    bool bool_value;
    double number_value;
    JSONArray array_value;
    JSONObject object_value;
    
    std::wstring StringifyImpl(size_t const indentDepth) const;
    static std::wstring Indent(size_t indentDepth);
    static std::wstring printString(const std::wstring &str);
    
};

#endif
