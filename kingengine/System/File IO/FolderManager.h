//
//  FolderManager.h
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-11.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//

#ifndef FolderManager_h
#define FolderManager_h
#include <stdio.h>
#include <vector>
namespace fm {
    enum {
        NSApplicationDirectory = 1,
        NSDemoApplicationDirectory,
        NSDeveloperApplicationDirectory,
        NSAdminApplicationDirectory,
        NSLibraryDirectory,
        NSDeveloperDirectory,
        NSUserDirectory,
        NSDocumentationDirectory,
        NSDocumentDirectory,
        NSCoreServiceDirectory,
        NSAutosavedInformationDirectory = 11,
        NSDesktopDirectory = 12,
        NSCachesDirectory = 13,
        NSApplicationSupportDirectory = 14,
        NSDownloadsDirectory = 15,
        NSInputMethodsDirectory = 16,
        NSMoviesDirectory = 17,
        NSMusicDirectory = 18,
        NSPicturesDirectory = 19,
        NSPrinterDescriptionDirectory = 20,
        NSSharedPublicDirectory = 21,
        NSPreferencePanesDirectory = 22,
        NSApplicationScriptsDirectory = 23,
        NSItemReplacementDirectory = 99,
        NSAllApplicationsDirectory = 100,
        NSAllLibrariesDirectory = 101,
        NSTrashDirectory = 102
    };
    typedef unsigned long SearchPathDirectory;

    enum {
        NSUserDomainMask = 1,       // user's home directory --- place to install user's personal items (~)
        NSLocalDomainMask = 2,      // local to the current machine --- place to install items available to everyone on this machine (/Library)
        NSNetworkDomainMask = 4,    // publically available location in the local area network --- place to install items available on the network (/Network)
        NSSystemDomainMask = 8,     // provided by Apple, unmodifiable (/System)
        NSAllDomainsMask = 0x0ffff  // all domains: all of the above and future items
    };
    typedef unsigned long SearchPathDomainMask;

    class FolderManager {
        public:

        FolderManager();
        ~FolderManager();

        const char *pathForDirectory(SearchPathDirectory directory, SearchPathDomainMask domainMask);
        const char *pathForDirectoryAppropriateForItemAtPath(SearchPathDirectory directory, SearchPathDomainMask domainMask, const char *itemPath, bool create = false);
        bool createDirectoryFromPath(const char* itemPath);
        //does file exist
        //get folders
        void contentsOfDirectory(const char *itemPath, std::vector<std::string>* contents);
        private:
            void *m_autoreleasePool;
    };
};

#endif /* FolderManager_h */
