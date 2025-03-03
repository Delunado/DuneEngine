#include "Entity.h"

Entity::Entity(const int id, Registry *registry): _id(id), _registry(registry) {
}

int Entity::GetId() const {
    return _id;
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
