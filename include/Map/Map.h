#pragma once
#include <iostream>
#include <functional>
#include <optional>

template<typename Key, typename Value>
class Map {
private:
    struct Node {
        Value value;
        Node* left;
        Node* right;
        int height;

        Node(const Value& v) : value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;
    int nodeCount;
    std::function<Key(const Value&)> getKey;
    std::function<bool(const Key&, const Key&)> compare;

    Node* insert(Node* node, const Value& value);
    Node* find(Node* node, const Key& key) const;
    Node* erase(Node* node, const Key& key);
    void destroy(Node* node);
    Node* copyNode(const Node* node);
    int height(Node* node) const;
    int balanceFactor(Node* node) const;
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
    Node* balance(Node* node);
    void traverseNode(Node* node, std::function<void(const Value&)> func) const;
    void traverseNode(Node* node, std::function<void(Value*)> func) const;

public:
    Map(std::function<Key(const Value&)> getKeyFunc, std::function<bool(const Key&, const Key&)> compareFunc = std::less<Key>());
    Map(const Map& other);
    Map(Map&& other) noexcept;
    ~Map();

    bool insert(const Value& value);
    Value* find(const Key& key) const;
    std::optional<const Value&> get(const Key& key) const;
    bool erase(const Key& key);
    void clear();
    void traverse(std::function<void(const Value&)> func) const;
    void traverse(std::function<void(Value*)> func) const;
    int getNodeCount() const;

    Map& operator=(const Map& other);
    Map& operator=(Map&& other) noexcept;

