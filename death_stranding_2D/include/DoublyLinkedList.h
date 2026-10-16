#pragma once

/**
 * DoublylinkedList
 * Note: Users are responsible for recycling memory of nodes' data.
 */
namespace GamesEngineeringBase::Structures
{
    template <typename T>
    struct ListNode
    {
        T data;
        ListNode<T> *next;
        ListNode<T> *prev;

        ListNode(const T &data) : data(data), next(nullptr), prev(nullptr) {}
    };

    template <typename T>
    class DoublylinkedList
    {
    public:
        DoublylinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
        virtual ~DoublylinkedList() = default;

        // Avoid different lists' head_ or tail_ points to same memory.
        DoublylinkedList(const DoublylinkedList &) = delete;
        DoublylinkedList &operator=(const DoublylinkedList &) = delete;

        ListNode<T> *back() { return tail_; }
        ListNode<T> *front() { return head_; }

        bool empty() { return size_ == 0; }

        void push_back(const T &data)
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

        void push_front(const T &data)
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

        void pop_back()
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

        void pop_front()
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

    private:
        ListNode<T> *head_;
        ListNode<T> *tail_;
        int size_;
    };
}
