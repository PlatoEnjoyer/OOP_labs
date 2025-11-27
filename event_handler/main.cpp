#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

// Базовый класс EventArgs
struct EventArgs {
    virtual ~EventArgs() = default;
};

// Интерфейс EventHandler
template<typename TEventArgs>
class EventHandler {
public:
    using HandlerFunc = std::function<void(const void*, const TEventArgs&)>;
    virtual ~EventHandler() = default;
    virtual void handle(const void* sender, const TEventArgs& args) const = 0;

    HandlerFunc get_function() const {
        return [this](const void* sender, const TEventArgs& args) {
            this->handle(sender, args);
        };
    }
};

// Класс Event (Broadcaster)
template<typename TSender, typename TEventArgs>
class Event {
private:
    struct Listener {
        std::weak_ptr<EventHandler<TEventArgs>> handler;
        typename EventHandler<TEventArgs>::HandlerFunc func;

        bool is_expired() const { return handler.expired(); }
    };

    mutable std::vector<Listener> listeners;

public:
    Event& operator+=(const std::shared_ptr<EventHandler<TEventArgs>>& handler) {
        if (!handler) return *this;
        listeners.push_back({std::weak_ptr(handler), handler->get_function()});
        return *this;
    }

    Event& operator-=(const std::shared_ptr<EventHandler<TEventArgs>>& handler) {
        auto it = std::remove_if(listeners.begin(), listeners.end(),
            [&handler](const Listener& l) {
                auto locked = l.handler.lock();
                return !locked || locked.get() == handler.get();
            });
        listeners.erase(it, listeners.end());
        return *this;
    }

    void invoke(const TSender* sender, const TEventArgs& args) const {
        for (auto it = listeners.begin(); it != listeners.end();) {
            if (it->is_expired()) {
                it = listeners.erase(it);
            } else {
                if (auto locked = it->handler.lock()) {
                    it->func(sender, args);
                }
                ++it;
            }
        }
    }
};


struct PropertyChangedEventArgs : EventArgs {
    std::string property_name;
    explicit PropertyChangedEventArgs(const std::string& name) : property_name(name) {}
};

struct PropertyChangingEventArgs : EventArgs {
    std::string property_name;
    const void* old_value;
    const void* new_value;
    mutable bool can_change;

    template<typename T>
    PropertyChangingEventArgs(const std::string& name, const T& old_val, const T& new_val)
        : property_name(name), old_value(&old_val), new_value(&new_val), can_change(true) {}

    template<typename T>
    const T& get_old_value() const { return *static_cast<const T*>(old_value); }

    template<typename T>
    const T& get_new_value() const { return *static_cast<const T*>(new_value); }
};

// Обработчики событий
// Логгер: выводит подробную информацию об изменениях
class ConsoleLogger : public EventHandler<PropertyChangedEventArgs> {
public:
    void handle(const void* sender, const PropertyChangedEventArgs& args) const override {
        std::cout << "[LOG] " << sender << " property '" << args.property_name << "' changed.\n";
    }
};

// Валидатор: проверяет корректность изменений
class PropertyValidator : public EventHandler<PropertyChangingEventArgs> {
public:
    void handle(const void* sender, const PropertyChangingEventArgs& args) const override {
        if (args.property_name == "age") {
            int new_age = args.get_new_value<int>();
            if (new_age < 0) {
                std::cout << "[VALIDATOR] " << sender
                          << ": Rejecting change of 'age' to " << new_age
                          << " (negative value not allowed)\n";
                const_cast<PropertyChangingEventArgs&>(args).can_change = false;
            }
        }
    }
};

class Person {
public:
    Event<Person, PropertyChangingEventArgs> property_changing;
    Event<Person, PropertyChangedEventArgs> property_changed;

private:
    std::string name;
    int age;
    double salary;

public:
    Person() : name(""), age(0), salary(0.0) {}

    const std::string& get_name() const { return name; }
    int get_age() const { return age; }
    double get_salary() const { return salary; }

    void set_name(const std::string& value) {
        if (name == value) return;

        PropertyChangingEventArgs changing("name", name, value);
        property_changing.invoke(this, changing);
        if (!changing.can_change) {
            std::cout << "[REJECTED] " << this << ": name change '" << name << "' → '" << value << "' blocked.\n";
            return;
        }

        std::cout << "[CHANGED] " << this << ": name = '" << name << "' → '" << value << "'\n";
        name = value;
        property_changed.invoke(this, PropertyChangedEventArgs("name"));
    }

