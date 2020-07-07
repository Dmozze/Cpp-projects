#include "vector.h"

void vector::split() {
    big_array = std::make_shared<std::vector<uint32_t> >(*big_array);
}

void vector::check_uniqueness() {
    if (!big_array.unique()) split();
}

void vector::expand_to_big_one() {
    big_array = std::make_shared<std::vector<uint32_t> >();
    (*big_array).resize(size_);
    for (size_t i = 0; i < size_; i++) {
        (*big_array)[i] = small_array[i];
    }
    is_small = false;
}

vector::vector() : big_array(nullptr), size_(0), is_small(true) {}

vector::vector(size_t x) : vector() {
    if (x > SMALL_SIZE) {
        big_array = std::make_shared<std::vector<uint32_t> >();
        (*big_array).resize(x);
        is_small = false;
    }
    size_ = x;
}

vector::vector(const vector &x) : vector() {
    small_array = x.small_array;
    big_array = x.big_array;
    is_small = x.is_small;
    size_ = x.size();
}

vector &vector::operator=(const vector &x) {
    vector copy(x);
    swap(copy);
    return (*this);
}

void vector::swap(vector &rhs) {
    std::swap(size_, rhs.size_);
    std::swap(is_small, rhs.is_small);
    std::swap(small_array, rhs.small_array);
    std::swap(big_array, rhs.big_array);
}

size_t vector::size() const {
    return size_;
}

uint32_t vector::operator[](const size_t i) const {
    return is_small ? small_array[i] : (*big_array)[i];
}

uint32_t& vector::operator[](size_t i) {
    if (is_small) {
        return small_array[i];
    } else {
        check_uniqueness();
        return (*big_array)[i];
    }
}

uint32_t vector::back() const {
    return is_small ? small_array[size_ - 1] : (*big_array).back();
}

bool vector::empty() const {
    return size_ == 0;
}

void vector::pop_back() {
    if (!is_small) {
        check_uniqueness();
        (*big_array).pop_back();
    }
    size_--;
}


void vector::resize(const size_t x, const uint32_t val) {
    if (x > size_) {
        if (is_small && x <= SMALL_SIZE) {
            for (size_t i = size_; i < x; i++) {
                small_array[i] = val;
            }
        } else {
            if (is_small) {
                expand_to_big_one();
            }
            check_uniqueness();
            (*big_array).resize(x, val);
        }
    }
    size_ = x;
}

