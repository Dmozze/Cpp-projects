#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <random>
#include <vector>
#include <utility>
#include <gtest/gtest.h>

#include "big_integer.h"
#include "big_integer_gmp.h"

TEST(correctness, two_plus_two) {
  EXPECT_EQ(big_integer(4), big_integer(2) + big_integer(2));
  EXPECT_EQ(4, big_integer(2) + 2); // implicit converion from int must work
  EXPECT_EQ(4, 2 + big_integer(2));
}

TEST(correctness, default_ctor) {
  big_integer a;
  big_integer b = 0;
  EXPECT_EQ(0, a);
  EXPECT_EQ(b, a);
}

TEST(correctness, ctor_limits) {
  big_integer a = std::numeric_limits<int>::min();
  std::string a_str  =to_string(a);
  big_integer b = std::numeric_limits<int>::max();
  std::string b_str = to_string(b);

  EXPECT_EQ(-1, a + b);
}

TEST(correctness, copy_ctor) {
  big_integer a = 3;
  big_integer b = a;

  EXPECT_EQ(b, a);
  EXPECT_EQ(3, b);
}

TEST(correctness, copy_ctor_real_copy) {
  big_integer a = 3;
  big_integer b = a;
  a = 5;

  EXPECT_EQ(3, b);
}

TEST(correctness, copy_ctor_real_copy2) {
  big_integer a = 3;
  big_integer b = a;
  b = 5;

  EXPECT_EQ(3, a);
}

TEST(correctness, assignment_operator) {
  big_integer a = 4;
  big_integer b = 7;
  b = a;

  EXPECT_TRUE(a == b);
}

TEST(correctness, self_assignment) {
  big_integer a = 5;
  a = a;

  EXPECT_TRUE(a == 5);
}

TEST(correctness, assignment_return_value) {
  big_integer a = 4;
  big_integer b = 7;
  (a = b) = a;

  EXPECT_TRUE(a == 7);
  EXPECT_TRUE(b == 7);
}

TEST(correctness, comparisons) {
  big_integer a = 100;
  big_integer b = 100;
  big_integer c = 200;
  big_integer d = -1;
    std::string a_str = to_string(a);
    std::string b_str = to_string(b);
    std::string c_str = to_string(c);
    std::string d_str = to_string(d);

  EXPECT_TRUE(a == b);
  EXPECT_TRUE(a != c);
  EXPECT_TRUE(a < c);
  EXPECT_TRUE(c > a);
  EXPECT_TRUE(a <= a);
  EXPECT_TRUE(a <= b);
  EXPECT_TRUE(a <= c);
  EXPECT_TRUE(c >= a);
  EXPECT_TRUE(d != 0);
}

TEST(correctness, compare_zero_and_minus_zero) {
  big_integer a;
  big_integer b = -a;

  EXPECT_TRUE(a == b);
}

TEST(correctness, add) {
  big_integer a = 5;
  big_integer b = 20;

  EXPECT_TRUE(a + b == 25);

  a += b;
  EXPECT_TRUE(a == 25);
}

TEST(correctness, add_signed) {
  big_integer a = 5;
  big_integer b = -20;
  EXPECT_TRUE(a + b == -15);

  a += b;
  EXPECT_TRUE(a == -15);
}

TEST(correctness, add_return_value) {
  big_integer a = 5;
  big_integer b = 1;

  (a += b) += b;
  EXPECT_EQ(7, a);
}

TEST(correctness, sub) {
  big_integer a = 20;
  big_integer b = 5;

  EXPECT_TRUE(a - b == 15);

  a -= b;
  EXPECT_TRUE(a == 15);
}

TEST(correctness, sub_signed) {
  big_integer a = 5;
  big_integer b = 20;

  EXPECT_TRUE(a - b == -15);

  a -= b;
  EXPECT_TRUE(a == -15);

  a -= -100;
  EXPECT_TRUE(a == 85);
}

