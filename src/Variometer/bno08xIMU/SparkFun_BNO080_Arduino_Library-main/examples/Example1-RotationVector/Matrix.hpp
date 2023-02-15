
#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "Assert.hpp"
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cmath>
#include <cstring>

template <uint32_t ROWS, uint32_t COLS, typename T = double>
class Matrix {
public:
  template <typename Item>
  class Col {
    friend Matrix;

    Item *items[ROWS];

    void insert(size_t n, Item &value)
    {
      items[n] = &value;
    }

  public:
    operator Matrix<ROWS, 1, T>() const
    {
      Matrix<ROWS, 1, T> m{};
      for (size_t i = 0; i < m.rows(); ++i) {
        m[i][0] = *items[i];
      }

      return m;
    }

    Item &operator[](size_t i)
    {
      Platform::Assert::Assert(i < ROWS);
      return *items[i];
    }

    Item operator[](size_t i) const
    {
      Platform::Assert::Assert(i < ROWS);
      return *items[i];
    }

    Col &operator=(const Matrix<ROWS, 1> &rhs)
    {
      for (size_t i = 0; i < ROWS; ++i)
        this->operator[](i) = rhs[i][0];
      return *this;
    }

    Matrix<ROWS, 1> operator-(const Matrix<ROWS, 1> &rhs)
    {
      auto &lhs{*this};
      Matrix<ROWS, 1> result{};

      for (size_t i = 0; i < ROWS; ++i) {
        result[i][0] = lhs[i] - rhs[i][0];
      }

      return result;
    }

    uint32_t size()
    {
      return ROWS;
    }
  };

  template <typename Item>
  class Row {
    Item (&items)[COLS];

  public:
    Row(Item (&values)[COLS]) : items{values}
    {
    }

    Item &operator[](size_t i)
    {
      Platform::Assert::Assert(i < COLS);
      return items[i];
    }

    Item operator[](size_t i) const
    {
      Platform::Assert::Assert(i < COLS);
      return items[i];
    }

    operator Matrix<1, COLS, T>() const
    {
      Matrix<1, COLS, T> m{};
      for (size_t i = 0; i < m.cols(); ++i) {
        m[0][i] = items[i];
      }

      return m;
    }

    T operator*(const Matrix<COLS, 1> &rhs)
    {
      auto &lhs{*this};
      T result{};
      for (size_t i = 0; i < COLS; ++i) {
        result += lhs[i] * rhs[i][0];
      }
      return result;
    }

    uint32_t size()
    {
      return COLS;
    }
  };

private:
  T items[ROWS][COLS];

public:
  Matrix()
  {
    for (size_t r = 0; r < ROWS; ++r) {
      for (size_t c = 0; c < COLS; ++c) {
        items[r][c] = static_cast<T>(0);
      }
    }
  }

  Matrix(const T (&values)[ROWS][COLS])
  {
    for (size_t r = 0; r < ROWS; ++r) {
      for (size_t c = 0; c < COLS; ++c) {
        items[r][c] = values[r][c];
      }
    }
  }

  Row<T> operator[](size_t i)
  {
    Platform::Assert::Assert(i < ROWS);
    return Row<T>{items[i]};
  }

  Row<const T> operator[](size_t i) const
  {
    Platform::Assert::Assert(i < ROWS);
    return Row<const T>{items[i]};
  }

  Matrix operator+(const Matrix &rhs) const
  {
    Matrix result{};

    auto &lhs{*this};

    for (size_t r = 0; r < ROWS; ++r) {
      for (size_t c = 0; c < COLS; ++c) {
        result[r][c] = lhs[r][c] + rhs[r][c];
      }
    }

    return result;
  }

  Matrix operator-(const Matrix &rhs) const
  {
    Matrix result{};

    auto &lhs{*this};

    for (size_t r = 0; r < ROWS; ++r) {
      for (size_t c = 0; c < COLS; ++c) {
        result[r][c] = lhs[r][c] - rhs[r][c];
      }
    }

    return result;
  }

  Matrix operator*(T scalar) const
  {
    Matrix result(*this);

    for (size_t r = 0; r < ROWS; ++r) {
      for (size_t c = 0; c < COLS; ++c) {
        result[r][c] *= scalar;
      }
    }

    return result;
  }

  template <uint32_t C>
  Matrix<ROWS, C, T> operator*(const Matrix<COLS, C, T> &rhs) const
  {
    Matrix<ROWS, C, T> result{};
    auto &lhs{*this};

    for (size_t r = 0; r < result.rows(); ++r) {
      for (size_t c = 0; c < result.cols(); ++c) {
        T product{0};
        for (size_t x = 0; x < lhs.cols(); ++x) {
          product += lhs[r][x] * rhs[x][c];
        }

        result[r][c] = product;
      }
    }

    return result;
  }

  bool operator==(const Matrix &rhs) const
  {
    auto &lhs{*this};
    return (0 == memcmp(lhs.items, rhs.items, sizeof(lhs.items)));
  }

