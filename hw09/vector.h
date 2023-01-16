#pragma once

#include <memory>
#include <ostream>
#include <stdexcept>

template <typename T>
class Vector {
public:
    Vector() = default;

    /**
     * Creates a vector of size n with values default_val.
     */
    Vector(size_t n, const T& default_val);

    /**
     * Creates a vector containing the elements in l.
     */
    Vector(std::initializer_list<T> l);

    Vector(const Vector& copy);

    Vector(Vector&& move);

    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(const Vector& copy);


    /**
     * Replaces the contents of the vector.
     */
    Vector& operator=(Vector&& move) noexcept;

    size_t size() const noexcept { return _size; }

    size_t capacity() const noexcept { return _capacity; }

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(const T& value);

    /**
     * Appends the given element value to the end of the vector.
     */
    void push_back(T&& value);

    /**
     * Removes the last element of the vector.
     */
    void pop_back();

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    T& at(const size_t pos) const;

    /**
     * Returns a reference to the element at specified location pos, with bounds checking.
     * If pos is not within the range of the vector, an exception of type std::out_of_range is thrown.
     */
    T& at(const size_t pos);

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    T& operator[](const size_t index) const;

    /**
     * Returns a reference to the element at specified location pos.
     * No bounds checking is performed.
     */
    T& operator[](const size_t index);

    friend std::ostream& operator<<(std::ostream& o, Vector<T> v) {
        o << "Size: " << v._size << ", Capacity: " << v._capacity << std::endl;
        for (size_t i = 0; i < v._size; ++i) {
            if (i > 0)
                o << ", ";
            o << v._data[i];
        }
        o << std::endl;
        return o;
    }

    ~Vector() = default;

private:
    // Defines how the `_capacity` is increased.
    size_t growth_factor = 2;
    size_t _size;
    size_t _capacity;

    // Holds vector's data.
    std::unique_ptr<T[]> _data;

    /**
     * Calculates the necessary capacity for new_size.
     * If necessary, double `_capacity` using `growth_factor`.
     */
    size_t calculate_capacity(size_t new_size);

    /**
    * Resizes the vector to new_capacity.
    * If the current capacity is less than new_capacity
    * the vector moves all elements to a new array.
    */
    void resize(size_t new_capacity);
};

//out-of-line definitions

template <typename T>
Vector<T>::Vector(size_t n, const T& default_val) : _size(n), _capacity(n) {
        _data = std::make_unique<T[]>(n);
        for (size_t i = 0; i < n; i++) {
            _data[i] = default_val;
        }
    }

template <typename T>
Vector<T>::Vector(std::initializer_list<T> l) : _size(l.size()), _capacity(l.size()) {
        _data = std::make_unique<T[]>(l.size());
        size_t i = 0;
        for (auto val : l) {
            _data[i++] = val;
        }
    }

template <typename T>
Vector<T>::Vector(const Vector& copy) : _size(copy._size), _capacity(copy._capacity) {
        _data = std::make_unique<T[]>(_size);
        std::copy(copy._data.get(), copy._data.get() + _size, _data.get());
    }

template <typename T>
Vector<T>::Vector(Vector&& move) : _size(move._size), _capacity(move._capacity) {
    _data = std::move(move._data);
    move._size = 0;
    move._capacity = 0;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& copy) {
    if (&copy == this) {
        return *this;
    }
    _size = copy._size;
    _capacity = copy._capacity;
    _data = std::make_unique<T[]>(_size);
    std::copy(copy._data.get(), copy._data.get() + _size, _data.get());
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& move) noexcept {
    if (&move == this) {
        return *this;
    }
    _size = move._size;
    _capacity = move._capacity;
    _data = std::move(move._data);
    move._size = 0;
    move._capacity = 0;
    return *this;
}

template <typename T>
void Vector<T>::push_back(const T& value) {
    if ((_size+1) > _capacity) {
        Vector<T>::resize(_capacity * growth_factor);
    }
    _data[_size++] = value;
}

template <typename T>
void Vector<T>::push_back(T&& value) {
    if ((_size+1) > _capacity) {
        Vector<T>::resize(_capacity * growth_factor);
    }
    _data[_size++] = std::move(value);
}

template <typename T>
void Vector<T>::pop_back() {
    if (_size == 0) {
        throw std::out_of_range("Vector<T>::pop_back(): vector is empty");
    }
    _size--;
}

template <typename T>
size_t Vector<T>::calculate_capacity(size_t new_size) {
    if (_capacity == 0) {
        return new_size;
    }
    if (new_size <= _capacity) {
        return _capacity;
    }
    return _capacity * growth_factor;
}

template <typename T>
void Vector<T>::resize(size_t new_capacity) {
    if (new_capacity <= _capacity) {
        return;
    }
    std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);
    std::copy(_data.get(), _data.get() + _size, new_data.get());
    _data = std::move(new_data);
    _capacity = new_capacity;
}

template <typename T>
T& Vector<T>::at(const size_t pos) const {
    if (pos >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _data[pos];
}

template <typename T>
T& Vector<T>::at(const size_t pos) {
    if (pos >= _size) {
        throw std::out_of_range("Index out of range");
    }
    return _data[pos];
}

template <typename T>
T& Vector<T>::operator[](const size_t index) const {
    return _data[index];
}

template <typename T>
T& Vector<T>::operator[](const size_t index) {
    return _data[index];
}