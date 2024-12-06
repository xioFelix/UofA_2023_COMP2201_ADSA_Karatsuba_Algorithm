#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

string add(string num1, string num2, int base) {
  string result = "";
  int carry = 0;
  int i = num1.size() - 1;
  int j = num2.size() - 1;
  while (i >= 0 || j >= 0 || carry != 0) {
    int sum = carry;
    if (i >= 0) {
      sum += num1[i] - '0';
      i--;
    }
    if (j >= 0) {
      sum += num2[j] - '0';
      j--;
    }
    carry = sum / base;
    sum = sum % base;
    result += to_string(sum);
  }
  reverse(result.begin(), result.end());
  return result;
}

string subtract(string num1, string num2, int base) {
  string result = "";
  int borrow = 0;
  int i = num1.size() - 1;
  int j = num2.size() - 1;
  while (i >= 0 || j >= 0 || borrow != 0) {
    int diff = borrow;
    if (i >= 0) {
      diff += num1[i] - '0';
      i--;
    }
    if (j >= 0) {
      diff -= num2[j] - '0';
      j--;
    }
    if (diff < 0) {
      diff += base;
      borrow = -1;
    } else {
      borrow = 0;
    }
    result += to_string(diff);
  }
  reverse(result.begin(), result.end());
  return result;
}

string karatsuba(string num1, string num2, int base) {
  int n = max(num1.size(), num2.size());
  if (n == 1) return to_string((num1[0] - '0') * (num2[0] - '0'));
  while (num1.size() < n) num1.insert(0, "0");
  while (num2.size() < n) num2.insert(0, "0");
  string a = num1.substr(0, n / 2);
  string b = num1.substr(n / 2, n - n / 2);
  string c = num2.substr(0, n / 2);
  string d = num2.substr(n / 2, n - n / 2);
  string ac = karatsuba(a, c, base);
  string bd = karatsuba(b, d, base);
  string abcd = karatsuba(add(a, b, base), add(c, d, base), base);
  abcd = subtract(abcd, add(ac, bd, base), base);
  for (size_t i = 0; i < 2 * (n - n / 2); i++) ac += "0";
  for (size_t i = 0; i < n - n / 2; i++) abcd += "0";
  string result = add(add(ac, abcd, base), bd, base);

  // Remove leading zeros from result
  size_t startpos = result.find_first_not_of("0");
  if (startpos != std::string::npos) result = result.substr(startpos);

  return result.empty() ? "0" : result;
}

string divide(string dividend, string divisor, int base) {
  string quotient = "";
  string remainder = "";
  for (char c : dividend) {
    remainder += c;
    int rem_int = stoi(remainder);
    quotient += to_string(rem_int / stoi(divisor));
    remainder = to_string(rem_int % stoi(divisor));
  }

  // Remove leading zeros from quotient
  size_t startpos = quotient.find_first_not_of("0");
  if (startpos != std::string::npos) quotient = quotient.substr(startpos);

  return quotient.empty() ? "0" : quotient;
}

int main() {
  string I1, I2;
  int B;
  cin >> I1 >> I2 >> B;
  cout << add(I1, I2, B) << " ";
  cout << karatsuba(I1, I2, B) << " ";
  cout << divide(I1, I2, B);

  return 0;
}
