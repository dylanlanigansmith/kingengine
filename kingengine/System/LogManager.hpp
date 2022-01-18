//
//  LogManager.hpp
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-18.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef LogManager_hpp
#define LogManager_hpp

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#define LOGEND LogManager::endl;

class LogManager {
private:
    std::vector<std::string> logs;
    bool logDebug = true;
    bool saveLog();
    void pushback();
    void add(const std::stringstream& ss, bool print = true);
public:
    LogManager();
    ~LogManager();
    std::string getLocalTime();
    void debug(const std::string&);
    void silent(const std::string&);
    void debug(const std::string&, int i = 0);
    void error(const std::string&);
    void fatal_error(const std::string&);
    template <typename T>
    LogManager& operator<<(const T& x)
    {
        std::cout << x;
        std::stringstream ss; //this shit is dumb but fix it later;
        ss << x;
        logs.back() += (ss.str());
        return *this;
    }

    // function that takes a custom stream, and returns it
    typedef LogManager& (*LogManagerManipulator)(LogManager&);

    // take in a function with the custom signature
    LogManager& operator<<(LogManagerManipulator manip)
    {
        // call the function, and return it's value
        return manip(*this);
    }

    // define the custom endl for this stream.
    // note how it matches the `LogManagerManipulator`
    // function signature
    static LogManager& endl(LogManager& stream)
    {
        // print a new line
        std::cout << std::endl;

        // do other stuff with the stream
        // std::cout, for example, will flush the stream
        stream.pushback();
        
        return stream;
    }
   
    // this is the type of std::cout
    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

    // this is the function signature of std::endl
    typedef CoutType& (*StandardEndLine)(CoutType&);

    // define an operator<< to take in std::endl
    LogManager& operator<<(StandardEndLine manip)
    {
        // call the function, but we cannot return it's value
        manip(std::cout);

        return *this;
    }

    
};
extern LogManager logger;
using lm = LogManager;

#endif /* LogManager_hpp */
