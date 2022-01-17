//
//  FileManager.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-11.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "FileManager.hpp"
#include "FolderManager.h"
#include "defines.hpp"


//Wrapper for system specific file handlers
#ifdef MAC_ARM64
using namespace fm;



FileManager::FileManager(){
    

    std::cout << "Using Filemanager for: " << PLATFORM;
    supportPath=NULL;
    FolderManager folderManager;
        supportPath = (char *)folderManager.pathForDirectory(NSApplicationSupportDirectory, NSUserDomainMask);
    if(supportPath){
        std::cout << "Found Store Path: "<< supportPath << std::endl;
        app_folder = supportPath + std::string(APP_FOLDER);
        std::cout << "Attempting to create/find app folder: "<< app_folder << std::endl;
        std::cout << "Returned: " << folderManager.createDirectoryFromPath(app_folder.c_str()) << std::endl;
        
        
        
    }
    
   
    
    

}

FileManager::~FileManager(){
    
}

std::string FileManager::makeWorldFolder(std::string name){
    FolderManager folderManager;
    std::string path = app_folder + WORLD_FOLDER + "/" + name + "/";
    std::cout << path << std::endl;
    if(folderManager.createDirectoryFromPath(path.c_str())){
        return path;
    }
    
}

bool FileManager::saveWorld(rapidjson::Document* data, std::string path){
   
    if(app_folder.length() > 0){
        
       
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        data->Accept(writer);
        std::ofstream file(path.c_str());
      //  file << data;
        file << buffer.GetString();
        file.close();
    }
    else{
        std::cout << "FileMgr: No app folder path on record!" << std::endl;
    }
}

rapidjson::Document FileManager::loadWorld(std::string name){
    std::string loadPath = app_folder + WORLD_FOLDER + "/" + name; // + files
    std::fstream file(loadPath.c_str());
    rapidjson::Document ret;
    if(file){
        std::string load;
        file >> load;
        //https://rapidjson.org/md_doc_stream.html
        file.close();
        //ret = json::JSON::Load(load);
    }
    return ret; 
    
}
bool FileManager::saveToFile(std::string toSave){ //change to log file function
    if(supportPath){
        
    }
    return true;
    
}

std::string FileManager::getAppFolderPath(){
    if(app_folder.length() > 0)
        return app_folder;
    return std::string("");
}
#endif
    
