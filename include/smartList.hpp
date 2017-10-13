#ifndef SMART_LIST_HPP
#define SMART_LIST_HPP

#include <memory>
#include <initializer_list>

namespace {
using std::initializer_list;
using std::make_unique;
using std::move;
using std::unique_ptr;

template <typename T>
class Node {
    T value;
    unique_ptr<Node> next;

public:
    Node(T _val, unique_ptr<Node> _n = nullptr)
        : value(move(_val)), next(move(_n))
    {
    }

    auto getValue() const { return value; }
    auto setValue(T _val) { value = _val; }
    auto &peekValue() { return value; }

    auto getNext() const { return next.get(); }
    auto setNext(unique_ptr<Node> &&_n) { next = move(_n); }
    auto moveNext() { return move(next); }
};
}  // anonymous namespace

template <typename T>
class SL {
    unique_ptr<Node<T>> head = nullptr;

public:
    auto insert(T value)
    {
        auto newNode = make_unique<Node<T>>(value);
        if (head == nullptr) {
            head = move(newNode);
            return;
        }

        auto current = head.get();
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        current->setNext(move(newNode));
    }

    auto insert(initializer_list<T> list)
    {
        if (list.size() < 1) {
            return;
        }

        size_t offset = 0;
        if (head == nullptr) {
            head = move(make_unique<Node<T>>(*list.begin()));
            offset += 1;
        }

        auto current = head.get();
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }

        for (auto i = list.begin() + offset; i != list.end(); ++i) {
            current->setNext(move(make_unique<Node<T>>(*i)));
            current = current->getNext();
        }
    }

    template <typename F>
    auto forEach(F func)
    {
        auto current = head.get();
        while (current != nullptr) {
            func(current->peekValue());
            current = current->getNext();
        }
    }

    auto reverse()
    {
        if (head == nullptr || head->getNext() == nullptr) {
            return;
        }

        unique_ptr<Node<T>> prev;
        auto next = move(head->moveNext());
        prev      = move(head);
        head      = move(next);

        while (head != nullptr) {
            next = move(head->moveNext());
            head->setNext(move(prev));
            prev = move(head);
            head = move(next);
        }
        head = move(prev);
    }

    ~SL()
    {
        auto current = move(head);
        while (current != nullptr) {
            std::exchange(current, current->moveNext());
        }
    }
};

#endif
