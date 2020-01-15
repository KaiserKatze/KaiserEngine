#pragma once

#include "stdafx.h"

class Event
{
public:
    Event();
    virtual ~Event();
};

// Forward declaration
// because "EventHandler.h" and "EventListener.h" are including each other
class EventHandler;
class EventListener;
