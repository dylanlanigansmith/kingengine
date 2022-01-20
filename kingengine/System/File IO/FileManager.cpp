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
#include "LogManager.hpp"

//Wrapper for system specific file handlers
#ifdef MAC_ARM64
using namespace fm;



FileManager::FileManager(){
    

    logger.silent("FileManager: Init for " + std::string(PLATFORM));
    supportPath=NULL;
    FolderManager folderManager;
        supportPath = (char *)folderManager.pathForDirectory(NSApplicationSupportDirectory, NSUserDomainMask);
    if(supportPath){
        logger.silent("Found Application Support Path: " + std::string(supportPath));
        app_folder = supportPath + std::string(APP_FOLDER);
        logger.silent("Attempting to create/find app folder: " + app_folder);
        bool success = folderManager.createDirectoryFromPath(app_folder.c_str());
        if(!success)
            logger.fatal_error("File Mgr: createDirectoryFromPath(app_folder) returned false");
            //did i even implement this shit
        else
            logger.silent("Attempting to create/find app folder: " + app_folder);
    }
    logger << "FileManager: OK" << lm::endl;
   
    
    

}

FileManager::~FileManager(){
    
}

std::string FileManager::makeWorldFolder(std::string name){
    FolderManager folderManager;
    std::string path = app_folder + WORLD_FOLDER + "/" + name + "/"; //this might be a problem
    logger.silent("Making World Folder at " + path);
    if(folderManager.createDirectoryFromPath(path.c_str())){
        logger.silent("Made Folder for " + name +" successfully");
        return path;
    }
    else{
        logger.fatal_error("File Mgr: createDirectoryFromPath("+path+") returned false");
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
        logger.fatal_error("FileMgr: No app folder path on record!");
    }
}

//this should move to Asset Manager
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


void FileManager::getFolders(const std::string& name, std::vector<std::string> &list){
    FolderManager folderManager;
    //not using name yet
    std::string path = app_folder + WORLD_FOLDER + "/"; //this might be a problem
                                                        //update: ig it wasnt
    folderManager.contentsOfDirectory(path.c_str(), list);
    
    for(int i = 0; i < list.size(); i++){
        if(list.at(i) == ".DS_Store"){
            list.erase(list.begin()+i);
        }
    }
    
}
#endif
    
