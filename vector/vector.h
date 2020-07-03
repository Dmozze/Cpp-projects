#pragma once

#include <cassert>
#include <cstddef>
#include <algorithm>

template<typename T>
struct vector {
    typedef T *iterator;
    typedef T const *const_iterator;

    vector() noexcept : data_(nullptr), size_(0), capacity_(0)                                // O(1) nothrow
    {}

    vector(vector const &other) : vector() {
        new_buffer(other.size_);
        copy_all(data_, other.data_, other.size_);
        size_ = other.size_;
        capacity_ = other.size_;
    }

    vector &operator=(vector const &rhs) {
        vector<T> copy(rhs);
        swap(copy);
        return *this;
    }

    ~vector() noexcept {
        destroy_all(data_, size_);
        operator delete(data_);
    }

    T &operator[](size_t i) noexcept {                // O(1) nothrow
        assert(!empty());
        return data_[i];
    }

    T const &operator[](size_t i) const noexcept {    // O(1) nothrow
        assert(!empty());
        return data_[i];
    }

    T *data() noexcept {                              // O(1) nothrow
        return begin();
    }

    T const *data() const noexcept {                  // O(1) nothrow
        return begin();
    }

    size_t size() const noexcept {                    // O(1) nothrow
        return size_;
    }

    T &front() noexcept {                             // O(1) nothrow
        assert(!empty());
        return data_[0];
    }

    T const &front() const noexcept{                 // O(1) nothrow
        assert(!empty());
        return data_[0];
    }

    T &back() noexcept {                              // O(1) nothrow
        assert(!empty());
        return data_[size_ - 1];
    }

    T const &back() const noexcept {                  // O(1) nothrow
        assert(!empty());
        return data_[size_ - 1];
    }

    void push_back(T const &element) {
        if (size_ < capacity_) {
            new(data_ + size_) T(element);
            size_++;
        } else {
            push_back_realloc(element);
        }
    }

    void pop_back() noexcept {
        assert(!empty());
        data_[size_ - 1].~T();
        size_--;
    }


    bool empty() const noexcept {                     // O(1) nothrow
        return size_ == 0;
    }

    size_t capacity() const noexcept {                // O(1) nothrow
        return capacity_;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            new_buffer(new_capacity);
        }
    }

    void shrink_to_fit() {
        if (size_ < capacity_) {
            new_buffer(size_);
        }
    }

    void clear() noexcept {
        destroy_all(data_, size_);
        size_ = 0;
    }

    void swap(vector &rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(size_, rhs.size_);
        std::swap(capacity_, rhs.capacity_);
    }

    iterator begin() noexcept {
        return data_;
    }

    iterator end() noexcept {
        return begin() + size();
    }

    const_iterator begin() const noexcept {
        return data_;
    }

    const_iterator end() const noexcept {
        return begin() + size();
    }

    iterator insert(iterator pos, T const &elem) {
        ptrdiff_t length_from_back = end() - pos;
        push_back(elem);
        iterator inserted_position = end() - 1;
        for (size_t i = 0; i < length_from_back; i++, inserted_position--) {
            std::swap(*(inserted_position - 1), *inserted_position);
        }
        return inserted_position;
    }

    iterator insert(const_iterator pos, T const &elem) {
        ptrdiff_t length_from_back = end() - pos;
        push_back(elem);
        iterator inserted_position = end() - 1;
        for (size_t i = 0; i < length_from_back; i++, inserted_position--) {
            std::swap(*(inserted_position - 1), *inserted_position);
        }
        return inserted_position;
    }

    iterator erase(iterator pos) {
        bool is_result_init = false;
        iterator result = nullptr;
        for (iterator i = pos; i < end() - 1; i++) {
            std::swap(*i, *(i + 1));
            if (!is_result_init) {
                result = i;
                is_result_init = true;
            }
        }
        pop_back();
        return result;
    }

    iterator erase(const_iterator pos) {
        bool is_result_init = false;
        iterator result = nullptr;
        for (iterator i = (iterator) pos; i < end() - 1; i++) {
            std::swap(*i, *(i + 1));
            if (!is_result_init) {
                result = i;
                is_result_init = true;
            }
        }
        pop_back();
        return result;
    }

    iterator erase(iterator first, iterator last) {
        bool is_result_init = false;
        iterator result = nullptr;
        for (iterator end_of_inter = last, begin_of_inter = first;
             end_of_inter < end(); begin_of_inter++, end_of_inter++) {
            std::swap(*begin_of_inter, *end_of_inter);
            if (!is_result_init) {
                result = begin_of_inter;
                is_result_init = true;
            }
        }
        for (ptrdiff_t len_of_inter = last - first; len_of_inter > 0; len_of_inter--) {
            pop_back();
        }
        return result;
    }

    iterator erase(const_iterator first, const_iterator last) {
        bool is_result_init = false;
        iterator result = nullptr;
        for (iterator end_of_inter = (iterator) last, begin_of_inter = (iterator) first;
             end_of_inter < end(); begin_of_inter++, end_of_inter++) {
            std::swap(*begin_of_inter, *end_of_inter);
            if (!is_result_init) {
                result = begin_of_inter;
                is_result_init = true;
            }
        }
        for (ptrdiff_t len_of_inter = last - first; len_of_inter > 0; len_of_inter--) {
            pop_back();
        }
        return result;
    }

private:

    size_t increase_capacity() const {
        if (capacity_ > 0) {
            return capacity_ * 2;
        } else {
            return 1;
        }
    }

    void push_back_realloc(T const &value) {
        T saved_value = value;
        new_buffer(increase_capacity());
        push_back(saved_value);
    }

    void new_buffer(size_t new_capacity) {
        assert(new_capacity >= size_);
        vector<T> tmp;
        if (new_capacity > 0) {
            tmp.data_ = static_cast<T *>(operator new(new_capacity * sizeof(T)));
            tmp.capacity_ = new_capacity;
            copy_all(tmp.data_, data_, size_);
            tmp.size_ = size_;
        }
        swap(tmp);
        return;
    }

    void destroy_all(T *data_, size_t size) {
        for (size_t i = size; i != 0; --i) {
            data_[i - 1].~T();
        }
    }

    void copy_all(T *dst, T const *src, size_t size) {
        size_t i = 0;
        try {
            for (; i != size; ++i) {
                new(dst + i) T(src[i]);
            }
        } catch (...) {
            destroy_all(dst, i);
            throw;
        }
    }


private:
    T *data_;
    size_t size_;
    size_t capacity_;
};
