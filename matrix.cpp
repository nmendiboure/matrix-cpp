//
// Created by nicolas on 23/12/23.
//

#include "matrix.h"
#include <fstream>
#include <sstream>

/*
 * Matrix class
 * A core is a 2D array of elements of type T
 * The core is implemented as a vector of vectors
 * The first vector represents the rows, the second the columns
 * The core is stored in row-major order
 * The core is templated to allow different types of elements
 * The core is moveable and copyable
 */

/*
 * Constructor
 */

// Default constructor
template<class T>
Matrix<T>::Matrix(){
    this->height_ = 0;
    this->width_ = 0;
    this->array_ = std::vector<std::vector<T>>();
}

// Constructor with specified size
template<class T>
Matrix<T>::Matrix(int rows, int cols){
    this->array_.resize(rows, std::vector<T>(cols));
    this->height_ = rows;
    this->width_ = cols;
}

// Constructor with specified size and default value
template<class T>
Matrix<T>::Matrix(int rows, int cols, T defaultValue){
    this->height_ = rows;
    this->width_ = cols;
    this->array_ = std::vector<std::vector<T>>(rows, std::vector<T>(cols, defaultValue));
}

// Constructor from a vector of vectors
template<class T>
Matrix<T>::Matrix(const std::vector<std::vector<T>>& array_){
    this->height_ = array_.size();
    this->width_ = this->height_ > 0 ? array_[0].size() : 0;
    this->array_ = array_;
}


// Move constructor (Matrix&& m)
template<class T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept{
    this->height_ = other.height_;
    this->width_ = other.width_;
    this->array_ = std::move(other.array_);
    other.height_ = 0;
    other.width_ = 0;
}


// Move constructor (std::vector<std::vector<T>>&& m)
template<class T>
Matrix<T>::Matrix(std::vector<std::vector<T>>&& other) noexcept{
    this->height_ = other.size();
    this->width_ = this->height_ > 0 ? other[0].size() : 0;
    this->array_ = std::move(other);
    other.clear();
}


/*
 * Methods
 */

template <class T>
void Matrix<T>::clear(){
    for (auto& row : this->array_) {
        row.clear();
    }
    this->array_.clear();
    this->height_ = 0;
    this->width_ = 0;
}

template <class T>
Matrix<T> Matrix<T>::duplicate() const{
    Matrix<T> newMatrix(height_, width_);
    // Copy the data from the current matrix to newMatrix
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            newMatrix(i, j) = (*this)(i, j);
        }
    }
    return newMatrix;
}

template<class T>
void Matrix<T>::erase(int index, int axis) {
    if (axis == 0) {
        if (index < 0 || index >= this->height_) {
            throw std::out_of_range("Index out of bounds for row deletion.");
        }
        this->array_.erase(this->array_.begin() + index);
        this->height_--;
    } else if (axis == 1) {
        if (index < 0 || index >= this->width_) {
            throw std::out_of_range("Index out of bounds for column deletion.");
        }
        for (size_t i = 0; i < this->height_; ++i) {
            this->array_[i].erase(this->array_[i].begin() + index);
        }
        this->width_--;
    } else {
        throw std::invalid_argument("Invalid axis. Use 0 for rows and 1 for columns.");
    }
}

template <class T>
void Matrix<T>::fill(const T& value) {
    for (auto &row : array_) {
        std::fill(row.begin(), row.end(), value);
    }
}

template<class T>
std::vector<T> Matrix<T>::getCol(int col) {
    if (col < 0) {
        col += this->width_;
    }
    std::vector<T> column;
    column.reserve(this->height_);
    for (const auto& row : this->array_) {
        column.push_back(row.at(col));
    }
    return column;
}

template<class T>
std::vector<T> Matrix<T>::getCol(int col) const {
    if (col < 0) {
        col += this->width_;
    }
    std::vector<T> column;
    column.reserve(this->height_);
    for (const auto& row : this->array_) {
        column.push_back(row.at(col));
    }
    return column;
}

