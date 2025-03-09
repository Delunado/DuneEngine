#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <list>
#include <map>
#include <memory>
#include <typeindex>

#include "EventCallback.h"

typedef std::list<std::unique_ptr<IEventCallback> > HandlerCollection;

class EventBus {
public:
    EventBus() = default;

    ~EventBus() = default;

    template<typename TEvent, typename TOwner>
    void Subscribe(TOwner *owner, void (TOwner::*callbackFunction)(TEvent &));

    template<typename TEvent, typename... TArgs>
    void Emit(TArgs &&... args);

    void Clear();

private:
    std::map<std::type_index, std::unique_ptr<HandlerCollection> > _subscribers;
};

template<typename TEvent, typename TOwner>
void EventBus::Subscribe(TOwner *owner, void (TOwner::*callbackFunction)(TEvent &)) {
    if (!_subscribers[typeid(TEvent)].get()) {
        _subscribers[typeid(TEvent)] = std::make_unique<HandlerCollection>();
    }

    std::unique_ptr<IEventCallback> subscriber = std::make_unique<EventCallback<TOwner, TEvent> >(
        owner, callbackFunction);
    _subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
}

template<typename TEvent, typename... TArgs>
void EventBus::Emit(TArgs &&... args) {
    auto handlers = _subscribers[typeid(TEvent)].get();

    if (!handlers)
        return;

    for (auto it = handlers->begin(); it != handlers->end(); ++it) {
        auto handler = it->get();
        TEvent event(std::forward<TArgs>(args)...);
        handler->Execute(event);
    }
}

#endif //EVENTBUS_H
