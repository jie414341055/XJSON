//
//  demo2.cpp
//  XJSON
//
//  Created by minjie on 3/30/15.
//  Copyright (c) 2015 minjie. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>


#include "../XJSON/JSON.h"

using namespace std;

int main() {
    const wchar_t* str = L" \
    { \
    \"obj_name\": { \"key1\": \"value1\", \"key2\": 100, \"key3\": true, \"key4\": null }, \
    \"arr_name\": [ { \"1\": 123 }, { \"2\": null }, { \"3\": true } ] \
    }";

    
    JSONValue *advance = JSON::Parse(str);
    
    wcout << advance->Stringify() << endl;
    
    JSONObject root = advance->getObject();
    
    // append new object element
    JSONObject obj;
    obj[L"hello"] = new JSONValue("world");
    obj[L"recur"] = advance;
    
    root[L"new_obj"] = new JSONValue(obj);
    
    advance = new JSONValue(root);
    
    wcout << advance->Stringify() << endl;
    
    // append new array element
    JSONArray arr;
    arr.push_back(new JSONValue(100.0));
    arr.push_back(new JSONValue(false));
    
    root[L"new_arr"] = new JSONValue(arr);
    
    advance = new JSONValue(root);
    
    wcout << advance->Stringify() << endl;
    
}