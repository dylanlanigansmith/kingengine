//
//  Development.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-09.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef Development_hpp
#define Development_hpp
#include <SFML/Graphics.hpp>
#include "imgui-SFML.h"
#include "imgui.h"


#include "includes.hpp"
#include "defines.hpp"

class FileManager;
/*\
_____________________________
|[Create World]
 (world settings)
|[Load World]
|(world_name)
|[Create Sprite]
|[Load Sprite]
|
|
[World Editor]
 
 
 
 
 */
enum DevGUIMode{
    GUISelectMode = 0,
    WorldCreator,
    WorldLoader,
    WorldEditor,
    ObjectEditor,
};

enum DevEditorMode{
    EditorSelectMode = 0,
    MoveMode,
    PaintMode,
    AddMode,
    RemoveMode
};

struct PaintCommand{
    int x, y;
    sf::Color prev_color;
};

class DevTools{
private:
    FileManager* fm;
    
    DevGUIMode GUIMode;
    DevEditorMode EditorMode;
    int EditorLayer;
    bool isGUIFocused;
    bool renderGUI;
    int toolMode;
    bool RunEditor;
    int worldOption;
    std::string activePath;
    sf::RectangleShape CurrentTile;
    struct {
        char worldName[50] = {"world name here"};
        int worldsize_x = 1024;
        int worldsize_y = 1024;
        int num_tiles = 32;
        int numOpts[3] = {worldsize_x, worldsize_y, num_tiles};
        float colorPicker[4] = { 0.5f,0.5f,0.5f,1.f};
        
    } CreatorOptions;
    
    //load
    std::vector<std::string> worldnames;
    int selectedWorld;
    
    //paint
    float colorPicker[4];
    bool showColorPicker;
    std::vector<std::string> tilenames;
    int selectedTile;
    int brushsize = 1;
    float zoom = 1.f;
    
    
    const char*  WorldTypes[3] =
    {
        "Tiled Side View",
        "Tiled Top View",
        "Development",
    };
    const char*  WorldLayers[3] =
    {
        "Background",
        "WorldObjects",
        "Entities",
    };
    sf::Color arrayToColor(float*);
    
    std::stack<PaintCommand> paintundo;
public:
    DevTools(int);
    void runGUI();
    ~DevTools();
    void render(sf::RenderTarget& target);
    
};
#endif /* Development_hpp */
