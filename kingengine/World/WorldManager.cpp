//
//  WorldManager.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-10.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "WorldManager.hpp"


WorldManager::WorldManager(){
    world_texture = sf::Texture();
    world_type = DefaultWorld;
    createdWorld = false;
    world_name = "world";
    world_texturename = "texture path";
    renderables.resize(RENDER_LAYERS);
  

}

WorldManager::~WorldManager(){
    delete assets;
    
}



//TILED WORLD
bool WorldManager::CreateWorld(WorldTypes type, int sizeX, int sizeY, int num_tiles, std::string name, std::string world_path){
    assets = new AssetManager(world_path);
    
    renderables.clear();
    //create tiles object
    createdWorld = false; //fuck
    world_type = type;
    world_name = name;
    size_x = sizeX;
    size_y = sizeY;
    world_texturename = world_path + "layer0.png";
   
    sf::Texture* wt = (sf::Texture*) assets->createAsset<AssetTexture>("layer0.png")->get(); //DO REAL CASTING
    //if this doesnt work then have a local texture asset!
    int rect_x = size_x / num_tiles;
    int rect_y= size_y / num_tiles;
    if(!world_texture.loadFromFile(world_texturename)){
        if(!world_texture.loadFromFile(resourcePath() + "layer0.png"))
            return false;
    }
    TextureMapAsset* t_map = static_cast<TextureMapAsset*>( assets->createAsset<AssetTextureMap>("tilemap.png"));
    t_map->setSavePath(world_path);
    //should just give texture name to tilemanager which can call the worlds asset manager.. somehow.. OR just pass the * to the asset and it can flag as unneeded itself, world can run the check!
   // renderables[RENDER_BASELAYER].push_back(new TileManager(size_x, size_y, rect_x, rect_y, &world_texture) ); //TILEMANAGER_INDEX = 0
    renderables[RENDER_BASELAYER].push_back(new TileMap(t_map, sf::Vector2u(32,32)));
    //add world objects
    
    //lighting
    
    //
    createdWorld = true;
    
    return true;
}

bool WorldManager::SaveWorld(std::string to_save){
    sf::RenderTexture* edited = new sf::RenderTexture();
    //engine-render(rendertext)
    
    edited->create(size_x,size_y);
    edited->clear();
    for ( auto const &layer : renderables ){
        for (auto const &toSave : layer){
            toSave->save(*edited);
        }
    }
    edited->display();
    sf::Texture finalproduct = edited->getTexture();
    
    sf::Image world_image(finalproduct.copyToImage());
    delete edited;
    if(!world_image.saveToFile(to_save.c_str())){
        if(!world_image.saveToFile(std::string(resourcePath() + to_save).c_str())){
            return false;
        }
    }
    
    return true;
}

TileMap& WorldManager::getTileMap(){
    return (TileMap&)*(renderables[RENDER_BASELAYER][TILEMANAGER_INDEX]); //greasy
    //use static cast or something
}

void WorldManager::updateWorld(){
    
    assets->update(); //do this like once in a while
}

void WorldManager::updateWorldFixed(){
    
    //do this like once in a while
}
void WorldManager::renderWorld(){
    
    
}
