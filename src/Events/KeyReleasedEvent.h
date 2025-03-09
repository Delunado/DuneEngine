#ifndef KEYRELEASEDEVENT_H
#define KEYRELEASEDEVENT_H

#include "../EventBus/Event.h"

#include <SDL.h>

class KeyReleasedEvent : public Event {
public:
    explicit KeyReleasedEvent(SDL_Keycode &symbol) : symbol(symbol) {
    }

    SDL_Keycode symbol;
};

#endif //KEYRELEASEDEVENT_H