    void set_age(int value) {
        if (age == value) return;

        PropertyChangingEventArgs changing("age", age, value);
        property_changing.invoke(this, changing);
        if (!changing.can_change) {
            std::cout << "[REJECTED] " << this << ": age change " << age << " → " << value << " blocked.\n";
            return;
        }

        std::cout << "[CHANGED] " << this << ": age = " << age << " → " << value << "\n";
        age = value;
        property_changed.invoke(this, PropertyChangedEventArgs("age"));
    }

    void set_salary(double value) {
        if (salary == value) return;

        PropertyChangingEventArgs changing("salary", salary, value);
        property_changing.invoke(this, changing);
        if (!changing.can_change) {
            std::cout << "[REJECTED] " << this << ": salary change " << salary << " → " << value << " blocked.\n";
            return;
        }

        std::cout << "[CHANGED] " << this << ": salary = " << salary << " → " << value << "\n";
        salary = value;
        property_changed.invoke(this, PropertyChangedEventArgs("salary"));
    }
};


class Settings {
public:
    Event<Settings, PropertyChangingEventArgs> property_changing;
    Event<Settings, PropertyChangedEventArgs> property_changed;

private:
    bool fullscreen;
    int volume;
    std::string theme;

public:
    Settings() : fullscreen(false), volume(50), theme("light") {}

    bool get_fullscreen() const { return fullscreen; }
    int get_volume() const { return volume; }
    const std::string& get_theme() const { return theme; }

    void set_fullscreen(bool value) {
        if (fullscreen == value) return;

        PropertyChangingEventArgs args("fullscreen", fullscreen, value);
        property_changing.invoke(this, args);
        if (!args.can_change) {
            std::cout << "[REJECTED] " << this << ": fullscreen change " << fullscreen << " → " << value << " blocked.\n";
            return;
        }

        std::cout << "[CHANGED] " << this << ": fullscreen = " << (fullscreen ? "true" : "false")
                  << " → " << (value ? "true" : "false") << "\n";
        fullscreen = value;
        property_changed.invoke(this, PropertyChangedEventArgs("fullscreen"));
    }

    void set_volume(int value) {
        if (volume == value) return;

        PropertyChangingEventArgs args("volume", volume, value);
        property_changing.invoke(this, args);
        if (!args.can_change) {
            std::cout << "[REJECTED] " << this << ": volume change " << volume << " → " << value << " blocked.\n";
            return;
        }

        int clamped = std::clamp(value, 0, 100);
        if (clamped != value) {
            std::cout << "[WARNING] Volume clamped from " << value << " to " << clamped << ".\n";
            value = clamped;
        }

        std::cout << "[CHANGED] " << this << ": volume = " << volume << " → " << value << "\n";
        volume = value;
        property_changed.invoke(this, PropertyChangedEventArgs("volume"));
    }

    void set_theme(const std::string& value) {
        if (theme == value) return;

        PropertyChangingEventArgs args("theme", theme, value);
        property_changing.invoke(this, args);
        if (!args.can_change) {
            std::cout << "[REJECTED] " << this << ": theme change '" << theme << "' → '" << value << "' blocked.\n";
            return;
        }

        std::cout << "[CHANGED] " << this << ": theme = '" << theme << "' → '" << value << "'\n";
        theme = value;
        property_changed.invoke(this, PropertyChangedEventArgs("theme"));
    }
};


int main() {
    Person person;
    Settings settings;

    auto logger = std::make_shared<ConsoleLogger>();
    auto validator = std::make_shared<PropertyValidator>();

    // Подписываемся
    person.property_changed += logger;
    person.property_changing += validator;

    settings.property_changed += logger;
    settings.property_changing += validator;

    // === Тестирование ===
    std::cout << "\n--- Testing Person ---\n";
    person.set_name("Alice");
    person.set_age(25);
    person.set_age(-5);   // будет отклонено
    person.set_salary(75000.0);

    std::cout << "\n--- Testing Settings ---\n";
    settings.set_fullscreen(true);
    settings.set_volume(120); // будет приведено к 100
    settings.set_theme("dark");

    return 0;
}