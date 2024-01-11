
#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <utility>  // for swap
#include <algorithm> // for sort
#include <functional>

using namespace std;

void removeNonLetters(string& s);

struct bucket
{
    int key;
    list<string> words;
};


class DictionaryImpl
{
  public:
    DictionaryImpl() : max(50000), m_words(new bucket[50000]) {};
    DictionaryImpl(int maxBuckets) : max(maxBuckets), m_words(new bucket[maxBuckets]) {};
    ~DictionaryImpl();
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
    int hash(string word) const;
    int loadFactor();
    bool isAnagram(string letters, string word) const;
  private:
    int max;
    bucket* m_words;
};


DictionaryImpl::~DictionaryImpl() {
    delete[] m_words;
}

int DictionaryImpl::hash(string word) const
{
    sort(word.begin(), word.end());
    unsigned int h = 2166136261u;
    for (int i = 0; i < word.size(); i++) {
        h += word[i] - '0';
        h *= 27;
    }
    return h % max;
}

void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
    if ( ! word.empty()) {
        int bucketNum = hash(word);
        bucket& currBucket = m_words[bucketNum];
        currBucket.key = bucketNum;
        currBucket.words.push_back(word);
    }
}


bool DictionaryImpl::isAnagram(string letters, string currWord) const{
    sort(letters.begin(), letters.end());
    sort(currWord.begin(), currWord.end());
    if (letters == currWord) {
        return true;
    }
    else {
        return false;
    }

}


void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    if (callback == nullptr)
        return;

    removeNonLetters(letters);
    if (letters.empty())
        return;
    sort(letters.begin(), letters.end());
    int bucketNum = hash(letters);
    bucket& currBucket = m_words[bucketNum];
    for (list<string>::iterator p = currBucket.words.begin(); p != currBucket.words.end(); p++) {
        if (isAnagram(letters, *p) == true) {
            callback(*p);
        }
    }
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());
}

//******************** Dictionary functions ******************************

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters, callback);
}

