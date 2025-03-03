﻿#ifndef REGISTRY_H
#define REGISTRY_H
#include <Component.h>
#include <Definitions.h>
#include <Entity.h>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Pool.h"

class System;

class Registry {
public:
    Registry() = default;

    Entity CreateEntity();

    // Components
    template<typename T, typename... TArgs>
    void AddComponent(Entity entity, TArgs &&... args);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    bool HasComponent(Entity entity) const;

    template<typename T>
    T &GetComponent(Entity entity) const;

    // Systems
    template<typename T, typename... TArgs>
    void AddSystem(TArgs &&... args);

    template<typename T>
    void RemoveSystem();

    template<typename T>
    bool HasSystem() const;

    template<typename T>
    T &GetSystem() const;

    void AddEntityToSystems(Entity entity);

    void Update();

private:
    int _entities = 0;
    std::set<Entity> _entitiesToAdd;
    std::set<Entity> _entitiesToRemove;

    std::vector<std::shared_ptr<IPool> > _componentPools;
    std::vector<Signature> _entitySignatures;
    std::pmr::unordered_map<std::type_index, std::shared_ptr<System> > _systems;
};

template<typename T, typename... TArgs>
void Registry::AddComponent(const Entity entity, TArgs &&... args) {
    const int entityId = entity.GetId();

    const int componentId = Component<T>::GetId();

    if (componentId >= _componentPools.size()) {
        _componentPools.resize(componentId + 1, nullptr);
    }

    if (_componentPools[componentId] == nullptr) {
        std::shared_ptr<Pool<T> > newPool = std::make_shared << Pool<T>();
        _componentPools[componentId] = newPool;
    }

    std::shared_ptr<Pool<T> > pool = std::static_pointer_cast<Pool<T> >(_componentPools[componentId]);

    if (entityId >= pool->GetSize()) {
        pool->Resize(_entities);
    }

    T newComponent(std::forward<TArgs>(args)...);

    pool->Set(entityId, newComponent);

    _entitySignatures[entityId].set(componentId, true);
}

template<typename T>
void Registry::RemoveComponent(const Entity entity) {
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    _entitySignatures[entityId].set(componentId, false);
}

template<typename T>
bool Registry::HasComponent(const Entity entity) const {
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    return _entitySignatures[entityId].test(componentId);
}

template<typename T, typename... TArgs>
void Registry::AddSystem(TArgs &&... args) {
    std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<TArgs>(args)...);
    _systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template<typename T>
void Registry::RemoveSystem() {
    _systems.erase(std::type_index(typeid(T)));
}

template<typename T>
bool Registry::HasSystem() const {
    return _systems.find(std::type_index(typeid(T))) != _systems.end();
}

template<typename T>
T &Registry::GetSystem() const {
    return std::static_pointer_cast<T>(_systems.at(std::type_index(typeid(T))));
}

#endif //REGISTRY_H
