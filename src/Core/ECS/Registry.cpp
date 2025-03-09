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
    int entityId;

    if (_freeEntityIds.empty()) {
        entityId = _entities++;

        if (entityId >= _entitySignatures.size()) {
            _entitySignatures.resize(entityId + 1);
        }
    } else {
        entityId = _freeEntityIds.front();
        _freeEntityIds.pop_front();
    }

    const auto entity = Entity(entityId, this);
    _entitiesToAdd.insert(entity);

    Logger::Log("Entity created: " + std::to_string(entityId));

    return entity;
}

void Registry::RemoveEntity(const Entity entity) {
    _entitiesToRemove.insert(entity);
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

void Registry::RemoveEntityFromSystems(const Entity &entity) {
    for (auto &[systemType, system]: _systems) {
        system->RemoveEntity(entity);
    }
}

void Registry::Update() {
    for (const auto entity: _entitiesToAdd) {
        AddEntityToSystems(entity);
    }
    _entitiesToAdd.clear();

    for (const auto entity: _entitiesToRemove) {
        RemoveEntityFromSystems(entity);
        _entitySignatures[entity.GetId()].reset();

        _freeEntityIds.push_back(entity.GetId());

        Logger::Log("Entity removed: " + std::to_string(entity.GetId()));
    }
    _entitiesToRemove.clear();
}
