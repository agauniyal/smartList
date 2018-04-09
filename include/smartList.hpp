#ifndef SMART_LIST_HPP
#define SMART_LIST_HPP

#include <memory>
#include <initializer_list>

template <typename T>
class SmartList {
    struct Node {
        T value;
        std::unique_ptr<Node> next;
        explicit Node(T val) : value(val), next(nullptr) {}
    };
    std::unique_ptr<Node> head{};

public:
    SmartList() = default;
    auto insert(const T &value)
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
    }

    auto insert(std::initializer_list<T> &&list)
    {
        for (auto &&value : list) {
            insert(value);
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
};

#endif
