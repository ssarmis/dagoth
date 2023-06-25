#pragma once

#include "general.h"

enum State : u8 {
    IDLE = 0,
    WALKING,
    StateSize
};

struct StateData {
    State oldState;
    State currentState;
};
