//
//  demo1.cpp
//  XJSON
//
//  Created by minjie on 3/29/15.
//  Copyright (c) 2015 minjie. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>


#include "../XJSON/JSON.h"

using namespace std;

int main() {
    // deal with basic data type
    const wchar_t* str = L" \
    { \
        \"null_name\": null, \
        \"string_name\": \"string_value\", \
        \"number_name\": 110, \
        \"bool_name\": true \
    }";

    JSONValue *basic = JSON::Parse(str);

    
    // print the json
    wcout << basic->Stringify() << endl;

    // print the json without indent
    wcout << basic->Stringify(false) << endl;

    // get the root
    JSONObject root = basic->getObject();

    // put new key-value
    root[L"new_string"] = new JSONValue("world");	// string
    root[L"new_bool"] = new JSONValue(false);		// bool
    root[L"new_number"] = new JSONValue(123.45);	// number
    root[L"new_null"] = new JSONValue();		// null
    

    basic = new JSONValue(root);

    wcout << basic->Stringify() << endl;

    delete basic;
}
