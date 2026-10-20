#pragma once

namespace GamesEngineeringBase::Math
{

    template <typename T>
    inline T min(const T &lhs, const T &rhs)
    {
        return lhs <= rhs ? lhs : rhs;
    }

    template <typename T>
    inline T max(const T &lhs, const T &rhs)
    {
        return lhs >= rhs ? lhs : rhs;
    }

    template <typename T>
    inline T abs(const T &value)
    {
        return value >= 0 ? value : -value;
    }
}