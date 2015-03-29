//
//  JSON.cpp
//  XJSON
//
//  Created by minjie on 3/26/15.
//  Copyright (c) 2015 minjie. All rights reserved.
//

#include "JSON.h"

/*
 * @access private
 */
JSON::JSON() {
}


/*
 * Parse  a JSON encoded string(Turing a char string to wchar_t string)
 * @access public
 * @param char* data, the JSON text
 * @return JSONValue*, a JSON Value, the root, or NULL|error
 */
JSONValue* JSON::Parse(const char *data) {
    size_t length = strlen(data) + 1;
    wchar_t *w_data = (wchar_t*)malloc(length * sizeof(wchar_t));
    
#if defined(WIN32) && !defined(__GUNC__)
    size_t ret_value = 0;
    if (mbstowcs_t(&ret_value, w_data, length, data, length) != 0) {
        free(w_data);
        return NULL:
    }
#else
    if (mbstowcs(w_data, data, length) == (size_t)-1) {
        free(w_data);
        return NULL;
    }
#endif
    JSONValue *value = JSON::Parse(w_data);
    free(w_data);
    return value;
}

/*
 * Parse a JSON encoded string
 * @access public
 * @param wchar_t* data
 * @return JSONValue*
 */
#include <iostream>
JSONValue* JSON::Parse(const wchar_t *data) {
    if (!SkipWhitespace(&data)) {
        return NULL;
    }
    
    JSONValue *value = JSONValue::Parse(&data);
    if (value == NULL) {
        return NULL;
    }
    if (SkipWhitespace(&data)) {
        delete value;
        return NULL;
    }
    
    return value;
}



/*
 * Skip proceeding whitespace characters
 * @access protected
 * @param wchar_t**, a pointer to a wchar_t* of a JSON text
 * @return bool, if there is more data
 */
bool JSON::SkipWhitespace(const wchar_t **data) {
    while (**data != 0 && (**data == L' ' || **data == L'\r' || **data == L'\n' || **data == L'\t')) {
        (*data) ++;
    }
    return **data != 0;
}


/*
 * Get the next element, key or value
 * @access protected
 * @param wchar_t**, pointer of the wchar_t*; wstring holds the answer
 * @return bool, success or failure
 */
bool JSON::ExtractString(const wchar_t** data, std::wstring &str) {
    // first a empty string
    str = L"";
    
    while (**data != 0) {
        wchar_t now = **data;
        
        // escape character
        if (now == L'\\') {
            (*data) ++;
            switch (**data) {
                case L'\\': now = L'\\'; break;
                case L'"': now = L'"'; break;
                case L'/': now = L'/'; break;
                case L't': now = L'\t'; break;
                case L'n': now = L'\n'; break;
                case L'b': now = L'\b'; break;
                case L'f': now = L'\f'; break;
                case L'r': now = L'\r'; break;
                case L'u': {
                    if (!hasLength(*data, 5)) {
                        return false;
                    }
                    
                    // calculate this hex
                    now = 0;
                    for (int i = 0;i < 4; ++i) {
                        (*data) ++;
                        now <<= 4;
                        if (**data >= L'0' && **data <= L'9') {
                            now |= **data - '0';
                        } else if (**data >= L'a' && **data <= L'z') {
                            now |= (**data - 'a' + 10);
                        } else if (**data >= L'A' && **data <= L'Z') {
                            now |= (**data - 'A' + 10);
                        } else {
                            // not legal hex
                            return false;
                        }
                    }
                    break;
                }
                default:
                    return false;
                    
            }
            
        } else if (now < L' ' && now != L'\t') {
            // not allowed, invisible characters
            return false;
        } else if (now == L'"') {
            // end of the element
            (*data) ++;
            str.reserve();
            return true;
        }
        
        str += now;
        (*data) ++;
        //std::wcout << str << std::endl;
    }
    
    
    return false;
}


/*
 * @access protected
 * Parse wchar_t** to int number
 */
double JSON::ParseInt(const wchar_t **data) {
    double res = 0.0;
    while (**data != 0 && **data >= L'0' && **data <= L'9') {
        res = res * 10 + (*(*data)++ - '0');
    }
    return res;
}


/*
 * @access protected
 * Parse wchar_t** to decimal number
 */
double JSON::ParseDecimal(const wchar_t **data) {
    double res = 0.0;
    double base = 0.1;
    while (**data != 0 && **data >= '0' && **data <= '9') {
        int tmp = (*(*data)++ - '0');
        res += base * tmp;
        base *= 0.1;
    }
    return res;
}

