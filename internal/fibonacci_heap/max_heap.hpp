#ifndef FIBONACCI_HEAP_HPP
#define FIBONACCI_HEAP_HPP

#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <cmath>
#include <bits/stdc++.h>

template <typename CoreValue, typename KeyType>
class FibonacciHeap
{
public:
    template <typename Core, typename Key>
    struct HeapNode
    {
        Key key;
        bool mark;
        int degree;
        std::shared_ptr<HeapNode<Core, Key>> child;
        std::shared_ptr<HeapNode<Core, Key>> left;
        std::shared_ptr<HeapNode<Core, Key>> right;
        std::weak_ptr<HeapNode> parent;
        Core value;

        explicit HeapNode(Key key, Core value)
            : key(key), mark(false), degree(0), child(nullptr),
              left(nullptr), right(nullptr), parent(), value(value) {}
    };

    explicit FibonacciHeap() : max_(nullptr), size_(0) {}

    std::shared_ptr<HeapNode<CoreValue, KeyType>> insert(double key, CoreValue value)
    {
        auto new_node = std::make_shared<HeapNode<CoreValue, KeyType>>(key, value);
        new_node->left = new_node;
        new_node->right = new_node;
        if (!max_)
        {
            max_ = new_node;
            max_->right = max_;
            max_->left = max_;
        }
        else
        {
            insert_to_root_list(new_node);
            if (new_node->key > max_->key)
            {
                max_ = new_node;
            }
        }
        size_++;
        return new_node;
    }

    void delete_node(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &node)
    {
        if (!node)
        {
            throw std::runtime_error("Node does not exist.");
        }

        // Установка нового ключа равным минимальному значению
        KeyType new_key = std::numeric_limits<KeyType>::max();
        if (node->key < new_key)
        {
            increase_key(node, new_key);
        }
        else
        {
            decrease_key(node, new_key);
        }

        // Удаление максимального узла (теперь это узел с минимальным ключом)
        delete_max();
    }

    CoreValue get_max_value()
    {
        if (!max_)
        {
            throw std::runtime_error("The Fibonacci heap is empty.");
        }
        return max_->value;
    }

    double get_max() const
    {
        if (!max_)
        {
            throw std::runtime_error("The Fibonacci heap is empty.");
        }
        return max_->key;
    }

    void decrease_key(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &node, double new_key)
    {
        if (!node)
        {
            throw std::runtime_error("Node does not exist.");
        }

        if (node->key < new_key)
        {
            throw std::runtime_error("New key is larger than the current key.");
        }

        node->key = new_key;
        auto parent = node->parent.lock();
        if (parent && node->key < parent->key)
        {
            cut(node, parent);
            cascading_cut(parent);
        }

        // Обновление значения max_, если необходимо
        auto current = max_;
        bool found_new_max = false;
        do
        {
            if (current->key > max_->key)
            {
                max_ = current;
                found_new_max = true;
            }
            current = current->right;
        } while (current != max_);

        // Если max_ установлено на уменьшенный узел, убедитесь, что оно корректно
        if (!found_new_max && node == max_)
        {
            max_ = max_->right;
            current = max_;
            do
            {
                if (current->key > max_->key)
                {
                    max_ = current;
                }
                current = current->right;
            } while (current != max_);
        }
    }

    void increase_key(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &node, double new_key)
    {
        if (!node)
        {
            throw std::runtime_error("Node does not exist.");
        }

        if (node->key > new_key)
        {
            throw std::runtime_error("New key is smaller than the current key.");
        }

        node->key = new_key;
        auto parent = node->parent.lock();
        if (parent && node->key > parent->key)
        {
            cut(node, parent);
            cascading_cut(parent);
        }

        if (node->key > max_->key)
        {
            max_ = node;
        }
    }

    void delete_max()
    {
        if (!max_)
        {
            throw std::runtime_error("The Fibonacci heap is empty.");
        }

        if (max_->child)
        {
            auto current = max_->child;
            do
            {
                current->parent.reset();
                current = current->right;
            } while (current != max_->child);

            // Remove the old minimum node from the root list
            if (max_->right != max_)
            {
                max_->right->left = max_->left;
                max_->left->right = max_->right;
                merge_root_lists(max_->right, max_->child);
                max_ = max_->right;
            }
            else
            {
                max_ = max_->child;
            }
        }
        else
        {
            if (max_->right != max_)
            {
                max_->right->left = max_->left;
                max_->left->right = max_->right;
                max_ = max_->right;
            }
            else
            {
                max_.reset();
            }
        }

        if (max_)
        {
            consolidate();
        }

        size_--;
    }

