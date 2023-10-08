#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

#include <vector>
#include <forward_list>

template <typename K, typename V>
class HashMap
{
  // Key, Value pair stored on the hashmap
  struct Entry
  {
    const K key;
    V value;
    Entry(const K &key, const V &value) : key(key), value(value) {}
  };

  // Define a new type for the hashmap buckets
  typedef std::vector<std::forward_list<Entry>> buckets_t; // k,v pair of a forward list container, stored in a vector
  buckets_t buckets;
  size_t sz; // Hash Map size

public:
  HashMap(size_t alloc) : buckets(alloc), sz(0) {} // Constructor with specified size
  HashMap() : buckets(256), sz(0) {}               // Default constructor

  // Put an element in the hashmap, returns true if the key was already existing
  bool put(const K &key, const V &value)
  {
    size_t h = std::hash<K>()(key);
    size_t target = h % buckets.size();

    // Verify if key already exists and modify the value
    for (auto &entry : buckets[target])
    {
      if (entry.key == key)
      {
        // hit
        entry.value = value;
        return true;
      }
    }
    // TODO: Grow the hashmap if needed
    buckets[target].emplace_front(key, value);
    return false;
  }

  // Get the value from a key already existing in the hashmap
  V *get(const K &key)
  {
    size_t h = std::hash<K>()(key);
    size_t target = h % buckets.size();

    for (auto &entry : buckets[target])
    {
      if (entry.key == key)
      {
        // Found
        return &entry.value;
      }
    }

    // Not found
    return nullptr;
  }

  // Increment the value of a key, (true if found key)
  bool increment(const K &key)
  {
    size_t h = std::hash<K>()(key);
    size_t target = h % buckets.size();
    for (auto &entry : buckets[target])
    {
      if (entry.key == key)
      {
        // Found
        entry.value++;
        return true;
      }
    }

    // Not found
    return false;
  }

  // Returns the size (number of elements) of the hashmap
  size_t size() const
  {
    return buckets.size();
  }

  void grow()
  {
    // Create a new Hashmap double the previous size
    HashMap tmp(2 * buckets.size());
    for (auto &bucket : buckets)
    {
      for (auto &element : bucket)
      {
        tmp.put(element.key, element.value);
      }
    }

    buckets = tmp.buckets;
  }

  // Non const iterator
  template <typename T>
  struct iterator
  {
    size_t index;
    typename std::forward_list<Entry>::iterator it; // Let the compiler know it is a type and not a static member
    typedef typename buckets_t::iterator buckets_it;

    iterator() {}

    T &operator++()
    {
    }

    T &operator*()
    {
    }

    // Optional
    T &operator->()
    {
    }

    T &operator!=(const iterator &other)
    {
    }
  };
};
