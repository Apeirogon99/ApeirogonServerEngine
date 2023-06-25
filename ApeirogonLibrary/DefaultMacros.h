#pragma once

template<typename T>
struct TypeName;

template<typename T>
std::string GetTypeName() {
    return TypeName<T>::name();
}

template<typename T>
struct TypeName {
    static std::string name() {
        return typeid(T).name();
    }
};