template<class T>
void Matrix<T>::insert(int index, const std::vector<T>& newData, int axis) {
    if (axis == 0) {
        if (newData.size() != this->width_ && this->width_ != 0) {
            throw std::invalid_argument("Row size does not match the number of columns.");
        }
        if (index < 0 || index > this->height_) {
            throw std::out_of_range("Index out of bounds for row insertion.");
        }
        this->array_.insert(this->array_.begin() + index, newData);
        this->height_++;
        if (this->width_ == 0) {
            this->width_ = newData.size();
        }
    } else if (axis == 1) {
        if (newData.size() != this->height_) {
            throw std::invalid_argument("Column size does not match the number of rows.");
        }
        if (index < 0 || index > this->width_) {
            throw std::out_of_range("Index out of bounds for column insertion.");
        }
        for (size_t i = 0; i < this->height_; ++i) {
            this->array_[i].insert(this->array_[i].begin() + index, newData[i]);
        }
        this->width_++;
    } else {
        throw std::invalid_argument("Invalid axis. Use 0 for rows and 1 for columns.");
    }
}

template<class T>
void Matrix<T>::pop_back(int axis) {
    if (axis == 0) {
        if (this->height_ == 0) {
            throw std::out_of_range("Cannot pop from an empty matrix.");
        }
        this->array_.pop_back();
        this->height_--;
    } else if (axis == 1) {
        if (this->width_ == 0) {
            throw std::out_of_range("Cannot pop from an empty matrix.");
        }
        for (size_t i = 0; i < this->height_; ++i) {
            this->array_[i].pop_back();
        }
        this->width_--;
    } else {
        throw std::invalid_argument("Invalid axis. Use 0 for rows and 1 for columns.");
    }
}

template <class T>
void Matrix<T>::print(std::ostream &flux) const {
    std::vector<int> maxLength(width_, 0);
    std::stringstream ss;

    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {
            ss << array_[i][j];
            maxLength[j] = std::max(maxLength[j], static_cast<int>(ss.str().size()));
            ss.str(std::string());
        }
    }

    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {
            ss << array_[i][j];
            flux << array_[i][j];
            for (int k = 0; k < maxLength[j] - static_cast<int>(ss.str().size()) + 1; k++) {
                flux << " ";
            }
            ss.str(std::string());
        }
        flux << std::endl;
    }
}

template<class T>
void Matrix<T>::push_back(const std::vector<T>& newData, int axis) {
    if (axis == 0) {
        if (newData.size() != this->width_ && this->width_ != 0) {
            throw std::invalid_argument("Row size does not match the number of columns.");
        }
        this->array_.push_back(newData);
        this->height_++;
        if (this->width_ == 0) {
            this->width_ = newData.size();
        }
    } else if (axis == 1) {
        if (newData.size() != this->height_) {
            throw std::invalid_argument("Column size does not match the number of rows.");
        }
        for (size_t i = 0; i < this->height_; ++i) {
            this->array_[i].push_back(newData[i]);
        }
        this->width_++;
    } else {
        throw std::invalid_argument("Invalid axis. Use 0 for rows and 1 for columns.");
    }
}

template <class T>
void Matrix<T>::put(int h, int w, const T& value){
    if(!(h>=0 && h<height_ && w>=0 && w<width_))
        throw std::invalid_argument("Index out of bounds.");

    array_[h][w] = value;
}

template <class T>
void Matrix<T>::reserve(int rows, int cols) {
    this->array_.reserve(rows);
    for (auto& row : this->array_) {
        row.reserve(cols);
    }
}

template <class T>
void Matrix<T>::resize(int rows) {
    this->array_.resize(rows);
    this->height_ = rows;
}

template <class T>
void Matrix<T>::resize(int rows, int cols) {
    std::vector<std::vector<T>> newarray_(rows, std::vector<T>(cols));

    for (int i = 0; i < std::min(rows, this->height_); i++) {
        for (int j = 0; j < std::min(cols, this->width_); j++) {
            newarray_[i][j] = this->array_[i][j];
        }
    }

    this->array_ = newarray_;
    this->height_ = rows;
    this->width_ = cols;
}


template <class T>
Matrix<T> Matrix<T>::subMat(int startH, int startW, int h, int w) const{
    if(!(startH>=0 && startH+h<=height_ && startW>=0 && startW+w<=width_))
        throw std::invalid_argument("Index out of bounds");

    Matrix<T> result(h,w);
    for (int i=startH ; i<startH+h ; i++){
        for (int j=startW ; j<startW+w ; j++){
            result.array_[i-startH][j-startW] = array_[i][j];
        }
    }
    return result;
}


