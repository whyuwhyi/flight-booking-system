#pragma once

#include <cstring>
#include <fstream>

class String {
private:
    char* data;
    size_t length;
public:
    String(const char* str = NULL);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    size_t size() const;
    const char* c_str() const;

    friend std::ostream& operator<<(std::ostream& out, const String& str);
    friend std::istream& operator>>(std::istream& in, String& str);

};







String::String(const char* str) {
    if (str) {
        length = std::strlen(str);
        data = new char[length + 1];
        std::strcpy(data, str);
    } else {
        data = NULL;
        length = 0;
    }
}

String::String(const String& other) {
    length = other.length;
    data = new char[length + 1];
    std::strcpy(data, other.data);
}

String::String(String&& other) noexcept : data(other.data), length(other.length) {
    other.data = NULL;
    other.length = 0;
}

String::~String() {
    delete[] data;
}

String& String::operator=(const String& other) {
    if (this == &other) return *this;
    delete[] data;
    length = other.length;
    data = new char[length + 1];
    std::strcpy(data, other.data);
    return *this;
}

String& String::operator=(String&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;
    data = other.data;
    length = other.length;
    other.data = NULL;
    other.length = 0;
    return *this;
}

size_t String::size() const {
    return length;
}

const char* String::c_str() const {
    return data;
}

std::ostream& operator<<(std::ostream& out, const String& str) {
    out << str.data;
    return out;
}

std::istream& operator>>(std::istream& in, String& str) {
    char buffer[1024];
    in >> buffer;
    str = String(buffer);
    return in;
}
