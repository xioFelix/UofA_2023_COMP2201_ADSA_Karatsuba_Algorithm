#include <algorithm>
#include <iostream>
#include <string>

std::string addBaseB(const std::string& num1, const std::string& num2, int B) {
  std::string n1 = num1, n2 = num2;

  // Make the two strings of equal length by prepending zeros
  while (n1.length() < n2.length()) n1 = "0" + n1;
  while (n2.length() < n1.length()) n2 = "0" + n2;

  std::string result = "";
  int carry = 0;

  for (int i = n1.length() - 1; i >= 0; i--) {
    int digit1 = n1[i] - '0';
    int digit2 = n2[i] - '0';

    int sum = digit1 + digit2 + carry;

    carry = sum / B;
    sum %= B;

    result = std::to_string(sum) + result;
  }

  if (carry > 0) {
    result = std::to_string(carry) + result;
  }

  return result;
}

std::string subtractBaseB(const std::string& num1, const std::string& num2,
                          int B) {
  std::string n1 = num1, n2 = num2;

  // Make the two strings of equal length by prepending zeros
  while (n1.length() < n2.length()) n1 = "0" + n1;
  while (n2.length() < n1.length()) n2 = "0" + n2;

  std::string result = "";
  int borrow = 0;

  for (int i = n1.length() - 1; i >= 0; i--) {
    int digit1 = n1[i] - '0';
    int digit2 = n2[i] - '0';

    int diff = digit1 - digit2 - borrow;

    if (diff < 0) {
      diff += B;
      borrow = 1;
    } else {
      borrow = 0;
    }

    result = std::to_string(diff) + result;
  }

  // Removing leading zeros from the result
  while (result.length() > 1 && result[0] == '0') {
    result = result.substr(1);
  }

  return result;
}

std::string karatsuba(std::string num1, std::string num2, int base) {
  size_t n = std::max(num1.size(), num2.size());
  if (n == 1) {
    int mul = (num1[0] - '0') * (num2[0] - '0');
    if (mul < base)
      return std::to_string(mul);
    else {
      // Handle the case where multiplication results in a number >= base
      int secondDigit = mul % base;
      int firstDigit = mul / base;
      return std::to_string(firstDigit) + std::to_string(secondDigit);
    }
  }
  while (num1.size() < n) num1.insert(0, "0");
  while (num2.size() < n) num2.insert(0, "0");
  std::string a = num1.substr(0, n / 2);
  std::string b = num1.substr(n / 2, n - n / 2);
  std::string c = num2.substr(0, n / 2);
  std::string d = num2.substr(n / 2, n - n / 2);
  std::string ac = karatsuba(a, c, base);
  std::string bd = karatsuba(b, d, base);
  std::string abcd =
      karatsuba(addBaseB(a, b, base), addBaseB(c, d, base), base);
  abcd = subtractBaseB(abcd, addBaseB(ac, bd, base), base);
  size_t k = n - n / 2;
  for (size_t i = 0; i < 2 * k; i++) ac += "0";  // Multiply ac by B^2k
  for (size_t i = 0; i < k; i++) abcd += "0";    // Multiply abcd by B^k
  std::string result = addBaseB(addBaseB(ac, abcd, base), bd, base);

  // Remove leading zeros from result
  size_t startpos = result.find_first_not_of("0");
  if (startpos != std::string::npos) result = result.substr(startpos);
  return result.empty() ? "0" : result;
}

std::string divideBaseB(const std::string& num1, const std::string& num2,
                        int B) {
  // Convert numbers to base 10 for easier calculations
  long long num1_base10 = std::stoll(num1, nullptr, B);
  long long num2_base10 = std::stoll(num2, nullptr, B);

  long long quotient_base10 = num1_base10 / num2_base10;

  // Convert the quotient back to base B
  std::string quotient_baseB = "";
  while (quotient_base10 > 0) {
    int remainder = quotient_base10 % B;
    quotient_base10 /= B;
    quotient_baseB = std::to_string(remainder) + quotient_baseB;
  }

  // If quotient is 0
  if (quotient_baseB.empty()) {
    quotient_baseB = "0";
  }

  return quotient_baseB;
}

int main() {
  std::string num1, num2;
  int B;
  std::cin >> num1 >> num2 >> B;
  std::cout << addBaseB(num1, num2, B) << " ";   // Addition
  std::cout << karatsuba(num1, num2, B) << " ";  // Multiplication
  std::cout << divideBaseB(num1, num2, B);       // Division
  return 0;
}