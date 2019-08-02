#pragma once

// Including SDKDDKVer.h defines the highest available Windows platform.

// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.

#include <SDKDDKVer.h>


#ifndef APP_FULLSCREEN
#   define APP_FULLSCREEN false
#endif
#ifndef APP_RESIZABLE
#   define APP_RESIZABLE false
#endif
#ifndef APP_USERINPUT
// enable user input (mouse, keyboard, etc.)
#   define APP_USERINPUT false
#   if APP_USERINPUT
// enable double clicks
#       define APP_USERINPUT_DBLCLKS true
#endif
#endif
