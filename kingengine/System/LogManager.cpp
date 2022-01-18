//
//  LogManager.cpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-18.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "LogManager.hpp"
#include <chrono>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "ResourcePath.hpp"

LogManager logger;


LogManager::LogManager(){ //i rly like doing brackets like this now, Java broke me
    logs.clear();
    logs.push_back(getLocalTime());
    pushback();
}

LogManager::~LogManager(){
    silent("Writing Logfile");
    if(!saveLog()){
       std::cout << "error saving log" << std::endl;
        return;
    }
    
    std::cout << "LogManager: saved " << logs.size() <<" lines to file" << std::endl;
    logs.clear();
    return;
}
bool LogManager::saveLog(){
    std::ofstream file(resourcePath() + "log.txt");
    if(!file)
        return false;
  //  file << data;
    for(const auto &s : logs){
        file << s << std::endl;
    }
   
    
    file << "Written at: " << getLocalTime() << std::endl;
    file.close();
   
    return true;
}
void LogManager::pushback(){
    logs.push_back("");
}
void LogManager::add(const std::stringstream& ss, bool print){
    logs.push_back(ss.str());
    pushback();
    if(print)
        std::cout << ss.str() << std::endl;
}

std::string LogManager::getLocalTime(){
    //r u ready for this
    std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::ostringstream oss;
    oss <<  std::put_time( std::localtime(&tt), "%c %Z");
    return oss.str();
    //woo boy, glad thats over
    //that was dumb
}
void LogManager::silent(const std::string &str){
    logs.push_back(str);
    pushback();
}
void LogManager::debug(const std::string &str){
    std::cout << str << std::endl;
    logs.push_back(str);
    pushback();
}

void LogManager::debug(const std::string &str, int i){
    std::stringstream ss;
    ss << str << " " << i;
    add(ss);
}

void LogManager::fatal_error(const std::string &str){
    std::stringstream ss;
    ss << "Fatal Error: " << str << " at " << getLocalTime() << std::endl;
    std::cerr << ss.str();
    
    add(ss, false);
}
void LogManager::error(const std::string &str){
    std::stringstream ss;
    ss << "Error: " << str << std::endl;
    std::cerr << ss.str();
    
    
    add(ss, false);
}
