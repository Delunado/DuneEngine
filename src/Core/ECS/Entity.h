#ifndef ENTITY_H
#define ENTITY_H
#include <memory>

class Registry;

class Entity {
public:
    explicit Entity(int id, Registry *registry);

    int GetId() const;

    void Kill() const;

    template<typename T, typename... TArgs>
    void AddComponent(TArgs &&... args);

    template<typename T>
    void RemoveComponent() const;

    template<typename T>
    bool HasComponent() const;

    template<typename T>
    T &GetComponent() const;

    bool operator ==(const Entity &other) const;

    bool operator !=(const Entity &other) const;

    bool operator <(const Entity &other) const;

private:
    int _id;

    Registry *_registry;
};

#endif //ENTITY_H
