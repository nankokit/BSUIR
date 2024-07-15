#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#endif // FUNCTIONS_H


#include <cstdio>
#include <iostream>
#include <string>


#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <climits>

#include <map>

#include <sys/types.h>
#include <pwd.h>


#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <cinttypes>
#include <unistd.h>
#define __int64 int64_t
#define _close close
#define _read read
#define _lseek64 lseek64
#define _O_RDONLY O_RDONLY
#define _open open
#define _lseeki64 lseek64
#define _lseek lseek
#define stricmp strcasecmp
#endif


using namespace std;


string startPage(string &previousDirectory, string* &directoriesArray, string* &filesArray, string* &permissionArray,
               int &numberOfDirectories, int &numberOfFiles, int &numberOfPermission);

char* displayPermission (int st_mode );

unsigned directoryBrowsing(string introducedDir, string* &directoriesArray, string* &filesArray, string* &permissionArray,
                           int &numberOfDirectories, int &numberOfFiles, int &numberOfPermission);
