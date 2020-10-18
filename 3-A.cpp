#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

constexpr int MAX_LENGTH = 20;
vector<float> eng_freq = {
    0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228, 0.02015,
    0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
    0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
    0.00978, 0.02360, 0.00150, 0.01974, 0.00074};

int getKey(string_view ciphertext);
constexpr int modulo(int a, int b);
string decrypt(string_view ciphertext, int key);

int main() {
  ifstream inFile("A.txt");
  string ciphertext;
  int key;

  getline(inFile, ciphertext);
  key = getKey(ciphertext);
  cout << "key: " << key << endl;
  cout << "plain: " << decrypt(ciphertext, key) << endl;
}

int getKey(string_view ciphertext) {
  float sum;
  vector<int> offsets;
  vector<float> chiSquareds(26, 0.f);
  vector<float> v(26, 0.f);
  int shift;

  for (int i=0;i<26;i++) {
    sum = 0.f;
    offsets.clear();
    for (char letter : ciphertext)
      offsets.push_back(modulo(letter - 'A' - i, 26) + 'A');
    fill(v.begin(), v.end(), 0.f);
    for (int o : offsets)
      v[o - 'A'] += 1.f;
    for (int j=0;j<26;j++)
      v[j] *= (1.f/ciphertext.length());

    for (int j=0;j<26;j++)
      sum += pow(v[j] - eng_freq[j], 2) / eng_freq[j];
    chiSquareds[i] = sum;
  }
  {
    auto found = min_element(chiSquareds.begin(), chiSquareds.end());
    shift = distance(chiSquareds.begin(), found);
  }
  return shift;
}

constexpr int modulo(int a, int b) {
  return ((a % b) + b) % b;
}

string decrypt(string_view ciphertext, int key) {
  string plain;

  for (int i = 0; i < ciphertext.length(); ++i)
    if ('A' <= ciphertext[i] && 'Z' >= ciphertext[i])
      plain += modulo(ciphertext[i] - key - 'A', 26) + 'A';
    else
      plain += ciphertext[i];
  return plain;
}
