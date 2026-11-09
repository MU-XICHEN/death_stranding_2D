#pragma once
#include <stdexcept>

// The list deletes its nodes. Delete any objects stored as pointers separately.

// Todo: change to arr, without jumping
namespace GamesEngineeringBase::Structures
{

    template <typename T>
    class DoublyLinkedList
    {
    public:
        template <typename T>
        struct ListNode
        {
            T data;
            ListNode<T> *next;
            ListNode<T> *prev;

            ListNode(const T &data) : data(data), next(nullptr), prev(nullptr) {}
        };

        DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
        virtual ~DoublyLinkedList()
        {
            clear();
        }

        // Disable copying so two lists cannot share the same nodes.
        DoublyLinkedList(const DoublyLinkedList &) = delete;
        DoublyLinkedList &operator=(const DoublyLinkedList &) = delete;

        ListNode<T> *back() { return tail_; }
        ListNode<T> *front() { return head_; }

        int size() { return size_; }
        bool empty() { return size_ == 0; }

        // Delete nodes; pointer elements do not own the objects they point to.
        void clear()
        {
            while (!empty())
            {
                removeFirst();
            }
        }

        void pushBack(const T &data)
        {
            ListNode<T> *newNode = new ListNode<T>(data);
            if (!head_)
            {
                head_ = newNode;
                tail_ = newNode;
            }
            else
            {
                tail_->next = newNode;
                newNode->prev = tail_;
                tail_ = newNode;
            }
            size_++;
        }

        void popBack()
        {
            if (!tail_)
                return;

            ListNode<T> *temp = tail_;
            tail_ = tail_->prev;
            if (tail_)
                tail_->next = nullptr;
            else
                head_ = nullptr;

            delete temp;
            size_--;
        }

        void addFirst(const T &data)
        {
            ListNode<T> *newNode = new ListNode<T>(data);
            if (!head_)
            {
                head_ = newNode;
                tail_ = newNode;
            }
            else
            {
                newNode->next = head_;
                head_->prev = newNode;
                head_ = newNode;
            }
            size_++;
        }

        void removeFirst()
        {
            if (!head_)
                return;

            ListNode<T> *temp = head_;
            head_ = head_->next;
            if (head_)
                head_->prev = nullptr;
            else
                tail_ = nullptr;

            delete temp;
            size_--;
        }

        void insertBefore(ListNode<T> *item, const T &data)
        {
            if (!item)
                return;
            ListNode<T> *newNode = new ListNode<T>(data);
            newNode->next = item;
            newNode->prev = item->prev;
            if (item->prev)
            {
                item->prev->next = newNode;
            }
            else
            {
                head_ = newNode;
            }
            item->prev = newNode;
            size_++;
        }

        void removeNode(ListNode<T> *item)
        {
            if (!item)
                return;
            // Link the neighbors together before deleting this node.
            if (item->prev)
            {
                item->prev->next = item->next;
            }
            else
            {
                head_ = item->next;
            }
            if (item->next)
            {
                item->next->prev = item->prev;
            }
            else
            {
                tail_ = item->prev;
            }
            delete item;
            size_--;
        }

    private:
        ListNode<T> *head_;
        ListNode<T> *tail_;
        int size_;
    };
}
