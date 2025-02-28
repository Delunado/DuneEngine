#include "Registry.h"

#include <Entity.h>
#include <Logger.h>

Entity Registry::CreateEntity() {
    int entityId = _entities++;
    Entity entity = Entity(entityId);
    _entitiesToAdd.insert(entity);

    Logger::Log("Entity created: " + std::to_string(entityId));

    if (_entities >= _entitySignatures.size()) {
        _entitySignatures.resize(entityId + 1);
    }

    return entity;
}

void Registry::AddEntityToSystem(Entity entity) {
}

void Registry::Update() {
}
