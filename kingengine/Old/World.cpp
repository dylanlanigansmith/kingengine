//
//  World.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "World.hpp"
#include <math.h>
#define AMBOFFSET 32


GameWorld::GameWorld(){
    gridLength = 64;
 
    setUpInitialState();
    
}

void GameWorld::setUpInitialState(){
    exitPos = sf::Vector2i(1,0);
    playerPos = sf::Vector2i(gridLength -1, gridLength - 1);
    setUpEnemyPositions();
    setUpTiles();
    
}

void GameWorld::setUpEnemyPositions(){
    enemyPositions.clear();
    enemyPositions.push_back(sf::Vector2i(0,2));
    enemyPositions.push_back(sf::Vector2i(6,0));
    enemyPositions.push_back(sf::Vector2i(2,7));
}

void GameWorld::setUpTiles(){
 
    /*
    tiles.clear();
    tiles.resize(gridLength);
    for (int i = 0; i < gridLength; i++){
        tiles[i].resize(gridLength);
        for (int j = 0; j < gridLength; j++){
            float x = j * 32.f;
            float y = i * 32.f;
            if(map[i][j] == 'G')
                tiles[i][j] = (new GameTile("grass.png",x, y, true, false)); //dont load same texture 1 million times
            else if(map[i][j] == 'C')
                tiles[i][j] = (new GameTile("concrete.png",x, y, true, false));
            else if(map[i][j] == 'W')
                tiles[i][j] = (new GameTile("wood.png",x, y, false, false));
            else if(map[i][j] == 'B')
                tiles[i][j] = (new GameTile("brick.png",x, y, false, false));
            else if(map[i][j] == 'D')
                tiles[i][j] = (new GameTile("dirt.png",x, y, true, false));
        }
    }    */
    //NEW from 2048*2048
   
    if(!levelText.loadFromFile(resourcePath() + "levelfullres.png"))
   {
       std::cout << "oh fuck no level texture";
       
   }
    
    //levelText.setSmooth(true);
    
    sf::Texture white;
    if(!white.loadFromFile(resourcePath() + "white.png"))
   {
       std::cout << "oh fuck no white texture";
       
   }
    
    if(!lampText.loadFromFile(resourcePath() + "lamp.png"))
   {
       std::cout << "oh fuck no lamp texture";
       
   }
  
    sf::Image worldLayer;
    if(!worldLayer.loadFromFile(resourcePath() + "world.png"))
   {
       std::cout << "oh fuck no level texture for world";
       
   }
    world.clear();
    sf::Vector2u worldSize = worldLayer.getSize();
    /*
    world.resize(32); //max ents/props for now, trying to do this way so i can do auto for in to render
    for(int i = 0; i < 32; i++){
        world[i].resize()
    }*/
    //tbh u could literally draw this onto the level for fixed stuff
    
    //go thru pixels and do certain thing per color, append to worlds if color,
    //Better way would be make an editor, load to JSON List of Entity, positio
    //LAMP sf::Color(15,200,200)
    
    for(int i = 0; i < worldSize.y; i++){//rows
        for(int j = 0; j < worldSize.x; j++){//cols
            sf::Color tC = worldLayer.getPixel(j, i); //use x,y next time as var names
            if(((14 < tC.r < 16) && (198 < tC.b < 201) && (198 < tC.g < 201) && (tC.a > 150))){
                
                world.push_back(new GameTile(&lampText, j, i, 0));
            }
            
        }
    }
    sf::Image lightLayer;
    if(!lightLayer.loadFromFile(resourcePath() + "light.png")){
        //all white worldtexture size img
       // lightLayer.create(2048,2048, sf::Color(0,0,0));
        //ok hear me out:
        //for every thing in world, draw circle around it..  for now.. then do gradients and then just draw that shit over with darkmode on -> move to class of its own, start passing window pointer..
        sf::RenderTexture lm;
        //tbh could just do this all with a render texture and not a image, but would b nice to "bake" them too / use in PS
        lm.create(2048,2048,false); //width, height, depth buffer
        lm.clear(sf::Color(50,50,75,100));
        for(auto const &entity : world){
            sf::Vector2f origin = entity->sprite.getPosition();
            sf::CircleShape cover(16);
            cover.setPointCount(12);
            sf::Vector2i pixO = lm.mapCoordsToPixel(origin);
            cover.setPosition(pixO.x,pixO.y);
            cover.setFillColor(sf::Color(253,187,140,100));
            sf::Color ambColor = sf::Color(253,213,190,90);
            sf::Color ambColor2 = sf::Color(15,15,15,90);
            
            pixO.x += (AMBOFFSET / 2);
            sf::VertexArray ambient(sf::Quads,4);

            ambient[0].position = sf::Vector2f(pixO.x - AMBOFFSET, pixO.y);
            ambient[0].color = ambColor;
            
            ambient[1].position = sf::Vector2f(pixO.x + AMBOFFSET, pixO.y);
            ambient[1].color = ambColor;
            
            ambient[2].position = sf::Vector2f(pixO.x + AMBOFFSET, pixO.y + AMBOFFSET);
            ambient[2].color = ambColor2;
            
            ambient[3].position = sf::Vector2f(pixO.x - AMBOFFSET, pixO.y + AMBOFFSET);
            ambient[3].color = ambColor2;
       //pos wrong but it works
        
            
            lm.draw(cover);
            lm.draw(ambient,sf::RenderStates(sf::BlendAdd));
            
            
            //lightLayer.setPixel((int)origin.x, (int)origin.y, sf::Color(15,200,200));
        }
        lm.display();
        lightLayer =  lm.getTexture().copyToImage();
                 
        lightLayer.saveToFile(resourcePath() + "light.png");
    }
    
    if(!lightmap.loadFromFile(resourcePath() + "light.png")){
        std::cout << "no lightmap" << std::endl;
    }
    
    
    std::cout << "found " << world.size() << " entities" << std::endl;
    
    sf::Image img;
    if(!img.loadFromFile(resourcePath() + "image.png"))
   {
       std::cout << "oh fuck no level texture for img";
       
   }
    tiles.clear();
    tiles.resize(gridLength);
    for (int i = 0; i < gridLength; i++){
        tiles[i].resize(gridLength);
        for (int j = 0; j < gridLength; j++){
            float x = j * 32.f;
            float y = i * 32.f;
            bool canWalk = true;
            int red = 0;
            int sum = 0;
            int its =0;
            for(int k = 0; k < 32; k++){
                for(int l = 0; l < 32; l++){
                    if( (img.getPixel(x + l,y + k).r > 250) && (img.getPixel(x + l,y + k).g < 250))
                    red++;
                    its++;
                }
            }
            
            if(red < (512)){
                canWalk = false;
               
            }
            tiles[i][j] = (new GameTile(&levelText,x, y, canWalk, false));
              //  tiles[i][j] = (new GameTile(&white,x, y, canWalk, false)); //use to debug
            
        }
    }
}
