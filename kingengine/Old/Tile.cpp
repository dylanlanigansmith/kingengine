//
//  Tile.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "Tile.hpp"


GameTile::GameTile(const sf::Texture* text, float x, float y, bool pass, bool isexit)
{
    pos = sf::Vector2f(x,y);
    setUpSprite(text);
    
    
    isPassable = pass;
    isExit = isexit;
    
}
GameTile::GameTile(const sf::Texture* text, float x, float y, int idx)
{
    pos = sf::Vector2f(x,y);
    setUpEntity(text);
    
    
    isPassable = true;
    isExit = false;
    
}
bool GameTile::setUpSprite(const sf::Texture* texture){
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(pos.x,pos.y,32,32)); //set size
    sprite.setOrigin(16.f,16.f);
    sprite.setPosition(pos);
  
   
    
    return true;
}
bool GameTile::setUpEntity(const sf::Texture* texture){
    sprite.setTexture(*texture);
 //   sprite.setTextureRect(sf::IntRect(0,0,32,32)); //set size
    sprite.setOrigin(44.f,10.f);
    sprite.setScale(-1.f,1.f);
    sprite.setPosition(pos);
  
   
    
    return true;
}


