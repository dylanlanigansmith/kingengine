//
//  GameEngine.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef GameEngine_hpp
#define GameEngine_hpp
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "imgui-SFML.h"
#include "imgui.h"
#include "includes.hpp"
#include "defines.hpp"

#include "BaseRenderable.hpp"
#include "Development.hpp"
#include "WorldManager.hpp"



class GameEngine {
private:
    sf::RenderWindow* window;
    sf::RenderTexture layer0;
    sf::View player_view;
    int winX;
    int winY;
    
    ImGuiWindowFlags window_flags;
    bool renderImGui;
    ImGuiIO imIO;
    sf::Font defaultFont;
    sf::Vector2i mouseposition;
    //move to dev
   
    
    sf::Clock render_clock; //gui, events, etc
    sf::Time rt;
    sf::Clock game_clock; //physics, animations, etc
    sf::Clock real_clock; //day night, pts decreasing over real time, etc 
                            //maybe some sort of tick between the 2 that equalizes them?
    
    
    //TextureManager textures;
    bool run;
    void gui();
 
    void debugGUI(int mode = 0);
    void debug(int mode = 0);
    friend class DevTools;
    const std::vector< std::vector< Renderable*> >* getList();
    const std::vector< Renderable*>* getListAtLayer(int);
    WorldManager* world;
    DevTools* dev;
public:
    std::stack<WorldManager*> worlds;
    WorldManager* ActiveWorld(){
        if(world->createdWorld)
            return world;
    }
    GameEngine();
    ~GameEngine();
    [[nodiscard]] bool setup();
    bool shouldQuit();
    void loop();
  
    sf::Vector2i getMousePosition();
    sf::Vector2f getWorldMousePosition();
    bool getLeftClick();
    bool isPointInCurrentView(sf::Vector2f);
    void moveView(sf::Vector2f);
    sf::View getView(){
        return window->getView();
    }
};
extern GameEngine* engine;

#endif /* GameEngine_hpp */
