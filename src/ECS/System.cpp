#include "System.h"

#include <algorithm>
#include <Entity.h>
#include <bits/streambuf_iterator.h>

void System::AddEntity(const Entity &entity) {
    _entities.push_back(entity);
}

void System::RemoveEntity(const Entity &entity) {
    _entities.erase(std::remove_if(
                        _entities.begin(),
                        _entities.end(),
                        [&entity](const Entity &other) {
                            return other == entity;
                        }), _entities.end());
}

std::vector<Entity> System::GetEntities() const {
    return _entities;
}

const Signature &System::GetSignature() const {
    return _componentSignature;
}