TEST(correctness, sub_return_value) {
  big_integer a = 5;
  big_integer b = 1;

  (a -= b) -= b;
  EXPECT_EQ(3, a);
}

TEST(correctness, mul) {
  big_integer a = 5;
  big_integer b = 20;
  EXPECT_TRUE(a * b == 100);

  a *= b;
  EXPECT_TRUE(a == 100);
}

TEST(correctness, mul_signed) {
  big_integer a = -5;
  big_integer b = 20;

  EXPECT_TRUE(a * b == -100);

  a *= b;
  EXPECT_TRUE(a == -100);
}

TEST(correctness, mul_return_value) {
  big_integer a = 5;
  big_integer b = 2;

  (a *= b) *= b;
  EXPECT_EQ(20, a);
}

TEST(correctness, div_) {
  big_integer a = 20;
  big_integer b = 5;
  big_integer c = 20;
  EXPECT_EQ(0, b / c);
  EXPECT_TRUE(a / b == 4);
  EXPECT_TRUE(a % b == 0);

  a /= b;
  EXPECT_TRUE(a == 4);

  c %= b;
  EXPECT_TRUE(c == 0);
}

TEST(correctness, div_int_min) {
  big_integer a = std::numeric_limits<int>::min();
  EXPECT_TRUE((a / a) == (a / std::numeric_limits<int>::min()));
}

TEST(correctness, div_int_min_2) {
  big_integer a = std::numeric_limits<int>::min();
  big_integer b = -1;
  big_integer c = a / b;
  EXPECT_TRUE(c == (a / -1));
  EXPECT_TRUE((c - std::numeric_limits<int>::max()) == 1);
}

TEST(correctness, div_signed) {
  big_integer a = -20;
  big_integer b = 5;

  EXPECT_TRUE(a / b == -4);
  EXPECT_TRUE(a % b == 0);
}

TEST(correctness, div_rounding) {
  big_integer a = 23;
  big_integer b = 5;

  EXPECT_TRUE(a / b == 4);
  EXPECT_TRUE(a % b == 3);
}

TEST(correctness, div_rounding_negative) {
  big_integer a = 23;
  big_integer b = -5;
  big_integer c = -23;
  big_integer d = 5;

  EXPECT_TRUE(a / b == -4);
  EXPECT_TRUE(c / d == -4);
  EXPECT_TRUE(a % b == 3);
  EXPECT_TRUE(c % d == -3);
}

TEST(correctness, div_return_value) {
  big_integer a = 100;
  big_integer b = 2;

  (a /= b) /= b;
  EXPECT_EQ(25, a);
}

TEST(correctness, unary_plus) {
  big_integer a = 123;
  big_integer b = +a;

  EXPECT_TRUE(a == b);

  // this code should not compile:
  // &+a;
}

TEST(correctness, negation) {
  big_integer a = 666;
  big_integer b = -a;

  EXPECT_TRUE(b == -666);
}

TEST(correctness, negation_int_min) {
  big_integer a = std::numeric_limits<int>::min();
  big_integer b = -a;

  EXPECT_EQ(b - 1, std::numeric_limits<int>::max());
}

TEST(correctness, and_) {
  big_integer a = 0x55;
  big_integer b = 0xaa;

  EXPECT_TRUE((a & b) == 0);
  EXPECT_TRUE((a & 0xcc) == 0x44);
  a &= b;
  EXPECT_TRUE(a == 0);
}

TEST(correctness, and_signed) {
  big_integer a = 0x55;
  big_integer b = 0xaa;
  big_integer x = a & (0xaa - 256);

  EXPECT_TRUE((b & -1) == 0xaa);
  EXPECT_TRUE((a & (0xaa - 256)) == 0);
  EXPECT_TRUE((a & (0xcc - 256)) == 0x44);

  big_integer c = 0x55;
  big_integer d = 0xcc;
  EXPECT_EQ(c & d, big_integer(0x44));
}

TEST(correctness, and_return_value) {
  big_integer a = 7;

  (a &= 3) &= 6;
  EXPECT_EQ(2, a);
}