    template<typename K, typename V>
    friend std::ostream& operator<<(std::ostream& out, const Map<K, V>& map);
    template<typename K, typename V>
    friend std::istream& operator>>(std::istream& in, Map<K, V>& map);
};



template<typename Key, typename Value>
Map<Key, Value>::Map(std::function<Key(const Value&)> getKeyFunc, std::function<bool(const Key&, const Key&)> compareFunc)
    : root(nullptr), nodeCount(0), getKey(getKeyFunc), compare(compareFunc) {}

template<typename Key, typename Value>
Map<Key, Value>::~Map() {
    destroy(root);
}

template<typename Key, typename Value>
Map<Key, Value>::Map(const Map& other)
    : root(nullptr), nodeCount(other.nodeCount), getKey(other.getKey), compare(other.compare) {
    root = copyNode(other.root);
}

template<typename Key, typename Value>
Map<Key, Value>& Map<Key, Value>::operator=(const Map& other) {
    if (this != &other) {
        clear();
        getKey = other.getKey;
        compare = other.compare;
        nodeCount = other.nodeCount;
        root = copyNode(other.root);
    }
    return *this;
}

template<typename Key, typename Value>
Map<Key, Value>::Map(Map&& other) noexcept
    : root(other.root), nodeCount(other.nodeCount), getKey(std::move(other.getKey)), compare(std::move(other.compare)) {
    other.root = nullptr;
    other.nodeCount = 0;
}

template<typename Key, typename Value>
Map<Key, Value>& Map<Key, Value>::operator=(Map&& other) noexcept {
    if (this != &other) {
        clear();
        root = other.root;
        nodeCount = other.nodeCount;
        getKey = std::move(other.getKey);
        compare = std::move(other.compare);
        other.root = nullptr;
        other.nodeCount = 0;
    }
    return *this;
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::copyNode(const Node* node) {
    if (!node) return nullptr;
    Node* newNode = new Node(node->value);
    newNode->left = copyNode(node->left);
    newNode->right = copyNode(node->right);
    newNode->height = node->height;
    return newNode;
}

template<typename Key, typename Value>
void Map<Key, Value>::clear() {
    destroy(root);
    root = nullptr;
    nodeCount = 0;
}

template<typename Key, typename Value>
void Map<Key, Value>::destroy(Node* node) {
    if (node) {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

template<typename Key, typename Value>
bool Map<Key, Value>::insert(const Value& value) {
    if (find(root, getKey(value))) {
        return false;
    }
    root = insert(root, value);
    nodeCount++;
    return true;
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::insert(Node* node, const Value& value) {
    if (!node) return new Node(value);

    if (compare(getKey(value), getKey(node->value))) {
        node->left = insert(node->left, value);
    } else {
        node->right = insert(node->right, value);
    }

    node->height = 1 + std::max(height(node->left), height(node->right));
    return balance(node);
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::find(Node* node, const Key& key) const {
    if (!node) return nullptr;

    if (compare(key, getKey(node->value))) {
        return find(node->left, key);
    } else if (compare(getKey(node->value), key)) {
        return find(node->right, key);
    } else {
        return node;
    }
}

template<typename Key, typename Value>
Value* Map<Key, Value>::find(const Key& key) const {
    Node* node = find(root, key);
    return node ? &node->value : nullptr;
}

template<typename Key, typename Value>
std::optional<const Value&> Map<Key, Value>::get(const Key& key) const {
    Node* node = find(root, key);
    if (!node) {
        return std::nullopt;
    }
    return node->value;
}

template<typename Key, typename Value>
int Map<Key, Value>::height(Node* node) const {
    return node ? node->height : 0;
}

template<typename Key, typename Value>
int Map<Key, Value>::balanceFactor(Node* node) const {
    return height(node->left) - height(node->right);
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    node->height = std::max(height(node->left), height(node->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;
    return newRoot;
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    node->height = std::max(height(node->left), height(node->right)) + 1;
    newRoot->height = std::max(height(newRoot->left), height(newRoot->right)) + 1;
    return newRoot;
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::balance(Node* node) {
    int balance = balanceFactor(node);
    if (balance > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    if (balance < -1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    return node;
}

template<typename Key, typename Value>
bool Map<Key, Value>::erase(const Key& key) {
    if (find(root, key)) {
        root = erase(root, key);
        nodeCount--;
        return true;
    }
    return false;
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::erase(Node* node, const Key& key) {
    if (!node) return nullptr;
    if (compare(key, getKey(node->value))) {
        node->left = erase(node->left, key);
    } else if (compare(getKey(node->value), key)) {
        node->right = erase(node->right, key);
    } else {
        if (!node->left) return node->right;
        if (!node->right) return node->left;
        Node* minNode = node->right;
        while (minNode->left) minNode = minNode->left;
        node->value = minNode->value;
        node->right = erase(node->right, getKey(minNode->value));
    }
    return balance(node);
}

template<typename Key, typename Value>
void Map<Key, Value>::traverse(std::function<void(const Value&)> func) const {
    traverseNode(root, func);
}

template<typename Key, typename Value>
void Map<Key, Value>::traverseNode(Node* node, std::function<void(const Value&)> func) const {
    if (node) {
        traverseNode(node->left, func);
        func(node->value);
        traverseNode(node->right, func);
    }
}

template<typename Key, typename Value>
void Map<Key, Value>::traverse(std::function<void(Value*)> func) const {
    traverseNode(root, func);
}

template<typename Key, typename Value>
void Map<Key, Value>::traverseNode(Node* node, std::function<void(Value*)> func) const {
    if (node) {
        traverseNode(node->left, func);
        func(&node->value);
        traverseNode(node->right, func);
    }
}

template<typename Key, typename Value>
int Map<Key, Value>::getNodeCount() const {
    return nodeCount;
}

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const Map<K, V>& map) {
    out << map.nodeCount << "\n";
    map.traverse([&out](const V& value) {
        out << value << "\n";
    });
    return out;
}

template<typename K, typename V>
std::istream& operator>>(std::istream& in, Map<K, V>& map) {
    int count;
    in >> count;
    map.clear();
    V value;
    while (count-- > 0 && in >> value) {
        map.insert(value);
    }
    return in;
}