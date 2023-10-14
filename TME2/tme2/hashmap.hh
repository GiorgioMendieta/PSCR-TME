#pragma once

#include <iostream>
#include <fstream>
// #include <regex>
// #include <chrono>

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
  size_t sz; // Hash Map size (Nb of entries)

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
    for (auto &list : buckets)
    {
      // For each Entry in the bucket
      for (auto &entry : list)
      {
        tmp.put(entry.key, entry.value);
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

    // typedef vector<forward_list<Entry>> buckets_t; // k,v pair of a forward list container, stored in a vector
    typename buckets_t::iterator vit;           // Bucket vector index
    typename forward_list<Entry>::iterator lit; // index for linked list in bucket
    buckets_t &buck;

    iterator() {}

    // prefix
    T &operator++()
    {
      ++lit;
      // Test to see if we're at the end of the list in the current bucket
      if (lit == buck[index].end())
      {
        // Advance bucket
        ++vit;
        for (; vit < buck.size() && buck[index].empty(); ++index)
        {
          // Skip empty buckets
        }

        // Check if bucket index is at the end
        if (vit < buck.size())
        {
          // Place list index at the beginning of the bucket
          lit = buck[index].begin();
        }
      }
      return *this;
    }

    // postfix
    // T &operator++(int /* ignored */)
    // {
    //   auto b4 = *this; // Copy
    //   ++(*this);
    //   return b4;
    // }

    T &operator*()
    {
      // Dereference lit (entry list iterator)
      return *lit;
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
        return index != other.index || lit != other.lit || &buck != &other.buck;
      }
    }
  };

  iterator begin()
  {
    // Look for the first non-empty bucket
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
  { // End of buckets vector, end of first element in buckets vector (nullptr), buckets vector
    return iterator(bucket.size(), buckets[0].end() /* nullptr */, buckets);
  }
};