TEST(correctness, or_) {
  big_integer a = 0x55;
  big_integer b = 0xaa;

  EXPECT_TRUE((a | b) == 0xff);
  a |= b;
  EXPECT_TRUE(a == 0xff);

  big_integer c = 0x55;
  big_integer d = 0xcc;
  EXPECT_EQ(c | d, big_integer(0xdd));
}

TEST(correctness, or_signed) {
  big_integer a = 0x55;
  big_integer b = 0xaa;
  EXPECT_TRUE((a | (b - 256)) == -1);
}

TEST(correctness, or_return_value) {
  big_integer a = 1;

  (a |= 2) |= 4;
  EXPECT_EQ(7, a);
}

TEST(correctness, xor_) {
  big_integer a = 0xaa;
  big_integer b = 0xcc;

  EXPECT_TRUE((a ^ b) == 0x66);

  big_integer c = 0x55;
  big_integer d = 0xcc;
  EXPECT_EQ(c ^ d, big_integer(0x99));
}

TEST(correctness, xor_signed) {
  big_integer a = 0xaa;
  big_integer b = 0xcc;

  EXPECT_TRUE((a ^ (b - 256)) == (0x66 - 256));
}

TEST(correctness, xor_return_value) {
  big_integer a = 1;

  (a ^= 2) ^= 1;
  EXPECT_EQ(2, a);
}

TEST(correctness, not_) {
  big_integer a = 0xaa;
  big_integer b = ~a;
  big_integer c = (-a - 1);
  EXPECT_TRUE(~a == (-a - 1));
}

TEST(correctness, shl_) {
  big_integer a = 23;

  EXPECT_TRUE((a << 5) == 23 * 32);

  a <<= 5;
  EXPECT_TRUE(a == 23 * 32);
}

TEST(correctness, shl_return_value) {
  big_integer a = 1;

  (a <<= 2) <<= 1;
  EXPECT_EQ(8, a);
}

TEST(correctness, shr_) {
  big_integer a = 23;

  EXPECT_EQ(5, a >> 2);

  a >>= 2;
  EXPECT_EQ(5, a);
}

TEST(correctness, shr_31) {
  big_integer a = 65536;

  EXPECT_EQ(2, (a * a) >> 31);
}

TEST(correctness, shr_signed) {
  big_integer a = -1234;

  EXPECT_EQ(-155, a >> 3);

  a >>= 3;
  EXPECT_EQ(-155, a);
}

TEST(correctness, shr_return_value) {
  big_integer a = 64;

  (a >>= 2) >>= 1;
  EXPECT_EQ(8, a);
}

TEST(correctness, add_long) {
  big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("100000000000000000000000000000000000000");
  big_integer c("10000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000");

  EXPECT_EQ(c, a + b);
}

TEST(correctness, add_long_signed) {
  big_integer a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

  EXPECT_EQ(0, a + b);
}

TEST(correctness, add_long_signed2) {
    big_integer c("-999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");
  big_integer a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("100000000000000000000000000000000000000");
  EXPECT_EQ(c, a + b);
}

TEST(correctness, add_long_pow2) {
  big_integer a("18446744073709551616");
  big_integer b("-18446744073709551616");
  big_integer c("36893488147419103232");

  EXPECT_EQ(c, a + a);
  EXPECT_EQ(a, b + c);
  EXPECT_EQ(a, c + b);
}

TEST(correctness, sub_long) {
  big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("100000000000000000000000000000000000000");
  big_integer c("9999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");

  EXPECT_EQ(c, a - b);
}

