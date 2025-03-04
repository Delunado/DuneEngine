#include "Registry.h"

#include <Entity.h>
#include <Logger.h>
#include <System.h>

Registry::Registry() {
    Logger::Log("Registry created");
}

Registry::~Registry() {
    Logger::Log("Registry destroyed");
}

Entity Registry::CreateEntity() {
    int entityId = _entities++;

    auto entity = Entity(entityId, this);
    _entitiesToAdd.insert(entity);

    if (_entities >= _entitySignatures.size()) {
        _entitySignatures.resize(entityId + 1);
    }

    Logger::Log("Entity created: " + std::to_string(entityId));

    return entity;
}

void Registry::AddEntityToSystems(const Entity &entity) {
    const int entityId = entity.GetId();

    const auto entitySignature = _entitySignatures[entityId];

    for (auto &[systemType, system]: _systems) {
        if (const auto &systemSignature = system->GetSignature();
            (entitySignature & systemSignature) == systemSignature) {
            system->AddEntity(entity);
        }
    }
}

void Registry::Update() {
    for (const auto entity: _entitiesToAdd) {
        AddEntityToSystems(entity);
    }

    _entitiesToAdd.clear();
}
