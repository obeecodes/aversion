#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <SDL2/SDL.h>

typedef struct Token Token;
typedef struct Object Object;

enum TokenType{
    OPEN_TAG,
    CLOSE_TAG,
    ATTRIBUTE,
    EQUAL_SIGN,
    VALUE,
    TEXT,
    COMMENT,
    COMMA,
    NEWLINE,
};

struct Token{
    std::string value;
    TokenType type;
    Token() : value(""), type(){};
    Token(std::string value, TokenType type) : value(value), type(type){};
};

struct Object{
    Uint32 id;
    std::string gid;
    Uint32 x;
    Uint32 y;
    Uint32 width;
    Uint32 height;
    Object() : id(), gid(), x(), y(), width(), height(){};
    Object(Uint32 id, std::string gid, Uint32 x, Uint32 y, Uint32 width, Uint32 height)
        : id(id), gid(gid), x(x), y(y), width(width), height(height){};
};

class TMXParser{
public:
    static TMXParser* Instance();
    void initTMXParser(std::string file_name);
    Uint32 getMapWidth() const;
    Uint32 getMapHeight() const;
    Uint32 getTileWidth() const;
    Uint32 getTileHeight() const;
    Uint32 getNextLayerID() const;
    std::vector<std::vector<std::vector<Uint32>>>& getMap();
    std::vector<Object>& getObjects();
    std::unordered_map<std::string, Uint32>& getLayers();
    void freeTMXParser();

private:
    TMXParser();
    void tokenize();
    void parse();
    void parseXMLDeclaration();
    void parseMap();
    void parseTileset();
    void parseLayer();
    void parseData();
    void parseObjectGroup();
    void parseObject();
    Token getToken();
    void printTokens() const;

    static TMXParser* instance;
    std::string file_name;
    std::string xml_version;
    std::string xml_encoding;
    std::string map_version;
    std::string tiled_version;
    std::string orientation;
    std::string renderorder;
    Uint32 map_width;
    Uint32 map_height;
    Uint32 tile_width;
    Uint32 tile_height;
    bool infinite;
    Uint32 layerID;
    std::string layer_name;
    Uint32 layer_width;
    Uint32 layer_height;
    Uint32 next_layerID;
    Uint32 next_objectID;
    Uint32 first_gID;
    std::string source;
    std::vector<Token> tokens;
    std::vector<std::vector<std::vector<Uint32>>> map;
    std::vector<Object> objects;
    Uint32 token_index;
    std::unordered_map<std::string, Uint32> layers;

    Uint32 layer_count;
    Uint32 objectgroup_count;

};
