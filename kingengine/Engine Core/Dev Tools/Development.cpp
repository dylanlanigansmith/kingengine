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

#include "LightingManager.hpp"
DevTools::DevTools(int setMode){
    GUIMode = GUISelectMode;
    toolMode = setMode;
    RunEditor = false;
    renderGUI = true;
    worldOption = 0;
    
    selectedWorld = 0;
    selectedTile = 1;
    showColorPicker= false;
  
    CurrentTile.setFillColor(sf::Color(15.f,15.f,15.f));
    CurrentTile.setSize(sf::Vector2f(128.f,128.f));
    fm = new FileManager();
    fm->getFolders("", worldnames);
    activePath = resourcePath();
    
    enableLighting = false;
    isAddingLight = false;
    c.setRadius(64.f);
    c.setFillColor(sf::Color::Yellow);
    
    
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
            CurrentTile.setPosition(mouse_posf);
            
            target.draw(CurrentTile);
            
            //adding tilemap support to this is another reason to merge tilemapinfo into the asset manager
            
        }
    }
    if(EditorMode == LightingEditor) {
        if(isAddingLight && !isGUIFocused){
            sf::Vector2f mouse_pos = engine->getWorldMousePosition();
            
            c.setFillColor(sf::Color::Yellow);
            c.setPosition(mouse_pos);
            target.draw(c);
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
    if(GUIMode == WorldLoader){
        ImGui::SetNextWindowPos(ImVec2( WIN_X / 4,WIN_Y / 4));
        ImGui::SetNextWindowSize(ImVec2(WIN_X / 2, WIN_Y / 2));
        ImGui::Begin("World Loader", &renderGUI);
        std::string error = "waiting";
        if(ImGui::Button("refresh")){
            fm->getFolders("", worldnames);
        }
        ImGui::Combo("Select Level", &selectedWorld, worldnames);
        ImGui::Separator();
        
        char str[255];
        if(selectedWorld < worldnames.size())
            sprintf(str, "Load World: %s", worldnames.at(selectedWorld).c_str());
        else
            sprintf(str, "Load World: %s", "select a world");
        if(ImGui::Button(str)){
            std::string name = worldnames.at(selectedWorld);
            std::string worldPath = fm->makeWorldFolder(name);
            //actually figure out what i wanna save/load
            if(engine->world->CreateWorld(Tiled, CreatorOptions.worldsize_x, CreatorOptions.worldsize_y, CreatorOptions.num_tiles, name, worldPath)){
                    GUIMode = WorldEditor;
                activePath = worldPath;
                //will crash without tilemap and tile save file moved into world folder 
                engine->world->getTileMap().getInfo()->getTileNames(tilenames);
            }
        }
        ImGui::Text("Status: %s", error.c_str());
        if(ImGui::Button("Back"))
            GUIMode = GUISelectMode;
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
                    engine->world->getTileMap().getInfo()->getTileNames(tilenames);
                }
                
            }
        }
        ImGui::Text("Error: %s", error.c_str());
        if(ImGui::Button("Back"))
            GUIMode = GUISelectMode;
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
        if(ImGui::Button("Lighting")){
            EditorMode = LightingEditor;
        }
        
        if(ImGui::Button("SAVE")){
            engine->world->getTileMap().getInfo()->save();
            
        }
        if(EditorMode == LightingEditor){
            ImGui::Text("Lighting Editor");
            ImGui::Separator();
            ImGui::Checkbox("Enable Lighting", &enableLighting);
            if(ImGui::Button("Add Light Source")){
                isAddingLight = true;
            }
            if(isAddingLight){
                ImGui::Text("Adding light at next mouse click");
                if(engine->getLeftClick() && !isGUIFocused){
                    LightSource toAdd;
                    toAdd.setIntensity(lightOpts[0]);
                    toAdd.setFalloff(lightOpts[1]);
                    toAdd.setRadius(lightOpts[2]);
                    toAdd.setColor(sf::Color::White);
                    //IFR
                    engine->world->getLightingManager().setShadow(shadowOpts[0], shadowOpts[1], shadowOpts[2]);
                    if(showColorPicker){
                        toAdd.setColor(arrayToColor(colorPicker));
                    }
                    toAdd.setPosition(engine->getWorldMousePosition());
                    engine->world->getLightingManager().addLightSource(toAdd);
                    engine->world->getLightingManager().update();
                    isAddingLight = false;
                }
            }
            ImGui::Text("255 - (R * F + I");
            ImGui::InputFloat3("I, F, R", lightOpts);
            ImGui::Text("ang=I, 255*F, itr=R");
            ImGui::InputFloat3("Shadow", shadowOpts);
            if(ImGui::Button("Update Lightmap")){
                engine->world->getLightingManager().update();
                engine->world->getLightingManager().setDebug(drawRays);
            }
            if(ImGui::Button("Clear Lightmap")){
                engine->world->getLightingManager().clearLights();
            }ImGui::SameLine();
            if(ImGui::Button("Clear Edges")){
                engine->world->getLightingManager().clearEdges();
            }ImGui::SameLine();
            ImGui::Checkbox("Draw Rays", &drawRays);
            ImGui::Text("Number of lights %i",  engine->world->getLightingManager().numberOfLights());
            ImGui::Checkbox("Show Color Picker", &showColorPicker);
            if(showColorPicker){
                ImGuiColorEditFlags flags = 0;
                flags = flags | ImGuiColorEditFlags_NoAlpha;
            
     ImGui::ColorPicker4("Tile Color", colorPicker, flags);
            }
            
        }
        if(EditorMode == MoveMode){
            ImGui::Text("Move");
            ImGui::Separator();
            std::string viewPos = std::to_string(engine->player_view.getCenter().x) + " " + std::to_string(engine->player_view.getCenter().y);
            ImGui::Text("%s", viewPos.c_str());
            
            if(ImGui::Button("Zoom +")){
                engine->player_view.zoom(0.8f);
            }ImGui::SameLine();
            if(ImGui::Button("Zoom -")){
                engine->player_view.zoom(1.2f);
            }ImGui::SameLine();
            if(ImGui::Button("Reset Zoom")){
                engine->player_view = engine->window->getDefaultView();
            }
        
            sf::Vector2f velocity = sf::Vector2f(0.f,0.f);
            float moveSpeed = 5.f;
    
            if (ImGui::Button("<-")  )
            {
                velocity.x += moveSpeed  * (-1) ;
            }ImGui::SameLine();
            if (ImGui::Button("-->") )
            {
                velocity.x += moveSpeed ;
            }
            if (ImGui::Button("/\\"  ))
            {
                velocity.y += moveSpeed * (-1) ;
            }ImGui::SameLine();
            if (ImGui::Button("\\/")  )
            {
                velocity.y += moveSpeed ;
            }
            engine->moveView(velocity);
        }
        if(EditorMode == EditorSelectMode ){
            ImGui::Text("Select");
            
        }
        if(EditorMode == PaintMode ){
            ImGui::Text("Paint");
            ImGui::Separator();
            ImGui::Combo("Layer", &worldOption, WorldLayers, 3);
            
            ImGui::SliderInt("brush size (1-10)", &brushsize, 0, 10); //do with rect intersects
            
            
            ImGui::Combo("Select Tile", &selectedTile, tilenames);
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
                    //gridy/x = win_x / 32 respectively
                    
                    engine->world->getTileMap().getInfo()->update(mouse_posf.x, mouse_posf.y, tilenames.at(selectedTile));
                    if(brushsize > 1){ //NOT ELEGANT
                        if(brushsize == 2){
                            engine->world->getTileMap().getInfo()->update(mouse_posf.x + (30), mouse_posf.y, tilenames.at(selectedTile));
                        }
                        else if(brushsize == 3){
                            for (int i = 1; i <= 15; i++){
                            engine->world->getTileMap().getInfo()->update(mouse_posf.x + i*(30), mouse_posf.y, tilenames.at(selectedTile));
                            engine->world->getTileMap().getInfo()->update(mouse_posf.x - i*(30), mouse_posf.y, tilenames.at(selectedTile));
                            engine->world->getTileMap().getInfo()->update(mouse_posf.x, mouse_posf.y - i*(30), tilenames.at(selectedTile));
                            engine->world->getTileMap().getInfo()->update(mouse_posf.x , mouse_posf.y + i*(30), tilenames.at(selectedTile));
                            }

                        }
                        else if(brushsize > 3){ //redundant
                            for(int i = 1; i <= (brushsize -3); i++){
                               //same row
                                engine->world->getTileMap().getInfo()->update(mouse_posf.x + (i*30), mouse_posf.y, tilenames.at(selectedTile));
                                engine->world->getTileMap().getInfo()->update(mouse_posf.x - (i*30), mouse_posf.y, tilenames.at(selectedTile));
                                //top row
                                engine->world->getTileMap().getInfo()->update(mouse_posf.x + (i*30), mouse_posf.y + (i*30), tilenames.at(selectedTile));
                                engine->world->getTileMap().getInfo()->update(mouse_posf.x - (i*30), mouse_posf.y + (i*30), tilenames.at(selectedTile));
                                engine->world->getTileMap().getInfo()->update(mouse_posf.x, mouse_posf.y + (i*30), tilenames.at(selectedTile));
                                //bottom row
                                engine->world->getTileMap().getInfo()->update(mouse_posf.x + (i*30), mouse_posf.y - (i*30), tilenames.at(selectedTile));
                                engine->world->getTileMap().getInfo()->update(mouse_posf.x - (i*30), mouse_posf.y - (i*30), tilenames.at(selectedTile));
                                engine->world->getTileMap().getInfo()->update(mouse_posf.x, mouse_posf.y - (i*30), tilenames.at(selectedTile));
                            }
                        }
                    }
                    engine->world->getTileMap().load(sf::Vector2u(32,32));
                   // engine->world->getTileManager().setTileColor(sf::Color(colorPicker[0] * 255,colorPicker[1] * 255,colorPicker[2] * 255, 255), mouse_posf.x,mouse_posf.y);
                }
            }
        }
        
        //finish base shit 
        //NEXT: Sprite adding
     
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
