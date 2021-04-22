/*
MIT License

Copyright (c) 2019 Michael Hancock

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#pragma once
#include <cmath>
template <class T >
class matrix
{
 private:
  T* data;
  int sizeR;
  int sizeC;

 public:
  matrix()
  {
	sizeR = 0;
	sizeC = 0;
  }

  matrix(int rows, int columns)
  {
	sizeR = rows;
	sizeC = columns;
	data = new T[sizeR * sizeC];
  }

  matrix(int rows, int columns, T initialValue)
  {
	sizeR = rows;
	sizeC = columns;
	data = new T[sizeR * sizeC];

	for (int i = 0; i < sizeR * sizeC; i++)
	{
	  data[i] = initialValue;
	}
  }

  matrix(const matrix& mat)
  {
	sizeR = mat.sizeR;
	sizeC = mat.sizeC;
	data = new T[sizeR * sizeC];

	for (int i = 0; i < sizeR * sizeC; i++)
	{
	  data[i] = mat.data[i];
	}
  }

  int rows() const
  {
	return sizeR;
  }

  int columns() const
  {
	return sizeC;
  }

  void set(int m, int n, T value)
  {
	if (m >= sizeR || n >= sizeC)
	{
	  throw std::out_of_range("Matix position arguments are out of range");
	}

	data[(m * sizeC) + n] = value;
  }

  void setBlock(int row1, int col1, int row2, int col2, T value)
  {
	for (int i = row1; i <= row2; i++)
	{
	  for (int j = col1; j <= col2; j++)
	  {
		set(i, j, value);
	  }
	}
  }

  void setBlock(int row1, int col1, matrix& mat)
  {
	int indexRow = 0;
	int indexCol = 0;
	int row2 = row1 + mat.sizeR - 1;
	int col2 = col1 + mat.sizeC - 1;

	for (int i = row1; i <= row2; i++)
	{
	  for (int j = col1; j <= col2; j++)
	  {
		set(i, j, mat.get(indexRow, indexCol));
		indexCol++;
	  }

	  indexCol = 0;
	  indexRow++;
	}
  }

  T get(int m, int n) const
  {
	if (m >= sizeR || n >= sizeC)
	{
	  throw std::out_of_range("Matix position arguments are out of range");
	}

	return data[(m * sizeC) + n];
  }

  matrix getBlock(int row1, int col1, int row2, int col2) const
  {
	if (row1 >= row2 || col1 >= col2)
	{
	  throw std::invalid_argument("Provided arguments are invalid. row1 < row2 and col1 < col2");
	}

	if (row1 < 0 || col1 < 0 || row2 >= sizeR || col2 >= sizeC)
	{
	  throw std::out_of_range("Matix position arguments are out of range");
	}

	matrix result = matrix(row2 - row1 + 1, col2 - col1 + 1);
	int indexRow = 0;
	int indexCol = 0;

	for (int i = row1; i <= row2; i++)
	{
	  for (int j = col1; j <= col2; j++)
	  {
		result.set(indexRow, indexCol, get(i, j));
		indexCol++;
	  }

	  indexRow++;
	  indexCol = 0;
	}

	return result;
  }

  void addRow()
  {
	const int sizeBeforeAdd = sizeR * sizeC;
	const int sizeAfterAdd = (sizeR + 1) * sizeC;
	sizeR++;

	T * dataCopy = new T[sizeAfterAdd];
	memcpy(dataCopy, data, sizeof(data[0]));

	delete[] data;
	data = dataCopy;
  }

  void addColumn()
  {
	const int sizeBeforeAdd = sizeR * sizeC;
	const int sizeAfterAdd = sizeR * (sizeC + 1);
	sizeC++;

	T * dataCopy = new T[sizeAfterAdd];
	memcpy(dataCopy, data, sizeof(data[0]));

	delete[] data;
	data = dataCopy;
  }

  void reverse()
  {
	const int matrixSize = sizeR * sizeC;
	const int numberOfSwapsRequired = matrixSize % 2 == 0 ? (int)matrixSize / 2 : (int)floor(matrixSize / 2);

	for (int i = 0; i < numberOfSwapsRequired; i++)
	{
	  const T toSwap = data[i];
	  const int upperSwapIndex = (matrixSize - 1) - i;

	  data[i] = data[upperSwapIndex];
	  data[upperSwapIndex] = toSwap;
	}
  }

  void clear()
  {
	sizeR = 0;
	sizeC = 0;

	delete[] data;
	data = new T[0];
  }

  T operator()(int m, int n) const
  {
	return get(m, n);
  }

  matrix operator=(const matrix& mat)
  {
	if (mat.sizeR == sizeR && mat.sizeC == sizeC)
	{
	  for (int i = 0; i < sizeR * sizeC; i++)
	  {
		data[i] = mat.data[i];
	  }

	  return *this;
	}

	delete[] data;

	sizeR = mat.sizeR;
	sizeC = mat.sizeC;
	data = new T[sizeR * sizeC];

	for (int i = 0; i < sizeR * sizeC; i++)
	{
	  data[i] = mat.data[i];
	}

	return *this;
  }

  bool operator==(const matrix& mat) const
  {
	if (sizeR != mat.sizeR || sizeC != mat.sizeC)
	{
	  return false;
	}

	for (int i = 0; i < sizeR * sizeC; i++)
	{
	  if (data[i] != mat.data[i])
	  {
		return false;
	  }
	}

	return true;
  }

  ~matrix()
  {
	delete[] data;
  }
};