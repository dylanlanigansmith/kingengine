//
//  GameEngine.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//




#include "GameEngine.hpp"

#include "Tiles.hpp"
GameEngine* engine;


GameEngine::GameEngine(){
    winX = WIN_X;
    winY = WIN_Y;
    run = true;
    window_flags = 0;
    //IMGUI WINDOW FLAGS
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    renderImGui = true;
    

 
    
}

GameEngine::~GameEngine(){
        //delete stuff u created
    delete world;
    delete window;
    
}

bool GameEngine::setup(){
    //https://bromeon.ch/articles/raii.html
    window = new sf::RenderWindow(sf::VideoMode(winX, winY),WIN_TITLE,sf::Style::Titlebar | sf::Style::Close);
    window->setVerticalSyncEnabled(WINOPT_VSYNC);
    player_view = window->getDefaultView();
    if(!window->isOpen()){
            logger.fatal_error("Could not open window"); return false;
    }
    logger << "Window (" << winX << "x" << winY << "): OK"<< lm::endl;
        
    if(!ImGui::SFML::Init(*window)){
        logger.fatal_error("ImGui Init Failed"); return false;
    }
    else
        logger.silent("ImGui Init Successful");
    imIO = ImGui::GetIO();
    if(!defaultFont.loadFromFile(resourcePath() + WIN_DEFAULTFONT)){
      logger.error("Could not load font: " + std::string(WIN_DEFAULTFONT));
      return false;
    }
    else imIO.Fonts->AddFontFromFileTTF(std::string(resourcePath() + WIN_DEFAULTFONT).c_str(), 12.f);
    if(!ImGui::SFML::UpdateFontTexture())
        logger.error("Could not update IMGUI font");
    logger << "ImGui/Fonts OK" << lm::endl;
    
    dev = new DevTools(0);
    //CREATE WORLD
    logger.silent("Creating Default WorldManager");
    worlds.push(new WorldManager());
    world = worlds.top();
    if(world->world_name.length() > 2)
        logger << "WorldManager: OK" << lm::endl;
    
   // world->CreateWorld(DefaultWorld, 2048, 2048, 64, "Test World", "savedworld.png");

    //INIT SETTINGS FOR WORLD
//    logger << "World: Loading Level and Settings" << lm::endl;
    
    //INIT ENTITIES & PLAYER
  //  log("World: Init Entities & Player");
    
   // log("Player: All good");
    
    logger << "Engine: OK" << lm::endl;
    
    return true;
}
void GameEngine::loop(){
    while(window->isOpen()){
        
        //Check for System Window Events or Exit
        sf::Event event;
        while (window->pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(*window, event);
            // Close window->: exit
            if (event.type == sf::Event::Closed) {
                window->close();
                run = false;
            }

            // Escape pressed: exit FORNOW
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window->close();
                run = false;
            }
        }
        if(!run)
            break;
        //lets do some input
        
        
        //see where tf that should go
        mouseposition = sf::Mouse::getPosition(*window);
        
        
       
        //gui and debug, most should get moved to devtools//GUI mgr should own devtolls
        ImGui::SFML::Update(*window, game_clock.restart());
        gui();
      
        window->clear();
        
        /*
        for(auto const &layer : *getList()){
            for( auto const &toRender : layer ) {
                toRender->draw(*window);
            }
        }*/
       
        
        for( auto const &toRender : *getListAtLayer(RENDER_BASELAYER) ) {
            toRender->draw(*window);
        }
        if(dev->displayLighting()){
            //world->getLightingManager().update();
            for( auto const &toRender : *getListAtLayer(RENDER_LIGHTLAYER) ) {
                toRender->draw(*window);
            }
        }
        
        debug(0);//run dev rendering
        
        
        ImGui::SFML::Render(*window);
        window->setView(player_view);
        window->display();
        
    }
    
    ImGui::SFML::Shutdown();
    
    return;
}
bool GameEngine::shouldQuit(){
    return !run;
}


void GameEngine::gui(){
    //if debug
    debugGUI(0);
}
void GameEngine::debugGUI(int mode){
    dev->runGUI();
}

void GameEngine::debug(int mode){
    dev->render(*window);
}

const std::vector< std::vector< Renderable*> >* GameEngine::getList(){
    return &world->renderables;
}
const std::vector< Renderable*>* GameEngine::getListAtLayer(int index){
    return &world->renderables[index];
}

bool GameEngine::isPointInCurrentView(sf::Vector2f point){
    sf::Vector2f viewCenter(window->getView().getCenter());
    sf::Vector2f viewSize(window->getView().getSize());
    sf::Rect<float> currentView (sf::Vector2f(viewCenter.x - viewSize.x / 2, // left
                                viewCenter.y - viewSize.y / 2), // top
                               sf::Vector2f( viewSize.x,
                                viewSize.y) );
    return (currentView.contains(point));
    
}

sf::Vector2i GameEngine::getMousePosition(){
    return sf::Mouse::getPosition(*window);
}
sf::Vector2f GameEngine::getWorldMousePosition(){
    return window->mapPixelToCoords(sf::Mouse::getPosition(*window));
}
bool GameEngine::getLeftClick(){
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void GameEngine::moveView(sf::Vector2f vel){
    player_view.move(vel);
}
    
