//
//  JSONValue.cpp
//  XJSON
//
//  Created by minjie on 3/26/15.
//  Copyright (c) 2015 minjie. All rights reserved.
//

#include "JSONValue.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

#define FREE_OBJECT(x) { JSONObject::iterator itr; for (itr  = x.begin(); itr != x.end(); ++itr) { delete (*itr).second; } }
#define FREE_ARRAY(x) { JSONArray::iterator itr; for (itr = x.begin(); itr != x.end(); ++itr) { delete *itr; } }


JSONValue *JSONValue::Parse(const wchar_t **data) {
    // string
    if (**data == '"') {
        std::wstring str;
        if (!JSON::ExtractString(&(++(*data)), str)) {
            return NULL;
        } else {
            return new JSONValue(str);
        }
        
    // boolean
    } else if ((hasLength(*data, 4) && wcsncasecmp(*data, L"true", 4) == 0) || (hasLength(*data, 5) && wcsncasecmp(*data, L"false", 5) == 0)) {
        bool value = wcsncasecmp(*data, L"true", 4) == 0;
        (*data) += value ? 4 : 5;
        return new JSONValue(value);
        
    // null
    } else if (hasLength(*data, 4) && wcsncasecmp(*data, L"null", 4) == 0) {
        (*data) += 4;
        return new JSONValue();
        
    // number
    } else if (**data == L'-' || (**data >= L'0' && **data <= L'9')) {
        // negative
        bool neg = **data == L'-';
        if (neg) (*data) ++;
        
        double value = 0.0;
        
        if (**data == L'0') {
            (*data) ++;
        } else if (**data > L'0' && **data <= L'9') {
            value = JSON::ParseInt(data);
        } else {
            return NULL;
        }
        
        // if it's a decimal
        if (**data == L'.') {
            (*data) ++;
            if (**data >= L'0' && **data <= L'9') {
                value += JSON::ParseDecimal(data);
            } else {
                return NULL;
            }
        }
        
        // if it's a exponent
        if (**data == L'e' || **data == L'E') {
            (*data) ++;
            
            bool neg_exp = false;
            
            // if it's a negative exp
            if (**data == L'-') {
                neg_exp = true;
                (*data) ++;
            }
            
            // is it legal?
            if (!(**data >= L'0' && **data <= L'9')) {
                return NULL;
            }
            
            double exp = JSON::ParseInt(data);
            
            for (double i = 0.0;i < exp; ++i) {
                value = neg_exp ? (value / 10.0) : (value * 10.0);
            }
            
            value *= neg_exp ? -1 : 1;
            
            return new JSONValue(value);
            
        }
    // object
    } else if (**data == L'{') {
        JSONObject obj;
        
        (*data) ++;
        
        while (**data != 0) {
            // skip whitespace
            if (!JSON::SkipWhitespace(data)) {
                FREE_OBJECT(obj);
                return NULL;
            }
            
            // empty object
            if (obj.size() == 0 && (**data) == L'}') {
                (*data) ++;
                return new JSONValue(obj);
            }
            
            // now should be a string
            if (**data != L'"') {
                FREE_OBJECT(obj);
                return NULL;
            }
            (*data) ++;
            
            std::wstring key;
            if (!JSON::ExtractString(data, key)) {
                FREE_OBJECT(obj);
                return NULL;
            }
            
            // skip whitespace
            if (!JSON::SkipWhitespace(data)) {
                FREE_OBJECT(obj);
                return NULL;
            }
            
            // now should be a ':'
            if (**data != L':') {
                FREE_OBJECT(obj);
                return NULL;
            }
            (*data) ++;
            
            // skip whitespace
            if (!JSON::SkipWhitespace(data)) {
                FREE_OBJECT(obj);
                return NULL;
            }
            
            JSONValue *value = Parse(data);
            
            // key:value
            if (obj.find(key) != obj.end()) delete obj[key];
            obj[key] = value;
            
            // skip whitespace
            if (!JSON::SkipWhitespace(data)) {
                FREE_OBJECT(obj);
                return NULL;
            }
            
            // if this is the end...
            if (**data == L'}') {
                (*data) ++;
                return new JSONValue(obj);
            }
            
            // if it's not over yet...
            if (**data == L',') {
                (*data) ++;
            } else {
                FREE_OBJECT(obj);
                return NULL;
            }
        }
        
    // a array
    } else if (**data == L'[') {
        JSONArray arr;
    
        (*data) ++;
    
        while (**data != 0) {
            // skip whitespace
            if (!JSON::SkipWhitespace(data)) {
                FREE_ARRAY(arr);
                return NULL;
            }
            
            // empty array
            if (arr.size() == 0 && **data == L']') {
                (*data) ++;
                return new JSONValue(arr);
            }
            
            JSONValue *value = Parse(data);
            
            if (value == NULL) {
                FREE_ARRAY(arr);
                return NULL;
            }
            
            arr.push_back(value);
            
            // skip whitespace
            if (!JSON::SkipWhitespace(data)) {
                FREE_ARRAY(arr);
                return NULL;
            }
            
            // if this is the end...
            if (**data == L']') {
                (*data) ++;
                return new JSONValue(arr);
            }
            
            // should be a ','
            if (**data != L',') {
                FREE_ARRAY(arr);
                return NULL;
            }
            
            (*data) ++;
            
        }
        
        FREE_ARRAY(arr);
        return NULL;
    
    } else {
        // don't know that type...
        return NULL;
    }
    return NULL;
}

