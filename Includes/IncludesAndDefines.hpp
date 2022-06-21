#ifndef _IncludesAndDefines_hpp_
#define _IncludesAndDefines_hpp_

#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <vector>
#include <sstream>
#include <ctime>

#define SERVER_IP "127.0.0.1"
#define COMMAND_BUF_SIZE 256
#define DATA_BUF_SIZE 4096
#define MAX_DATA_SIZE 16384
#define MAX_CONNECTIONS 40
#define COMMAND_PORT 8000
#define DATA_PORT 8001

enum HostState{waiting, reciving, sending};

typedef uint8_t byte;

#endif