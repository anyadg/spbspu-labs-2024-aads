#ifndef BINARY_SEARCH_TREE_AVL_HPP
#define BINARY_SEARCH_TREE_AVL_HPP

#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <iterator>

namespace ponomarev
{
  template< typename Key, typename Value, typename Compare >
  class BinarySearchTree
  {
  public:
    class Iterator;
    class ConstIterator;

    using value_t = typename std::pair< Key, Value >;

    BinarySearchTree();
    BinarySearchTree(const BinarySearchTree & tree);
    BinarySearchTree(BinarySearchTree && tree) noexcept;
    ~BinarySearchTree() = default;

    size_t getSize() const noexcept;
    bool isEmpty() const noexcept;

    Value & operator[](const Key & key);
    Value & operator[](Key && key);
    Value & at(const Key & key);
    const Value & at(const Key & key) const;

    void insert(const value_t & pair);
    void insert(value_t && pair);
    Iterator erase(Iterator pos);
    size_t erase(const Key & key);

    void clear();
    void swap(BinarySearchTree & other) noexcept;

    Iterator find(const Key & key);
    ConstIterator find(const Key & key) const;
    std::pair< Iterator, Iterator > equal_range(const Key & key);
    std::pair< ConstIterator, ConstIterator> equalRange(const Key & key) const;

    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;
  private:
    class Node;

    Node * push(const Key & key, const Value & value, Node * node_);
    Node * rightRotate(Node * node_);
    Node * leftRotate(Node * node_);
    Node * leftRightRotate(Node * node_);
    Node * rightLeftRotate(Node * node_);

    size_t getHeight(Node * node_);
    void deleteAll(Node * node);

    Node * root_;
    size_t size_;
  };
}

using namespace ponomarev;

template< typename Key, typename Value, typename Compare >
class BinarySearchTree< Key, Value, Compare >::Node
{
  friend class BinarySearchTree;
public:
  Node(const Key key, const Value value):
    left(nullptr),
    right(nullptr),
    parent(nullptr),
    height(1),
    data(value_t(key, value))
  {}
private:
  Node * left;
  Node * right;
  Node * parent;
  size_t height;
  value_t data;
};

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::BinarySearchTree()
{
  root_ = nullptr;
  size_ = 0;
}

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::BinarySearchTree(const BinarySearchTree< Key, Value, Compare > & tree)
{
  root_ = tree.root_;
  size_ = tree.size_;
}

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::BinarySearchTree(BinarySearchTree< Key, Value, Compare > && tree) noexcept
{
  root_ = tree.root_;
  size_ = std::move(tree.size_);
}

template< typename Key, typename Value, typename Compare >
size_t BinarySearchTree< Key, Value, Compare >::getSize() const noexcept
{
  return size_;
}

template< typename Key, typename Value, typename Compare >
bool BinarySearchTree< Key, Value, Compare >::isEmpty() const noexcept
{
  return size_ == 0;
}

template< typename Key, typename Value, typename Compare >
Value & BinarySearchTree< Key, Value, Compare >::operator[](const Key & key)
{
  Iterator iter(find(key));
  if (iter == end())
  {
    insert(std::pair< Key, Value>(key, Value()));
    iter = find(key);
  }
  return iter->second;
}

template< typename Key, typename Value, typename Compare >
Value & BinarySearchTree< Key, Value, Compare >::operator[](Key && key)
{
  Iterator iter(find(key));
  if (iter == end())
  {
    insert(std::pair< Key, Value>(key, Value()));
    iter = find(key);
  }
  return iter->second;
}

template< typename Key, typename Value, typename Compare >
Value & BinarySearchTree< Key, Value, Compare >::at(const Key & key)
{
  if (find(key) == end())
  {
    throw std::out_of_range("The element does not exist\n");
  }
  else
  {
    return (find(key))->second;
  }
}

template< typename Key, typename Value, typename Compare >
const Value & BinarySearchTree< Key, Value, Compare >::at(const Key & key) const
{
  if (find(key) == end())
  {
    throw std::out_of_range("The element does not exist\n");
  }
  else
  {
    return (find(key))->second;
  }
}

template< typename Key, typename Value, typename Compare >
void BinarySearchTree< Key, Value, Compare >::insert(const value_t & pair)
{
  root_ = push(pair.first, pair.second, root_);
  size_++;
}

