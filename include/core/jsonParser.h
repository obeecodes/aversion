#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <stack>
#include <set>

typedef uint32_t Uint32;

enum JSONTokenType{
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    STRING,
    NUMBER,
    BOOL,
    NULL_VALUE,
    KEY,
    JVALUE,
    COLON,
    JCOMMA,
    JMINUS,
};

struct JSONObject;

using JSONValue = std::variant<
    std::string,
    Uint32,
    bool,
    double,
    std::nullptr_t,
    JSONObject*,
    std::vector<JSONObject*>,
    std::vector<std::string>,
    std::vector<float>
>;

using JSONMap = std::unordered_map<
    std::string, std::unordered_map<
        std::string, std::unordered_map<
            std::string, JSONValue
        >
    >
>;

struct JSONObject {
    std::unordered_map<std::string, JSONValue> value;
    
    JSONValue& operator[](const std::string& key) {
        return value[key];
    }

    // Const version of the subscript operator for accessing values by key in const objects
    const JSONValue& operator[](const std::string& key) const {
        return value.at(key);
    }
    
    JSONObject() = default;
};

struct Value{
    bool vector;
    bool string;
    bool uint;
    bool floating;

    std::vector<Uint32> ints;
    std::vector<double> doubles;
    std::string text;
    Uint32 number;
    Uint32 floatingNumber;

    Value() : vector(false), string(false), uint(false), floating(false) {}
};

struct JSONToken{
    std::string value;
    JSONTokenType type;
    JSONToken() : value(""), type(){};
    JSONToken(std::string value, JSONTokenType type) : value(value), type(type){}
};

class JSONParser{
public:
    static JSONParser* Instance();
    void initJSONParser(std::string file);
    JSONObject getJSONObject();
    void print();

    std::set<std::string> getEntities();
    JSONMap getJSONMap();

private:
    JSONParser();
    static JSONParser* instance;

    JSONObject json;
    std::string key; 


    void tokenize();
    void processTokens();
    void parseJSON();
    //pass key
    JSONObject parseObject();
    void parseArray(std::string key, JSONObject& object);
    void parsePairs(std::string key, JSONObject& object);
    void parsePair(std::string key, JSONObject& object);
    void parseString(std::string key, JSONObject& object);
    void parseNumber(std::string key, JSONObject& object);
    void parseBool(std::string key, JSONObject& object);
    void parseNull(std::string key, JSONObject& object);
    void parseValue(std::string key, JSONObject& object);
    void parseElements(std::string key, JSONObject& object);

    JSONToken getToken();
    void printTokens() const;

    std::string file;
    std::vector<JSONToken> tokens;
    Uint32 tokenIndex;

    JSONMap jsonMap;
    std::vector<std::string> keys;

    //useful to know the entities available in game
    std::set<std::string> entities;

    //store address of allocated memory for freeing later
    std::stack<JSONObject*> allocatedJSONObjects;
    void freeJSONObjects();

};