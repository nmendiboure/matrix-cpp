//
// Created by nicolas on 23/12/23.
//

#include <vector>
#include <utility>
#include <stdexcept>

#include "./proto/matrix.pb.h"

#ifndef MATRIX_H
#define MATRIX_H


template<typename T>
class Matrix {
public:
    Matrix();
    Matrix(int row, int cols);
    Matrix(int rows, int cols, T defaultValue);
    Matrix(Matrix<T>&& m) noexcept ;
    explicit Matrix(std::vector<std::vector<T>>&& m) noexcept ;
    explicit Matrix(const std::vector<std::vector<T>>& m);

    // Prevent copying (deleted copy constructor and assignment operator)
    Matrix(const Matrix<T>& m) = delete;

    virtual ~Matrix() = default;

    /*
     * Inline element access functions for performance
     */
    inline T& operator()(int h, int w) { return array_[h][w];}
    inline const T& operator()(int h, int w) const { return array_[h][w];}
    inline std::vector<T>& operator()(int h) { if (h < 0) {h += height_ ;} return array_[h];}
    inline const std::vector<T>& operator()(int h) const { if (h < 0) {h += height_ ;} return array_[h];}

    inline T& get(int h, int w) { return array_[h][w];}
    inline const T& get(int h, int w) const { return array_[h][w];}
    inline std::vector<T>& get(int h) { if (h < 0) {h += height_ ;} return array_[h];}
    inline const std::vector<T>& get(int h) const { if (h < 0) {h += height_ ;} return array_[h];}

    // Inline getters
    [[nodiscard]] inline int getHeight() const { return height_; }
    [[nodiscard]] inline int getWidth() const { return width_; }
    [[nodiscard]] inline std::pair<int, int> getShape() const { return std::make_pair(height_, width_); }

    // Methods
    void clear();
    Matrix<T> duplicate() const;
    void erase(int index, int axis=0);
    void fill(const T& value);
    std::vector<T> getCol(int col);
    std::vector<T> getCol(int col) const;
    void insert(int index, const std::vector<T>& newData, int axis=0);
    void pop_back(int axis=0);
    void print(std::ostream &flux) const;
    void push_back(const std::vector<T>& newData, int axis=0);
    void put(int h, int w, const T& value);
    void reserve(int rows, int cols);
    void resize(int rows);
    void resize(int rows, int cols);
    Matrix<T> subMat(int startH, int startW, int h, int w) const;

    // Maths operations
    Matrix<T> add(const Matrix<T>& m) const;
    Matrix<T> subtract(const Matrix<T>& m) const;
    Matrix<T> multiply(const T& value) const;
    Matrix<T> multiply(const std::vector<T>& v) const;
    Matrix<T> multiply(const Matrix<T>& m) const;
    Matrix<T> divide(const T& value) const;
    Matrix<T> divide(const std::vector<T>& v) const;
    Matrix<T> divide(const Matrix<T>& m) const;
    Matrix<T> dot(const Matrix<T>& m) const;
    Matrix<T> transpose() const;

    T max() const;
    std::vector<T> max(int axis) const;
    T min() const;
    std::vector<T> min(int axis) const;
    T sum() const;
    std::vector<T> sum(int axis) const;
    Matrix<T> cumuSum(int axis) const;

    // Operators
    bool operator==(const Matrix<T>& m);
    bool operator!=(const Matrix<T>& m);
    Matrix<T>& operator+=(const Matrix<T>& m);
    Matrix<T>& operator-=(const Matrix<T>& m);
    Matrix<T>& operator*=(const Matrix<T>& m);
    Matrix<T>& operator*=(const std::vector<T>& v);
    Matrix<T>& operator*=(const T &s);
    Matrix<T>& operator/=(const T &s);
    Matrix<T>& operator/=(const std::vector<T>& v);
    Matrix<T>& operator/=(const Matrix<T>& m);
    Matrix<T>& operator=(const Matrix<T>& m) = delete;
    Matrix<T>& operator=(const std::vector<std::vector<T>>& m);
    Matrix<T>& operator=(Matrix<T>&& m) noexcept;
    Matrix<T>& operator=(std::vector<std::vector<T>>&& v) noexcept;

    // Serialization & deserialization
    void dumpToProto(const std::string& filePath) const;
    static Matrix<T> loadFromProto(const std::string& filePath);


private:
    std::vector<std::vector<T>> array_;
    int height_ = 0;
    int width_ = 0;
};

template <class T> inline Matrix<T> operator+(const Matrix<T>& a, const Matrix<T>& b) { return a.add(b); };
template <class T> inline Matrix<T> operator-(const Matrix<T>& a, const Matrix<T>& b) { return a.subtract(b); };
template <class T> inline Matrix<T> operator*(const Matrix<T>& a, const Matrix<T>& b) { return a.multiply(b); };
template <class T> inline Matrix<T> operator*(const Matrix<T>& a, const std::vector<T>& b) { return a.multiply(b); };
template <class T> inline Matrix<T> operator*(const Matrix<T>& a, const T &b) { return a.multiply(b); };
template <class T> inline Matrix<T> operator/(const Matrix<T>& a, const T &b) { return a.divide(b); };
template <class T> inline Matrix<T> operator/(const Matrix<T>& a, const std::vector<T>& b) { return a.divide(b); };
template <class T> inline Matrix<T> operator/(const Matrix<T>& a, const Matrix<T>& b) { return a.divide(b); };
template <class T> inline std::ostream& operator<<(std::ostream &flux, const Matrix<T>& m) { m.print(flux); return flux; }


/*
 * Serialization & deserialization
 * The serialization is done using protobuf
 */

template<typename T>
void MatrixToProto(const Matrix<T>& matrix, protoMatrix& protoMat);

template<typename T>
Matrix<T> ProtoToMatrix(const protoMatrix& protoMat);



#endif // MATRIX_H
