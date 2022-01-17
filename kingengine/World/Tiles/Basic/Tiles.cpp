//
//  Tiles.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-10.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "Tiles.hpp"
#include "GameEngine.hpp"
extern GameEngine* engine;

Tile::Tile(sf::IntRect tRect , sf::Vector2f tPos, sf::Texture* text){
    texture_rect = tRect;
    position = tPos;
    sprite.setTexture(*text);
    sprite.setTextureRect(tRect);
    sprite.setPosition(position);
    float originX = tRect.width / 2;
    float originY = tRect.height / 2;
    sprite.setOrigin(sf::Vector2f(originX,originY));
    render_type = RenderOptions::WorldObject;
}
Tile::~Tile(){
    delete this;
}
sf::Vector2f Tile::getPosition(){
    return sprite.getPosition();
}
void Tile::draw(sf::RenderTarget &target) const {
    target.draw(sprite);
}
RenderOptions::RenderTypes Tile::getType() const{
    return render_type;
}
TileManager::TileManager(int sX, int sY, int rX, int rY, sf::Texture* st){
    sizeX = sX;
    sizeY= sY;
    rectX = rX;
    rectY = rY;
    numX = (sizeX / rectX);
    numY = (sizeY / rectX);//greasy
    sourceTexture = *st;
    render_type = RenderOptions::WorldObjectManager;
    initTiles();
    return;
    
}
TileManager::~TileManager(){
    for ( auto const &row : tiles ){
        for (auto const &tile : row){
            //if engine->screen rect contains( position ) aka isVisible
            delete tile;
        }
        
    }
  
    tiles.clear();
}
bool TileManager::initTiles(){
    tiles.clear();
    
    tiles.resize(numY);
    for (int y = 0; y < numY; y++){
        tiles[y].resize(numX);
        for (int x = 0; x < numX; x++){
            float worldX = x * rectX;
            float worldY = y * rectY;
            //sf::Rect<int> texture_rect = sf::Rect<int>(sf::Vector2<float>(worldX, worldY), sf::Vector2i(rectX, rectY)) ;
            
            //tiles[y][x] = (new Tile(texture_rect, sf::Vector2f(worldX, worldY), &sourceTexture));
        }
    }
    return true; //beautiful error handling
}

void TileManager::draw(sf::RenderTarget &target) const{
    for ( auto const &row : tiles ){
        for (auto const &tile : row){
            if(engine->isPointInCurrentView(tile->getPosition()))
                tile->draw(target);
        }
    }
}


RenderOptions::RenderTypes TileManager::getType() const{
                return render_type;
}
void Tile::setColor(sf::Color setColor){
    sprite.setColor(setColor);
}
sf::FloatRect Tile::getBounds(){
    return sprite.getGlobalBounds();
}
void TileManager::setTileColor(sf::Color setColor, int x, int y){
        for ( auto const &row : tiles ){
            for (auto const &tile : row){
                if(tile->getBounds().contains(sf::Vector2f(x, y) )){
                    if(setColor.r > 0 || setColor.b > 0 || setColor.g >0){
                        tile->setColor(setColor);
                    }
                   
                }
            }
        }
}


void TileManager::save(sf::RenderTarget &target) const{
    for ( auto const &row : tiles ){
        for (auto const &tile : row){
                tile->save(target);
        }
    }
}

void Tile::save(sf::RenderTarget &target) const {
    draw(target);
    //write individual special stuff
}

Tile* TileManager::nearestTileTo(sf::Vector2f pos){
    //worldX = tileX * rectX ->tileX = worldX / rectX
    int tileX = pos.x / rectX;
    int tileY = pos.y / rectY;
    if(tileY < tiles.size()){
        if(tileX < tiles.size()){ 
            return tiles[tileY][tileX];
        }
    }
}