/*
 * Matrix operations (Mathematical operations)
 */


template <class T>
Matrix<T> Matrix<T>::add(const Matrix& m) const{
    if(!(height_==m.height_ && width_==m.width_))
        throw std::invalid_argument("Matrix dimension must be the same.");

    Matrix result(height_, width_);
    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            result.array_[i][j] = array_[i][j] + m.array_[i][j];
        }
    }

    return result;
}

template <class T>
Matrix<T> Matrix<T>::subtract(const Matrix& m) const{
    if(!(height_==m.height_ && width_==m.width_))
        throw std::invalid_argument("Matrix dimension must be the same.");

    Matrix result(height_, width_);
    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            result.array_[i][j] = array_[i][j] - m.array_[i][j];
        }
    }
    return result;
}

template <class T>
Matrix<T> Matrix<T>::multiply(const T& value) const{
    Matrix result(array_);
    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            result.array_[i][j] *= value;
        }
    }

    return result;
}

template <class T>
Matrix<T> Matrix<T>::multiply(const std::vector<T>& v) const{
    if(this->width_!=v.size())
        throw std::invalid_argument("Vector size must be the same as the core width_.");

    Matrix result(this->height_, this->width_);
    for (int i=0 ; i<this->height_ ; i++){
        for (int j=0 ; j<this->width_ ; j++){
            result.array_[i][j] = this->array_[i][j] * v[j];
        }
    }
    return result;
}

template <class T>
Matrix<T> Matrix<T>::multiply(const Matrix& m) const{
    if(!(height_==m.height_ && width_==m.width_))
        throw std::invalid_argument("Matrix dimension must be the same.");

    Matrix result(height_, width_);

    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            result.array_[i][j] = array_[i][j] * m.array_[i][j];
        }
    }
    return result;
}

template <class T>
Matrix<T> Matrix<T>::divide(const T& value) const{
    Matrix result(array_);
    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            result.array_[i][j] /= value;
        }
    }

    return result;
}

template <class T>
Matrix<T> Matrix<T>::divide(const std::vector<T>& v) const{
    if(this->width_!=v.size())
        throw std::invalid_argument("Vector size must be the same as the core width_.");

    Matrix result(this->height_, this->width_);
    for (int i=0 ; i<this->height_ ; i++){
        for (int j=0 ; j<this->width_ ; j++){
            result.array_[i][j] = this->array_[i][j] / v[j];
        }
    }
    return result;
}

template <class T>
Matrix<T> Matrix<T>::divide(const Matrix& m) const{
    if(!(height_==m.height_ && width_==m.width_))
        throw std::invalid_argument("Matrix dimension must be the same.");

    Matrix result(height_, width_);

    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            result.array_[i][j] = array_[i][j] / m.array_[i][j];
        }
    }
    return result;
}

template <class T>
Matrix<T> Matrix<T>::dot(const Matrix& m) const{
    if(this->width_ != m.height_)
        throw std::invalid_argument("Dot product not compatible.");

    T w=0;
    int mwidth_ = m.width_;

    Matrix<T> result(this->height_, mwidth_);
    for (int i=0 ; i<this->height_ ; i++){
        for (int j=0 ; j<mwidth_ ; j++){
            for (int h=0 ; h<this->width_ ; h++){
                w += this->array_[i][h]*m.array_[h][j];
            }
            result.array_[i][j] = w;
            w=0;
        }
    }

    return result;
}

template <class T>
Matrix<T> Matrix<T>::transpose() const{
    Matrix<T> result(width_, height_);

    for (int i=0 ; i<width_ ; i++){
        for (int j=0 ; j<height_ ; j++){
            result.array_[i][j] = array_[j][i];
        }
    }
    return result;
}

template <class T>
T Matrix<T>::max() const{
    T max = array_[0][0];
    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            if(array_[i][j]>max){
                max = array_[i][j];
            }
        }
    }
    return max;
}

