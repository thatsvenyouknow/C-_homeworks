#include "vector.h"
#include <cmath>
#include <iterator>
#include <algorithm>
#include <numeric>

//linalg start
namespace linalg {
auto operator<<(std::ostream& ostr, const Vector& x) -> std::ostream& {
    ostr << "[ ";
    std::copy(x.begin(), x.end(), std::ostream_iterator<float>(ostr, " "));
    ostr << "]";
    return ostr;
}

//vector constructors/functions
Vector::Vector(std::size_t n) {
    std::vector<float> data_(n);};

Vector::Vector(std::size_t n, float val) {
    std::vector<float> data_(n, val);};

Vector::Vector(std::initializer_list<float> list) {
    std::vector<float> data_{list};};

Vector& Vector::operator=(float val) {
    this -> data_;
    for (auto &x : data_) {
        x = val;
    }
    return *this;
};

void Vector::assign(float val) {
    for (auto &x : data_) {
        x = val;
    }
};

void Vector::assign(Vector v) {
    std::vector<float> data_v = v.data_;
    std::vector<float> data_ = data_v;
};

std::size_t Vector::size() const {
    return Vector::data_.size();
};

Vector::iterator Vector::begin() {
    return Vector::data_.begin();
}

Vector::iterator Vector::end() {
    return Vector::data_.end();
}

Vector::const_iterator Vector::begin() const {
    return Vector::data_.begin();
}

Vector::const_iterator Vector::end() const {
    return Vector::data_.end();
}

Vector::const_iterator Vector::cbegin() const {
    return Vector::data_.begin();
}

Vector::const_iterator Vector::cend() const {
    return Vector::data_.end();
}

float& Vector::operator[](int idx) {
    if (idx>=0) {
        return Vector::data_.at(idx);
    }else{
        std::size_t vec_size{Vector::data_.size()};
        int new_idx{vec_size + idx};
        return Vector::data_.at(new_idx);
    }
}

const float& Vector::operator[](int idx) const {
    if (idx>=0) {
        return Vector::data_.at(idx);
    }else{
        std::size_t vec_size{Vector::data_.size()};
        int new_idx{vec_size + idx};
        return Vector::data_.at(new_idx);
    }
}

float& Vector::coeff(int idx) {
    std::size_t vec_size{Vector::data_.size()};
    if (idx>=0 && idx<=vec_size) {
        return Vector::data_.at(idx);
    }else {
        throw std::out_of_range("idx out of bounds");
    }
}

const float& Vector::coeff(int idx) const {
    std::size_t vec_size{Vector::data_.size()};
    if (idx>=0 && idx<=vec_size) {
        return Vector::data_.at(idx);
    }else {
        throw std::out_of_range("idx out of bounds");
    }
}

Vector& Vector::operator+=(float val) {
    for (auto &x : Vector::data_) {
        x += val;
    }
    return *this;
}

Vector& Vector::operator-=(float val) {
    for (auto &x : Vector::data_) {
        x -= val;
    }
    return *this;
}

Vector& Vector::operator*=(float val) {
    for (auto &x : Vector::data_) {
        x = x * val;
    }
    return *this;
}

Vector& Vector::operator/=(float val) {
    for (auto &x : Vector::data_) {
        x = x / val;
    }
    return *this;
}

Vector& Vector::operator+=(const Vector &y) {
    if(y.size() != Vector::data_.size()) {throw std::invalid_argument("vectors have diff size");}
    else {std::size_t i=0;
        while(i<y.size()){
            Vector::data_[i] += y[i];
            i++;}
            }
    return *this;
};

Vector& Vector::operator-=(const Vector &y) {
    if(y.size() != Vector::data_.size()) {throw std::invalid_argument("vectors have diff size");}
    else {std::size_t i=0;
        while(i<y.size()){
            Vector::data_[i] -= y[i];
            i++;}
            }
    return *this;
};


//linalg functions
float min(const Vector &x) {
    if (x.size() < 1){throw std::invalid_argument("vector empty");};
    return *std::min_element(x.begin(), x.end());
}

float max(const Vector &x) {
    if (x.size() < 1){throw std::invalid_argument("vector empty");};
    return *std::max_element(x.begin(), x.end());
}


std::size_t argmin(const Vector &x) {
    if (x.size() < 1){throw std::invalid_argument("vector empty");};
    std::size_t dist = std::distance(x.begin(), std::min_element(x.begin(), x.end()));
    return dist;
};

std::size_t argmax(const Vector &x) {
    if (x.size() < 1){throw std::invalid_argument("vector empty");};
    std::size_t dist = std::distance(x.begin(), std::max_element(x.begin(), x.end()));
    return dist;
};

std::size_t non_zeros(const Vector &x) {
    std::size_t v_len = x.size();
    return v_len - std::count(x.begin(), x.end(), 0.0);
}

float sum(const Vector &x) {
    float out = std::accumulate(x.begin(), x.end(), 0.0);
    return out;
};

float prod(const Vector &x) {
    float out = std::accumulate(x.begin(), x.end(), 1.0, std::multiplies<float>());
    return out;
};

float dot(const Vector &x, const Vector &y) {
    if (x.size() != y.size()) {throw std::invalid_argument("vectors of diff size");}
    float out = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
    return out;
};

float norm(const Vector &x) {
    auto dotted_vec = dot(x, x);
    return std::sqrt(dotted_vec);
};

void normalize(Vector &x) {
    float norm_x = norm(x);
    x = x / norm_x;
}

Vector normalized(const Vector &x) {
    float norm_x = norm(x);
    return x / norm_x;
}

Vector floor(const Vector &x) {
    Vector y = x;
    for (float &i : y) {
        i = std::floor(i);}
    return y;
}

Vector ceil(const Vector &x) {
    Vector y = x;
    for (float &i : y) {
        i = std::ceil(i);}
    return y;
}

Vector operator+(const Vector &x) {
    Vector y = x;
    return y;
}

Vector operator-(const Vector &x) {
    Vector y = x;
    y *= -1;
    return y;
}

Vector operator+(const Vector &x, const Vector &y) {
    Vector x_copy = x;
    Vector y_copy = y;
    x_copy += y_copy;
    return x_copy;
}

Vector operator-(const Vector &x, const Vector &y) {
    Vector x_copy = x;
    Vector y_copy = y;
    x_copy -= y_copy;
    return x_copy;
}

Vector operator+(const Vector &x, float val) {
    Vector y = x;
    y += val;
    return y;
}

Vector operator-(const Vector &x, float val) {
    Vector y = x;
    y -= val;
    return y;
}

Vector operator*(const Vector &x, float val) {
    Vector x_copy = x;
    x_copy *= val;
    return x_copy;
}

Vector operator/(const Vector &x, float val) {
    Vector y = x;
    y /= val;
    return y;
}

Vector operator+(float val, const Vector &x) {
    Vector y = x;
    y += val;
    return y;
}

Vector operator-(float val, const Vector &x) {
    Vector y = x;
    y -= val;
    return -y;
}

Vector operator*(float val, const Vector &x) {
    Vector y = x;
    y *= val;
    return y;
}

}



/*
 * Please Implement all functions from the header file here!#
 * Have fun! :-) And don't forget maybe you can use standard algorithms ;-)
 */

