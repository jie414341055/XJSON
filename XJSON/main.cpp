//
//  main.cpp
//  XJSON
//
//  Created by minjie on 3/26/15.
//  Copyright (c) 2015 minjie. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include "JSON.h"

JSONValue *json;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::wstring str, line;

    std::wifstream in;
    in.open("/Users/user/tmp/Xcode/XJSON/XJSON/json_example.txt");
    while (std::getline(in, line)) {
        //std::wcout << line << std::endl;

        str += line;
    }
    //std::wcout << str << std::endl;
    json = JSON::Parse(str.c_str());
    std::cout << json->getType();
    std::wcout << json->Stringify() << std::endl;
    return 0;
}
