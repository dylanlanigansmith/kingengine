//
//  AssetManager.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-12.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef AssetManager_hpp
#define AssetManager_hpp


#include "includes.hpp"

#include "defines.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>

//JSON Include
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
enum AssetTypes{
    AssetBasic = 0,
    AssetTexture,
    AssetTextureMap,
    AssetImage,
    AssetAudio,
    AssetJSON,
};

class Asset { //suck it up and make a classs
public:
    AssetTypes type;
    std::string name;
    bool inUse;
    bool loaded;
    //bool loaded = dynamic loading
    Asset(AssetTypes t, std::string n){
        type = t;  name = n;
        inUse = true;
        loaded = false;
    }
    virtual ~Asset(){
        inUse = false;
        loaded = false;
        
    }
    virtual void* get() const{
        return;
    }
   
    virtual bool load(std::string) = 0;
    AssetTypes getType(){
        return type;
    }
   
};

class TextureAsset : public Asset{
    sf::Texture* texture;
    std::string text_name;
    std::string text_path;
    bool isLoaded(){
        return loaded;
    }
    void setLoaded(bool l){
        loaded = l;
    }
public:
    TextureAsset(AssetTypes t,  std::string n) : Asset(t, n){
        texture = new sf::Texture();
        text_name = n;
    }
    virtual ~TextureAsset(){
        delete texture;
    }
    sf::Texture* get_t() const{
        return texture;
    }
   
    virtual void* get() const {
        if(loaded)
            return get_t();
    }
    virtual bool load(std::string path)  {
        text_path = path;
        setLoaded(texture->loadFromFile(path));
        return isLoaded();
        //figure out that CONST keyword
    }
    
};
class TextureMapAsset : public Asset{
    sf::Texture* texture;
    rapidjson::Document data;
    std::string map_name;
    std::string map_path;
    sf::Vector2i size;
    bool isLoaded(){
        return loaded;
    }
    void setLoaded(bool l){
        loaded = l;
    }
public:
    std::string getPath(){
        return map_path;
    }
    std::string getName(){
        return map_path;
    }
    sf::Vector2i getSize(){
        return size;
    }
    
    TextureMapAsset(AssetTypes t,  std::string n) : Asset(t, n){
        texture = new sf::Texture();
        map_name = n;
        size = sf::Vector2i(WIN_X, WIN_Y);
    }
    ~TextureMapAsset(){
        delete texture;
    }
    sf::Texture* get_t() const{
        return texture;
    }
   
    void* get() const override{
        if(loaded)
            return get_t();
    }
    bool load(std::string path) override {
        setLoaded(texture->loadFromFile(path));
        map_path = path;
        return isLoaded();
        //figure out that CONST keyword
    }
    bool save(std::string path){
        //write json
    }
    
};
class AssetManager{
private:
    int key;
    //worship this: https://en.cppreference.com/w/cpp/container/unordered_map
    std::unordered_map<std::string, Asset*> assets;
    
    std::string app_path;
    std::string resource_path;
    std::string world_path;
    std::string texture_path;
    std::string json_path;
    
    bool init();
    void cleanup();
public:
    template <AssetTypes asset_t>
    Asset* createAsset(const std::string& filename);
    
    Asset* createAsset(const std::string& filename, AssetTypes type);
    
    Asset* requestAsset(const std::string& name);
    
    void update();
    void removeAsset(const std::string& name);
    AssetManager(std::string wp);
    ~AssetManager();
    
};



#endif /* AssetManager_hpp */
