#ifndef SYSTEM_H
#define SYSTEM_H
#include <bitset>
#include <Component.h>
#include <vector>

#include "Definitions.h"

class Entity;

class System {
public:
    System() = default;

    ~System() = default;

    void AddEntity(const Entity &entity);

    void RemoveEntity(const Entity &entity);

    std::vector<Entity> GetEntities() const;

    const Signature &GetSignature() const;

private:
    template<typename T>
    void RequireComponent();

    Signature _componentSignature;

    std::vector<Entity> _entities;
};

template<typename T>
void System::RequireComponent() {
    _componentSignature.set(Component<T>::GetId());
}

#endif //SYSTEM_H
