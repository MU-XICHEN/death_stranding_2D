#pragma once

static const int DEFAULT_CAPACITY = 20;

namespace GamesEngineeringBase::Structures
{

    template <typename T>
    class Array
    {
    public:
        Array() : data_(new T[DEFAULT_CAPACITY]), size_(0), capacity_(DEFAULT_CAPACITY) {}

        ~Array() { delete[] data_; }

        T &operator[](int index) const { return data_[index]; }

        int size() const { return size_; }

        void push_back(const T &value)
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

    private:
        T *data_;
        int size_;
        int capacity_;
    };
}