  template <uint32_t R = ROWS, uint32_t C = COLS>
  typename std::enable_if<(R == C) && (1 < R), T>::type det() const
  {
    auto &self{*this};
    T determinant{0};
    int sign{1};

    for (size_t r = 0; r < ROWS; ++r) {
      determinant += sign * self[0][r] * cof(0, r).det();
      sign = -sign;
    }

    return determinant;
  }

  template <uint32_t R = ROWS, uint32_t C = COLS>
  typename std::enable_if<(R == C) && (R == 1), T>::type det() const
  {
    return items[0][0];
  }

  Matrix<ROWS - 1, COLS - 1, T> cof(size_t row, size_t column) const
  {
    Matrix<ROWS - 1, COLS - 1, T> cofactor{};
    auto &self{*this};

    size_t ri{0};
    size_t ci{0};
    for (size_t r = 0; r < ROWS; ++r) {
      for (size_t c = 0; c < COLS; ++c) {
        if ((r != row) && (c != column)) {
          cofactor[ri][ci] = self[r][c];
          if (ci < (cofactor.cols() - 1)) {
            ++ci;
          } else {
            ci = 0;
            ++ri;
          }
        }
      }
    }

    return cofactor;
  }

  template <uint32_t R = ROWS, uint32_t C = COLS>
  typename std::enable_if<(1 == R) && (1 == C), Matrix<C, R, T>>::type
  adj() const
  {
    Matrix<C, R, T> adjugate{};
    adjugate[0][0] = 1;
    return adjugate;
  }

  template <uint32_t R = ROWS, uint32_t C = COLS>
  typename std::enable_if<(R == C) && (1 < R), Matrix<C, R, T>>::type adj() const
  {
    Matrix<C, R, T> adjugate{};

    for (size_t r = 0; r < ROWS; ++r) {
      for (size_t c = 0; c < COLS; ++c) {
        auto cofactor = cof(r, c);
        int sign = ((r + c) % 2 == 0) ? 1 : -1;
        adjugate[c][r] = (sign)*cofactor.det();
      }
    }

    return adjugate;
  }

  template <uint32_t R = ROWS, uint32_t C = COLS>
  typename std::enable_if<(R == C), Matrix<R, C, T>>::type inv() const
  {
    auto determinant = det();
    Platform::Assert::Assert(0 != determinant);

    auto adjugate = adj();
    Matrix inverse{};

    for (size_t r = 0; r < inverse.rows(); ++r)
      for (size_t c = 0; c < inverse.cols(); ++c)
        inverse[r][c] = adjugate[r][c] / determinant;

    return inverse;
  }

  Matrix<COLS, ROWS, T> transpose() const
  {
    Matrix<COLS, ROWS, T> tr{};
    auto &self{*this};

    for (size_t r = 0; r < tr.rows(); ++r) {
      for (size_t c = 0; c < tr.cols(); ++c) {
        tr[r][c] = self[c][r];
      }
    }

    return tr;
  }

  template <uint32_t R = ROWS, uint32_t C = COLS>
  typename std::enable_if<(R == C), Matrix<R, C, T>>::type sqrt() const
  {
    Matrix<R, C, T> lower{};
    auto &self{*this};

    for (size_t r = 0; r < rows(); ++r) {
      // Off-diagonal elements
      for (size_t k = 0; k < r; ++k) {
        auto value = self[r][k];
        for (size_t j = 0; j < k; ++j)
          value -= lower[r][j] * lower[k][j];
        lower[r][k] = value / lower[k][k];
      }

      // Diagonal element
      auto value = self[r][r];
      for (size_t j = 0; j < r; ++j)
        value -= lower[r][j] * lower[r][j];
      lower[r][r] = std::sqrt(value);
    }

    return lower;
  }

  Matrix &fill(T v)
  {
    auto &self{*this};
    for (size_t r = 0; r < rows(); ++r) {
      for (size_t c = 0; c < cols(); ++c) {
        self[r][c] = v;
      }
    }

    return (*this);
  }

  Row<T> row(size_t i)
  {
    return this->operator[](i);
  }

  Row<T> row(size_t i) const
  {
    return this->operator[](i);
  }

  Col<T> col(size_t i)
  {
    Col<T> col{};
    for (size_t r = 0; r < ROWS; ++r) {
      col.insert(r, items[r][i]);
    }
    return col;
  }

  static uint32_t rows()
  {
    return ROWS;
  }

  static uint32_t cols()
  {
    return COLS;
  }
};

template <uint32_t ROWS, uint32_t COLS, typename T>
Matrix<ROWS, COLS, T> operator*(T scalar, const Matrix<ROWS, COLS, T> &rhs)
{
  return rhs * scalar;
}

#if defined(SIMULATOR)

#include <iostream>

template <uint32_t R, uint32_t C, typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<R, C, T> &m)
{
  os << std::endl << "[";

  for (size_t i = 0; i < m.rows(); ++i) {
    if (0 < i)
      os << std::endl << " ";

    for (size_t j = 0; j < m.cols(); ++j) {
      os << m[i][j];
      if (j < (m.cols() - 1))
        os << "\t";
    }
  }

  os << "]" << std::endl;

  return os;
}

#endif

#endif
