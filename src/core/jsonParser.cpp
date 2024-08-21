#include "../../include/core/jsonParser.h"
#include <iostream>
#include <cctype>
#include <fstream>
#include <stdexcept>



JSONParser* JSONParser::instance = nullptr;

JSONParser::JSONParser(){}

auto error = [](const std::string& message, std::string value) -> void{
    std::cerr << message << " Error!" << " :: " << value << std::endl;
};
    
JSONParser* JSONParser::Instance(){
    if(!instance){
        instance = new JSONParser();
    }
    return instance;
}

void JSONParser::initJSONParser(std::string file){
    this->file = file;
    this->key = "key";

    tokenize();
    parseJSON();
    freeJSONObjects();
}


void JSONParser::tokenize(){
    std::ifstream file(this->file);
    char c;

    if(!file.is_open()){
        error("File", "Unable to read");
        exit(1);
    }

    while(file.get(c)){
        JSONToken token;
        
        if(c == '['){
            token.value = '[';
            token.type = OPEN_BRACKET;

            this->tokens.push_back(token);

        }else if(c == '{'){
            token.value = '{';
            token.type = OPEN_BRACE;

            this->tokens.push_back(token);

        }else if(c == '"'){
            while(file.get(c) && c != '"'){
                token.value += c;
            }

            //can't do this
            //token.type = KEY;
            
            token.type = STRING;

            this->tokens.push_back(token);
        }else if(c == ':'){
            token.value = ':';
            token.type = COLON;

            this->tokens.push_back(token);
        }else if(c == ','){
            token.value = ',';
            token.type = JCOMMA;

            this->tokens.push_back(token);
        }else if(c == '-'){
            token.value = '-';
            token.type = JMINUS;

            this->tokens.push_back(token);
        }else if(std::isdigit(c)){
            token.value += c;
            
            while(file.get(c) && c != ',' && !isspace(c) && c != ']'){
                token.value += c;
            }
            file.unget();

            token.type = NUMBER;
            this->tokens.push_back(token);

        }else if(c == ']'){
            token.value = ']';
            token.type = CLOSE_BRACKET;

            this->tokens.push_back(token);

        }else if(c == '}'){
            token.value = '}';
            token.type = CLOSE_BRACE;

            this->tokens.push_back(token);

        }

    }

    file.close();
    processTokens();
}

//changes type of every token in front of a colon token to KEY
//makes parsing more convenient
void JSONParser::processTokens(){
    for(Uint32 i = 0; i < this->tokens.size(); i++){
        if(this->tokens[i].type == COLON)
            this->tokens[i - 1].type = KEY;
    }
}

JSONToken JSONParser::getToken(){
    return this->tokens[this->tokenIndex++];
}

void JSONParser::printTokens() const{
  for(int i = 0; i < this->tokens.size(); i++){
    std::cout << this->tokens[i].value << std::endl;
  }
}

void JSONParser::parseJSON(){
    JSONToken token = this->tokens[this->tokenIndex];
    //JSONObject jo;

    switch(token.type){

        //[]
        case OPEN_BRACKET:
            
            //this->json = parseArray();
            break;

        //{}
        case OPEN_BRACE:
            //jo = JSONObject{};
            
            this->json["Entities"] = new JSONObject(parseObject());
            this->keys.push_back("Entities");
            this->allocatedJSONObjects.push(std::get<JSONObject*>(this->json["Entities"]));

            break;

        default:
            break;
    }
}

JSONObject JSONParser::parseObject(){
    std::string key;
    JSONObject object;

    if(getToken().type != OPEN_BRACE){
        error("Object", this->tokens[this->tokenIndex - 1].value);
    }

    parsePairs(key, object);


    if(getToken().type != CLOSE_BRACE){
        error("Object", this->tokens[this->tokenIndex - 1].value);
    }

    return object;

}

void JSONParser::parsePairs(std::string key, JSONObject& object){

    while(this->tokens[this->tokenIndex].type == KEY){
        key = this->tokens[this->tokenIndex].value;
        
        parsePair(key, object);

        if(getToken().type != JCOMMA){
            this->tokenIndex--;
            break;
        }

    }
    
}

