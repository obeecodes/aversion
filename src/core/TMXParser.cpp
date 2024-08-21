#include "../../include/core/TMXParser.h"
#include <iostream>
#include <vector>
#include <cctype>
#include <fstream>
#include <stdexcept>

TMXParser* TMXParser::instance = nullptr;

auto error = [](const std::string& message, std::string value) -> void{
    std::cerr << message << " Error!" << " :: " << value << std::endl;
};
    
TMXParser* TMXParser::Instance(){
    if(!instance){
        instance = new TMXParser();
    }
    return instance;
}

void TMXParser::initTMXParser(std::string file_name){
    this->file_name = file_name;
    this->token_index = 0;
    this->layer_count = 0;
    this->objectgroup_count = 0;
    tokenize();
}

TMXParser::TMXParser(){
}

void TMXParser::tokenize(){
    std::ifstream file(this->file_name);
    char c;

    if(!file.is_open()){
        error("File", "Unable to read");
        exit(1);
    }

    while(file.get(c)){
        Token token;
        if(c == '<'){
            if(file.get(c) && c != '/'){
                file.unget();
                token.value += '<';

                while(file.get(c) && !std::isspace(c)){
                    token.value += c;
                }

                token.type = OPEN_TAG;
                this->tokens.push_back(token);
            }else if(c == '/'){
                token.value += '<';
                token.value += c;
                
                while(file.get(c) && c != '>'){
                    token.value += c;
                }

                token.value += '>';

                token.type = CLOSE_TAG;
                this->tokens.push_back(token);
            }
        }else if(std::isalpha(c)){
            token.value += c;

            while(file.get(c) && c != '='){
                token.value += c;
            }

            token.type = ATTRIBUTE;
            this->tokens.push_back(token);

            this->tokens.emplace_back(std::string(1, c), EQUAL_SIGN);

        }else if(c == '"'){
            while(file.get(c) && c != '"'){
                token.value += c;
            }

            token.type = VALUE;
            this->tokens.push_back(token);
        }else if(c == '?'){
            token.value += c;
            file.get(c);
            token.value += c;

            token.type = CLOSE_TAG;
            this->tokens.push_back(token);
        }else if(c == '/'){
            token.value += c;
            file.get(c);
            token.value += c;

            token.type = CLOSE_TAG;
            this->tokens.push_back(token);
        }else if(std::isdigit(c)){
            token.value += c;
            
            while(file.get(c) && c != ',' && !std::isspace(c)){
                token.value += c;
            }

            token.type = VALUE;
            this->tokens.push_back(token);
            if(c == ','){
                this->tokens.emplace_back(std::string(1, c), COMMA);
            } else if(c == '\n'){
                this->tokens.emplace_back(std::string(1, c), NEWLINE);
            }
        }else if(c == '>'){
            token.value += c;
            token.type = CLOSE_TAG;
            this->tokens.push_back(token);
        }

        if(this->tokens.back().value == "<layer"){
            this->layer_count++;
        }else if (this->tokens.back().value == "<objectgroup"){
            this->objectgroup_count++;
        }

    }

    file.close();
    parse();
}

void TMXParser::parse(){
    parseXMLDeclaration();
    parseMap();
    //printTokens();
}

void TMXParser::parseXMLDeclaration(){
    
    if(getToken().type != OPEN_TAG){
        error("XML Declaration", this->tokens[this->token_index - 1].value);
    }

    if(getToken().type != ATTRIBUTE){
        error("XML Declaration", this->tokens[this->token_index - 1].value);
    }

    if(getToken().type != EQUAL_SIGN){
        error("XML Declaration", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("XML Declaration", this->tokens[this->token_index - 1].value);
    }

    this->xml_version = this->tokens[this->token_index - 1].value;

    if(getToken().type != ATTRIBUTE){
        error("XML Declaration", this->tokens[this->token_index - 1].value);
    }

    if(getToken().type != EQUAL_SIGN){
        error("XML Declaration", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("XML Declaration", this->tokens[this->token_index - 1].value);
    }

    this->xml_encoding = this->tokens[this->token_index - 1].value;
    
    
    if(getToken().type != CLOSE_TAG){
        error("XML Declaration", this->tokens[this->token_index - 1].value);
    }
    
}

void TMXParser::parseMap(){
    
    if(getToken().type != OPEN_TAG){
        error("Map", this->tokens[this->token_index - 1].value);
    } 
    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    this->map_version = this->tokens[this->token_index - 1].value;

    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    this->tiled_version = this->tokens[this->token_index - 1].value;
    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    this->orientation = this->tokens[this->token_index - 1].value;
    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    this->renderorder = this->tokens[this->token_index - 1].value;
    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->map_width = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->map_height = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }

    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->tile_width = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }

    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->tile_height = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->infinite = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->next_layerID = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }

    //this->map.resize(this->next_layerID - 1);
    this->map.resize(this->layer_count);
    
    if(getToken().type != ATTRIBUTE){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Map", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->next_objectID = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }

    if(getToken().type != CLOSE_TAG){
        error("Map", this->tokens[this->token_index - 1].value);
    }

    parseTileset();

    while(getToken().value == "<layer"){
      this->token_index--;
      parseLayer();
    }

    this->token_index--;

    while(getToken().value == "<objectgroup"){
      this->token_index--;
      parseObjectGroup();
    }

    this->token_index--;

    if(getToken().type != CLOSE_TAG){
        error("Map", this->tokens[this->token_index - 1].value);
    }

}

