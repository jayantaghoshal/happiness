#pragma once
#include <tuple>

template <typename Base_>
class CommonApiEnumeration
{
    public:
    CommonApiEnumeration() = default;
    CommonApiEnumeration(const Base_ &_value) :
            value_(_value) {
    }

    inline CommonApiEnumeration &operator=(const Base_ &_value) {
        value_ = _value;
        return (*this);
    }

    inline operator const Base_() const {
        return value_;
    }

    inline bool operator==(const CommonApiEnumeration<Base_> &_other) const {
        return (value_ == _other.value_);
    }

    inline bool operator!=(const CommonApiEnumeration<Base_> &_other) const {
        return (value_ != _other.value_);
    }

    inline bool operator<(const CommonApiEnumeration<Base_> &_other) const {
        return (value_ < _other.value_);
    }

    inline bool operator<=(const CommonApiEnumeration<Base_> &_other) const {
        return (value_ <= _other.value_);
    }

    inline bool operator>(const CommonApiEnumeration<Base_> &_other) const {
        return (value_ > _other.value_);
    }

    inline bool operator>=(const CommonApiEnumeration<Base_> &_other) const {
        return (value_ >= _other.value_);
    }

    Base_ value_;
};



namespace CommonAPI {
template<typename... Types_>
struct Struct {
    std::tuple<Types_...> values_;
};
}