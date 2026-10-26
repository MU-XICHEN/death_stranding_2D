#pragma once

#include "Array.h"
/**
 * HashSet
 */

namespace GamesEngineeringBase::Structures
{

    template <typename T, int BucketCount = 128>
    class HashSet
    {
    private:
        GEB::Structures::Array<T> *buckets_[BucketCount];
        int size_ = 0;

        int bucketIndex(const T &value) const { return value % BucketCount; }

    public:
        HashSet()
        {
            for (int i = 0; i < BucketCount; i++)
            {
                buckets_[i] = new GEB::Structures::Array<T>();
            }
        };

        virtual ~HashSet()
        {
            for (int i = 0; i < BucketCount; i++)
            {
                if (buckets_[i])
                {
                    delete buckets_[i];
                    buckets_[i] = nullptr;
                }
            }
        }

        HashSet(const HashSet &) = delete;
        HashSet &operator=(const HashSet &) = delete;

        int size() const { return size_; }

        bool insert(const T &value)
        {
            int index = bucketIndex(value);

            GEB::Structures::Array<T> *array = buckets_[index];
            for (int i = 0; i < array->size(); i++)
            {
                if (!array)
                    return false;

                if ((*array)[i] == value)
                    return false;
            }
            array->push_back(value);
            size_++;
            return true;
        }

        bool contains(const T &value) const
        {
            int index = bucketIndex(value);

            GEB::Structures::Array<T> *array = buckets_[index];

            for (int i = 0; i < array->size(); i++)
            {
                if ((*array)[i] == value)
                    return true;
            }

            return false;
        }

        void clear()
        {
            for (int i = 0; i < BucketCount; i++)
            {
                auto *array = buckets_[i];
                if (!array)
                    continue;

                for (int j = 0; j < array->size(); j++)
                {
                    (*array)[j] = T();
                }
            }
            size_ = 0;
        }
    };
};
