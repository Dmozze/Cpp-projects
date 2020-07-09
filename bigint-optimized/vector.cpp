#include "vector.h"

void vector::split() {
    if (std::get<big_array>(storage).unique()) return;
    std::get<big_array>(storage) = std::make_shared<std::vector<uint32_t> >(*std::get<big_array>(storage));
}


void vector::expand_to_big_one() {
    std::shared_ptr<std::vector<uint32_t> > temp = std::make_shared<std::vector<uint32_t> >();
    (*temp).resize(size_);
    for (size_t i = 0; i < size_; i++) {
        (*temp)[i] = std::get<small_array>(storage)[i];
    }
    storage = temp;
    is_small = false;
}

vector::vector() : size_(0), is_small(true) {}

vector::vector(size_t x) : vector() {
    if (x > SMALL_SIZE) {
        storage = std::make_shared<std::vector<uint32_t> >();
        (*std::get<big_array>(storage)).resize(x);
        is_small = false;
    } else {
        storage = small_array();
    }
    size_ = x;
}

vector::vector(const vector &x) : vector() {
    is_small = x.is_small;
    if (is_small){
        storage = std::get<small_array>(x.storage);
    } else {
        storage = std::get<big_array>(x.storage);
    }
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
    std::swap(storage, rhs.storage);
}

size_t vector::size() const {
    return size_;
}

uint32_t vector::operator[](const size_t i) const {
    return is_small ? std::get<small_array>(storage)[i] : (*std::get<big_array>(storage))[i];
}

uint32_t& vector::operator[](size_t i) {
    if (is_small) {
        return std::get<small_array>(storage)[i];
    } else {
        split();
        return (*std::get<big_array>(storage))[i];
    }
}

uint32_t vector::back() const {
    return is_small ? std::get<small_array>(storage)[size_ - 1] : (*std::get<big_array>(storage)).back();
}

bool vector::empty() const {
    return size_ == 0;
}

void vector::pop_back() {
    if (!is_small) {
        split();
        (*std::get<big_array>(storage)).pop_back();
    }
    size_--;
}


void vector::resize(const size_t x, const uint32_t val) {
    if (x > size_) {
        if (is_small && x <= SMALL_SIZE) {
            for (size_t i = size_; i < x; i++) {
                std::get<small_array>(storage)[i] = val;
            }
        } else {
            if (is_small) {
                expand_to_big_one();
            }
            split();
            (*std::get<big_array>(storage)).resize(x, val);
        }
    }
    size_ = x;
}

