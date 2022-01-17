//
//  Development.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "Development.hpp"
#include "GameEngine.hpp"
#include "FileManager.hpp"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "custom_imgui.h"
extern GameEngine* engine;

DevTools::DevTools(int setMode){
    GUIMode = GUISelectMode;
    toolMode = setMode;
    RunEditor = false;
    renderGUI = true;
    worldOption = 0;
    showColorPicker= false;
  
    CurrentTile.setFillColor(sf::Color(15.f,15.f,15.f));
    CurrentTile.setSize(sf::Vector2f(128.f,128.f));
    fm = new FileManager();
   
    activePath = resourcePath();
  
    
}
DevTools::~DevTools(){
    delete fm;
}

void DevTools::render(sf::RenderTarget& target){
    if(EditorMode == PaintMode){
        //Render World
        if(!isGUIFocused)
        {
            sf::Vector2i mouse_pos = engine->getMousePosition();
            CurrentTile.setFillColor(sf::Color(colorPicker[0] * 255,colorPicker[1] * 255,colorPicker[2] * 255, 255));
            float size = CreatorOptions.worldsize_x / CreatorOptions.num_tiles;
            CurrentTile.setSize(sf::Vector2f(size,size));
            sf::Vector2f mouse_posf =engine->window->mapPixelToCoords(mouse_pos);
            CurrentTile.setPosition(mouse_posf.x,mouse_posf.y);
            
            target.draw(CurrentTile);
            
            //create tile world,
        }
    }
}

