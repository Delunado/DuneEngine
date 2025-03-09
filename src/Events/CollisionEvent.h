#ifndef COLLISIONEVENT_H
#define COLLISIONEVENT_H

#include "Entity.h"
#include "../EventBus/Event.h"

class CollisionEvent : public Event {
public:
    CollisionEvent(Entity entityA, Entity entityB) : entityA(entityA), entityB(entityB) {
    }

    Entity entityA;
    Entity entityB;
};

#endif //COLLISIONEVENT_H
