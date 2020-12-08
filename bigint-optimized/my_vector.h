#pragma once

#include <iostream>
#include <cstdio>
#include <vector>
#include <array>
#include <memory>
#include <variant>

class my_vector {
public:
    my_vector();

    explicit my_vector(size_t x);

    my_vector(const my_vector &x);

    my_vector &operator=(const my_vector &x);

    ~my_vector();

    void swap(my_vector &rhs);

    size_t size() const;


    bool empty() const;

    uint32_t operator[](size_t i) const;

    uint32_t &operator[](size_t i);

    uint32_t back() const;

    void pop_back();

    void resize(size_t x, uint32_t val);

private:

    static constexpr size_t SMALL_SIZE = 8;

    using small_array = std::array<uint32_t, SMALL_SIZE>;
    using big_array = std::shared_ptr<std::vector<uint32_t> >;

    union any {
        big_array big;
        small_array small{};

        any() {};

        ~any() {};
    } storage;

    size_t size_;
    bool is_small;

    void split();

    void expand_to_big_one();
};
