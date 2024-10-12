#pragma once

#include <optional>
#include <stdexcept>
#include <functional>
#include <iostream>

// 自定义常数组替代动态数组
constexpr size_t MAX_CAPACITY = 1000;

// HashMap 类定义
template<typename Key, typename Value>
class HashMap {
public:
    HashMap(size_t capacity = MAX_CAPACITY, double max_load_factor = 0.75);
    bool insert(const Key& key, const Value& value);
    std::optional<Value> get(const Key& key) const;
    bool erase(const Key& key);
    bool contains(const Key& key) const;
    void rehash();
    size_t getSize() const;
    bool update(const Key& key, const Value& value);

    // 新增功能
    Value& operator[](const Key& key);  // 支持通过 key 访问或插入
    const Value& operator[](const Key& key) const;
    bool operator==(const HashMap& other) const;
    bool operator!=(const HashMap& other) const;

    void traverse(std::function<void(const Key&, const Value&)> func) const;

    // 输入输出重载
    template<typename K, typename V>
    friend std::ostream& operator<<(std::ostream& out, const HashMap<K, V>& map);

    template<typename K, typename V>
    friend std::istream& operator>>(std::istream& in, HashMap<K, V>& map);

private:
    struct HashNode {
        Key key;
        Value value;
        bool is_deleted;

        HashNode(const Key& k, const Value& v) : key(k), value(v), is_deleted(false) {}
    };

    size_t hash(const Key& key) const;
    size_t find_position(const Key& key) const;
    size_t find_insert_position(const Key& key) const;

    std::optional<HashNode> table[MAX_CAPACITY];
    size_t capacity;
    size_t size;
    double max_load_factor;
};

// Implementation
template<typename Key, typename Value>
HashMap<Key, Value>::HashMap(size_t capacity, double max_load_factor)
    : capacity(capacity), size(0), max_load_factor(max_load_factor) {}

template<typename Key, typename Value>
size_t HashMap<Key, Value>::hash(const Key& key) const {
    std::hash<std::string> str_hash;
    return str_hash(key.c_str()) % capacity;
}

template<typename Key, typename Value>
size_t HashMap<Key, Value>::find_position(const Key& key) const {
    size_t index = hash(key);
    size_t original_index = index;
    while (table[index]) {
        if (!table[index]->is_deleted && table[index]->key == key) {
            return index;
        }
        index = (index + 1) % capacity;
        if (index == original_index) {
            break;
        }
    }
    return capacity;
}

template<typename Key, typename Value>
size_t HashMap<Key, Value>::find_insert_position(const Key& key) const {
    size_t index = hash(key);
    size_t original_index = index;
    while (table[index] && !table[index]->is_deleted) {
        index = (index + 1) % capacity;
        if (index == original_index) {
            throw std::overflow_error("HashMap is full");
        }
    }
    return index;
}

template<typename Key, typename Value>
bool HashMap<Key, Value>::insert(const Key& key, const Value& value) {
    if (static_cast<double>(size) / capacity >= max_load_factor) {
        rehash();
    }
    size_t index = find_position(key);
    if (index != capacity && table[index] && !table[index]->is_deleted && table[index]->key == key) {
        return false;
    }
    index = find_insert_position(key);
    table[index] = HashNode(key, value);
    ++size;
    return true;
}

template<typename Key, typename Value>
std::optional<Value> HashMap<Key, Value>::get(const Key& key) const {
    size_t index = find_position(key);
    if (index != capacity && table[index] && !table[index]->is_deleted && table[index]->key == key) {
        return table[index]->value;
    }
    return std::nullopt;
}

template<typename Key, typename Value>
bool HashMap<Key, Value>::erase(const Key& key) {
    size_t index = find_position(key);
    if (index != capacity && table[index] && !table[index]->is_deleted && table[index]->key == key) {
        table[index]->is_deleted = true;
        --size;
        return true;
    }
    return false;
}

template<typename Key, typename Value>
bool HashMap<Key, Value>::contains(const Key& key) const {
    return get(key).has_value();
}

template<typename Key, typename Value>
void HashMap<Key, Value>::rehash() {
    size_t new_capacity = capacity * 2;
    std::optional<HashNode> new_table[MAX_CAPACITY];

    for (size_t i = 0; i < capacity; ++i) {
        if (table[i] && !table[i]->is_deleted) {
            std::hash<std::string> str_hash;
            size_t index = str_hash(table[i]->key.c_str()) % new_capacity;
            while (new_table[index]) {
                index = (index + 1) % new_capacity;
            }
            new_table[index] = table[i];
        }
    }

    for (size_t i = 0; i < new_capacity; ++i) {
        table[i] = new_table[i];
    }
    capacity = new_capacity;
}

template<typename Key, typename Value>
size_t HashMap<Key, Value>::getSize() const {
    return size;
}

template<typename Key, typename Value>
bool HashMap<Key, Value>::update(const Key& key, const Value& value) {
    size_t index = find_position(key);
    if (index != capacity && table[index] && !table[index]->is_deleted) {
        table[index]->value = value;
        return true;
    }
    return false;
}

// 运算符重载：[] 支持通过键访问或插入
template<typename Key, typename Value>
Value& HashMap<Key, Value>::operator[](const Key& key) {
    size_t index = find_position(key);
    if (index == capacity || table[index]->is_deleted) {
        // 如果 key 不存在，则插入一个默认值
        insert(key, Value());
        index = find_position(key);
    }
    return table[index]->value;
}

template<typename Key, typename Value>
const Value& HashMap<Key, Value>::operator[](const Key& key) const {
    size_t index = find_position(key);
    if (index == capacity || table[index]->is_deleted) {
        throw std::out_of_range("Key not found");
    }
    return table[index]->value;
}

// 运算符重载：比较
template<typename Key, typename Value>
bool HashMap<Key, Value>::operator==(const HashMap& other) const {
    if (size != other.size) {
        return false;
    }
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i] && !table[i]->is_deleted) {
            auto other_value = other.get(table[i]->key);
            if (!other_value.has_value() || other_value.value() != table[i]->value) {
                return false;
            }
        }
    }
    return true;
}

template<typename Key, typename Value>
bool HashMap<Key, Value>::operator!=(const HashMap& other) const {
    return !(*this == other);
}

template<typename Key, typename Value>
void HashMap<Key, Value>::traverse(std::function<void(const Key&, const Value&)> func) const {
    for (size_t i = 0; i < capacity; ++i) {
        if (table[i] && !table[i]->is_deleted) {
            func(table[i]->key, table[i]->value);
        }
    }
}

// 输入输出重载
template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const HashMap<K, V>& map) {
    out << map.size << "\n";
    map.traverse([&out](const K& key, const V& value) {
        out << key << " " << value << "\n";
    });
    return out;
}

template<typename K, typename V>
std::istream& operator>>(std::istream& in, HashMap<K, V>& map) {
    size_t count;
    in >> count;
    map.clear();
    K key;
    V value;
    while (count-- > 0 && in >> key >> value) {
        map.insert(key, value);
    }
    return in;
}