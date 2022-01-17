//
//  World.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "ResourcePath.hpp"
#include "Tile.hpp"
class GameWorld {
    sf::Vector2i exitPos;
    sf::Vector2i playerPos;
    std::vector<sf::Vector2i> enemyPositions;
    void setUpInitialState();
    void setUpTiles();
    void setUpEnemyPositions();
    sf::Texture levelText;
    
    //entity
    sf::Texture lampText;
    public:
        std::vector<std::vector<GameTile * > > tiles;
        std::vector<GameTile * > world;
        int gridLength;
        GameWorld();
        sf::Texture lightmap;
};

#endif /* World_hpp */