template< typename Key, typename Value, typename Compare >
void BinarySearchTree< Key, Value, Compare >::insert(value_t && pair)
{
  root_ = push(pair.first, pair.second, root_);
  size_++;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Node * BinarySearchTree< Key, Value, Compare >::push(const Key & key, const Value & value, Node * node_)
{
  if (node_ == nullptr)
  {
    node_ = new Node(key, value);
  }
  else if (key < node_->data.first)
  {
    node_->left = push(key, value, node_->left);
    node_->left->parent = node_;
    node_->height++;
    if (height(node_->left) - height(node_->right) > 1)
    {
      if (node_->right == nullptr || key < node_->left->data.first)
      {
        node_ = rightRotate(node_);
      }
      else
      {
        node_ = leftRightRotate(node_);
      }
    }
  }
  else if (key > node_->data.first)
  {
    node_->right = push(key, value, node_->right);
    node_->right->parent = node_;
    node_->height++;
    if ((height(node_->right) - height(node_->left)) > 1)
    {
      if (node_->left == nullptr || key > node_->right->data.first)
      {
        node_ = leftRotate(node_);
      }
      else
      {
        node_ = rightLeftRotate(node_);
      }
    }
  }
  return node_;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Node * BinarySearchTree< Key, Value, Compare >::rightRotate(Node * node_)
{
  Node * temp = node_->left;
  node_->left = temp->right;
  if (node_->left != nullptr)
  {
    node_->left->parent = node_;
  }
  temp->parent = node_->parent;
  node_->parent = temp;
  temp->right = node_;
  node_->height = std::max(height(node_->left), height(node_->right)) + 1;
  temp->height = std::max(node_->height, height(temp->left)) + 1;
  return temp;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Node * BinarySearchTree< Key, Value, Compare >::leftRotate(Node * node_)
{
  Node * temp = node_->right;
  node_->right = temp->left;
  if (node_->right != nullptr)
  {
    node_->right->parent = node_;
  }
  temp->parent = node_->parent;
  node_->parent = temp;
  temp->left = node_;
  node_->height = std::max(height(node_->left), height(node_->right)) + 1;
  temp->height = std::max(node_->height, height(temp->right)) + 1;
  return temp;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Node * BinarySearchTree< Key, Value, Compare >::leftRightRotate(Node * node_)
{
  node_->left = leftRotate(node_->left);
  return rightRotate(node_);
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Node * BinarySearchTree< Key, Value, Compare >::rightLeftRotate(Node * node_)
{
  node_->right = rightRotate(node_->right);
  return leftRotate(node_);
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::erase(Iterator pos)
{
  if (pos == ConstIterator(nullptr, root_))
  {
    return end();
  }
  Node * temp = pos.iterator.node_;
  Node * parent = temp->parent_;
  Node * successor = nullptr;
  if (temp->left_ && temp->right_)
  {
    successor = temp->right_;
    while (successor->left_)
    {
      successor = successor->left_;
    }
    temp = successor;
    parent = successor->parent_;
  }
  Node* child = temp->left_ ? temp->left_ : temp->right_;
  if (child)
  {
    child->parent_ = parent;
  }
  if (!parent)
  {
    root_ = child;
  }
  else if (temp == parent->left_)
  {
    parent->left_ = child;
  }
  else
  {
    parent->right_ = child;
  }
  while (parent)
  {
    updateHeight(parent);
    parent = balance(parent);
    parent = parent->parent_;
  }
  delete temp;
  return Iterator(ConstIterator(child ? child : parent, root_));
}

template < typename Key, typename Value, typename Compare >
size_t BinarySearchTree< Key, Value, Compare >::erase(const Key & key)
{
  Iterator iter = find(key);
  if (iter == ConstIterator())
  {
    return 0;
  }
  else
  {
    erase(iter);
    return 1;
  }
}

template< typename Key, typename Value, typename Compare >
size_t BinarySearchTree< Key, Value, Compare >::getHeight(Node * node_)
{
  return (node_ == nullptr ? 0 : node_->height);
}

template< typename Key, typename Value, typename Compare >
void BinarySearchTree< Key, Value, Compare >::deleteAll(Node * node_)
{
  if (node_ != nullptr)
  {
    deleteAll(node_->left);
    deleteAll(node_->right);
    delete node_;
  }
}

template< typename Key, typename Value, typename Compare >
void BinarySearchTree< Key, Value, Compare >::clear()
{
  deleteAll(root_);
  root_ = nullptr;
  size_ = 0;
}

template< typename Key, typename Value, typename Compare >
void BinarySearchTree< Key, Value, Compare >::swap(BinarySearchTree & el) noexcept
{
  Node * temp = el.root_;
  el.root_ = root_;
  root_ = temp;
  size_t tempSize = el.size_;
  el.size_ = size_;
  size_ = tempSize;
}

template< typename Key, typename Value, typename Compare >
std::pair< typename BinarySearchTree< Key, Value, Compare >::Iterator,
           typename BinarySearchTree< Key, Value, Compare >::Iterator > BinarySearchTree< Key, Value, Compare >::equal_range(const Key & key)
{
  if (find(key) == end())
  {
    Iterator iter(begin());
    while (iter != end())
    {
      if ((*iter).first > key)
      {
        break;
      }
      iter++;
    }
    return std::make_pair(iter, iter);
  }
  else
  {
    return std::make_pair(find(key), find(key)++);
  }
}

template< typename Key, typename Value, typename Compare >
class BinarySearchTree< Key, Value, Compare >::Iterator : public std::iterator< std::input_iterator_tag, Key, Value, Compare >
{
  friend class BinarySearchTree;
public:
  Iterator();
  Iterator(Node * node);
  Iterator(const Iterator & iter);
  ~Iterator() = default;

  Iterator & operator=(const Iterator & iter);
  Iterator & operator=(Iterator && rhs);
  value_t & operator*();
  value_t * operator->();
  Iterator & operator++();
  Iterator operator++(int);
  Iterator & operator--();
  Iterator operator--(int);
  bool operator==(const Iterator & rhs) const;
  bool operator!=(const Iterator & rhs) const;
private:
  Node * node_;
};

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::Iterator::Iterator()
{
  node_(nullptr);
}

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::Iterator::Iterator(Node * node)
{
  node_ = node;
}

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::Iterator::Iterator(const Iterator & iter)
{
  node_ = iter.node_;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator & BinarySearchTree< Key, Value, Compare >::Iterator::operator=(const Iterator & iter)
{
  node_ = iter.node_;
  return *this;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator & BinarySearchTree< Key, Value, Compare >::Iterator::operator=(Iterator && rhs)
{
  node_ = rhs.node_;
  return *this;
}

template< typename Key, typename Value, typename Compare >
std::pair< Key, Value > & BinarySearchTree< Key, Value, Compare >::Iterator::operator*()
{
  return node_->data;
}

template< typename Key, typename Value, typename Compare >
std::pair< Key, Value > * BinarySearchTree< Key, Value, Compare >::Iterator::operator->()
{
  return std::addressof(node_->data);
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator & BinarySearchTree< Key, Value, Compare >::Iterator::operator++()
{
  if (node_ == nullptr)
  {
    return *this;
  }
  else if (node_->right == nullptr)
  {
    Node * temp = node_->parent;
    while (temp != nullptr && node_ == temp->right)
    {
      node_ = temp;
      temp = node_->parent;
    }
    node_ = temp;
    return *this;
  }
  else
  {
    node_ = node_->right;
    if (node_->left == nullptr)
    {
      return *this;
    }
    while (node_->left != nullptr)
    {
      node_ = node_->left;
    }
    return *this;
  }
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::Iterator::operator++(int)
{
  Iterator temp(*this);
  ++(*this);
  return temp;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator & BinarySearchTree< Key, Value, Compare >::Iterator::operator--()
{
  if (node_ == nullptr)
  {
    return *this;
  }
  else if (node_->left == nullptr)
  {
    Node * temp = node_->parent;
    while (temp != nullptr && node_ == temp->left)
    {
      node_ = temp;
      temp = node_->parent;
    }
    node_ = temp;
    return *this;
  }
  else
  {
    node_ = node_->left;
    while (node_->right != nullptr)
    {
      node_ = node_->right;
    }
    return *this;
  }
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::Iterator::operator--(int)
{
  Iterator temp(*this);
  --(*this);
  return temp;
}

template< typename Key, typename Value, typename Compare >
bool BinarySearchTree< Key, Value, Compare >::Iterator::operator==(const Iterator & rhs) const
{
  return node_ == rhs.node_;
}

template< typename Key, typename Value, typename Compare >
bool BinarySearchTree< Key, Value, Compare >::Iterator::operator!=(const Iterator & rhs) const
{
  return node_ != rhs.node_;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::begin() noexcept
{
  Node * temp = root_;
  while (temp->left != nullptr)
  {
    temp = temp->left;
  }
  return Iterator(temp);
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::end() noexcept
{
  return Iterator(nullptr);
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::ConstIterator BinarySearchTree< Key, Value, Compare >::cbegin() const noexcept
{
  Node * temp = root_;
  while (temp->left != nullptr)
  {
    temp = temp->left;
  }
  return ConstIterator(temp);
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::ConstIterator BinarySearchTree< Key, Value, Compare >::cend() const noexcept
{
  return ConstIterator(nullptr);
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::Iterator BinarySearchTree< Key, Value, Compare >::find(const Key & key)
{
  Node * temp = root_;
  while (temp)
  {
    if (temp->data.first == key)
    {
      return Iterator(temp);
    }
    else if (key > temp->data.first)
    {
      temp = temp->right;
    }
    else if (key < temp->data.first)
    {
      temp = temp->left;
    }
  }
  return end();
}

template< typename Key, typename Value, typename Compare >
class BinarySearchTree< Key, Value, Compare >::ConstIterator : public std::iterator< std::input_iterator_tag, Key, Value, Compare >
{
  friend class BinarySearchTree;
public:
  ConstIterator();
  explicit ConstIterator(const Node * node);
  ConstIterator(const ConstIterator & iter);
  ~ConstIterator() = default;

  ConstIterator & operator=(const ConstIterator & iter);
  ConstIterator & operator=(ConstIterator && rhs);
  const value_t & operator*() const;
  const value_t * operator->() const;
  ConstIterator & operator++();
  ConstIterator operator++(int);
  ConstIterator & operator--();
  ConstIterator operator--(int);
  bool operator==(const ConstIterator & rhs) const;
  bool operator!=(const ConstIterator & rhs) const;
private:
  const Node * node_;
};

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::ConstIterator::ConstIterator()
{
  node_(nullptr);
}

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::ConstIterator::ConstIterator(const Node * node)
{
  node_ = node;
}

template< typename Key, typename Value, typename Compare >
BinarySearchTree< Key, Value, Compare >::ConstIterator::ConstIterator(const ConstIterator & iter)
{
  node_ = iter.node_;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::ConstIterator & BinarySearchTree< Key, Value, Compare >::ConstIterator::operator=(const ConstIterator &iter)
{
  node_ = iter.node_;
  return *this;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::ConstIterator & BinarySearchTree< Key, Value, Compare >::ConstIterator::operator=(ConstIterator && rhs)
{
  node_(rhs.node_);
  return *this;
}

template< typename Key, typename Value, typename Compare >
const std::pair< Key, Value > & BinarySearchTree< Key, Value, Compare >::ConstIterator::operator*() const
{
  return node_->data;
}

template< typename Key, typename Value, typename Compare >
const std::pair< Key, Value > * BinarySearchTree< Key, Value, Compare >::ConstIterator::operator->() const
{
  return std::addressof(node_->data);
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::ConstIterator & BinarySearchTree< Key, Value, Compare >::ConstIterator::operator++()
{
  if (node_ == nullptr)
  {
    return *this;
  }
  else if (node_->right == nullptr)
  {
    Node * temp = node_->parent;
    while (temp != nullptr && node_ == temp->right)
    {
      node_ = temp;
      temp = node_->parent;
    }
    node_ = temp;
    return *this;
  }
  else
  {
    node_ = node_->right;
    if (node_->left == nullptr)
    {
      return *this;
    }
    while (node_->left != nullptr)
    {
      node_ = node_->left;
    }
    return *this;
  }
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::ConstIterator BinarySearchTree< Key, Value, Compare >::ConstIterator::operator++(int)
{
  ConstIterator temp(*this);
  ++(*this);
  return temp;
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::ConstIterator & BinarySearchTree< Key, Value, Compare >::ConstIterator::operator--()
{
  if (node_ == nullptr)
  {
    return *this;
  }
  else if (node_->left == nullptr)
  {
    Node * temp = node_->parent;
    while (temp != nullptr && node_ == temp->left)
    {
      node_ = temp;
      temp = node_->parent;
    }
    node_ = temp;
    return *this;
  }
  else
  {
    node_ = node_->left;
    while (node_->right != nullptr)
    {
      node_ = node_->right;
    }
    return *this;
  }
}

template< typename Key, typename Value, typename Compare >
typename BinarySearchTree< Key, Value, Compare >::ConstIterator BinarySearchTree< Key, Value, Compare >::ConstIterator::operator--(int)
{
  ConstIterator temp(*this);
  --(*this);
  return temp;
}

template< typename Key, typename Value, typename Compare >
bool BinarySearchTree< Key, Value, Compare >::ConstIterator::operator==(const ConstIterator & rhs) const
{
  return node_ == rhs.node_;
}

template< typename Key, typename Value, typename Compare >
bool BinarySearchTree< Key, Value, Compare >::ConstIterator::operator!=(const ConstIterator & rhs) const
{
  return node_ != rhs.node_;
}

#endif
