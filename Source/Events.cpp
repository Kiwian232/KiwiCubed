#include "Events.h"


EventManager::EventManager() = default;
EventManager::~EventManager() = default;

EventManager& EventManager::GetInstance() {
    static EventManager instance;
    return instance;
}

void EventManager::RegisterEvent(std::string eventName) {
    if (eventMap.find(eventName) != eventMap.end()) {
        WARN("Tried to register event \"" + eventName + "\" twice");
        return;
    }

    registeredEvents.emplace_back(eventName);
    Event* event = &registeredEvents.back();
    eventMap.insert({eventName, event});
}

void EventManager::UnregisterEvent(std::string eventName) {
    OVERRIDE_LOG_NAME("Events");
    auto it = std::remove_if(registeredEvents.begin(), registeredEvents.end(),
        [&eventName](const Event& event) { return event.eventName == eventName; });
    if (it == registeredEvents.end()) {
        WARN("Tried to unregister non-existent event \"" + eventName + "\"");
    }
    registeredEvents.erase(it, registeredEvents.end());
    eventMap.erase(eventName);
}

void EventManager::TriggerEvent(std::string eventName) {
    auto event = eventMap.find(eventName);
    if (event != eventMap.end()) {
        event->second->TriggerEvent();
    } else {
        WARN("Tried to trigger non-existent event \"" + eventName + "\"");
    }
}

void EventManager::AddEventToDo(std::string eventName, std::function<void()> eventTodo) {
    auto event = eventMap.find(eventName);
    if (event != eventMap.end()) {
        event->second->AddToDo(eventTodo);
    } else {
        WARN("Tried to trigger non-existent event \"" + eventName + "\"");
    }
}

Event::Event(std::string eventName) : eventName(std::move(eventName)) {}
Event::~Event() = default;

void Event::TriggerEvent() {
    for (unsigned int i = 0; i < eventToDo.size(); i++) {
        eventToDo[i]();
    }
}

void Event::AddToDo(std::function<void()> todo) {
    eventToDo.emplace_back(todo);
}