TEST(correctness, mul_long) {
  big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("100000000000000000000000000000000000000");
  big_integer c("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                "00000000000000000000000000000000000000");

  EXPECT_EQ(c, a * b);
}

TEST(correctness, mul_long_signed) {
  big_integer a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("100000000000000000000000000000000000000");
  big_integer c("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                "00000000000000000000000000000000000000");

  EXPECT_EQ(c, a * b);
}

TEST(correctness, mul_long_signed2) {
  big_integer a("-100000000000000000000000000");
  big_integer c("100000000000000000000000000"
                "00000000000000000000000000");

  EXPECT_EQ(c, a * a);
}

TEST(correctness, mul_long_pow2) {
  big_integer a("18446744073709551616");
  big_integer b("340282366920938463463374607431768211456");
  big_integer c("115792089237316195423570985008687907853269984665640564039457584007913129639936");

  EXPECT_EQ(b, a * a);
  EXPECT_EQ(c, b * b);
}

TEST(correctness, div_long) {
  big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("100000000000000000000000000000000000000");
  big_integer c("100000000000000000000000000000000000000000000000000000");

  big_integer d = a / b;
  EXPECT_EQ(c, a / b);
}

TEST(correctness, div_long_signed) {
  big_integer a("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("100000000000000000000000000000000000000");
  big_integer c("-100000000000000000000000000000000000000000000000000000");

  EXPECT_EQ(c, a / b);
}

TEST(correctness, div_long_signed2) {
  big_integer a("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  big_integer b("-100000000000000000000000000000000000000");
  big_integer c("100000000000000000000000000000000000000000000000000000");

  EXPECT_EQ(c, a / b);
}

TEST(correctness, negation_long) {
  big_integer a("10000000000000000000000000000000000000000000000000000");
  big_integer c("-10000000000000000000000000000000000000000000000000000");

  EXPECT_EQ(c, -a);
  EXPECT_EQ(-c, a);
}

TEST(correctness, shl_long) {
  EXPECT_EQ(big_integer("1091951238831590836520041079875950759639875963123939936"),
            big_integer("34123476213487213641251283746123461238746123847623123") << 5);

  EXPECT_EQ(big_integer("-104637598388784443044449444577438556334703518260785595038524928"),
            big_integer("-817481237412378461284761285761238721364871236412387461238476") << 7);

  EXPECT_EQ(big_integer("26502603392713913241969902328696116541550413468869982914247384891392"),
            big_integer("12341236412857618761234871264871264128736412836643859238479") << 31);
}

TEST(correctness, shr_long) {
  EXPECT_EQ(big_integer("4730073393008085198307104580698364137020387111323398632330851"),
            big_integer("151362348576258726345827346582347652384652387562348756234587245") >> 5);

  EXPECT_EQ(big_integer("1118311528397465815295799577134738919815767762822175104787"),
            big_integer("143143875634875624357862345873246581736418273641238413412741") >> 7);

  EXPECT_EQ(big_integer("-1591563309890326054125627839548891585559049824963"),
            big_integer("-3417856182746231874623148723164812376512852437523846123876") >> 31);

}

TEST(correctness, string_conv) {
  EXPECT_EQ("100", to_string(big_integer("100")));
  EXPECT_EQ("100", to_string(big_integer("0100")));
  EXPECT_EQ("0", to_string(big_integer("0")));
  EXPECT_EQ("0", to_string(big_integer("-0")));
  EXPECT_EQ("-1000000000000000", to_string(big_integer("-1000000000000000")));

  big_integer lim = std::numeric_limits<int32_t>::max();
  EXPECT_EQ("2147483647", to_string(lim));
  lim++;
  EXPECT_EQ("2147483648", to_string(lim));
  lim = std::numeric_limits<int32_t>::min();
  EXPECT_EQ("-2147483648", to_string(lim));
  lim--;
  EXPECT_EQ("-2147483649", to_string(lim));
}

namespace {
size_t const number_of_iterations = 10;
size_t const max_size = 2048;
size_t const number_of_multipliers = 1000;

int myrand() {
  int val = rand() - RAND_MAX / 2;
  if (val != 0)
    return val;
  else
    return 1;
}
}

TEST(correctness, mul_div_randomized) {
  for (unsigned itn = 0; itn != number_of_iterations; ++itn) {
    std::vector<int> multipliers;

    for (size_t i = 0; i != number_of_multipliers; ++i)
      multipliers.push_back(myrand());

    big_integer accumulator = 1;

    for (size_t i = 0; i != number_of_multipliers; ++i)
      accumulator *= multipliers[i];

    std::shuffle(multipliers.begin(), multipliers.end(), std::mt19937(std::random_device()()));

    for (size_t i = 1; i != number_of_multipliers; ++i)
      accumulator /= multipliers[i];

    EXPECT_TRUE(accumulator == multipliers[0]);
  }
}

namespace {
template<typename T>
void erase_unordered(std::vector<T>& v, typename std::vector<T>::iterator pos) {
  std::swap(v.back(), *pos);
  v.pop_back();
}

template<typename T>
T extract_random_element(std::vector<T>& v) {
  size_t index = rand() % v.size();
  T copy = v[index];
  erase_unordered(v, v.begin() + index);
  return copy;
}

template<typename T>
void merge_two(std::vector<T>& v) {
  assert(v.size() >= 2);

  T a = extract_random_element(v);
  T b = extract_random_element(v);

  T ab = a * b;
  ASSERT_TRUE(ab / a == b);
  ASSERT_TRUE(ab / b == a);

  v.push_back(ab);
}

template<typename T>
T merge_all(std::vector<T> v) {
  assert(!v.empty());

  while (v.size() >= 2)
    merge_two(v);

  return v[0];
}
}

TEST(correctness, mul_merge_randomized) {
  for (unsigned itn = 0; itn != number_of_iterations; ++itn) {
    std::vector<big_integer> x;
    for (size_t i = 0; i != number_of_multipliers; ++i)
      x.emplace_back(myrand());

    big_integer a = merge_all(x);
    big_integer b = merge_all(x);

    EXPECT_TRUE(a == b);
  }
}

namespace {
big_integer rand_big(size_t size) {
  big_integer result = rand();

  for (size_t i = 0; i != size; ++i) {
    result *= RAND_MAX;
    result += rand();
  }

  return result;
}
}

TEST(correctness, div_randomized) {
  for (size_t itn = 0; itn != number_of_iterations * number_of_multipliers; ++itn) {
    big_integer divident = rand_big(10);
    big_integer divisor = rand_big(6);
    big_integer quotient = divident / divisor;
    big_integer residue = divident % divisor;
    ASSERT_EQ(divident - quotient * divisor, residue);
    EXPECT_GE(residue, 0);
    EXPECT_LT(residue, divisor);
  }
}

// y2019 tests

TEST(correctness_random, cmp) {
  std::default_random_engine rng(42);
  for (size_t itn = 0; itn != number_of_iterations; ++itn) {
    big_integer_gmp a, b;
    a.random(max_size, rng);
    b.random(max_size, rng);
    big_integer A = big_integer(to_string(a));
    big_integer B = big_integer(to_string(b));
    EXPECT_EQ(a < b, A < B);
    EXPECT_EQ(a > b, A > B);
    EXPECT_EQ(a <= b, A <= B);
    EXPECT_EQ(a >= b, A >= B);
    EXPECT_EQ(a == b, A == B);
  }
}

TEST(correctness_random, add) {
  std::default_random_engine rng(42);
  for (size_t itn = 0; itn != number_of_iterations; ++itn) {
    big_integer_gmp a, b;
    a.random(max_size, rng);
    b.random(max_size, rng);
    big_integer_gmp c = a + b;
    big_integer R = big_integer(to_string(a)) + big_integer(to_string(b));
    EXPECT_EQ(to_string(c), to_string(R));
  }
}

TEST(correctness_random, sub) {
  std::default_random_engine rng(42);
  for (size_t itn = 0; itn != number_of_iterations; ++itn) {
    big_integer_gmp a, b;
    a.random(max_size, rng);
    b.random(max_size, rng);
    big_integer_gmp c = a - b;
    big_integer R = big_integer(to_string(a)) - big_integer(to_string(b));
    EXPECT_EQ(to_string(c), to_string(R));
  }
}

TEST(correctness_random, mul) {
  std::default_random_engine rng(42);
  for (size_t itn = 0; itn != number_of_iterations; ++itn) {
    big_integer_gmp a, b;
    a.random(max_size, rng);
    b.random(max_size, rng);
    big_integer_gmp c = a * b;
    big_integer R = big_integer(to_string(a)) * big_integer(to_string(b));
    EXPECT_EQ(to_string(c), to_string(R));
  }
}

TEST(correctness_random, div) {
  std::default_random_engine rng(322);
  for (size_t itn = 0; itn != number_of_iterations; ++itn) {
    big_integer_gmp a, b;
    a.random(max_size, rng);
    b.random(max_size / 2, rng);
    big_integer_gmp c = a / b;
    big_integer R = big_integer(to_string(a)) / big_integer(to_string(b));
    EXPECT_EQ(to_string(c), to_string(R));

    c = b / a;
    R = big_integer(to_string(b)) / big_integer(to_string(a));
    EXPECT_EQ(to_string(c), to_string(R));
  }
}

TEST(correctness_random, mod) {
  std::default_random_engine rng(322);
  for (size_t itn = 0; itn != number_of_iterations; ++itn) {
    big_integer_gmp a, b;
    a.random(max_size, rng);
    b.random(max_size / 2, rng);
    big_integer_gmp c = a % b;
    big_integer R = big_integer(to_string(a)) % big_integer(to_string(b));
    EXPECT_EQ(to_string(c), to_string(R));

    c = b % a;
    R = big_integer(to_string(b)) % big_integer(to_string(a));
    EXPECT_EQ(to_string(c), to_string(R));
  }
}

TEST(correctness_random, bitwise) {
  std::default_random_engine rng(42);
  for (size_t itn = 0; itn != number_of_iterations; ++itn) {
    big_integer_gmp a, b;
    a.random(max_size, rng);
    b.random(max_size, rng);
    big_integer_gmp c = a & b;
    big_integer R = big_integer(to_string(a)) & big_integer(to_string(b));
    EXPECT_EQ(to_string(c), to_string(R));

    c = a | b;
    R = big_integer(to_string(a)) | big_integer(to_string(b));
    EXPECT_EQ(to_string(c), to_string(R));

    c = a ^ b;
    R = big_integer(to_string(a)) ^ big_integer(to_string(b));
    EXPECT_EQ(to_string(c), to_string(R));
  }
}

TEST(correctness_random, bit_shifts) {
  std::default_random_engine rng(42);
  for (size_t itn = 0; itn != number_of_iterations; ++itn) {
    big_integer_gmp a;
    a.random(max_size, rng);
    int shift = myrand() % max_size;
    big_integer R = big_integer(to_string(a));

    EXPECT_EQ(to_string(a << shift), to_string(R << shift));
    EXPECT_EQ(to_string(a >> shift), to_string(R >> shift));
  }
}

// TODO: extend due to idea
TEST(correctness_twos_complement, simple) {
  std::string a = "-36893488147419103232"; // -(1 << 65)
  std::string b = "147573952589676412928"; //  (1 << 67)

  big_integer_gmp gmp_a(a), gmp_b(b);
  big_integer your_a(a), your_b(b);

  big_integer_gmp gmp_ans = gmp_a | gmp_b;
  big_integer your_ans = your_a | your_b;

  EXPECT_EQ(to_string(gmp_ans), to_string(your_ans));
}

TEST(correctness_twos_complement, all_ones) {
  std::string a = "-18446744073709551615"; // -((1 << 64) - 1)
  std::string b = "147573952589676412929"; // (1 << 67)

  big_integer_gmp gmp_a(a), gmp_b(b);
  big_integer your_a(a), your_b(b);

  big_integer_gmp gmp_ans = gmp_a & gmp_b;
  big_integer your_ans = your_a & your_b;

  EXPECT_EQ(to_string(gmp_ans), to_string(your_ans));
}