template<typename T>
std::vector<T> Matrix<T>::max(int axis) const {
    std::vector<T> maxElements;
    if (axis == 0) {
        for (const auto& row : array_) {
            maxElements.push_back(*std::max_element(row.begin(), row.end()));
        }
    } else if (axis == 1) {
        for (int j = 0; j < width_; ++j) {
            T colMax = array_[0][j];
            for (int i = 0; i < height_; ++i) {
                if (array_[i][j] > colMax) {
                    colMax = array_[i][j];
                }
            }
            maxElements.push_back(colMax);
        }
    }
    return maxElements;
}

template <class T>
T Matrix<T>::min() const{
    T min = array_[0][0];
    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            if(array_[i][j]<min){
                min = array_[i][j];
            }
        }
    }
    return min;
}


template<typename T>
std::vector<T> Matrix<T>::min(int axis) const {
    std::vector<T> minElements;
    if (axis == 0) {
        for (const auto& row : array_) {
            minElements.push_back(*std::min_element(row.begin(), row.end()));
        }
    } else if (axis == 1) {
        for (int j = 0; j < width_; ++j) {
            T colMin = array_[0][j];
            for (int i = 0; i < height_; ++i) {
                if (array_[i][j] < colMin) {
                    colMin = array_[i][j];
                }
            }
            minElements.push_back(colMin);
        }
    }
    return minElements;
}

template <class T>
T Matrix<T>::sum() const{
    T sum = 0;
    for (int i=0 ; i<this->height_ ; i++){
        for (int j=0 ; j<this->width_ ; j++){
            sum += this->array_[i][j];
        }
    }
    return sum;
}

template <class T>
std::vector<T> Matrix<T>::sum(int axis) const{
    if(axis==0){
        std::vector<T> result(this->height_, 0);
        for (int i=0 ; i<this->height_ ; i++){
            for (int j=0 ; j<this->width_ ; j++){
                result[i] += this->array_[i][j];
            }
        }
        return result;
    }
    else if(axis==1){
        std::vector<T> result(this->width_, 0);
        for (int i=0 ; i<this->width_ ; i++){
            for (int j=0 ; j<this->height_ ; j++){
                result[i] += this->array_[j][i];
            }
        }
        return result;
    }
    else{
        throw std::invalid_argument("Axis must be 0 or 1.");
    }
}

template <class T>
Matrix<T> Matrix<T>::cumuSum(int axis) const{
    if(axis==0){
        Matrix<T> result(this->height_, this->width_);
        for (int i=0 ; i<this->height_ ; i++){
            for (int j=0 ; j<this->width_ ; j++){
                if(i==0){
                    result.array_[i][j] = this->array_[i][j];
                }
                else{
                    result.array_[i][j] = this->array_[i][j] + result.array_[i-1][j];
                }
            }
        }
        return result;
    }
    else if(axis==1){
        Matrix<T> result(this->height_, this->width_);
        for (int i=0 ; i<this->width_ ; i++){
            for (int j=0 ; j<this->height_ ; j++){
                if(i==0){
                    result.array_[j][i] = this->array_[j][i];
                }
                else{
                    result.array_[j][i] = this->array_[j][i] + result.array_[j][i-1];
                }
            }
        }
        return result;
    }
    else{
        throw std::invalid_argument("Axis must be 0 or 1.");
    }
}


/*
 * Operators
 * The operators are overloaded to allow a more natural syntax
 */


template <class T>
bool Matrix<T>::operator==(const Matrix& m){
    if(height_==m.height_ && width_==m.width_){
        for (int i=0 ; i<height_ ; i++){
            for (int j=0 ; j<width_ ; j++){
                if(array_[i][j]!=m.array_[i][j]){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

template <class T>
bool Matrix<T>::operator!=(const Matrix& m){
    return !operator==(m);
}

template <class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& m) {
    if (height_ != m.height_ || width_ != m.width_) {
        throw std::invalid_argument("Matrix dimensions must be the same.");
    }

    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            this->array_[i][j] += m.array_[i][j];
        }
    }

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator-=(const Matrix& m){
    if (height_ != m.height_ || width_ != m.width_) {
        throw std::invalid_argument("Matrix dimensions must be the same.");
    }

    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            this->array_[i][j] -= m.array_[i][j];
        }
    }

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator*=(const T &s){
    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            this->array_[i][j] *= s;
        }
    }

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator*=(const std::vector<T>& v){
    if(this->width_!=v.size())
        throw std::invalid_argument("Vector size must be the same as the core width_.");

    for (int i=0 ; i<this->height_ ; i++){
        for (int j=0 ; j<this->width_ ; j++){
            this->array_[i][j] *= v[j];
        }
    }
    return *this;
}


template <class T>
Matrix<T>& Matrix<T>::operator*=(const Matrix& m){
    if(!(height_==m.height_ && width_==m.width_))
        throw std::invalid_argument("Matrix dimension must be the same.");

    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            this->array_[i][j] *= m.array_[i][j];
        }
    }
    return *this;
}


