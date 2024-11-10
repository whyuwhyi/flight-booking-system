#pragma once

#include <cstddef>
#include <iostream>

class String {
public:
    String();
    String(const char* str);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    String& operator=(String other);

    String operator+(const String& other) const;
    String operator+(const char* other) const;

    bool operator<(const String& other) const;
    bool operator>(const String& other) const;
    bool operator<=(const String& other) const;
    bool operator>=(const String& other) const;
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    bool operator==(const char* other) const;
    bool operator!=(const char* other) const;

    size_t size() const;
    const char* c_str() const;

    friend std::ostream& operator<<(std::ostream& out, const String& str);
    friend std::istream& operator>>(std::istream& in, String& str);
    friend String operator+(const char* lhs, const String& rhs);

private:
    char* data;
    size_t length;

    String(char* data, size_t length);
};

