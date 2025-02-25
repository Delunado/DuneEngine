#include "Entity.h"

Entity::Entity(const int id): _id(id) {
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