void JSONParser::parsePair(std::string key, JSONObject& object){
    
    parseString(key, object);

    //NEW
    this->keys.push_back(key);

    
    if(getToken().type != COLON){
        error("Pair", this->tokens[this->tokenIndex - 1].value);
    }

    parseValue(key, object);

    if(keys.size() == 3)
        jsonMap[keys[0]][keys[1]][keys[2]] = object[key];
        this->entities.insert(this->keys[0]);


    //NEW
    this->keys.pop_back();
    
}

void JSONParser::parseValue(std::string key, JSONObject& object){
    //std::cout << key << std::endl;

    JSONToken token = this->tokens[this->tokenIndex];

    switch(token.type){
        case STRING:
            parseString(key, object);
            break;
        
        case OPEN_BRACE:
            object[key] = new JSONObject(parseObject());
            this->allocatedJSONObjects.push(std::get<JSONObject*>(object[key]));
            break;

        case OPEN_BRACKET:
            parseArray(key, object);
            break;

        case NUMBER:
            parseNumber(key, object);
            break;

        case BOOL:
            parseBool(key, object);
            break;

        case NULL_VALUE:
            parseNull(key, object);
            break;

        default:
            break;

    }
}

void JSONParser::parseString(std::string key, JSONObject& object){
    object[key] = getToken().value;
}

void JSONParser::parseNumber(std::string key, JSONObject& object){
    object[key] = static_cast<Uint32>(std::stoul(getToken().value));
}

void JSONParser::parseBool(std::string key, JSONObject& object){
    object[key] = getToken().value;
}

void JSONParser::parseNull(std::string key, JSONObject& object){
    object[key] = getToken().value;
}

JSONObject JSONParser::getJSONObject(){
    return this->json;
}

void JSONParser::parseArray(std::string key, JSONObject& object){

    if(getToken().type != OPEN_BRACKET){
        error("Array", this->tokens[this->tokenIndex - 1].value);
    }

    parseElements(key, object);

    if(getToken().type != CLOSE_BRACKET){
        error("Array", this->tokens[this->tokenIndex - 1].value);
    }

}

void JSONParser::parseElements(std::string key, JSONObject& object){
    std::vector<float> elements;
    
    while(this->tokens[this->tokenIndex].type == JMINUS || this->tokens[this->tokenIndex].type == NUMBER){
        if(this->tokens[this->tokenIndex].type == JMINUS){
            std::string str = getToken().value;
            str += getToken().value;
            elements.push_back(std::stof(str));
        }else{
            elements.push_back(std::stof(getToken().value));
        }

        if(getToken().type != JCOMMA){
            this->tokenIndex--;
            break;
        }
    }

    object[key] = elements;
    
}

void JSONParser::print() {

    for(const auto& pair : (*std::get<JSONObject*>(json["Entities"])).value){
        std::string& png = std::get<std::string>(
            (*std::get<JSONObject*>(
                (*std::get<JSONObject*>(
                    (*std::get<JSONObject*>(
                        json["Entities"]
                        ))["Player"]
                ))["Sprite"]
            ))["texture"]
        );
        
        //std::cout << png << std::endl;
    }
        
        
        std::cout << std::get<std::string>(jsonMap["Player"]["Sprite"]["texture"]) << std::endl;


    /*
    std::cout << (*std::get<JSONObject*>(
                    json["Entities"]
                    )).value.size() << std::endl;

    */
}

std::set<std::string> JSONParser::getEntities(){
    return this->entities;
}

JSONMap JSONParser::getJSONMap(){
    return this->jsonMap;
}

void JSONParser::freeJSONObjects(){
    while(!allocatedJSONObjects.empty()){
        delete(allocatedJSONObjects.top());
        allocatedJSONObjects.pop();
    }
}



/*
void JSONParser::print(){

    JSONObject* entitiesObject = std::get<JSONObject*>(json["Entities"]);
    
    JSONObject* playerObject = std::get<JSONObject*>((*(entitiesObject))["Player"]);
    JSONObject* spriteValue = std::get<JSONObject*>((*(playerObject))["Sprite"]);
    std::string& png = std::get<std::string>((*(spriteValue))["texture"]);
    

    std::cout << png << std::endl;

}
*/


