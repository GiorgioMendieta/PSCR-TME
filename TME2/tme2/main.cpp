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
  typedef std::vector<std::forward_list<Entry>> buckets_t;
  buckets_t buckets;
  size_t sz;                                // Hash Map size
  HashMap(size_t alloc) : buckets(alloc) {} // Constructor

public:
  // Put an element in the hashmap
  bool
  put(const K &key, const V &value)
  {
    size_t h = std::hash<K>()(key);
    size_t target = h % buckets.size();
    for (auto &ent : buckets[target])
    {
      if (ent.key == key)
      {
        // hit
        ent.value = value;
        return true;
      }
    }
    // TODO: Grow the hashmap if needed
    buckets[target].emplace_front(key, value);
    return false;
  }

  // Get an element from the hashmap
  V *get(const K &key, const V &value) const
  {
    size_t h = std::hash<K>()(key);
    size_t target = h % buckets.size();
    for (auto &ent : buckets[target])
    {
      if (ent.key == key)
      {
        // Found
        return &ent.value;
      }
    }

    // Not found
    return nullptr;
  }

  void grow()
  {
    // HashMap tmp(2 * buckets.size());

    // for (auto &e = buckets)
    // {
    //   for (auto &e = l)
    //   {
    //     tmp.put(e.key, e.value)
    //   }
    // }

    // buckets = tmp.buckets;
  }
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
  // prochain mot lu
  string word;
  // une regex qui reconnait les caractères anormaux (négation des lettres)
  regex re(R"([^a-zA-Z])");

  vector<string> unique_words;
  vector<pair<string, uint>> word_occurrence;

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

    is_new_word(unique_words, word);
    count_word_frequency(word_occurrence, word);

    nombre_lu++;
  }
  input.close();

  cout << "Finished Parsing War and Peace" << endl;

  auto end = steady_clock::now();
  cout << "Parsing took "
       << duration_cast<milliseconds>(end - start).count()
       << "ms.\n";

  // QUESTION 1
  cout << "Found a total of " << nombre_lu << " words." << endl;
  // QUESTION 2
  cout << "Found a total of " << unique_words.size() << " unique words." << endl;
  // QUESTION 3
  cout << "'war' was found : " << get_word_frequency(word_occurrence, "war") << " times." << endl;
  cout << "'peace' was found : " << get_word_frequency(word_occurrence, "peace") << " times." << endl;
  cout << "'toto' was found : " << get_word_frequency(word_occurrence, "toto") << " times." << endl;

  return 0;
}
