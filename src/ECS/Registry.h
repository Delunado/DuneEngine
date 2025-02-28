#ifndef REGISTRY_H
#define REGISTRY_H
#include <Component.h>
#include <Definitions.h>
#include <Entity.h>
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

    template<typename T, typename... TArgs>
    void AddComponent(Entity entity, TArgs &&... args);

    template<typename T>
    void RemoveComponent(Entity entity);

    template<typename T>
    bool HasComponent(Entity entity) const;

    template<typename T>
    T &GetComponent(Entity entity) const;

    void AddEntityToSystem(Entity entity);

    void Update();

private:
    int _entities = 0;
    std::set<Entity> _entitiesToAdd;
    std::set<Entity> _entitiesToRemove;

    std::vector<IPool *> _componentPools;
    std::vector<Signature> _entitySignatures;
    std::pmr::unordered_map<std::type_index, System *> _systems;
};

template<typename T, typename... TArgs>
void Registry::AddComponent(const Entity entity, TArgs &&... args) {
    const int entityId = entity.GetId();

    const int componentId = Component<T>::GetId();

    if (componentId >= _componentPools.size()) {
        _componentPools.resize(componentId + 1, nullptr);
    }

    if (_componentPools[componentId] == nullptr) {
        Pool<T> *newPool = new Pool<T>();
        _componentPools[componentId] = newPool;
    }

    Pool<T> *pool = Pool<T>(_componentPools[componentId]);

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

#endif //REGISTRY_H
