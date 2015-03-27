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

}
