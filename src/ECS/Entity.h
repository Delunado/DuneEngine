#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
    explicit Entity(int id);

    int GetId() const;

    bool operator ==(const Entity &other) const;
    bool operator !=(const Entity &other) const;
    bool operator <(const Entity &other) const;

private:
    int _id;
};

#endif //ENTITY_H
