//
//  Player.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "Player.hpp"

#include <cmath>
PlayerManager::PlayerManager(float x, float y){
    health = 100;
    money = 1000;
    money_ticks = 0;
    stress = 0;
    stress_ticks = 0;
//items.clear();
    state = 0;
   
    loadText(resourcePath() +"player_idle.png", &player_idle);
    
    loadText(resourcePath() +"player_walk1.png",&player_walk1);
 
    loadText(resourcePath() + "player_walk2.png", &player_walk2);
   
    scale = sf::Vector2f(0.5f,0.5f);
    velocity = sf::Vector2f(0.f,0.f);
    moveSpeed = 0.1f;
    maxVel = 0.3f;
    player.setTextureRect(sf::IntRect(8,0,16,30));
    player.setTexture(player_idle);
    player.setOrigin(8.f,15.f);
    player.setPosition(sf::Vector2f(x,y)); //256.f,450.f
    player.setScale(scale);
    canMoveLeft = true;
    canMoveRight = true;
    canMoveUp = true;
    canMoveDown = true;
    anim = false;
    shouldAnim = false;
    animationState = 0;
    items.reserve(10); //INV SIZE
    for (int i = 0; i < 10; i++ ){
        items.push_back(new Item("empty", i, -1));
    }
}

void PlayerManager::loadText(std::string name, sf::Texture *text_to){
    
    if(!(text_to->loadFromFile(name)))
    {
        std::cout << "oh fuck no texture " << name << std::endl;
        //fuck it return anyways;
    }
    
}

void PlayerManager::run(sf::Time elapsed, sf::Time delta, GameWorld* world){
    
    if(elapsed.asMilliseconds() >= 80){
        money_ticks++;
        stress_ticks++;
        shouldAnim = true;
    }
    else if (elapsed.asMilliseconds() < 80) shouldAnim = false;
    if(stress_ticks > 300){
        stress++;
        stress_ticks = 0;
    }
    if(money_ticks > 3000){
        money--;
        money_ticks = 0;
    }
   
   
   
    sf::Vector2f lastV = velocity;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && canMoveRight )
    {
        velocity.x += moveSpeed * delta.asMilliseconds();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && canMoveLeft)
    {
        velocity.x += moveSpeed * delta.asMilliseconds() * (-1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && canMoveUp )
    {
        velocity.y += moveSpeed * delta.asMilliseconds() * (-1);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && canMoveDown )
    {
        velocity.y += moveSpeed * delta.asMilliseconds();
    }
    //CLAMP, should also do for deltaV prev and angle , but rly should add decel
    velocity.x = ((velocity.x > maxVel) && (velocity.x > 0))  ? maxVel : velocity.x;
    velocity.x = ((velocity.x < (-1 * maxVel) ) && (velocity.x < 0))  ? (-1 * maxVel) : velocity.x;
    
    velocity.y = ((velocity.y > maxVel) && (velocity.y > 0))  ? maxVel : velocity.y;
    velocity.y = ((velocity.y < ( -1 * maxVel) ) && (velocity.y < 0))  ? (-1 * maxVel) : velocity.y;
   
    if (lastV == velocity) {
        velocity.x = (0.f < std::abs(velocity.x) < 0.03f) ?  (0.f) : (velocity.x * 0.5f);
        velocity.y = (0.f < std::abs(velocity.y) < 0.03f) ?  (0.f) : (velocity.y * 0.5f);
    }
    
    
    canMoveLeft = true;
    canMoveRight = true;
    canMoveUp = true;
    canMoveDown = true;
    sf::Sprite testMove;
    testMove.setTextureRect(sf::IntRect(8,0,16,30));
    testMove.setPosition(player.getPosition());
    testMove.move(velocity);
    for (auto const &row : world->tiles){
        for (auto const &tile : row){
            if(tile->isPassable == false){
                if(tile->sprite.getGlobalBounds().contains( testMove.getPosition() ) ) {
                    if(tile->sprite.getPosition().x < testMove.getPosition().x) //TILE TO LEFT OF P
                        velocity.x = (velocity.x < 0.f) ? 0.f : velocity.x;
                    if(tile->sprite.getPosition().x > testMove.getPosition().x) //TILE TO RIGHT OF P
                        velocity.x = (velocity.x > 0.f) ? 0.f : velocity.x;
                    if(tile->sprite.getPosition().y < testMove.getPosition().y) //TILE IS UP FROM P
                        velocity.y = (velocity.y < 0.f) ? 0.f : velocity.y;
                    if(tile->sprite.getPosition().y > testMove.getPosition().y) //TILE  IS DOWN FROM P
                        velocity.y = (velocity.y > 0.f) ? 0.f : velocity.y;
        
                }
            }
        }
    }
    float mapSize = 2048.f;
    float zoom = 0.1f;
    if(testMove.getPosition().x < 0.f) //BORDER TO LEFT OF P
        velocity.x = (velocity.x < 0.f) ? 0.f : velocity.x;
    if (testMove.getPosition().x > mapSize) //BORDER TO RIGHT OF P
        velocity.x = (velocity.x > 0.f) ? 0.f : velocity.x;
    if(testMove.getPosition().y < 0.f) //BORDER IS UP FROM P
        velocity.y = (velocity.y < 0.f) ? 0.f : velocity.y;
    if(testMove.getPosition().y > mapSize) //BORDER  IS DOWN FROM P
        velocity.y = (velocity.y > 0.f) ? 0.f : velocity.y;
    
    player.move(velocity);
    
    if(velocity.x < 0.f)
        player.setScale(scale.x * -1, scale.y);
    else
        player.setScale(scale.x * 1, scale.y);
    
    if( (std::abs(velocity.x) >= 0.03f || std::abs(velocity.y) >= 0.03f) && shouldAnim){
        switch (animationState){
            case 0:
                player.setTexture(player_walk1);
                animationState++;
                break;
            case 1:
                player.setTexture(player_walk2);
                animationState++;
                break;
            case 2:
                player.setTexture(player_walk1);
                animationState++;
                break;
            case 3:
                player.setTexture(player_idle);
                animationState = 0;
                break;
        }
    }
    else if (!shouldAnim && ( ( std::abs(velocity.x) <= 0.03f ) && ( std::abs(velocity.y) <= 0.03f ) ) ) {
        player.setTexture(player_idle);
        animationState = 0;
    }
 //   velocity = sf::Vector2f(0,0);
}




