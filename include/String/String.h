#pragma once

#include <cstring>
#include <fstream>

class String {
private:
    char* data;
    size_t length;
public:
    String(const char* str = nullptr);
    String(const String& other);
    String(String&& other) noexcept;
    ~String();

    String& operator=(const String& other);
    String& operator=(String&& other) noexcept;
    String operator+(const String& other) const;
    char* operator+(const char *) const;
    bool operator<(const String& other) const;
    bool operator>(const String& other) const;
    bool operator<=(const String& other) const;
    bool operator>=(const String& other) const;
    bool operator==(const String& other) const;
    bool operator==(const char* other) const;
    size_t size() const;
    const char* c_str() const;

    friend std::ostream& operator<<(std::ostream& out, const String& str);
    friend std::istream& operator>>(std::istream& in, String& str);
};