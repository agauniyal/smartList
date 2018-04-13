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

    size_t len{};
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
        ++len;
    }

    auto insert(std::initializer_list<T> &&list)
    {
        for (auto &&value : list) {
            insert(value);
        }
    }

    auto remove(const T &value)
    {
        if (head) {
            while (head->value == value) {
                --len;
                std::exchange(head, std::move(head->next));
            }

            Node *current = head->next.get(), *prev = head.get();
            while (current != nullptr) {
                if (current->value == value) {
                    --len;
                    std::exchange(prev->next, std::move(current->next));
                } else {
                    prev = current;
                }
                current = prev->next.get();
            }
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

    auto size() const { return len; }
    auto empty() const { return len == 0; }

    ~SmartList()
    {
        while (head) {
            std::exchange(head, std::move(head->next));
        }
    }
};

#endif
