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
    window = new sf::RenderWindow(sf::VideoMode(winX, winY),WIN_TITLE);
    window->setVerticalSyncEnabled(WINOPT_VSYNC);
    player_view = window->getDefaultView();
    
    log(std::string("Created Window: ") + std::to_string(winX) + std::string("x") + std::to_string(winY));
    ImGui::SFML::Init(*window);
    imIO = ImGui::GetIO();
    if(!defaultFont.loadFromFile(resourcePath() + WIN_DEFAULTFONT)){
        log(std::string("Could not load: ") + WIN_DEFAULTFONT);
        
        return false;
    }
    else imIO.Fonts->AddFontFromFileTTF(std::string(resourcePath() + WIN_DEFAULTFONT).c_str(), 12.f);
    ImGui::SFML::UpdateFontTexture();
    log("Initialized Fonts and ImGui");
    dev = new DevTools(0);
    //CREATE WORLD
    log("Creating World ");
    worlds.push(new WorldManager());
    world = worlds.top();
    
    
    
   // world->CreateWorld(DefaultWorld, 2048, 2048, 64, "Test World", "savedworld.png");

    //INIT SETTINGS FOR WORLD
    log("World: Loading Level and Settings");
    
    //INIT ENTITIES & PLAYER
    log("World: Init Entities & Player");
    
    log("Player: All good");
    
    log("Starting Loop");
    
   
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
            }
        }
        if(run == false) break;
        //lets do some input
        
        
        //see where tf that should go
        mouseposition = sf::Mouse::getPosition(*window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            TileManager& tile_mgr = (TileManager&) *(world->renderables[RENDER_BASELAYER][TILEMANAGER_INDEX]);
            
          
           
            
        }
        
        //gui and debug, most should get moved to devtools
        ImGui::SFML::Update(*window, game_clock.restart());
       
        gui();
        window->clear();
        
        
       
       
        
        for( auto const &toRender : *getListAtLayer(RENDER_BASELAYER) ) {
            toRender->draw(*window);
        }
        debug(0);
        
        
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

void GameEngine::log(std::string msg){
    std::string toLog = msg;
    std::cout << toLog << std::endl;
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
bool GameEngine::getLeftClick(){
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void GameEngine::moveView(sf::Vector2f vel){
    player_view.move(vel);
}
    
