//
//  FileManager.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-11.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef FileManager_hpp
#define FileManager_hpp

#include <iostream>
#include <fstream>
#include "ResourcePath.hpp"
#include <CoreServices/CoreServices.h>

#define APP_FOLDER "/KingEngine"
#define WORLD_FOLDER "/Worlds"
#define SAVE_FOLDER "/Saves"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include <vector>
#include <string>


class FileManager {
private:
    std::string app_folder;
    char *supportPath;
public:
    FileManager();
    ~FileManager();
    bool saveToFile(std::string);
    bool saveWorld(rapidjson::Document*, std::string name);
    rapidjson::Document loadWorld(std::string name);
    void getFolders(const std::string& name, std::vector<std::string> &list);
    std::string getAppFolderPath();
    
    std::string makeWorldFolder(std::string name);
    
};


#endif /* FileManager_hpp */