sf::Sprite PlayerManager::getSprite(){
    return player;
}



void PlayerManager::damage(int toSet){
    health -= toSet;
}
void PlayerManager::heal(int toSet){
    health += toSet;
}
int PlayerManager::getHealth(){
    return health;
}
void triggerEvent(int eventID){
    return;
}

int PlayerManager::getMoney(){
    return money;
}
int PlayerManager::getStress(){
    return stress;
}

void PlayerManager::addItem(std::string item_name, int item_slot, int item_id){
    try {
        items.pop_back();
        items.insert(items.begin() + item_slot, new Item(item_name, item_slot, item_id));
       // items.emplace(items.begin() + item_slot, new Item(item_name, item_slot, item_id));
    }
    catch (const std::out_of_range& e) {
        std::cout << "could not add item at " << item_slot << std::endl;
    }
}
void PlayerManager::removeItem(std::string item_name, int item_slot, int item_id){
   //
    try {
        items.erase(items.begin() + item_slot);
    }
    catch (std::exception& e) {
        std::cout << "could not remove item at " << item_slot << std::endl;
    }
}

void PlayerManager::render(sf::RenderWindow* window){
    window->draw(player);
}

Item *PlayerManager::getItem(int slot){
    try {
        return items.at(slot);
    }
    catch (const std::out_of_range& e) {
        std::cout << "no item at " << slot << " returning nullptr and breaking everything" << std::endl;
        std::exit(1);
        return nullptr; //greasy!
    }
    
}
int PlayerManager::inventorySize(){
    return items.size();
}

Item::Item(std::string item_name, int item_slot, int item_id){
    name = item_name;
    slot = item_slot;
    id = item_id;
    if(!texture.loadFromFile(resourcePath() + "modski.png"))
    {
        std::cout << "oh fuck no texture " << item_name << std::endl;
    
    }
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0,0,32,32));
    sprite.setPosition(sf::Vector2f(110.49f,949.f));
}

void Item::render(sf::RenderWindow* window){
    window->draw(sprite);
}
