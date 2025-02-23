#ifndef ENTITY_H
#define ENTITY_H

class Entity {
public:
    explicit Entity(int id);

    int GetId() const;

private:
    int _id;
};

#endif //ENTITY_H
