//
//  FolderManager.mm
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-11.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#include "FolderManager.h"
#import <Foundation/Foundation.h>
#include <string>
using namespace fm;
//https://developer.apple.com/documentation/foundation/nsfilemanager
//https://stackoverflow.com/questions/14098836/file-creation-in-c-on-xcode

FolderManager::FolderManager() {
    m_autoreleasePool = [[NSAutoreleasePool alloc] init];
}

FolderManager::~FolderManager() {
    [(NSAutoreleasePool *)m_autoreleasePool release];
}

const char * FolderManager::pathForDirectory(SearchPathDirectory directory, SearchPathDomainMask domainMask) {
    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSArray *URLs = [fileManager URLsForDirectory:(NSSearchPathDirectory)directory inDomains:domainMask];
    if (URLs.count == 0) return NULL;

    NSURL *URL = [URLs objectAtIndex:0];
    NSString *path = URL.path;

    // `fileSystemRepresentation` on an `NSString` gives a path suitable for POSIX APIs
    return path.fileSystemRepresentation;
}

const char * FolderManager::pathForDirectoryAppropriateForItemAtPath(SearchPathDirectory directory,
 SearchPathDomainMask domainMask, const char *itemPath, bool create) {

    NSFileManager *fileManager = [NSFileManager defaultManager];
    NSString *nsPath = [fileManager stringWithFileSystemRepresentation:itemPath length:strlen(itemPath)];
    NSURL *itemURL = (nsPath ? [NSURL fileURLWithPath:nsPath] : nil);

    NSURL *URL = [fileManager URLForDirectory:(NSSearchPathDirectory)directory
                       inDomain:domainMask
              appropriateForURL:itemURL
                         create:create error:NULL];
    return URL.path.fileSystemRepresentation;
}

bool FolderManager::createDirectoryFromPath(const char* itemPath){
    NSFileManager *fileManager = [NSFileManager defaultManager];

    NSString *nsPath = [fileManager stringWithFileSystemRepresentation:itemPath length:strlen(itemPath)];
    BOOL makeDir = [fileManager createDirectoryAtPath:nsPath
              withIntermediateDirectories:true
                               attributes:nil
                                    error:NULL];
    return makeDir;
}

void FolderManager::contentsOfDirectory(const char *itemPath, std::vector<std::string>* contents){
    NSFileManager *fileManager = [NSFileManager defaultManager];

    NSString *nsPath = [fileManager stringWithFileSystemRepresentation:itemPath length:strlen(itemPath)];
    NSArray<NSString *> *array = [fileManager contentsOfDirectoryAtPath:nsPath
                                                      error:nil];
   for(NSString *str in array){
       if(str != nil){
           std::string folderpath([str UTF8String]);
           contents->push_back(folderpath);
       }
       
    }
}
