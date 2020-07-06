#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <cassert>
#include <cstddef>
#include <algorithm>

template<typename T>
struct vector {
    using iterator = T*;
    using const_iterator = T const*;

    vector() noexcept : data_(nullptr), size_(0), capacity_(0)
    {}

    vector(vector const &other) : vector() {
        new_buffer(other.size_);
        copy_all(data_, other.data_, other.size_);
        size_ = other.size_;
        capacity_ = other.size_;
    }

    vector &operator=(vector const &rhs) {
        vector copy(rhs);
        swap(copy);
        return *this;
    }

    ~vector() noexcept {
        destroy_all(data_, size_);
        operator delete(data_);
    }

    T &operator[](size_t i) noexcept {
        assert(!empty());
        assert(i < size_);
        return data_[i];
    }

    T const &operator[](size_t i) const noexcept {
        assert(!empty());
        assert(i < size_);
        return data_[i];
    }

    T *data() noexcept {
        return begin();
    }

    T const *data() const noexcept {
        return begin();
    }

    size_t size() const noexcept {
        return size_;
    }

    T &front() noexcept {
        assert(!empty());
        return data_[0];
    }

    T const &front() const noexcept{
        assert(!empty());
        return data_[0];
    }

    T &back() noexcept {
        assert(!empty());
        return data_[size_ - 1];
    }

    T const &back() const noexcept {
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


    bool empty() const noexcept {
        return size_ == 0;
    }

    size_t capacity() const noexcept {
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


    iterator insert(const_iterator pos, T const &elem) {
        ptrdiff_t length_from_back = end() - pos;
        push_back(elem);
        iterator inserted_position = end() - 1;
        for (size_t i = 0; i < length_from_back; i++, inserted_position--) {
            std::swap(*(inserted_position - 1), *inserted_position);
        }
        return inserted_position;
    }



    iterator erase(const_iterator pos) {
        return erase(pos, pos + 1);
    }


    iterator erase(const_iterator first, const_iterator last) {
        bool is_result_init = false;
        iterator result = nullptr;
        for (iterator end_of_interval = begin() + static_cast<size_t>(last - begin()), begin_of_interval = (iterator) first;
             end_of_interval < end(); begin_of_interval++, end_of_interval++) {
            std::swap(*begin_of_interval, *end_of_interval);
            if (!is_result_init) {
                result = begin_of_interval;
                is_result_init = true;
            }
        }
        for (ptrdiff_t len_of_interval = last - first; len_of_interval > 0; len_of_interval--) {
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

#endif
