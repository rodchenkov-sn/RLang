#include "ReiBaseTypes.hpp"
#include <stdexcept>
#include <cmath>
#include <iostream>

std::string to_string(BaseType e)
{
    switch (e) {
    case BaseType::integer :
        return "integer";
    case BaseType::real :
        return "real";
    case BaseType::none :
        return "none";
    default :
        return "unknown";
    }
}


BaseTypeVariable::BaseTypeVariable(BaseType t):
    type(t),
    isConst(false),
    isTypeLocked(true)
{
    switch (t) {
    case BaseType::integer:
        value = 0;
        break;
    case BaseType::real:
        value = 0.0;
        break;
    default:
        isTypeLocked = false;
    }
}

BaseTypeVariable::BaseTypeVariable(const int v, const bool c, const bool l):
    type(BaseType::integer),
    isConst(c),
    isTypeLocked(l),
    value(v)
{
}

BaseTypeVariable::BaseTypeVariable(const double v, const bool c, const bool l):
    type(BaseType::real),
    isConst(c),
    isTypeLocked(l),
    value(v)
{
}

void print(BaseTypeVariable v)
{
    switch (v.type) {
    case BaseType::integer:
        std::cout << std::get<int>(v.value) << " :: ";
        break;
    case BaseType::real:
        std::cout << std::get<double>(v.value) << " :: ";
        break;
    case BaseType::none:
        std::cout << "??? :: ";
    }
    std::cout << to_string(v.type);
}

//
// WARNING : Lots of wet code
// ToDo: dry this
//
BaseTypeVariable arithmetic::sum(BaseTypeVariable l, BaseTypeVariable r)
{
    if (l.type == BaseType::real || r.type == BaseType::real) {
        l = casting::toReal(l);
        r = casting::toReal(r);
        return { std::get<double>(l.value) + std::get<double>(r.value) };
    }
    return { std::get<int>(l.value) + std::get<int>(r.value) };
}

BaseTypeVariable arithmetic::dif(BaseTypeVariable l, BaseTypeVariable r)
{
    if (l.type == BaseType::real || r.type == BaseType::real) {
        l = casting::toReal(l);
        r = casting::toReal(r);
        return { std::get<double>(l.value) - std::get<double>(r.value) };
    }
    return { std::get<int>(l.value) - std::get<int>(r.value) };
}

BaseTypeVariable arithmetic::mul(BaseTypeVariable l, BaseTypeVariable r)
{
    if (l.type == BaseType::real || r.type == BaseType::real) {
        l = casting::toReal(l);
        r = casting::toReal(r);
        return { std::get<double>(l.value) * std::get<double>(r.value) };
    }
    return { std::get<int>(l.value) * std::get<int>(r.value) };
}

BaseTypeVariable arithmetic::div(BaseTypeVariable l, BaseTypeVariable r)
{
    l = casting::toReal(l);
    r = casting::toReal(r);
    return { std::get<double>(l.value) / std::get<double>(r.value) };
}

BaseTypeVariable arithmetic::mod(BaseTypeVariable l, BaseTypeVariable r)
{
    if (l.type == BaseType::real || r.type == BaseType::real) {
        l = casting::toReal(l);
        r = casting::toReal(r);
        return { std::fmod(std::get<double>(l.value), std::get<double>(r.value)) };
    }
    return { std::get<int>(l.value) % std::get<int>(r.value) };
}

BaseTypeVariable arithmetic::int_div(BaseTypeVariable l, BaseTypeVariable r)
{
    l = casting::toReal(l);
    r = casting::toReal(r);
    return { static_cast<int>(std::get<double>(l.value) / std::get<double>(r.value)) };
}

BaseTypeVariable arithmetic::neg(BaseTypeVariable v)
{
    if (v.type == BaseType::real) {
        return { std::get<double>(v.value) * -1 };
    }
    return { std::get<int>(v.value) * -1 };
}

BaseTypeVariable casting::toReal(BaseTypeVariable v)
{
    if (v.type == BaseType::real) {
        return v;
    }
    if (v.type == BaseType::integer) {
        int val = std::get<int>(v.value);
        return { static_cast<double>(val), v.isConst, v.isTypeLocked };
    }
    throw std::invalid_argument("Invalid casting (" + to_string(v.type) + " -> " + to_string(BaseType::real) + ").");
}

BaseTypeVariable casting::toConst(BaseTypeVariable v)
{
    v.isConst = true;
    v.isTypeLocked = true;
    return v;
}

BaseTypeVariable casting::toTypeLocked(BaseTypeVariable v)
{
    v.isTypeLocked = true;
    return v;
}


