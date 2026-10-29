#pragma once

static const int DEFAULT_CAPACITY = 20;
static const int DEFAULT_SIZE = 0;

namespace GamesEngineeringBase::Structures
{

    // Variable-Length Array
    // Users need to manage the memory of the elements themselves if they are pointers.
    template <typename T, int defaultSize = DEFAULT_SIZE>
    class Array
    {
    public:
        Array(int size = defaultSize)
            : size_(size),
              capacity_(size ? size : DEFAULT_CAPACITY),
              data_(new T[capacity_]{}) {}

        ~Array() { delete[] data_; }

        Array(const Array &) = delete;
        Array &operator=(const Array &) = delete;

        T &operator[](int index) const { return data_[index]; }

        inline int size() const { return size_; }

        inline T *head() { return data_; }

        void clear() { size_ = 0; }

        void resize(int newSize)
        {
            if (newSize > capacity_)
            {
                int newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
                while (newCapacity < newSize)
                    newCapacity *= 2;

                T *newData = new T[newCapacity];
                for (int i = 0; i < size_; ++i)
                    newData[i] = data_[i];
                delete[] data_;

                data_ = newData;
                capacity_ = newCapacity;
            }
            size_ = newSize;
        }

        void pushBack(const T &value)
        {
            if (size_ >= capacity_)
            {
                int newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;

                T *newData = new T[newCapacity];
                for (int i = 0; i < size_; ++i)
                    newData[i] = data_[i];
                delete[] data_;

                data_ = newData;
                capacity_ = newCapacity;
            }
            data_[size_++] = value;
        }

        void removeAt(int index)
        {
            if (index < 0 || index >= size_)
                return;

            for (int i = index; i < size_ - 1; ++i)
                data_[i] = data_[i + 1];

            --size_;
        }

    private:
        int size_;
        int capacity_;
        T *data_;
    };
}
