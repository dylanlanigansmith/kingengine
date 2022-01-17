//
//  Tile.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef Tile_hpp
#define Tile_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
class GameTile {
public:
    bool isPassable;
    bool isExit;
    sf::Vector2f pos;
    sf::Sprite sprite;
    GameTile(const sf::Texture*, float, float, bool, bool);
    GameTile(const sf::Texture*, float, float, int);
    bool setUpSprite(const sf::Texture*);
    bool setUpEntity(const sf::Texture*);
};

#endif /* Tile_hpp */
