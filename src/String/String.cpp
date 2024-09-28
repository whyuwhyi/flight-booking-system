#include <String/String.h>


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

String String::operator+(const String& other) const {
    size_t newLength = length + other.length;
    char* newData = new char[newLength + 1];
    std::copy(data, data + length, newData);
    std::copy(other.data, other.data + other.length, newData + length);
    newData[newLength] = '\0';
    return String(newData);
}

char* String::operator+(const char* other) const {
    size_t otherLength = strlen(other);
    size_t newLength = length + otherLength;
    char* newData = new char[newLength + 1];
    std::copy(data, data + length, newData);
    std::copy(other, other + otherLength, newData + length);
    newData[newLength] = '\0';
    return newData;
}

bool String::operator==(const String& other) const {
        if (length != other.length) return false;
        return std::strcmp(data, other.data) == 0;
    }

bool String::operator==(const char* other) const {
    if (other == nullptr) return (data == NULL);
    return std::strcmp(data, other) == 0;
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
