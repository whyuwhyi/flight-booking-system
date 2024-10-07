#pragma once
#include <vector>
#include <optional>
#include <stdexcept>
#include <functional>

template<typename Key, typename Value>
class HashMap {
public:
    HashMap(size_t capacity = 101, double max_load_factor = 0.75);
    bool insert(const Key& key, const Value& value);
    std::optional<Value> get(const Key& key) const;
    bool erase(const Key& key);
    bool contains(const Key& key) const;
    void rehash();
    size_t getSize() const;
    bool update(const Key& key, const Value& value);

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

    std::vector<std::optional<HashNode>> table;
    size_t capacity;
    size_t size;
    double max_load_factor;
};

// Implementation

template<typename Key, typename Value>
HashMap<Key, Value>::HashMap(size_t capacity, double max_load_factor)
    : table(capacity), capacity(capacity), size(0), max_load_factor(max_load_factor) {}

template<typename Key, typename Value>
size_t HashMap<Key, Value>::hash(const Key& key) const {
    return std::hash<Key>{}(key) % capacity;
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
    return capacity; // Indicates not found
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
        return false; // Key already exists
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
    std::vector<std::optional<HashNode>> new_table(new_capacity);

    for (const auto& node : table) {
        if (node && !node->is_deleted) {
            size_t index = std::hash<Key>{}(node->key) % new_capacity;
            while (new_table[index]) {
                index = (index + 1) % new_capacity;
            }
            new_table[index] = node;
        }
    }

    table = std::move(new_table);
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