#pragma once

/**
 * HashSet
 * Note:
 *      Node<T> will be recycled automatically,
 *      Users are responsible for recycling memory of node->data.
 */

namespace GamesEngineeringBase::Structures
{

    template <typename T, int BucketCount = 128>
    class HashSet
    {
    private:
        struct Node
        {
            T value;
            Node *next;

            Node(const T &value, Node *next)
                : value(value), next(next) {}
        };

        Node *buckets_[BucketCount] = {};
        int size_ = 0;

        int bucketIndex(const T &value) const { return value % BucketCount; }

    public:
        HashSet() = default;

        ~HashSet() { clear(); }

        HashSet(const HashSet &) = delete;
        HashSet &operator=(const HashSet &) = delete;

        int size() const { return size_; }

        bool insert(const T &value)
        {
            int index = bucketIndex(value);

            Node *current = buckets_[index];

            while (current)
            {
                if (current->value == value)
                    return false;

                current = current->next;
            }

            buckets_[index] = new Node(value, buckets_[index]);
            ++size_;

            return true;
        }

        bool contains(const T &value) const
        {
            int index = bucketIndex(value);

            const Node *current = buckets_[index];

            while (current)
            {
                if (current->value == value)
                    return true;

                current = current->next;
            }

            return false;
        }

        void clear()
        {
            for (int i = 0; i < BucketCount; ++i)
            {
                Node *current = buckets_[i];

                while (current)
                {
                    Node *next = current->next;
                    delete current;
                    current = next;
                }

                buckets_[i] = nullptr;
            }

            size_ = 0;
        }
    };
}
