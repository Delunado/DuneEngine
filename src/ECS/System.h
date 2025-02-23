#ifndef SYSTEM_H
#define SYSTEM_H
#include <bitset>
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

    Signature& GetSignature() const;

private:
    Signature _componentSignature;

    std::vector<Entity> _entities;
};


#endif //SYSTEM_H