    double get_next_max()
    {
        if (!max_)
        {
            throw std::runtime_error("Heap is empty.");
        }

        std::shared_ptr<HeapNode<CoreValue, KeyType>> next_max = nullptr;
        double next_max_value = INT_MIN;

        auto current = max_;
        do
        {
            // Проверка соседей текущего узла
            std::shared_ptr<HeapNode<CoreValue, KeyType>> neighbor = current->right;
            while (neighbor != current)
            {
                if (neighbor->key < max_->key && neighbor->key > next_max_value)
                {
                    next_max_value = neighbor->key;
                    next_max = neighbor;
                }
                neighbor = neighbor->right;
            }

            // Проверка детей текущего узла
            if (current->child)
            {
                std::shared_ptr<HeapNode<CoreValue, KeyType>> child = current->child;
                do
                {
                    if (child->key < max_->key && child->key > next_max_value)
                    {
                        next_max_value = child->key;
                        next_max = child;
                    }
                    child = child->right;
                } while (child != current->child);
            }

            current = current->right;
        } while (current != max_);

        if (!next_max)
        {
            throw std::runtime_error("No next maximum element found.");
        }

        return next_max_value;
    }

    bool is_empty() const
    {
        return max_ == nullptr;
    }

    FibonacciHeap<CoreValue, KeyType> copy_without_max()
    {
        if (!max_)
        {
            throw std::runtime_error("The Fibonacci heap is empty.");
        }

        FibonacciHeap<CoreValue, KeyType> new_heap;

        // Функция для добавления узлов в новую кучу
        std::function<void(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &)> add_nodes_to_new_heap;
        add_nodes_to_new_heap = [&](const std::shared_ptr<HeapNode<CoreValue, KeyType>> &src)
        {
            if (!src)
                return;
            auto current = src;
            do
            {
                if (current != max_)
                {
                    new_heap.insert(current->key, current->value);
                }
                if (current->child)
                {
                    add_nodes_to_new_heap(current->child);
                }
                current = current->right;
            } while (current != src);
        };

        // Добавляем узлы в новую кучу
        add_nodes_to_new_heap(max_);

        return new_heap;
    }

    std::shared_ptr<HeapNode<CoreValue, KeyType>> find_node_by_value(const CoreValue value)
    {
        if (!max_)
        {
            return nullptr;
        }

        std::function<std::shared_ptr<HeapNode<CoreValue, KeyType>>(std::shared_ptr<HeapNode<CoreValue, KeyType>>)> find_node_recursive;
        find_node_recursive = [&](std::shared_ptr<HeapNode<CoreValue, KeyType>> node) -> std::shared_ptr<HeapNode<CoreValue, KeyType>>
        {
            if (!node)
            {
                return nullptr;
            }

            auto current = node;
            do
            {
                if (current->value == value)
                {
                    return current;
                }

                auto child_result = find_node_recursive(current->child);
                if (child_result)
                {
                    return child_result;
                }

                current = current->right;
            } while (current != node);

            return nullptr;
        };

        return find_node_recursive(max_);
    }

private:
    std::shared_ptr<HeapNode<CoreValue, KeyType>> max_;
    int size_;

    void insert_to_root_list(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &node)
    {
        node->right = max_->right;
        node->left = max_;
        max_->right->left = node;
        max_->right = node;
    }

    void merge_root_lists(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &a, const std::shared_ptr<HeapNode<CoreValue, KeyType>> &b)
    {
        a->right->left = b->left;
        b->left->right = a->right;
        a->right = b;
        b->left = a;
    }

    void cut(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &node, const std::shared_ptr<HeapNode<CoreValue, KeyType>> &parent)
    {
        if (node->right == node)
        {
            parent->child.reset();
        }
        else
        {
            node->right->left = node->left;
            node->left->right = node->right;
            parent->child = node->right;
        }

        parent->degree--;

        insert_to_root_list(node);
        node->parent.reset();
        node->mark = false;
    }

    void cascading_cut(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &node)
    {
        auto parent = node->parent.lock();
        if (parent)
        {
            if (!node->mark)
            {
                node->mark = true;
            }
            else
            {
                cut(node, parent);
                cascading_cut(parent);
            }
        }
    }

    void consolidate()
    {
        std::vector<std::shared_ptr<HeapNode<CoreValue, KeyType>>> A(static_cast<size_t>(std::log2(size_)) + 1);

        // Create a vector of nodes from the root list
        std::vector<std::shared_ptr<HeapNode<CoreValue, KeyType>>> root_nodes;
        auto current = max_;
        do
        {
            root_nodes.push_back(current);
            current = current->right;
        } while (current != max_);

        // Process each node in the root list
        for (auto x : root_nodes)
        {
            int d = x->degree;

            while (A[d])
            {
                auto y = A[d];
                if (x->key < y->key)
                {
                    std::swap(x, y);
                }
                link(y, x);
                A[d].reset();
                d++;
            }
            A[d] = x;
        }

        max_.reset();
        for (auto &node : A)
        {
            if (node)
            {
                if (!max_)
                {
                    max_ = node;
                    max_->right = max_;
                    max_->left = max_;
                }
                else
                {
                    insert_to_root_list(node);
                    if (node->key > max_->key)
                    {
                        max_ = node;
                    }
                }
            }
        }
    }

    void link(const std::shared_ptr<HeapNode<CoreValue, KeyType>> &y, const std::shared_ptr<HeapNode<CoreValue, KeyType>> &x)
    {
        y->right->left = y->left;
        y->left->right = y->right;

        if (!x->child)
        {
            x->child = y;
            y->right = y;
            y->left = y;
        }
        else
        {
            merge_root_lists(x->child, y);
        }

        y->parent = x;
        x->degree++;
        y->mark = false;
    }
};

#endif