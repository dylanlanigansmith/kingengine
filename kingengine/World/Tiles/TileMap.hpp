//
//  TileMap.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-14.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef TileMap_hpp
#define TileMap_hpp
#include "BaseRenderable.hpp"
#include "AssetManager.hpp"

class TileMapInformation{
private:
    std::string path;
    //have some stuff for diff size maps and smart position finding
    
    std::string tilemap_name;
    uint32_t map_size_x,map_size_y; //should start using H/W instead this is kinda arbitrary
    uint32_t tile_size_x, tile_size_y;
    uint8_t max_tile_index;
    
    std::unordered_map<std::string, int> name_to_position;
    
    std::vector<int> tiles;
    
    bool loadJSON();
    bool createJSON();
    bool saveJSON();
    bool saveTo();
 
    friend class TileMap;
    friend class Development;
public:
    void save(){
        saveJSON();
    }
    TileMapInformation(std::string p){
        path = p;
        loadJSON();
    }
    void getTileNames(std::vector<std::string> &vec){
        vec.clear();
        for (auto& n : name_to_position){
            vec.push_back(n.first);
        }
    }
    void update(int x, int y, const std::string& name){
        int idx = (x / tile_size_x) + ( (y / tile_size_y)*32);
        if((name_to_position.find(name) != name_to_position.end()) && idx < tiles.size())
            tiles[idx] = name_to_position[name];
        else if( idx < tiles.size())
            logger << "no tile for " << name << lm::endl;
        else
            logger.silent("out of range tile at XY: " + std::to_string(x) + std::to_string(y));
    }
};


class TileMap : public Renderable, public sf::Transformable
{
public:
    TileMap(TextureMapAsset* ass, sf::Vector2u tileSize){
        asset = ass;
        
        tmi = new TileMapInformation(asset->getSavePath()+"save.json");
        
        load(tileSize);
    }
    virtual ~TileMap(){
        delete tmi;
        asset->inUse = false;
        delete asset;
    }
    bool load( sf::Vector2u tileSize);
    
    void save(sf::RenderTarget& target) const;
    virtual RenderOptions::RenderTypes getType() const{
        return RenderOptions::WorldObjectManager;
    }
    virtual void draw(sf::RenderTarget& target) const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void update();
    TileMapInformation* getInfo(){
        return tmi;
    }

private:
    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
    TextureMapAsset* asset;
    TileMapInformation* tmi;
};


#endif /* TileMap_hpp */