void DevTools::runGUI(){
    if(GUIMode==GUISelectMode){
        ImGui::SetNextWindowPos(ImVec2(0,0));
        ImGui::SetNextWindowSize(ImVec2(WIN_X, 100));
        ImGui::Begin("DevTools", &renderGUI);
        if(ImGui::Button("Create World")){
            GUIMode = WorldCreator;
        }
        if(ImGui::Button("Load World")){
            GUIMode = WorldLoader;
        }
        if(ImGui::Button("Edit Object")){
            GUIMode = ObjectEditor;
        }
        ImGui::End();
    }
    if(GUIMode==WorldCreator){

        ImGui::SetNextWindowPos(ImVec2( WIN_X / 4,WIN_Y / 4));
        ImGui::SetNextWindowSize(ImVec2(WIN_X / 2, WIN_Y / 2));
        ImGui::Begin("World Creator", &renderGUI);
        ImGui::InputText("worldname", CreatorOptions.worldName, sizeof(CreatorOptions.worldName));
        ImGui::Combo("World Type", &worldOption, WorldTypes, 3);
        ImGui::InputInt3("Size X,Y, #Tiles", CreatorOptions.numOpts);
        if(CreatorOptions.numOpts[0] > 16 && CreatorOptions.numOpts[1] > 16 && CreatorOptions.numOpts[2] >= 2){
            CreatorOptions.worldsize_x = CreatorOptions.numOpts[0];
            CreatorOptions.worldsize_y = CreatorOptions.numOpts[1];
            CreatorOptions.num_tiles = CreatorOptions.numOpts[2];
            
        }
        ImGui::ColorPicker4("Background Layer Color", CreatorOptions.colorPicker);
        std::string error = "waiting";
        if(ImGui::Button("Create World")){
          
            if(CreatorOptions.worldsize_x < 16 || CreatorOptions.worldsize_y < 16 || std::string(CreatorOptions.worldName).length() < 1){
                error = "invalid options";
            }
            else{
                error = "creating";
                std::string worldPath = fm->makeWorldFolder(CreatorOptions.worldName); //ends with slash
                sf::Image layer;
                layer.create(CreatorOptions.worldsize_x, CreatorOptions.worldsize_y, arrayToColor(CreatorOptions.colorPicker));
                layer.saveToFile(worldPath + "layer0.png");
                
               
                layer.create(CreatorOptions.worldsize_x, CreatorOptions.worldsize_y, sf::Color(0,0,0,0));
                layer.saveToFile(worldPath + "layer1.png");
                rapidjson::Document worldDoc;
                //https://rapidjson.org/md_doc_tutorial.html#ValueDocument
                //FUCK JSON
                //FUCK STATIC LIBRARIES
                //FUCK YOU
                //this works now..
                worldDoc.SetObject();
                worldDoc.AddMember("worldName","worldname",worldDoc.GetAllocator());
                worldDoc["worldName"].SetString(rapidjson::StringRef(std::string(CreatorOptions.worldName).c_str()));
                worldDoc.AddMember("worldX",CreatorOptions.worldsize_x,worldDoc.GetAllocator());
                worldDoc.AddMember("worldY",CreatorOptions.worldsize_x,worldDoc.GetAllocator());
                worldDoc.AddMember("numTiles",CreatorOptions.num_tiles,worldDoc.GetAllocator());
                activePath = worldPath;
                fm->saveWorld(&worldDoc,worldPath + "worldinfo.json");
                //create actual world, load, switch to editor mode
                if(engine->world->CreateWorld(Tiled, CreatorOptions.worldsize_x, CreatorOptions.worldsize_y, CreatorOptions.num_tiles, CreatorOptions.worldName, worldPath)){
                        GUIMode = WorldEditor;
                    activePath = worldPath;
                }
                
            }
        }
        ImGui::Text(error.c_str());
        
        ImGui::End();
    }
    if(GUIMode == WorldEditor){
        ImGui::SetNextWindowPos(ImVec2(0,WIN_Y /3 ));
        ImGui::SetNextWindowSize(ImVec2(WIN_X / 4, WIN_Y / 2));
        ImGui::Begin("World Creator", &renderGUI);
        isGUIFocused = ImGui::IsWindowFocused();
        if(ImGui::Button("Select"))
            EditorMode = EditorSelectMode;
        if(ImGui::Button("Move"))
            EditorMode = MoveMode;
        if(ImGui::Button("Paint Tile"))
            EditorMode = PaintMode;
        
        
        if(ImGui::Button("SAVE"))
            engine->world->SaveWorld(activePath + "layer0.png");
        
        if(EditorMode == MoveMode){
            ImGui::Text("Move");
            std::string viewPos = std::to_string(engine->player_view.getCenter().x) + " " + std::to_string(engine->player_view.getCenter().y);
            ImGui::Text(viewPos.c_str());
            engine->player_view.zoom((static_cast<float>(static_cast<int>(zoom * 10.)) / 10.)); //greasy
            ImGui::SliderFloat("Zoom", &zoom, 0.1f, 2.f);
            sf::Vector2f velocity = sf::Vector2f(0.f,0.f);
            float moveSpeed = 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) )
            {
                velocity.x += moveSpeed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) )
            {
                velocity.x += moveSpeed  * (-1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) )
            {
                velocity.y += moveSpeed * (-1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) )
            {
                velocity.y += moveSpeed;
            }
            engine->moveView(velocity);
        }
        if(EditorMode == EditorSelectMode ){
            ImGui::Text("Select");
        }
        if(EditorMode == PaintMode ){
            ImGui::Text("Paint");
            ImGui::Separator();
            ImGui::Combo("Layer", &worldOption, WorldTypes, 3);
            int size = 500;
            ImGui::SliderInt("brush size (0-1000%)", &size, 0, 1000); //do with rect intersects
            std::vector<std::string> tilenames; engine->world->getTileMap().getInfo()->getTileNames(tilenames);
            int idx;
            ImGui::Combo("Select Tile", &idx, tilenames);
            ImGui::Checkbox("Show Color Picker", &showColorPicker);
            if(showColorPicker){
                ImGuiColorEditFlags flags = 0;
                flags = flags | ImGuiColorEditFlags_NoAlpha;
     ImGui::ColorPicker4("Tile Color", colorPicker, flags);
            }
            if(engine->getLeftClick()){
                if(!isGUIFocused){
                    sf::Vector2i mouse_pos = engine->getMousePosition();
                    //fix
                    sf::Vector2f mouse_posf =engine->window->mapPixelToCoords(mouse_pos);
                   // engine->world->getTileManager().setTileColor(sf::Color(colorPicker[0] * 255,colorPicker[1] * 255,colorPicker[2] * 255, 255), mouse_posf.x,mouse_posf.y);
                }
            }
            
        }
        //finish base shit 
        //NEXT: Sprite adding
        // tile texture map changing
        //SAVING
        //LOADING FROM SAVE
        
        //then we back to what started this whole mess LMAO:
        //then: lighting and render pipeline
        //then: clock
        
        ImGui::End();
    }
    
    
    
   
}

sf::Color DevTools::arrayToColor(float * array){
    int r,g,b,a;
    r = ((array[0] * 255) > 255) ? 255 : array[0] * 255;
    b = ((array[1] * 255) > 255) ? 255 : array[1] * 255;
    g = ((array[2] * 255) > 255) ? 255 : array[2] * 255;
    a = ((array[3] * 255) > 255) ? 255 : array[3] * 255;
    return sf::Color(r,b,g,a);
}
