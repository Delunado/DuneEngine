#include "Entity.h"

#include <Registry.h>

Entity::Entity(const int id, Registry *registry): _id(id), _registry(registry) {
}

int Entity::GetId() const {
    return _id;
}

void Entity::Kill() const {
    _registry->RemoveEntity(*this);
}

bool Entity::operator==(const Entity &other) const {
    return _id == other._id;
}

bool Entity::operator!=(const Entity &other) const {
    return _id != other._id;
}

bool Entity::operator<(const Entity &other) const {
    return _id < other._id;
}
