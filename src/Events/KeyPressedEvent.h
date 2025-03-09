#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include "../EventBus/Event.h"

#include <SDL.h>

class KeyPressedEvent : public Event {
public:
    explicit KeyPressedEvent(SDL_Keycode &symbol) : symbol(symbol) {
    }

    SDL_Keycode symbol;
};

#endif //KEYPRESSEDEVENT_H
