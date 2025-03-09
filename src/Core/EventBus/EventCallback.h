#ifndef EVENTCALLBACK_H
#define EVENTCALLBACK_H

#include <functional>

#include "Event.h"

class IEventCallback {
public:
    virtual ~IEventCallback() = default;

    void Execute(Event &e) {
        Call(e);
    }

private:
    virtual void Call(Event &e) = 0;
};

template<typename TOwner, typename TEvent>
class EventCallback : public IEventCallback {
    typedef void (TOwner::*CallbackFunction)(TEvent &);

public:
    EventCallback(TOwner *owner, CallbackFunction function) : _owner(owner), _function(function) {
    }

    virtual ~EventCallback() override = default;

private:
    TOwner *_owner;
    CallbackFunction _function;

    virtual void Call(Event &e) override {
        std::invoke(_function, _owner, static_cast<TEvent &>(e));
    }
};

#endif //EVENTCALLBACK_H