void TMXParser::parseTileset(){
    
    if(getToken().type != OPEN_TAG){
        error("Tileset", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Tileset", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Tileset", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Tileset", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->first_gID = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }

    if(getToken().type != ATTRIBUTE){
        error("Tileset", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Tileset", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Tileset", this->tokens[this->token_index - 1].value);
    }

    this->source = this->tokens[this->token_index - 1].value;

    if(getToken().type != CLOSE_TAG){
        error("Tileset", this->tokens[this->token_index - 1].value);
    }
}

void TMXParser::parseLayer(){
    
    if(getToken().type != OPEN_TAG){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Layer", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->layerID = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Layer", this->tokens[this->token_index - 1].value);
    }

    this->layer_name = this->tokens[this->token_index - 1].value;

    this->layers[this->layer_name] = this->layerID;
    
    if(getToken().type != ATTRIBUTE){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Layer", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->layer_width = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Layer", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Layer", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->layer_height = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }

    if(getToken().type != CLOSE_TAG){
        error("Layer", this->tokens[this->token_index - 1].value);
    }

    this->map[this->layerID - 1].resize(this->layer_height);

    parseData();

    if(getToken().type != CLOSE_TAG){
        error("Layer", this->tokens[this->token_index - 1].value);
    }

}

void TMXParser::parseData(){
    
    if(getToken().type != OPEN_TAG){
        error("Data", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Data", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Data", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Data", this->tokens[this->token_index - 1].value);
    }

    if(getToken().type != CLOSE_TAG){
        error("Data", this->tokens[this->token_index - 1].value);
    }

    for(Uint32 i = 0; i < this->layer_height; i++){
        for(Uint32 j = 0; j < this->layer_width; j++){

            try{   
                this->map[this->layerID - 1][i].push_back(std::stoi(getToken().value));
            } catch(const std::exception& e){
                std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
            }
            this->token_index++;
        }
    }

    if(this->tokens[this->token_index - 1].type != NEWLINE){
        error("Data", this->tokens[this->token_index - 1].value);
    }

    if(getToken().type != CLOSE_TAG){
        error("Data", this->tokens[this->token_index - 1].value);
    }

}

void TMXParser::parseObjectGroup(){
    
    if(getToken().type != OPEN_TAG){
        error("Object Group", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Object Group", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Object Group", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Object Group", this->tokens[this->token_index - 1].value);
    }

    try{   
        this->layerID = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    if(getToken().type != ATTRIBUTE){
        error("Object Group", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Object Group", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Object Group", this->tokens[this->token_index - 1].value);
    }

    this->layer_name = this->tokens[this->token_index - 1].value;

    if(getToken().type != CLOSE_TAG){
        error("Object Group", this->tokens[this->token_index - 1].value);
    }

    while(getToken().value == "<object"){
      this->token_index--;
      parseObject();
    }

    this->token_index--;

    if(getToken().type != CLOSE_TAG){
        error("Object", this->tokens[this->token_index - 1].value);
    }
 
}

void TMXParser::parseObject(){
    //printTokens();

    Object object;
    
    if(getToken().type != OPEN_TAG){ 
        error("Object", this->tokens[this->token_index - 1].value);

    }
    
    //id attribute
    if(getToken().type != ATTRIBUTE){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Object", this->tokens[this->token_index - 1].value);
    }

    try{   
        object.id = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }

    // collision layer should not have gid, I made a mistake
    /*
    if(getToken().type != ATTRIBUTE){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Object", this->tokens[this->token_index - 1].value);
    }

    try{   
        object.gid = this->tokens[this->token_index - 1].value;
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }*/
    

    // x
    if(getToken().type != ATTRIBUTE){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Object", this->tokens[this->token_index - 1].value);
    }

    try{   
        object.x = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    // y
    if(getToken().type != ATTRIBUTE){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Object", this->tokens[this->token_index - 1].value);
    }

    try{   
        object.y = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    // width
    if(getToken().type != ATTRIBUTE){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Object", this->tokens[this->token_index - 1].value);
    }

    try{   
        object.width = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }
    
    // height
    if(getToken().type != ATTRIBUTE){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != EQUAL_SIGN){
        error("Object", this->tokens[this->token_index - 1].value);
    }
    
    if(getToken().type != VALUE){
        error("Object", this->tokens[this->token_index - 1].value);
    }

    try{   
        object.height = std::stoi(this->tokens[this->token_index - 1].value);
    } catch(const std::exception& e){
        std::cerr << "Exception caught: " << e.what() << "\n" << "Line: " << __LINE__ << std::endl;
    }

    if(getToken().type != CLOSE_TAG){
        error("Object", this->tokens[this->token_index - 1].value);
    }

    this->objects.push_back(object);

}

Token TMXParser::getToken(){
    return this->tokens[this->token_index++];
}

Uint32 TMXParser::getMapWidth() const{
    return this->map_width;
}

Uint32 TMXParser::getMapHeight() const {
    return this->map_height;
}

Uint32 TMXParser::getTileWidth() const{
    return this->tile_width;
}

Uint32 TMXParser::getTileHeight() const{
    return this->tile_height;
}

Uint32 TMXParser::getNextLayerID() const{
    return this->next_layerID;
}

std::vector<std::vector<std::vector<Uint32>>>& TMXParser::getMap(){
    return this->map;
}

std::vector<Object>& TMXParser::getObjects(){
    return this->objects;
}

std::unordered_map<std::string, Uint32>& TMXParser::getLayers(){
    return this->layers;
}

void TMXParser::printTokens() const{
  for(int i = 0; i < this->tokens.size(); i++){
    std::cout << this->tokens[i].value << std::endl;
  }
}

void TMXParser::freeTMXParser(){
    this->tokens.clear();
    this->objects.clear();
    this->map.clear();
    //delete instance;
    //instance = nullptr;
    delete this;
}

