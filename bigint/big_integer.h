#pragma once

#include <vector>
#include <string>

struct big_integer {
    big_integer();

    big_integer(big_integer const &other);

    big_integer(int a);

    explicit big_integer(uint64_t a);

    explicit big_integer(std::string const &str);

    ~big_integer();

    big_integer &operator=(big_integer const &other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer &operator++();

    const big_integer operator++(int);

    big_integer &operator--();

    const big_integer operator--(int);

    friend bool operator==(big_integer const &a, big_integer const &b);

    friend bool operator!=(big_integer const &a, big_integer const &b);

    friend bool operator<(big_integer const &a, big_integer const &b);

    friend bool operator>(big_integer const &a, big_integer const &b);

    friend bool operator<=(big_integer const &a, big_integer const &b);

    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend big_integer abs(big_integer const &a);

    friend std::string to_string(big_integer const &a);

    [[nodiscard]] uint32_t get_digit(size_t index) const;

private:

    static const uint32_t BIT_DEPTH = 32;
    static const uint64_t BASE = static_cast<uint64_t>(1) + UINT32_MAX;

    std::vector<uint32_t> data;
    bool sign;

    [[nodiscard]] big_integer mul_by_uint32_t(uint32_t rhs) const;

    friend big_integer copy(big_integer const &left, size_t l, size_t r);

    big_integer Karatsuba_mul(big_integer const & left, big_integer const & right);

    [[nodiscard]]  std::pair<big_integer, uint32_t> div_by_uint32_t(uint32_t rhs) const;

    friend int32_t comparator(big_integer const &a, big_integer const &b);

    void set_sign();

    void shrink_to_fit();

    uint32_t empty_block() const;

    template<typename T>
    void set_digit(size_t index, T digit);
};

big_integer abs(big_integer const &a);

big_integer operator+(big_integer a, big_integer const &b);

big_integer operator-(big_integer a, big_integer const &b);

big_integer operator*(big_integer a, big_integer const &b);

big_integer operator/(big_integer a, big_integer const &b);

big_integer operator%(big_integer a, big_integer const &b);

big_integer operator&(big_integer a, big_integer const &b);

big_integer operator|(big_integer a, big_integer const &b);

big_integer operator^(big_integer a, big_integer const &b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);

std::string to_string(big_integer const& a);
std::ostream& operator<<(std::ostream& s, big_integer const& a);
