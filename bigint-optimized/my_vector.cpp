#include "my_vector.h"
#include <cassert>

void my_vector::split() {
    assert(!is_small);
    if (storage.big.unique()) {
        return;
    }
    storage.big = std::make_shared<std::vector<uint32_t>>(*storage.big);
}


void my_vector::expand_to_big_one() {
    assert(is_small);
    std::shared_ptr<std::vector<uint32_t>> temp = std::make_shared<std::vector<uint32_t>>(size_);
    for (size_t i = 0; i < size_; i++) {
        temp->at(i) = storage.small[i];
    }
    new(&storage.big) std::shared_ptr<std::vector<uint32_t>>(temp);
    is_small = false;
}

my_vector::my_vector() : size_(0), is_small(true) {}

my_vector::my_vector(size_t x) : my_vector() {
    if (x > SMALL_SIZE) {
        std::shared_ptr<std::vector<uint32_t>> temp = std::make_shared<std::vector<uint32_t>>(size_);
        new(&storage.big) std::shared_ptr<std::vector<uint32_t>>(temp);
        is_small = false;
    } else {
        storage.small = small_array();
    }
    size_ = x;
}

my_vector::~my_vector() noexcept {
    if (!is_small){
        storage.big.reset();
    }
}

my_vector::my_vector(const my_vector &x) : my_vector() {
    is_small = x.is_small;
    if (is_small) {
        storage.small = x.storage.small;
    } else {
        new(&storage.big) std::shared_ptr<std::vector<uint32_t>>(x.storage.big);
    }
    size_ = x.size();
}

my_vector &my_vector::operator=(const my_vector &x) {
    my_vector copy(x);
    swap(copy);
    return (*this);
}

void my_vector::swap(my_vector &rhs) {
    if (is_small && rhs.is_small) {
        std::swap(rhs.size_, size_);
        std::swap(is_small, rhs.is_small);
        std::swap(storage.small, rhs.storage.small);
        return;
    }
    if (is_small) {
        expand_to_big_one();
    }
    if (rhs.is_small) {
        rhs.expand_to_big_one();
    }
    std::swap(rhs.size_, size_);
    std::swap(rhs.storage.big, storage.big);
}

size_t my_vector::size() const {
    return size_;
}

uint32_t my_vector::operator[](const size_t i) const {
    return is_small ? storage.small[i] : storage.big->at(i);
}

uint32_t &my_vector::operator[](size_t i) {
    if (is_small) {
        return storage.small[i];
    } else {
        split();
        return storage.big->at(i);
    }
}

uint32_t my_vector::back() const {
    return is_small ? storage.small[size_ - 1] : storage.big->back();
}

bool my_vector::empty() const {
    return size_ == 0;
}

void my_vector::pop_back() {
    if (!is_small) {
        split();
        storage.big->pop_back();
    }
    size_--;
}


void my_vector::resize(const size_t x, const uint32_t val) {
    if (x > size_) {
        if (is_small && x <= SMALL_SIZE) {
            for (size_t i = size_; i < x; i++) {
                storage.small[i] = val;
            }
        } else {
            if (is_small) {
                expand_to_big_one();
            }
            split();
            storage.big->resize(x, val);
        }
    }
    size_ = x;
}
