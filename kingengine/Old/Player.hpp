//
//  Player.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "ResourcePath.hpp"
#include "World.hpp"

class Item{
    int slot;
    int id;
    
    sf::Texture texture;
public:
    int state; //0 = ground,
    std::string name;
    sf::Sprite sprite;
    void render(sf::RenderWindow* window);
    Item(std::string item_name, int item_slot, int item_id);
};
class PlayerManager {
    sf::Vector2f scale;
    float moveSpeed;
    float maxVel;
    int health; //0-100hp
    int state;
    int money; //starting 1000$
    int money_ticks;
    int stress; //0-100, things like beer, completing tasks, lower
    int stress_ticks;
    std::vector<Item *> items;
    void loadText(std::string name, sf::Texture *text_to);
    sf::Texture player_idle;
    sf::Texture player_walk1;
    sf::Texture player_walk2;
    sf::Vector2f velocity;
    bool anim;
    bool shouldAnim;
    int animationState;
public:
    sf::Sprite player; //NONE TOUCH THIS
    bool canMoveLeft;
    bool canMoveRight;
    bool canMoveUp;
    bool canMoveDown;
    PlayerManager(float x, float y);
    void run(sf::Time elapsed, sf::Time delta, GameWorld* world);
    sf::Sprite getSprite();
    void damage(int);
    void heal(int);
    int getHealth();
    int getStress();
    int getMoney();
    void triggerEvent(int);
    void addItem(std::string item_name, int item_slot, int item_id);
    void removeItem(std::string item_name, int item_slot, int item_id);
    Item *getItem(int slot);
    int inventorySize();
    void render(sf::RenderWindow* window);
};


#endif /* Player_hpp */
