#pragma once
#include <klogger.hpp>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>


class Event;

class EventManager {
    public:
        static EventManager& GetInstance();

        void RegisterEvent(std::string eventName);
        void UnregisterEvent(std::string eventName);

        void TriggerEvent(std::string eventName);
        void AddEventToDo(std::string eventName, std::function<void()> eventTodo);

    private:
        EventManager();
        ~EventManager();

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;

        std::vector<Event> registeredEvents;
        std::unordered_map<std::string, Event*> eventMap;
};

class Event {
    public:
        std::string eventName;
        
        Event(std::string eventName);
        ~Event();

        void TriggerEvent();
        void AddToDo(std::function<void()> todo);

    private:
        std::vector<std::function<void()>> eventToDo;
};