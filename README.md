#XJSON
XJSON is a JSON Parse for decoding and encoding JSON in C++.

##Examples

-   Create a JSON
    
    ```cpp
    const wchar_t* str = L" \
    { \
        \"null_name\": null, \
        \"string_name\": \"string_value\", \
        \"number_name\": 110, \
        \"bool_name\": true \
    }";
    JSONValue *basic = JSON::Parse(str);
    JSONObject obj = basic->getObject();
    ```
-   Append new item

    ```cpp
    obj[L"new_string"] = new JSONValue("world");	// string
    obj[L"new_bool"] = new JSONValue(false);		// bool
    obj[L"new_number"] = new JSONValue(123.45);	// number
    obj[L"new_null"] = new JSONValue();		// null
    basic = new JSONValue(root);    // now basic has 4 more (key-value)s
    // print JSON
    std::wcout << basic->Stringify() << std::endl;
    // print JSON without indent
    std::wcout << basic->Stringify(false) << std::endl;
    ```
-   Deal with array and object

    ```cpp
    const wchar_t* str = L" \
    { \
    \"obj_name\": { \"key1\": \"value1\", \"key2\": 100, \"key3\": true, \"key4\": null }, \
    \"arr_name\": [ { \"1\": 123 }, { \"2\": null }, { \"3\": true } ] \
    }";
    JSONValue *advance = JSON::Parse(str); 
    JSONObject obj = advance->getObject();
    // append new object element
    JSONObject obj2;
    obj2[L"hello"] = new JSONValue("world");
    obj2[L"json"] = new JSONValue(true);
    obj[L"new_obj"] = new JSONValue(obj2);
    advance = new JSONValue(root);
    
    // append new array element
    JSONArray arr;
    arr.push_back(new JSONValue(100.0));
    arr.push_back(new JSONValue("There is my cat."));
    root[L"new_arr"] = new JSONValue(arr);
    advance = new JSONValue(root);

    ```
