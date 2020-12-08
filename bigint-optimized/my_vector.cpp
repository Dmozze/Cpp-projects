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
    std::shared_ptr<std::vector<uint32_t>> temp = std::make_shared<std::vector<uint32_t>>(storage.small.begin(), storage.small.begin() + size_);
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
    std::swap(is_small, rhs.is_small);
    std::swap(rhs.size_, size_);
    if (is_small && rhs.is_small){
        std::swap(storage.small, rhs.storage.small);
        return;
    }
    if (!is_small && !rhs.is_small){
        std::swap(storage.big, rhs.storage.big);
        return;
    }
    if (rhs.is_small){
        small_array temp = storage.small;
        new(&storage.big) std::shared_ptr<std::vector<uint32_t>>(rhs.storage.big);
        rhs.storage.big.reset();
        rhs.storage.small = temp;
        return;
    }
    if (is_small){
        small_array temp = rhs.storage.small;
        new(&rhs.storage.big) std::shared_ptr<std::vector<uint32_t>>(storage.big);
        storage.big.reset();
        storage.small = temp;
    }
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
    if (x > size_ && is_small)  {
        if (x <= SMALL_SIZE){
            for (size_t i = size_; i < x; i++) {
                storage.small[i] = val;
            }
        } else {
            expand_to_big_one();
        }
    }
    if (!is_small){
        split();
        storage.big->resize(x, val);
    }
    size_ = x;
}
