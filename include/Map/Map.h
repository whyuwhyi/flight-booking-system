#pragma once

template<typename Key, typename Value>
class Map {
private:
    struct Node {
        Value value;
        Node *left;
        Node *right;
        int height;

        Node(const Value &v) : value(v), left(nullptr), right(nullptr), height(1) {}
    };

    Node *root;

    Node* insert(Node* node, const Value &value);
    Node* find(Node* node, const Key &key) const;
    void destroy(Node* node);
    int height(Node* node) const;
    int balanceFactor(Node* node) const;
    Node* rotateRight(Node* node);
    Node* rotateLeft(Node* node);
    Node* balance(Node* node);

public:
    Map();
    ~Map();
    void insert(const Value &value);
    Value* get(const Key &key) const;
};

template<typename Key, typename Value>
Map<Key, Value>::Map() : root(nullptr) {}

template<typename Key, typename Value>
Map<Key, Value>::~Map() {
    destroy(root);
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
void Map<Key, Value>::insert(const Value &value) {
    root = insert(root, value);
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::insert(Node* node, const Value &value) {
    if (!node) return new Node(value);

    if (value.getKey() < node->value.getKey()) {
        node->left = insert(node->left, value);
    } else {
        node->right = insert(node->right, value);
    }

    node->height = 1 + std::max(height(node->left), height(node->right));
    return balance(node);
}

template<typename Key, typename Value>
typename Map<Key, Value>::Node* Map<Key, Value>::find(Node* node, const Key &key) const {
    if (!node) return nullptr;

    if (key < node->value.getKey()) {
        return find(node->left, key);
    } else if (key > node->value.getKey()) {
        return find(node->right, key);
    } else {
        return node;
    }
}

template<typename Key, typename Value>
Value* Map<Key, Value>::get(const Key &key) const {
    Node *node = find(root, key);
    return node ? &node->value : nullptr;
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