/*
 * @access public
 * constructor
 */
JSONValue::JSONValue() {
    type = JSONType_null;
}

JSONValue::JSONValue(const std::wstring &value) {
    type = JSONType_string;
    string_value = value;
}

JSONValue::JSONValue(const wchar_t *value) {
    type = JSONType_string;
    string_value = std::wstring(value);
}

JSONValue::JSONValue(bool value) {
    type = JSONType_bool;
    bool_value = value;
}

JSONValue::JSONValue(double value) {
    type = JSONType_number;
    number_value = value;
}

JSONValue::JSONValue(const JSONArray &value) {
    type = JSONType_array;
    array_value = value;
}

JSONValue::JSONValue(const JSONObject &value) {
    type = JSONType_object;
    object_value = value;
}

/*
 * @access public
 * destructor
 */
JSONValue::~JSONValue() {
    if (type == JSONType_object) {
        FREE_OBJECT(object_value);
    } else if (type == JSONType_array) {
        FREE_ARRAY(array_value);
    }
}

/*
 * if this is a null value
 */
bool JSONValue::isNull() const {
    return type == JSONType_null;
}

/*
 * if this is a bool value
 */
bool JSONValue::isBool() const {
    return type == JSONType_bool;
}

/*
 * if this is a number value
 */
bool JSONValue::isNumber() const {
    return type == JSONType_number;
}

/*
 * if this is a array value
 */
bool JSONValue::isArray() const {
    return type == JSONType_array;
}

/*
 * if this is a object value
 */
bool JSONValue::isObject() const {
    return type == JSONType_object;
}


/*
 * get string value, should use isString first
 * @access public
 */
const std::wstring &JSONValue::getString() const {
    if (type != JSONType_string) {
        throw "The value is not a string type!";
    }
    return string_value;
}

/*
 * get bool value, should use isBool first
 * @access public
 */
bool JSONValue::getBool() const {
    if (type != JSONType_bool) {
        throw "The value is not a bool type!";
    }
    return bool_value;
}

/*
 * get number value, should use isNumber first
 * @access public
 */
double JSONValue::getNumber() const {
    if (type != JSONType_bool) {
        throw "The value is not a number type!";
    }
    return number_value;

}


/*
 * get array value, should use isArray first
 * @access public
 */
const JSONArray &JSONValue::getArray() const {
    if (type != JSONType_array) {
        throw "The value is not a array type!";
    }
    return array_value;
}

/*
 * get object value, should use isObject first
 * @access public
 */
const JSONObject &JSONValue::getObject() const {
    if (type != JSONType_object) {
        throw "The value is not a object type!";
    }
    return object_value;
}

/*
 * @access public
 * return the number of children
 */
std::size_t JSONValue::numOfChildren() const {
    if (type == JSONType_object) {
        return object_value.size();
    } else if (type == JSONType_array) {
        return array_value.size();
    } else {
        return 0;
    }
}

/*
 * @access public
 * check child of array
 */
bool JSONValue::hasChild(std::size_t index) const {
    if (type == JSONType_array && index < array_value.size()) {
        return true;
    }
    return false;
}

/*
 * @access public
 * get child of array
 */
JSONValue* JSONValue::getChild(std::size_t index) {
    if (type == JSONType_array && index < array_value.size()) {
        return array_value[index];
    }
    return NULL;
}


/*
 * @access public
 * check child of object
 */
bool JSONValue::hasChild(const wchar_t *name) const {
    if (type == JSONType_object && object_value.find(name) != object_value.end()) {
        return true;
    }
    return false;
}

