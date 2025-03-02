#ifndef POOL_H
#define POOL_H
#include <vector>

class IPool {
public:
    virtual ~IPool() {
    };
};

template<typename T>
class Pool : public IPool {
public:
    explicit Pool(int size = 100);

    ~Pool() override = default;

    bool IsEmpty() const;

    int GetSize() const;

    void Resize(int size);

    void Clear();

    void Add(T item);

    void Set(int index, T item);

    T &Get(int index);

    void Remove(T item);

    T &operator [](unsigned int index);

private:
    std::vector<T> _data;
};


template<typename T>
Pool<T>::Pool(int size) {
    _data.resize(size);
}

template<typename T>
bool Pool<T>::IsEmpty() const {
    return _data.empty();
}

template<typename T>
int Pool<T>::GetSize() const {
    return _data.size();
}

template<typename T>
void Pool<T>::Resize(int size) {
    _data.resize(size);
}

template<typename T>
void Pool<T>::Clear() {
    _data.clear();
}

template<typename T>
void Pool<T>::Add(T item) {
    _data.push_back(item);
}

template<typename T>
void Pool<T>::Set(int index, T item) {
    _data[index] = item;
}

template<typename T>
T &Pool<T>::Get(int index) {
    return static_cast<T &>(_data[index]);
}

template<typename T>
T &Pool<T>::operator[](unsigned int index) {
    return _data[index];
}

#endif //POOL_H
