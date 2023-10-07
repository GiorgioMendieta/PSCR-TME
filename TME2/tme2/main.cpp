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

// QUESION 1
// Pass by reference both the unique word list and the current word
bool is_new_word(std::vector<std::string> &unique_words, const std::string &word)
{
  // For each word in unique_words vector, verify if it already exists
  for (const auto &w : unique_words)
  {
    if (w == word)
    {
      // Found word !
      return false;
    }
  }

  // Word is new, add it to the list
  unique_words.push_back(word);

  return true;
}

// QUESTION 2
// Pass by reference both the unique word list and the current word
void count_word_frequency(std::vector<std::pair<std::string, uint>> &word_occurrence, const std::string &word)
{
  // For each pair of word & occurrence in word_occurrence vector
  for (auto &p : word_occurrence)
  {
    // If word is found, increment the occurrence count
    if (p.first == word)
    {
      p.second++;
      return;
    }
  }
  // New word -> Start the count at 0
  word_occurrence.emplace_back(word, 0);
}

uint get_word_frequency(std::vector<std::pair<std::string, uint>> &word_occurrence, const std::string &word)
{
  // For each pair of word & occurrence in word_occurrence vector
  for (auto &p : word_occurrence)
  {
    // If word is found, increment the occurrence count
    if (p.first == word)
    {
      return p.second;
    }
  }

  return 0;
}

int main()
{
  using namespace std;
  using namespace std::chrono;

  ifstream input = ifstream("./WarAndPeace.txt");

  auto start = steady_clock::now();
  cout << "Parsing War and Peace" << endl;

  size_t nombre_lu = 0;
  size_t nombre_lu_hm = 0;
  // prochain mot lu
  string word;
  // une regex qui reconnait les caractères anormaux (négation des lettres)
  regex re(R"([^a-zA-Z])");

  vector<string> unique_words;
  vector<pair<string, uint>> word_occurrence;
  HashMap<string, uint> word_hm; // Hash Map of word & occurrences

  while (input >> word)
  {
    // élimine la ponctuation et les caractères spéciaux
    word = regex_replace(word, re, "");
    // passe en lowercase
    transform(word.begin(), word.end(), word.begin(), ::tolower);

    // word est maintenant "tout propre"
    if (nombre_lu % 100 == 0)
    {
      // on affiche un mot "propre" sur 100
      cout << nombre_lu << ": " << word << endl;
    }

    // is_new_word(unique_words, word);
    // count_word_frequency(word_occurrence, word);
    nombre_lu++;

    if (!word_hm.get(word))
    {
      // Word doesn't exist in the hashmap, put it
      nombre_lu_hm++;
      // word_hm.put(word, 1);
    }
    // {
    //   word_hm.increment(word);
    // }
    // else
    // {
    //   word_hm.put(word, 1);
    // }
  }
  input.close();

  cout << "Finished Parsing War and Peace" << endl;

  auto end = steady_clock::now();
  cout << "Parsing took "
       << duration_cast<milliseconds>(end - start).count()
       << "ms.\n";

  // QUESTION 1
  cout << "Found a total of " << nombre_lu << " words." << endl;
  cout << "Found a total of " << nombre_lu_hm << " words." << endl;
  // QUESTION 2
  // cout << "Found a total of " << unique_words.size() << " unique words." << endl;
  // QUESTION 3
  // cout << "'war' was found : " << get_word_frequency(word_occurrence, "war") << " times." << endl;
  // cout << "'peace' was found : " << get_word_frequency(word_occurrence, "peace") << " times." << endl;
  // cout << "'toto' was found : " << get_word_frequency(word_occurrence, "toto") << " times." << endl;

  // QUESTION 6
  cout << "Found a total of " << word_hm.size() << " unique words." << endl;

  cout << "'war' was found : " << word_hm.get("war") << " times." << endl;
  cout << "'peace' was found : " << word_hm.get("peace") << " times." << endl;
  cout << "'toto' was found : " << word_hm.get("toto") << " times." << endl;

  return 0;
}
