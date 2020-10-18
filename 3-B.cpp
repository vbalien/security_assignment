#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

constexpr int MAX_LENGTH = 20;
string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
vector<float> eng_freq = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
    0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
    0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
    0.00978, 0.02360, 0.00150, 0.01974, 0.00074};

int guessLength(string_view ciphertext);
float getIC(string_view ciphertext);
string getKey(string_view ciphertext, int length);
char getMostFreqChar(string_view seq);
constexpr int modulo(int a, int b);
string decrypt(string_view ciphertext, string_view key);

int main() {
  ifstream inFile("B.txt");
  string ciphertext;
  string key;
  int length;

  getline(inFile, ciphertext);
  length = guessLength(ciphertext);
  key = getKey(ciphertext, length);
  cout << "key: " << key << endl;
  cout << "plain: " << decrypt(ciphertext, key) << endl;
}

int guessLength(string_view ciphertext) {
  vector<float> icTable;
  vector<float> sorted;
  float sum, avg;
  string seq;

  for (int guessLen=0;guessLen<MAX_LENGTH;++guessLen) {
    sum = avg = 0.f;
    for (int i=0;i<guessLen;++i) {
      seq.clear();
      for (int j=0;j<ciphertext.length() - i;j += guessLen)
        seq += ciphertext[i + j];
      sum += getIC(seq);
    }

    if (guessLen != 0)
      avg = sum / guessLen;
    icTable.push_back(avg);
  }
  sorted = icTable;
  sort(sorted.begin(), sorted.end(), greater<float>());
  int best;
  int second;
  {
    auto found = find(icTable.begin(), icTable.end(), sorted[0]);
    best = distance(icTable.begin(), found);
  }
  {
    auto found = find(icTable.begin(), icTable.end(), sorted[1]);
    second = distance(icTable.begin(), found);
  }
  if (best % second == 0)
    return second;
  else
    return best;
}

float getIC(string_view ciphertext) {
  float len = ciphertext.length();
  float sum = 0.f;

  for (auto letter : alphabet) {
    float cnt = count(ciphertext.begin(), ciphertext.end(), letter);
    sum +=  cnt * (cnt - 1);
  }
  if (len == 1) return 0;
  return sum / (len * (len  - 1));
}

string getKey(string_view ciphertext, int length) {
  string key;
  string seq;

  for (int i=0;i<length;i++) {
    seq.clear();
    for (int j=0;j<ciphertext.length() - i;j += length)
      seq += ciphertext[i+j];
    key += getMostFreqChar(seq);
  }
  return key;
}

char getMostFreqChar(string_view seq) {
  float sum;
  vector<int> offsets;
  vector<float> chiSquareds(26, 0.f);
  vector<float> v(26, 0.f);
  int shift;

  for (int i=0;i<26;i++) {
    sum = 0.f;
    offsets.clear();
    for (char letter : seq)
      offsets.push_back(modulo(letter - 'A' - i, 26) + 'A');
    fill(v.begin(), v.end(), 0.f);
    for (int o : offsets)
      v[o - 'A'] += 1.f;
    for (int j=0;j<26;j++)
      v[j] *= (1.f/seq.length());

    for (int j=0;j<26;j++)
      sum += pow(v[j] - eng_freq[j], 2) / eng_freq[j];
    chiSquareds[i] = sum;
  }
  {
    auto found = min_element(chiSquareds.begin(), chiSquareds.end());
    shift = distance(chiSquareds.begin(), found);
  }
  return shift + 'A';
}

constexpr int modulo(int a, int b) {
  return ((a % b) + b) % b;
}

string decrypt(string_view ciphertext, string_view key) {
  string plain;

  for (int i=0;i<ciphertext.length();++i)
    plain += (((ciphertext[i] - key[i % key.length()]) + 26) % 26) + 'A';
  return plain;
}
