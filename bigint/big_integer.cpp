#include "big_integer.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <tuple>
#include <cmath>


big_integer::big_integer() : data(), sign(false) {
}

big_integer::big_integer(big_integer const &other) = default;

big_integer::big_integer(int a) : big_integer() {
    set_digit(0, a);
    set_sign();
    shrink_to_fit();
}

big_integer::big_integer(uint64_t a) : big_integer() {
    data.push_back(a & UINT32_MAX);
    data.push_back(a >> BIT_DEPTH);
    data.push_back(0);
    set_sign();
    shrink_to_fit();
}

big_integer::big_integer(const std::string &s) : big_integer() {
    big_integer &res = (*this);
    for (char i : s) {
        if (i == '-') continue;
        res = res.mul_by_uint32_t(10) + (int) (i - '0');
    }
    if (!s.empty() && s[0] == '-') {
        res = -res;
    }
    shrink_to_fit();
}

big_integer::~big_integer() = default;

big_integer &big_integer::operator=(big_integer const &other) = default;


big_integer &big_integer::operator+=(big_integer const &rhs) {
    size_t max_len = std::max(data.size(), rhs.data.size()) + 1;
    uint64_t carry = 0;
    for (size_t i = 0; i < max_len; i++) {
        carry = carry + get_digit(i) + rhs.get_digit(i);
        set_digit(i, carry & UINT32_MAX);
        carry >>= BIT_DEPTH;
    }
    set_sign();
    shrink_to_fit();
    return *this;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    return *this += (-rhs);
}

big_integer copy(big_integer const &left, size_t l, size_t r){
    big_integer temp;
    for (size_t i = l; i < r; i ++){
        temp.set_digit(i - l, left.data[i]);
    }
    return temp;
}


big_integer big_integer::Karatsuba_mul(big_integer const &left, big_integer const &right) {
    if (left.data.size() == 0 || right.data.size() == 0){
        return 0;
    }
    size_t n = std::min(left.data.size(), right.data.size());
    if (left.data.size() == 1) {
        return right.mul_by_uint32_t(left.get_digit(0));
    }
    if (right.data.size() == 1){
        return left.mul_by_uint32_t(right.get_digit(0));
    }
    n *= BIT_DEPTH;
    size_t ndiv2 = n / 2;
    big_integer left_l = left >> (ndiv2);
    big_integer left_r = (left - (left_l << (ndiv2)));
    big_integer right_l = right >> (ndiv2);
    big_integer right_r = (right - (right_l << (ndiv2)));


    big_integer product_1 = Karatsuba_mul(left_l, right_l);
    big_integer product_2 = Karatsuba_mul(left_r, right_r);
    big_integer product_3 = Karatsuba_mul(left_l + left_r, right_l + right_r);

    return (product_1 << (n)) +((product_3 - product_1 - product_2) << ndiv2) + product_2;
}


big_integer &big_integer::operator*=(big_integer const &rhs) {
    big_integer left = abs(*this);
    big_integer right = abs(rhs);
    big_integer &result = *this;
    bool result_sign = sign ^rhs.sign;
    result = 0; //Karatsuba_mul(left, right);
    for (size_t i = 0; i < right.data.size(); i++) {
        result += left.mul_by_uint32_t(right.data[i]) << (static_cast<int32_t>(BIT_DEPTH * i));
    }
    if (result_sign) {
        result = -result;
    }
    return result;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    bool result_sign = sign ^ rhs.sign;
    big_integer rhs_abs = abs(rhs), &this_abs = *this;
    this_abs = abs(this_abs);
    if (rhs_abs > this_abs) {
        return this_abs = 0;
    }
    if (rhs_abs.data.size() == 1) {
        this_abs = this_abs.div_by_uint32_t(rhs_abs.data.back()).first;
        if (result_sign) {
            this_abs = -this_abs;
        }
        return this_abs;
    }
    
    size_t n = rhs_abs.data.size(), m = this_abs.data.size();
    uint64_t f = BASE / static_cast<uint64_t>(rhs_abs.data.back() + 1);
    big_integer r = this_abs.mul_by_uint32_t(static_cast<uint32_t>(f));
    big_integer d = rhs_abs.mul_by_uint32_t(static_cast<uint32_t>(f));
    big_integer result;

    result.data.resize(m - n + 1, empty_block());

    for (ptrdiff_t k = m - n; k >= 0; k--) {
        r.data.resize(m + n + 1, empty_block());
        unsigned __int128 dividend =
                ((((static_cast<unsigned __int128>(1) * r.get_digit(n + k)) << BIT_DEPTH) + r.data[n + k - 1])
                        << BIT_DEPTH) + r.data[n + k - 2];
        uint64_t divisor = ((static_cast<uint64_t>(1) * d.get_digit(n - 1)) << BIT_DEPTH) + d.data[n - 2];
        uint32_t qt = dividend / divisor > BASE - 1 ? BASE - 1 : dividend / divisor;
        big_integer dq = (d.mul_by_uint32_t(qt) << (BIT_DEPTH * k));
        if (r < dq) {
            qt--;
            dq = (d.mul_by_uint32_t(qt) << (BIT_DEPTH * k));
        }
        result.set_digit(k, qt);
        r -= dq;
    }


    if (result_sign) {
        result = -result;
    }
    result.shrink_to_fit();
    return *this = result;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    return *this = *this - (*this / rhs) * rhs;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    size_t max_len = std::max(data.size(), rhs.data.size()) + 1;
    for (size_t i = 0; i < max_len; i++) {
        set_digit(i, get_digit(i) & rhs.get_digit(i));
    }
    set_sign();
    shrink_to_fit();
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    size_t max_len = std::max(data.size(), rhs.data.size()) + 1;
    for (size_t i = 0; i < max_len; i++) {
        set_digit(i, get_digit(i) | rhs.get_digit(i));
    }
    set_sign();
    shrink_to_fit();
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    size_t max_len = std::max(data.size(), rhs.data.size()) + 1;
    for (size_t i = 0; i < max_len; i++) {
        set_digit(i, get_digit(i) ^ rhs.get_digit(i));
    }
    set_sign();
    shrink_to_fit();
    return *this;
}

