#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <array>
#include <memory>
#include <variant>

class vector {
    static const size_t SMALL_SIZE = 32;

    typedef std::array<uint32_t, SMALL_SIZE> small_array;
    typedef std::shared_ptr<std::vector<uint32_t> > big_array;

    std::variant<small_array, big_array> storage;
    size_t size_;
    bool is_small;

    void split();

    void expand_to_big_one();

public:

    vector();

    explicit vector(size_t x);

    vector(const vector &x);

    vector &operator=(const vector &x);

    void swap(vector &rhs);

    size_t size() const;

    bool empty() const;

    uint32_t operator[](size_t i) const;

    uint32_t &operator[](size_t i);

    uint32_t back() const;

    void pop_back();

    void resize(size_t x, uint32_t val);
};
