#ifndef REI_BASE_TYPES
#define REI_BASE_TYPES

#include <variant>
#include <string>

enum class BaseType : unsigned char
{
    integer = 1,
    real = 2,
    none = 3
};

std::string to_string(BaseType e);

struct BaseTypeVariable
{
    BaseTypeVariable() = default;
    BaseTypeVariable(const BaseTypeVariable&) = default;
    BaseTypeVariable(BaseTypeVariable&&) = default;
    BaseTypeVariable& operator = (const BaseTypeVariable&) = default;
    BaseTypeVariable& operator = (BaseTypeVariable&&) = default;
    BaseTypeVariable(BaseType t);
    BaseTypeVariable(int v, bool c = false, bool l = false);
    BaseTypeVariable(double v, bool c = false, bool l = false);
    BaseType type;
    bool isConst;
    bool isTypeLocked;
    std::variant<int, double> value;
};

void print(BaseTypeVariable v);

namespace arithmetic {
    BaseTypeVariable sum(BaseTypeVariable l, BaseTypeVariable r);
    BaseTypeVariable dif(BaseTypeVariable l, BaseTypeVariable r);
    BaseTypeVariable mul(BaseTypeVariable l, BaseTypeVariable r);
    BaseTypeVariable div(BaseTypeVariable l, BaseTypeVariable r);
    BaseTypeVariable mod(BaseTypeVariable l, BaseTypeVariable r);
    BaseTypeVariable int_div(BaseTypeVariable l, BaseTypeVariable r);
    BaseTypeVariable neg(BaseTypeVariable v);
}

namespace casting {
    BaseTypeVariable toReal(BaseTypeVariable v);
    BaseTypeVariable toConst(BaseTypeVariable v);
    BaseTypeVariable toTypeLocked(BaseTypeVariable v);
}

#endif // REI_BASE_TYPES