template <class T>
Matrix<T>& Matrix<T>::operator/=(const T &s){
    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            this->array_[i][j] /= s;
        }
    }

    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator/=(const std::vector<T>& v){
    if(this->width_!=v.size())
        throw std::invalid_argument("Vector size must be the same as the core width_.");

    for (int i=0 ; i<this->height_ ; i++){
        for (int j=0 ; j<this->width_ ; j++){
            this->array_[i][j] /= v[j];
        }
    }
    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator/=(const Matrix& m){
    if(!(height_==m.height_ && width_==m.width_))
        throw std::invalid_argument("Matrix dimension must be the same.");

    for (int i=0 ; i<height_ ; i++){
        for (int j=0 ; j<width_ ; j++){
            this->array_[i][j] /= m.array_[i][j];
        }
    }
    return *this;
}


template<class T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
    if (this != &other) {
        this->array_ = std::move(other.array_);
        this->height_ = other.height_;
        this->width_ = other.width_;
    }
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator=(const std::vector<std::vector<T>>& m) {
    this->array_ = m;
    this->height_ = m.size();
    this->width_ = m[0].size();
    return *this;
}

template<class T>
Matrix<T>& Matrix<T>::operator=(std::vector<std::vector<T>>&& v) noexcept {
    this->height_ = v.size();
    this->width_ = v[0].size();
    this->array_ = std::move(v);

    v.clear();

    return *this;
}


/*
 * Serialization & deserialization
 */

template<class T>
void Matrix<T>::dumpToProto(const std::string& filePath) const {
    protoMatrix protoMat;
    MatrixToProto(*this, protoMat);
    std::ofstream outFile(filePath, std::ios::binary);
    protoMat.SerializeToOstream(&outFile);
    outFile.close();
}

template<class T>
Matrix<T> Matrix<T>::loadFromProto(const std::string& filePath) {
    protoMatrix loadedProtoMat;
    std::ifstream inFile(filePath, std::ios::binary);
    loadedProtoMat.ParseFromIstream(&inFile);
    inFile.close();
    return ProtoToMatrix<T>(loadedProtoMat);
}

template<class T>
void MatrixToProto(const Matrix<T>& matrix, protoMatrix& protoMat) {
    protoMat.set_height(matrix.getHeight());
    protoMat.set_width(matrix.getWidth());
    for (int i = 0; i < matrix.getHeight(); ++i) {
        for (int j = 0; j < matrix.getWidth(); ++j) {
            protoMat.add_data(static_cast<double>(matrix(i, j)));
        }
    }
}

template<class T>
Matrix<T> ProtoToMatrix(const protoMatrix& protoMat) {
    Matrix<T> matrix(protoMat.height(), protoMat.width());
    int index = 0;
    for (int i = 0; i < protoMat.height(); ++i) {
        for (int j = 0; j < protoMat.width(); ++j) {
            matrix.put(i, j, static_cast<T>(protoMat.data(index++)));
        }
    }
    return matrix;
}

// Explicit instantiation of the template class
template class Matrix<int>;
template class Matrix<float>;
template class Matrix<double>;

template void MatrixToProto<int>(const Matrix<int>& matrix, protoMatrix& protoMat);
template Matrix<int> ProtoToMatrix<int>(const protoMatrix& protoMat);
template void MatrixToProto<double>(const Matrix<double>& matrix, protoMatrix& protoMat);
template Matrix<double> ProtoToMatrix<double>(const protoMatrix& protoMat);
