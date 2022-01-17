//
//  WorldManager.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-10.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef WorldManager_hpp
#define WorldManager_hpp

#include "AssetManager.hpp"
#include "includes.hpp"
#include "defines.hpp"

#include "Tiles.hpp"
#include "TileMap.hpp"

enum WorldTypes{
    DefaultWorld = 0,
    Tiled,
    Sidescroll,
    Custom
};

class WorldManager {
private:
    //https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
    std::vector< std::vector< Renderable*> > renderables;
    std::string world_name;
    std::string world_texturename;
    sf::Texture world_texture;
    WorldTypes world_type;
    int size_x,size_y;
    bool createdWorld;
    AssetManager* assets;
public:
    friend class GameEngine;
    virtual bool CreateWorld(WorldTypes, int, int, int, std::string, std::string); //
    virtual bool SaveWorld(std::string);
    virtual void renderWorld();
    virtual void updateWorld(); //cleanup 
    virtual void updateWorldFixed(); //physics
    TileMap& getTileMap();
    WorldManager();
    ~WorldManager();
    
    
};

#endif /* WorldManager_hpp */
