#include <String/String.h>
#include <cstring>
#include <iostream>
#include <utility>

String::String() : data(nullptr), length(0) {}

String::String(const char* str) {
    if (str) {
        length = std::strlen(str);
        data = new char[length + 1];
        std::memcpy(data, str, length + 1);
    } else {
        data = nullptr;
        length = 0;
    }
}

String::String(const String& other) {
    if (other.data) {
        length = other.length;
        data = new char[length + 1];
        std::memcpy(data, other.data, length + 1);
    } else {
        data = nullptr;
        length = 0;
    }
}

String::String(String&& other) noexcept : data(other.data), length(other.length) {
    other.data = nullptr;
    other.length = 0;
}

String::String(char* data, size_t length) : data(data), length(length) {}

String::~String() {
    delete[] data;
}

String& String::operator=(String other) {
    std::swap(data, other.data);
    std::swap(length, other.length);
    return *this;
}

String String::operator+(const String& other) const {
    size_t newLength = length + other.length;
    char* newData = new char[newLength + 1];
    if (data) {
        std::memcpy(newData, data, length);
    }
    if (other.data) {
        std::memcpy(newData + length, other.data, other.length);
    }
    newData[newLength] = '\0';
    return String(newData, newLength);
}

String String::operator+(const char* other) const {
    if (!other) return *this;
    size_t otherLength = std::strlen(other);
    size_t newLength = length + otherLength;
    char* newData = new char[newLength + 1];
    if (data) {
        std::memcpy(newData, data, length);
    }
    std::memcpy(newData + length, other, otherLength);
    newData[newLength] = '\0';
    return String(newData, newLength);
}

bool String::operator<(const String& other) const {
    if (!data || !other.data) return false;
    return std::strcmp(data, other.data) < 0;
}

bool String::operator>(const String& other) const {
    if (!data || !other.data) return false;
    return std::strcmp(data, other.data) > 0;
}

bool String::operator<=(const String& other) const {
    if (!data || !other.data) return false;
    return std::strcmp(data, other.data) <= 0;
}

bool String::operator>=(const String& other) const {
    if (!data || !other.data) return false;
    return std::strcmp(data, other.data) >= 0;
}

bool String::operator==(const String& other) const {
    if (length != other.length) return false;
    if (!data && !other.data) return true;
    if (!data || !other.data) return false;
    return std::strcmp(data, other.data) == 0;
}

bool String::operator==(const char* other) const {
    if (!data && !other) return true;
    if (!data || !other) return false;
    return std::strcmp(data, other) == 0;
}

size_t String::size() const {
    return length;
}

const char* String::c_str() const {
    return data ? data : "";
}

std::ostream& operator<<(std::ostream& out, const String& str) {
    if (str.data) {
        out << str.data;
    }
    return out;
}

std::istream& operator>>(std::istream& in, String& str) {
    char buffer[1024];
    in >> buffer;
    str = String(buffer);
    return in;
}

String operator+(const char* lhs, const String& rhs) {
    if (!lhs) return rhs;
    size_t lhsLength = std::strlen(lhs);
    size_t newLength = lhsLength + rhs.length;
    char* newData = new char[newLength + 1];
    std::memcpy(newData, lhs, lhsLength);
    if (rhs.data) {
        std::memcpy(newData + lhsLength, rhs.data, rhs.length);
    }
    newData[newLength] = '\0';
    return String(newData, newLength);
}