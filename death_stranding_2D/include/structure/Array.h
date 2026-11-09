#pragma once

static const int DEFAULT_CAPACITY = 10;

namespace GamesEngineeringBase::Structures
{

    // A growing array. If it stores pointers, delete the objects separately.
    template <typename T>
    class Array
    {
    public:
        // Start empty with some space ready to use.
        Array()
            : size_(0),
              capacity_(DEFAULT_CAPACITY),
              data_(new T[capacity_]{}) {}

        // Start with this many elements. The array can still grow later.
        Array(int size)
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
            // Grow the storage and keep the old values.
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

            // Shift later elements left to fill the gap.
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
