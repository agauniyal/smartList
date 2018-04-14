#ifndef SMART_LIST_HPP
#define SMART_LIST_HPP

#include <initializer_list>
#include <memory>
#include <optional>

template <typename T>
class SmartList {
    struct Node {
        T value;
        std::unique_ptr<Node> next;
        explicit Node(T val) : value(val), next(nullptr) {}
    };

    size_t len{};
    std::unique_ptr<Node> head{};

public:
    SmartList() = default;
    auto push_back(const T &value)
    {
        if (head) {
            Node *current = head.get();
            while (current->next != nullptr) {
                current = current->next.get();
            }
            current->next = std::make_unique<Node>(value);
        } else {
            head = std::make_unique<Node>(value);
        }
        ++len;
    }

    auto push_back(std::initializer_list<T> &&list)
    {
        for (auto &&value : list) {
            push_back(value);
        }
    }

    auto remove_first(const T &value)
    {
        if (empty()) {
            return;
        }

        if (head->value == value) {
            std::exchange(head, std::move(head->next));
            --len;
            return;
        }

        auto current = head->next.get(), previous = head.get();
        while (current != nullptr) {
            if (current->value == value) {
                std::exchange(previous->next, std::move(current->next));
                --len;
                return;
            } else {
                previous = current;
                current  = current->next.get();
            }
        }
    }

    auto remove(const T &value)
    {
        if (empty()) {
            return;
        }

        while (head->value == value) {
            std::exchange(head, std::move(head->next));
            --len;
        }

        auto current = head->next.get(), previous = head.get();
        while (current != nullptr) {
            if (current->value == value) {
                std::exchange(previous->next, std::move(current->next));
                --len;
            } else {
                previous = current;
            }
            current = previous->next.get();
        }
    }

    template <typename F>
    auto forEach(F func)
    {
        Node *current = head.get();
        while (current) {
            func(current->value);
            current = current->next.get();
        }
    }

    auto reverse()
    {
        Node *current  = head.get();
        Node *previous = nullptr;
        while (current) {
            const auto nextNode = current->next.release();
            current->next.reset(previous);
            previous = current;
            current  = nextNode;
        }
        head.release();
        head.reset(previous);
    }

    std::optional<T> value_at(size_t index) const
    {
        Node *current = head.get();
        while (current != nullptr && index > 0) {
            current = current->next.get();
            --index;
        }
        if (current != nullptr) {
            return current->value;
        } else {
            return {};
        }
    }

    auto size() const { return len; }
    auto empty() const { return len == 0; }

    auto push_front(const T &value)
    {
        std::unique_ptr<T> temp = std::move(head);
        head                    = make_unique(value);
        head->next              = std::move(temp);
        ++len;
    }

    std::optional<T> pop_front()
    {
        if (empty()) {
            return {};
        }

        const auto temp = std::exchange(head, std::move(head->next));
        --len;
        return temp->value;
    }

    std::optional<T> pop_back()
    {
        if (empty()) {
            return {};
        }

        auto current = head.get();
        for (size_t i = 1; i < len - 1; ++i) {
            current = current->next();
        }
        const auto temp = std::move(current->next);
        return temp->value;
    }

    std::optional<T> front() const
    {
        if (empty()) {
            return {};
        } else {
            return head->value;
        }
    }

    std::optional<T> back() const
    {
        if (empty()) {
            return {};
        }

        auto current = head.get();
        for (size_t i = 1; i < len; ++i) {
            current = current->next();
        }
        return current->value;
    }

    auto insert(size_t index, const T &value)
    {
        if (index > len) {
            return;
        }

        auto current = head.get();
        while (index > 1 && current != nullptr) {
            current = current->next.get();
            --index;
        }
        auto temp           = std::move(current->next);
        current->next       = std::make_unique(value);
        current->next->next = std::move(temp);
        ++len;
    }

    auto erase(size_t index)
    {
        if (index >= len) {
            return;
        }

        auto current = head.get();
        while (index > 1) {
            current = current->next.get();
            --index;
        }

        std::exchange(current->next, std::move(current->next->next));
        --len;
    }

    ~SmartList()
    {
        while (head) {
            std::exchange(head, std::move(head->next));
        }
    }
};

#endif
