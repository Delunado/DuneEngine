#ifndef ENTITY_INL
#define ENTITY_INL

template<typename T, typename... TArgs>
void Entity::AddComponent(TArgs &&... args) {
    _registry->AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template<typename T>
void Entity::RemoveComponent() const {
    _registry->RemoveComponent<T>(*this);
}

template<typename T>
bool Entity::HasComponent() const {
    return _registry->HasComponent<T>(*this);
}

template<typename T>
T &Entity::GetComponent() const {
    return _registry->GetComponent<T>(*this);
}

#endif //ENTITY_INL
