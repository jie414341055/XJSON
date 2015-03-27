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
        
    }
}