/*
 * @access public
 * get child of object
 */
JSONValue* JSONValue::getChild(const wchar_t *name) {
    //if (type == JSONType_object && object_value.find(name) != object_value.end()) {
    //    return object_value[name];
    //}
    JSONObject::iterator itr = object_value.find(name);
    if (itr != object_value.end()) {
        return itr->second;
    }
    return NULL;
}


/*
 * @access public
 * if the type is object, return the keys, or empty vector
 */
std::vector<std::wstring> JSONValue::objectKeys() const {
    std::vector<std::wstring> vec;
    if (type != JSONType_object) return vec;
    
    JSONObject::const_iterator itr = object_value.begin();
    for (; itr != object_value.end(); ++itr) {
        vec.push_back(itr->first);
    }
    return vec;
}


/*
 * @access public
 * @return wstring, the JSON string
 */
std::wstring JSONValue::Stringify(bool const prettyindent) {
    size_t const indentDepth = prettyindent ? 1 : 0;
    return StringifyImpl(indentDepth);
}

/*
 * @access private
 * @return wstring, the JSON string
 */

std::wstring JSONValue::StringifyImpl(const size_t indentDepth) const {
    std::wstring result;
    
    // indent spaces
    size_t depth = indentDepth ? indentDepth + 1 : 0;
    std::wstring s1 = Indent(indentDepth);
    std::wstring s2 = Indent(depth);
    
    switch (type) {
        // null
        case JSONType_null: {
            result = L"null";
            break;
        }
        case JSONType_bool: {
            result = bool_value ? L"true" : L"false";
            break;
        }
        case JSONType_number: {
            if (isinf(number_value) || isnan(number_value)) {
                result = L"null";
            } else {
                std::wstringstream ss;
                ss.precision(10);
                ss << number_value;
                
                result = ss.str();
            }
            break;
        }
        case JSONType_string: {
            result = printString(string_value);
            break;
        }
        case JSONType_array: {
            // indent first
            result = indentDepth ? L"[\n" + s2 : L"[";
            JSONArray::const_iterator itr = array_value.begin();
            
            while (itr != array_value.end()) {
                result += (*itr)->StringifyImpl(depth);
                
                if (++itr != array_value.end()) {
                    result += L", ";
                }
            }
            result += indentDepth ? L"\n" + s1 + L"]" : L"]";
            break;
        }
        case JSONType_object: {
            // indent first
            result = indentDepth ? L"{\n" + s2 : L"{";
            JSONObject::const_iterator itr = object_value.begin();
            
            while (itr != object_value.end()) {
                result += printString((*itr).first);
                result += L":";
                result += (*itr).second->StringifyImpl(depth);
                
                
                if (++itr != object_value.end()) {
                    result += indentDepth ? L",\n" + s2 : L", ";
                }
            }
            result += indentDepth ? L"\n" + s1 + L"}" : L"}";
            break;
        }
            
    }
    return result;
}


/*
 * @access private
 * print string value to string(including escaped characters)
 * @return wstring
 */
std::wstring JSONValue::printString(const std::wstring &str) {
    std::wstring result = L"\"";
    
    sd::wstring::const_iterator iter = str.begin();
    
    while (iter != str.end()) {
        wchar_t c = *iter;
        
        if (c == L'\\' || c == L'"' || c == L'/') {
            
            result += L'\\';
            result += c;
            
        } else if (c == L'\n') {
            
            result += L"\\n";
            
        } else if (c == L'\f') {
            
            result += L"\\f";
            
        } else if (c == L'\t') {
            
            result += L"\\t";
            
        } else if (c == L'\b') {
            
            result += L"\\b";
            
        } else if (c == L'\r') {
            
            result += L"\\r";
            
        } else if (c < L' ' || c > 126) {
            // need escape
            result += L"\\u";
            
            for (int i = 0;i < 4; ++i) {
                // get the left four bits
                int tmp = (c >> 12) & 0xf;
                if (tmp >= 0 && tmp <= 9) {
                    result += (wchar_t)(tmp + '0');
                } else {
                    result += (wchar_t)(tmp - 10 + 'A');
                }
            }
            c <<= 4;
            
        } else {
            result += c;
        }
        
        ++ iter;
    }
    
    
    
    result += L"\"";
    return result;
}

/*
 * @access private
 * @return indent spaces
 */
std::wstring JSONValue::Indent(size_t indentDepth) {
    std::wstring spaces(indentDepth * 2, L' ');
    return spaces;
}