big_integer &big_integer::operator<<=(int rhs) {
    if (rhs == 0) {
        return *this;
    }
    int32_t shift_blocks = rhs / 32;
    int32_t shift_bits = rhs % 32;
    data.insert(data.begin(), shift_blocks + (shift_bits > 0), 0);
    (*this) >>= ((-shift_bits + 32) % 32);
    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    if (rhs == 0) {
        return *this;
    }
    size_t shift_block = rhs / BIT_DEPTH;
    size_t shift_bits = rhs % BIT_DEPTH;
    if (shift_block > data.size()){
        (*this) = 0;
        return *this;
    }
    data.erase(data.begin(), data.begin() + shift_block);
    if (shift_bits == 0){
        return *this;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        uint32_t next_block = empty_block();
        if (i + 1 <= data.size()) {
            next_block = get_digit(i + 1);
        }
        set_digit(i, (shift_bits == 0 ? 0 : next_block << (BIT_DEPTH - shift_bits)) + (get_digit(i) >> shift_bits));
    }
    shrink_to_fit();
    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer r = ~*this + 1;
    return r;
}

big_integer big_integer::operator~() const {
    big_integer r = *this;
    for (size_t i = 0; i < data.size(); i++) {
        r.data[i] = ~r.data[i];
    }
    r.sign = !r.sign;
    r.shrink_to_fit();
    return r;
}


big_integer &big_integer::operator++() {
    return *this = *this + 1;
}

const big_integer big_integer::operator++(int) {
    big_integer r = *this;
    *this += 1;
    return r;
}

big_integer &big_integer::operator--() {
    return *this = *this - 1;
}

const big_integer big_integer::operator--(int) {
    big_integer r = *this;
    *this -= 1;
    return r;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

int32_t comparator(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign ? -1 : 1;
    }
    for (ptrdiff_t index = std::max(a.data.size(), b.data.size()); index >= 0; index--) {
        uint32_t a_value = a.get_digit(index), b_value = b.get_digit(index);
        if (a_value != b_value) {
            return a_value > b_value ? 1 : -1;
        }
    }
    return 0;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return comparator(a, b) == 0;
}

bool operator<(big_integer const &a, big_integer const &b) {
    return comparator(a, b) < 0;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return comparator(a, b) != 0;
}

bool operator>(big_integer const &a, big_integer const &b) {
    return comparator(a, b) > 0;
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return comparator(a, b) <= 0;
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return comparator(a, b) >= 0;
}

std::string to_string(const big_integer &rhs) {
    if (rhs == 0) {
        return "0";
    }
    big_integer x = abs(rhs);
    std::string res;
    while (x > 0) {
        std::pair<big_integer, uint32_t> div_mod_10 = x.div_by_uint32_t(10);
        uint32_t cur = div_mod_10.second;
        res.push_back((char) (cur + '0'));
        x = div_mod_10.first;
    }
    if (rhs.sign) {
        res.push_back('-');
    }
    reverse(res.begin(), res.end());
    return res;
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}


big_integer abs(big_integer const &a) {
    return a.sign ? -a : a;
}

uint32_t big_integer::get_digit(size_t index) const {
    if (index < data.size()) {
        return data[index];
    } else {
        return empty_block();
    }
}

uint32_t big_integer::empty_block() const {
    return static_cast<uint32_t>(-sign);
}

template<typename T>
void big_integer::set_digit(size_t index, T digit) {
    if (index == data.size()) {
        data.push_back(static_cast<uint32_t>(digit));
        return;
    }
    data[index] = static_cast<uint32_t>(digit);
}

void big_integer::set_sign() {
    sign = bool(data.back() >> 31);
}

std::pair<big_integer, uint32_t> big_integer::div_by_uint32_t(uint32_t const rhs) const {
    big_integer res;
    res.data.resize(data.size());
    uint64_t temp = 0;
    for (ptrdiff_t i = res.data.size() - 1; i >= 0; i--) {
        temp = temp * static_cast<uint64_t>(UINT32_MAX + 1ll) + get_digit(i);
        res.set_digit(i, temp / rhs);
        temp %= rhs;
    }
    res.shrink_to_fit();
    return std::make_pair(res, temp);
}

big_integer big_integer::mul_by_uint32_t(uint32_t const rhs) const {
    big_integer res;
    uint64_t carry = 0;
    for (size_t i = 0; i < data.size(); i++) {
        carry = static_cast<uint64_t>(data[i]) * rhs + carry;
        res.set_digit(i, carry & UINT32_MAX);
        carry >>= BIT_DEPTH;
    }
    if (res.data.size() <= data.size()) {
        res.set_digit(data.size(), carry);
    } else {
        if (carry != 0) {
            throw std::runtime_error("Overflow");
        }
    }
    res.shrink_to_fit();
    return res;
}

void big_integer::shrink_to_fit() {
    while (!data.empty() && data.back() == empty_block()) {
        data.pop_back();
    }
}
