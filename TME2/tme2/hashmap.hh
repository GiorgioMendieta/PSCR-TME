#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>

#include <vector>
#include <forward_list>

using namespace std;
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
  typedef vector<forward_list<Entry>> buckets_t; // k,v pair of a forward list container, stored in a vector
  buckets_t buckets;
  size_t sz; // Hash Map size

public:
  HashMap(size_t alloc) : buckets(alloc), sz(0) {} // Constructor with specified size
  HashMap() : buckets(256), sz(0) {}               // Default constructor

  // Put an element in the hashmap, returns true if the key was already existing
  bool put(const K &key, const V &value)
  {
    size_t h = hash<K>()(key);          // Hash the key
    size_t target = h % buckets.size(); // Store the key in the number of buckets

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
    if ((float)size() >= (buckets.size() * 0.8))
    {
      cout << "nb_buckets: " << buckets.size() << endl;
      cout << "Hashmap size: " << size() << endl;
      cout << "Doubling the size of the hashmap..." << endl;
      grow();
    }

    buckets[target].emplace_front(key, value);
    sz++;
    return false;
  }

  // Get the value from a key already existing in the hashmap
  V *get(const K &key)
  {
    size_t h = hash<K>()(key);
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
    size_t h = hash<K>()(key);
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

  // Returns the size (number of buckets) of the hashmap
  size_t num_buckets() const
  {
    return buckets.size();
  }

  // Returns the number of non-emtpy elements of the hashmap
  size_t size() const
  {
    return sz;
  }

  void grow()
  {
    // Create a new Hashmap double the previous size
    HashMap tmp(2 * buckets.size());

    // For each bucket in the buckets vector
    for (auto &bucket : buckets)
    {
      // For each Entry in the bucket
      for (auto &ent : bucket)
      {
        tmp.put(ent.key, ent.value);
      }
    }

    // buckets = tmp.buckets;
    buckets.swap(tmp.buckets);
  }

  // Non const iterator
  template <typename T>
  struct iterator
  {
    size_t index;
    // typename lets the compiler know it is a type and not a static member
    typename vector<forward_list<Entry>>::iterator v_it; // Iterator Vector
    typename forward_list<Entry>::iterator l_it;         // Iterator list

    typedef typename buckets_t::iterator buckets_it;
    buckets_t &buck;

    iterator() {}

    T &operator++()
    {
      ++l_it;
      if (l_it == buck[index].end())
      {
        ++index;
        for (; index < buck.size() && buck[index].empty(); ++index)
        {
          // NOP
          // Skip empty buckets
        }
        if (index < bucket.size())
          l_it = buck[index].begin();
      }
      return *this;
    }

    T &operator*()
    {
      return *l_it;
    }

    // Optional
    T &operator->()
    {
    }

    T &operator!=(const iterator &other)
    {
      if (index == buck.size() && other.index == index)
      {
        return false;
      }
      else
      {
        return index != other.index || l_it != other.l_it || &buck != &other.buck;
      }
    }
  };

  iterator begin()
  {
    for (int index = 0; index < buckets.size(); index++)
    {
      if (!buckets[index].empty())
      {
        return iterator(index, buckets[index].begin(), buckets);
      }
    }
    return end();
  }

  iterator end()
  {
    return iterator(bucket.size(), buckets[0].end() /* nullptr */, buckets);
  }
};
