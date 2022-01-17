//
//  AssetManager.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-12.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "AssetManager.hpp"
#include "FileManager.hpp"


AssetManager::AssetManager(std::string wp){
    world_path = wp;
    resource_path = resourcePath();
    key = 0;
    if(!init()){
        std::cout << "AssetManager Init Failed" << std::endl;
    }
    
}
AssetManager::~AssetManager(){
    for (auto const& asset : assets){
        asset.second->~Asset();
        delete asset.second;
    }
    assets.clear();
}
bool AssetManager::init(){
    //find/make app folder
    //try n get away with not keeping an FM around
    FileManager fm;
    fm.makeWorldFolder(world_path);
    texture_path = world_path + "textures/"; //fix this shit
    
    return true;
}

template<>
Asset*  AssetManager::createAsset<AssetTexture>(const std::string& filename){
    //if assets has no filename
    //assets.contains is c++ 20 >:(
    if(assets.find(filename) != assets.end())
        return assets.at(filename);
    
    assets.insert(std::pair<std::string, Asset*>(filename, (new TextureAsset(AssetTexture, filename) )) );
        if(assets.at(filename)->load(world_path + filename)){
            return assets.at(filename); //UNKNOWN
        }
}
    
template<>
    Asset* AssetManager::createAsset<AssetJSON>(const std::string& filename){
        if(assets.find(filename) != assets.end())
            return assets.at(filename);
    
}

template<>
Asset*  AssetManager::createAsset<AssetTextureMap>(const std::string& filename){
    assets.insert(std::pair<std::string, Asset*>(filename, (new TextureMapAsset(AssetTexture, filename) )) );
        if(assets.at(filename)->load(world_path + filename)){
            return assets.at(filename); //UNKNOWN
        }
    
    //TextureMapAsset should act as a mini mgr with IntRects map loaded from json that accompanies the TextureMap
    //so that a tilemgr can be like hey! need "map7" and "worldname"->"tile data", and a tile can say hey need "tile data"(myPos), then okay now I need IntRect for map7->"grass"..
    //https://www.sfml-dev.org/tutorials/2.5/graphics-vertex-array.php
}

Asset* AssetManager::createAsset(const std::string& filename, AssetTypes type){
    switch (type){
        case AssetTexture:
            createAsset<AssetTexture>(filename);
            break;
        case AssetJSON:
            createAsset<AssetJSON>(filename);
            break;
        case AssetBasic:
        default:
            std::cout << "lazy developer hasnt fixed this yet" << std::endl;
    }
}
Asset* AssetManager::requestAsset(const std::string& name){
    return assets.at(name);
}


void AssetManager::removeAsset(const std::string& name){
    assets.at(name)->inUse = false;
}
void AssetManager::cleanup(){
    for (auto const& asset : assets){
        if(asset.second->inUse == false){
            asset.second->~Asset();
            delete asset.second;
            assets.erase(asset.first);
        }
    }
}

void AssetManager::update(){
    
}
