#pragma once
#include <iostream>
#include <string>
#include <complex>
#include <math.h>
#include <algorithm>
#include <vector>
#include <iterator>
#include <iomanip>
#include<conio.h>
#pragma warning (disable : 4996)
using namespace std;

double epsilon = 0.000000001;


template<typename T>
class matrix
{
private:
	vector<vector<T>>  data;
	size_t _columns, _rows;

public:
	matrix() {
		srand(time(0));
		_rows = rand() % 3 + 2;
		_columns = _rows;
		data.resize(_rows);
		for (size_t i = 0; i < _rows; ++i)
		{
			data[i].resize(_columns);
		}
		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				data[i][j] = (rand() % 300) / 10.0;
			}
		}
	}

	matrix(int rows, int columns) {
		if ((rows < 1) or (columns < 1)) throw "Invalid matrix size";

		_columns = columns;
		_rows = rows;

		data.resize(_rows);
		for (size_t i = 0; i < _rows; ++i)
		{
			data[i].resize(_columns);
		}

		cout << "Input your values" << "\n";
		for (size_t i = 0; i < _rows; i++) {
			for (size_t j = 0; j < _columns; j++)
			{
				double _temp;
				cout << "Input value of index [" << i << "][" << j << "]: ";
				cin >> _temp;
				data[i][j] = _temp;
				cout << endl;
			}
		}
	}

	matrix(const matrix<T>& m) = default;

	~matrix() = default;

	auto begin() { return data.begin(); }

	auto end() { return data.end(); }

	//Getters
	size_t get_rows() const { return data.size(); }

	size_t get_columns() const { return data[0].size();}

	//Перегрузка cout
	friend ostream& operator << (ostream& os, const matrix<T>& m)
	{
		for (auto i : m){
			for (auto &j : i){
				os << left << j << '\t';
			}
			os << endl;
		}
		return os;
	}

	// оператор () для чтения/записи элемента матрицы по указанным индексам;
	T& operator()(int i, int j) {
		if ((i < 0 or i >= _rows) or (j < 0 or j >= _columns)) throw "Invalid index";
		return data[i][j];
	}

	T operator()(int i, int j) const {
		if ((i < 0 or i> _rows) and (j < 0 or j > _columns)) throw "Invalid index";
		return data[i][j];
	}


	//операторы сложения и вычитания матриц;
	matrix<T>& operator+=(const matrix<T>& m) {
		if (_rows != m._rows or _columns != m._columns) throw "Size of matrix doesn't equal";

		vector<vector<T>> temp(_rows, vector<T>(_columns));

		for (size_t i = 0; i < _rows; ++i)
		{
			for (size_t j = 0; j < _columns; ++j)
			{
				temp[i][j] = data[i][j] + m.data[i][j];
			}
		}

		data = temp;
		return *this;
	}

	matrix<T> operator+(const matrix<T>& m) const {
		matrix temp(*this);
		temp += m;
		return temp;
	}

	matrix<T>& operator-=(const matrix<T>& m) {
		if (_rows != m._rows or _columns != m._columns) throw "Size of matrix doesn't equal";

		vector<vector<T>> temp(_rows, vector<T>(_columns));

		for (size_t i = 0; i < _rows; ++i)
		{
			for (size_t j = 0; j < _columns; ++j)
			{
				temp[i][j] = data[i][j] - m.data[i][j];
			}
		}

		return *this;
	}

	matrix<T> operator-(const matrix<T>& m) const {
		matrix temp(*this);
		temp -= m;
		return temp;
	}

	//оператор умножения матриц;
	matrix<T>& operator*=(const matrix<T>& m) {

		if (_columns != m._rows) throw "Dimensions do not match!";

		vector<vector<T>> temp(_rows, vector<T>(m._columns));

		for (size_t rows = 0; rows < m._rows; rows++)
		{
			for (size_t col = 0; col < _columns; col++)
			{
				temp[rows][col] = 0;

				for (size_t inner = 0; inner < _columns; inner++)
				{
					temp[rows][col] += data[rows][inner] * m.data[inner][col];
				}
			}
		}

		_columns = m._columns;

		data = temp;

		return *this;

	}

	matrix<T> operator*(const matrix<T>& m) {
		matrix temp(*this);
		temp *= m;
		return temp;
	}

	//оператор умножения матрицы на скаляр(обеспечить коммутативность);

	matrix<T>& operator *= (T n) {
		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				data[i][j] = data[i][j] * n;
			}
		}
		return *this;
	}

	matrix<T> operator*(T n) {
		matrix temp(*this);
		temp *= n;
		return temp;
	}

	friend matrix<T> operator*(T value, const matrix<T>& m) {
		matrix temp(m);
		temp *= value;
		return temp;
	}

	//оператор деления матрицы на скаляр;

	matrix& operator/=(T n) {
		if (n == 0) throw "Division by zero";
		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				data[i][j] = static_cast<T>(data[i][j] / n);
			}
		}
		return *this;
	}

	matrix operator/(T n) {
		matrix temp(*this);
		temp /= n;
		return temp;
	}

	//метод вычисления следа матрицы - сумма членов главной диагонали, при условии, что матрица - квадратичная
	T trace() {
		if (_rows != _columns) throw "The matrix is not square";
		T trace = 0;
		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				if (i == j) trace += data[i][j];
			}
		}
		return trace;
	}

	//Сравнение матриц
	bool operator==(const matrix<T>& m) {
		if (_rows != m._rows or _columns != m._columns) {
			return false;
		}
		else {
			for (size_t i = 0; i < _rows; i++)
			{
				for (size_t j = 0; j < _columns; j++)
				{
					if (abs(data[i][j] - m.data[i][j]) > epsilon) {
						return false;
					}
				}
			}
			return true;
		}
	}

	bool operator!=(const matrix<T>& m) {
		return !(*this == m);
	}

	friend double determinant(const matrix<T>& m, int N) {
		if (m._columns != m._rows) {
			return 0;
		}
		else if (N == 1) {
			return m.data[0][0];
		}
		else if (N == 2) {
			return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
		}
		else if (N >= 3) {
			return m.data[0][0] * m.data[1][1] * m.data[2][2] + m.data[1][0] * m.data[2][1] * m.data[0][2] + m.data[0][1] * m.data[1][2] * m.data[2][0] -
				m.data[0][2] * m.data[1][1] * m.data[2][0] - m.data[2][1] * m.data[1][2] * m.data[0][0] - m.data[1][0] * m.data[0][1] * m.data[2][2];
		}
	}

	friend bool complanarns(const matrix<T>& a) {
		if (a._columns != 3 and a._rows != 3) throw("Invalid size of matrix");
		T  det = determinant(a, a.get_columns());
		if (det == 0) return true;
		else return false;
	}
};

template<typename T>
class matrix<complex<T>>
{
private:
	complex<T>** data;
	size_t _columns, _rows;

public:
	matrix() {
		srand(time(0));
		_rows = rand() % 3 + 2;
		_columns = _rows;
		data = new complex<T> *[_rows];
		for (size_t i = 0; i < _rows; ++i)
		{
			data[i] = new complex<T>[_columns];
		}
		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				data[i][j] = (rand() % 300) / 10.0;
			}
		}
	}

	matrix(int rows, int columns) {
		if ((rows < 1) or (columns < 1)) throw "Invalid matrix size";

		_columns = columns;
		_rows = rows;

		data = new complex<T> *[_rows];
		for (size_t i = 0; i < _rows; ++i)
		{
			data[i] = new complex<T>[_columns];
		}

		cout << "Input your values" << "\n";
		for (size_t i = 0; i < _rows; i++) {
			for (size_t j = 0; j < _columns; j++)
			{
				cout << "Input value of index [" << i << "][" << j << "]: " << endl;
				T temp_real, temp_imag;
				cout << "Real:";
				cin >> temp_real;
				cout << "Imaginary:";
				cin >> temp_imag;
				data[i][j] = complex<T>(temp_real, temp_imag);
				cout << endl;
			}
		}
	}

	matrix(const matrix<complex<T>>& m) {
		data = new complex<T> *[m._rows];
		for (size_t i = 0; i < m._rows; ++i)
		{
			data[i] = new complex<T>[m._columns];
		}

		_rows = m._rows;
		_columns = m._columns;

		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				data[i][j] = m.data[i][j];
			}
		}

	}

	~matrix() {
		for (size_t i = 0; i < _rows; i++)
		{
			delete[] data[i];
		}
		delete[] data;
	}

	//Getters
	size_t get_rows() const { return _rows; }

	size_t get_columns() const { return _columns; }

	//Перегрузка cout
	friend ostream& operator << (ostream& os, const matrix<complex<T>>& m)
	{
		for (size_t i = 0; i < m._rows; ++i)
		{
			for (size_t j = 0; j < m._columns; ++j)
			{
				cout.width(15);
				os << left << m.data[i][j] << '\t';
			}
			os << endl;
		}
		return os;
	}

	// оператор () для чтения/записи элемента матрицы по указанным индексам;
	complex<T>& operator()(int i, int j) {
		if ((i < 0 or i >= _rows) or (j < 0 or j >= _columns)) throw "Invalid index";
		return data[i][j];
	}

	complex<T> operator()(int i, int j) const {
		if ((i < 0 or i> _rows) and (j < 0 or j > _columns)) throw "Invalid index";
		return data[i][j];
	}


	//операторы сложения и вычитания матриц;
	matrix<complex<T>>& operator+=(const matrix<complex<T>>& m) {
		if (_rows != m._rows or _columns != m._columns) throw "Size of matrix doesn't equal";

		for (size_t i = 0; i < m._rows; i++)
		{
			for (size_t j = 0; j < m._columns; j++)
			{
				data[i][j] += m.data[i][j];
			}
		}
		return *this;
	}

	matrix<complex<T>> operator+(const matrix<complex<T>>& m) const {
		matrix temp(*this);
		temp += m;
		return temp;
	}

	matrix<complex<T>>& operator-=(const matrix<complex<T>>& m) {
		if (_rows != m._rows or _columns != m._columns) throw "Size of matrix doesn't equal";
		for (size_t i = 0; i < m._rows; i++)
		{
			for (size_t j = 0; j < m._columns; j++)
			{
				data[i][j] -= m.data[i][j];
			}
		}
		return *this;
	}

	matrix<complex<T>> operator-(const matrix<complex<T>>& m) const {
		matrix temp(*this);
		temp -= m;
		return temp;
	}

	//оператор умножения матриц;
	matrix<complex<T>>& operator*=(const matrix<complex<T>>& m) {

		if (_columns != m._rows) throw "Dimensions do not match!";

		complex<T>** temp = new complex<T> *[_rows];
		for (size_t i = 0; i < _rows; ++i)
		{
			temp[i] = new complex<T>[m._columns];
		}

		for (size_t rows = 0; rows < _rows; rows++)
		{
			for (size_t col = 0; col < m._columns; col++)
			{
				temp[rows][col] = 0;
				for (size_t inner = 0; inner < _columns; inner++)
				{
					temp[rows][col] += data[rows][inner] * m.data[inner][col];
				}
			}
		}

		_columns = m._columns;

		for (size_t i = 0; i < _rows; i++)
		{
			delete[] data[i];
		}
		delete[] data;

		data = temp;
		return *this;

	}

	matrix<complex<T>> operator*(const matrix<complex<T>>& m) {
		matrix temp(*this);
		temp *= m;
		return temp;
	}

	//оператор умножения матрицы на скаляр(обеспечить коммутативность);

	matrix<complex<T>>& operator *= (T n) {
		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				data[i][j] = data[i][j] * n;
			}
		}
		return *this;
	}

	matrix<complex<T>> operator*(T n) {
		matrix temp(*this);
		temp *= n;
		return temp;
	}

	friend matrix<complex<T>> operator*(T value, const matrix<complex<T>>& m) {
		matrix temp(m);
		temp *= value;
		return temp;
	}

	//оператор деления матрицы на скаляр;

	matrix<complex<T>>& operator/=(complex<T> n) {
		if (n.real() == 0 and n.imag() == 0) throw "Division by zero";
		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				data[i][j] = static_cast<complex<T>>(data[i][j] / n);
			}
		}
		return *this;
	}

	matrix operator/(complex<T> n) {
		matrix temp(*this);
		temp /= n;
		return temp;
	}

	//метод вычисления следа матрицы - сумма членов главной диагонали, при условии, что матрица - квадратичная
	complex<T> trace() {
		if (_rows != _columns) throw "The matrix is not square";
		complex<T> trace = 0;
		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				if (i == j) trace += data[i][j];
			}
		}
		return trace;
	}

	//Сравнение матриц
	bool operator==(const matrix<complex<T>>& m) {
		if (_rows != m._rows or _columns != m._columns) {
			return false;
		}
		else {
			for (size_t i = 0; i < _rows; i++)
			{
				for (size_t j = 0; j < _columns; j++)
				{
					if (fabs(data[i][j].real() - m.data[i][j].real()) > epsilon or fabs(data[i][j].imag() - m.data[i][j].imag()) > epsilon) {
						return false;
					}
				}
			}
			return true;
		}
	}


	bool operator!=(const matrix<complex<T>>& m) {
		return !(*this == m);
	}

	friend complex<T> determinant(const matrix<complex<T>>& m, int N) {
		if (m._columns != m._rows) {
			return 0;
		}
		else if (N == 1) {
			return m.data[0][0];
		}
		else if (N == 2) {
			return m.data[0][0] * m.data[1][1] - m.data[0][1] * m.data[1][0];
		}
		else if (N >= 3) {
			return m.data[0][0] * m.data[1][1] * m.data[2][2] + m.data[1][0] * m.data[2][1] * m.data[0][2] + m.data[0][1] * m.data[1][2] * m.data[2][0] -
				m.data[0][2] * m.data[1][1] * m.data[2][0] - m.data[2][1] * m.data[1][2] * m.data[0][0] - m.data[1][0] * m.data[0][1] * m.data[2][2];
		}
	}

	matrix<complex<T>>& operator = (const matrix<complex<T>>& m) {
		for (size_t i = 0; i < _rows; i++)
		{
			delete[] data[i];
		}
		delete[] data;
		data = new complex<T> *[m._rows];
		for (size_t i = 0; i < m._rows; ++i)
		{
			data[i] = new complex<T>[m._columns];
		}

		_rows = m._rows;
		_columns = m._columns;

		for (size_t i = 0; i < _rows; i++)
		{
			for (size_t j = 0; j < _columns; j++)
			{
				data[i][j] = m.data[i][j];
			}
		}
		return *this;
	}

	friend bool complanarns(const matrix<complex<T>>& a) {
		if (a._columns != 3 and a._rows != 3) throw("Invalid size of matrix");
		complex<T> det = determinant(a, a.get_columns());
		if (det.real() == 0 and det.imag() == 0) return true;
		else return false;
	}

};

int main()
{

	cout << "1 - Work with int data" << endl;
	cout << "2 - Work with double data" << endl;
	cout << "3 - Work with complex<double> data" << endl;



	while (true)
	{
		int num_of_menu = getch();

		if (num_of_menu == '1') {
			matrix<int> a;
			matrix<int> b;
			double n = 1;
			while (true) {

				system("cls");
				cout << "first matrix\n\n";
				cout << a << endl;
				cout << "second matrix\n\n";
				cout << b << endl;
				cout << "1 - Create matrix" << endl;
				cout << "2 - Enter value by index" << endl;
				cout << "3 - Matrix addition" << endl;
				cout << "4 - Matrix subtraction" << endl;
				cout << "5 - Matrix multiplication" << endl;
				cout << "6 - Multiplication matrix by scalar" << endl;
				cout << "7 - Division matrix by scalar" << endl;
				cout << "8 - Matrix trace calculation" << endl;
				cout << "9 - Task" << endl;
				cout << "0 - Matrix comparison" << endl;
				cout << "esc - Exit" << endl;
				int z = _getch();
				int l = '3';
				system("cls");
				if (z == '1' || z == '2' || (z > '5' && z <= '9'))
				{
					cout << "1 - Work with first matrix" << endl;
					cout << "2 - Work with second matrix" << endl;
					while (l != '1' && l != '2') l = _getch();
					system("cls");
				}
				if (z == '6' || z == '7')
				{
					cout << "Enter  scalar value" << endl;
					cin >> n;
					system("cls");
				}
				if (z == '1')
				{
					try
					{
						int x = '3';
						cout << "1 - Create random matrix" << endl;
						cout << "2 - Create matrix manually" << endl;
						while (x != '1' && x != '2') x = _getch();

						if (x == '1')
						{
							matrix<int> d;
							if (l == '1') a = d;
							else b = d;
						}
						else
						{
							int rows, columns;
							cout << "Enter number of rows" << endl;
							cin >> rows;
							cout << "Enter number of columns" << endl;
							cin >> columns;
							matrix<int> d(rows, columns);
							if (l == '1') a = d;
							else b = d;
						}
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (_getch()) z = 0;
					}
					system("cls");
				}
				if (z == '2')
				{
					try
					{
						int x = '3';
						int index_row, index_column;
						double value;
						cout << "1 - Change value" << endl;
						cout << "2 - Get value" << endl;
						while (x != '1' && x != '2') x = _getch();
						if (x == '1') {
							cout << "Enter index_row" << endl;
							cin >> index_row;
							cout << "Enter index_column" << endl;
							cin >> index_column;
							cout << "Enter value" << endl;
							cin >> value;

							if (l == '1') a(index_row, index_column) = value;
							else b(index_row, index_column) = value;
						}

						if (x == '2') {
							cout << "Enter index_row" << endl;
							cin >> index_row;
							cout << "Enter index_column" << endl;
							cin >> index_column;

							if (l == '1') value = a(index_row, index_column);
							else  value = b(index_row, index_column);

							cout << "Value of index(" << index_row << "," << index_column << ") = " << value;
							if (_getch()) z = 0;
						}
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (_getch()) z = 0;
					}
					system("cls");
				}
				if (z == '3')
				{
					try
					{
						matrix<int> d = a + b;
						cout << "Matrix addition" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '4')
				{
					try
					{
						matrix<int> d = a - b;
						cout << "Matrix subtraction" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '5')
				{
					try
					{
						matrix<int> d = a * b;
						cout << "Matrix multiplication" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '6')
				{
					matrix<int> d;
					matrix<int> f;
					cout << "Multiplication matrix by scalar" << endl;
					if (l == '1') d = a * n;
					else d = b * n;
					cout << d;
					cout << "\nMultiplication scalar by matrix" << endl;
					if (l == '1') f = n * a;
					else f = n * b;
					cout << f;
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '7')
				{
					try
					{
						matrix<int> d;
						if (l == '1') d = a / n;
						else d = b / n;
						cout << "Division matrix by scalar" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '8')
				{
					int trace_ = 0;
					try
					{
						if (l == '1') trace_ = a.trace();
						else trace_ = b.trace();
						cout << "Matrix trace" << endl;
						cout << trace_;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '9')
				{
					try
					{
						if (l == '1') {
							if (complanarns(a) == 1) cout << "Vectors are coplanar";
							else cout << "Vectors are not coplanar";
						}

						if (l == '2') {
							if (complanarns(b) == 1) cout << "Vectors are coplanar";
							else cout << "Vectors are not coplanar";
						}

					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '0')
				{
					cout << "Enter epsilon" << endl;
					cin >> epsilon;
					if (a == b) cout << "Matrices are the same";
					if (a != b) cout << "Matrices are different";
					if (_getch()) z = 0;
				}
				if (z == 27)
				{
					return 0;
				}
			}
		}

		if (num_of_menu == '2') {

			matrix<double> a;
			matrix<double> b;
			double n = 1;
			while (true) {

				system("cls");
				cout << "first matrix\n\n";
				cout << a << endl;
				cout << "second matrix\n\n";
				cout << b << endl;
				cout << "1 - Create matrix" << endl;
				cout << "2 - Enter value by index" << endl;
				cout << "3 - Matrix addition" << endl;
				cout << "4 - Matrix subtraction" << endl;
				cout << "5 - Matrix multiplication" << endl;
				cout << "6 - Multiplication matrix by scalar" << endl;
				cout << "7 - Division matrix by scalar" << endl;
				cout << "8 - Matrix trace calculation" << endl;
				cout << "9 - Task" << endl;
				cout << "0 - Matrix comparison" << endl;
				cout << "esc - Exit" << endl;
				int z = _getch();
				int l = '3';
				system("cls");
				if (z == '1' || z == '2' || (z > '5' && z <= '9'))
				{
					cout << "1 - Work with first matrix" << endl;
					cout << "2 - Work with second matrix" << endl;
					while (l != '1' && l != '2') l = _getch();
					system("cls");
				}
				if (z == '6' || z == '7')
				{
					cout << "Enter  scalar value" << endl;
					cin >> n;
					system("cls");
				}
				if (z == '1')
				{
					try
					{
						int x = '3';
						cout << "1 - Create random matrix" << endl;
						cout << "2 - Create matrix manually" << endl;
						while (x != '1' && x != '2') x = _getch();

						if (x == '1')
						{
							matrix<double> d;
							if (l == '1') a = d;
							else b = d;
						}
						else
						{
							int rows, columns;
							cout << "Enter number of rows" << endl;
							cin >> rows;
							cout << "Enter number of columns" << endl;
							cin >> columns;
							matrix<double> d(rows, columns);
							if (l == '1') a = d;
							else b = d;
						}
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (_getch()) z = 0;
					}
					system("cls");
				}
				if (z == '2')
				{
					try
					{
						int x = '3';
						int index_row, index_column;
						double value;
						cout << "1 - Change value" << endl;
						cout << "2 - Get value" << endl;
						while (x != '1' && x != '2') x = _getch();
						if (x == '1') {
							cout << "Enter index_row" << endl;
							cin >> index_row;
							cout << "Enter index_column" << endl;
							cin >> index_column;
							cout << "Enter value" << endl;
							cin >> value;

							if (l == '1') a(index_row, index_column) = value;
							else b(index_row, index_column) = value;
						}

						if (x == '2') {
							cout << "Enter index_row" << endl;
							cin >> index_row;
							cout << "Enter index_column" << endl;
							cin >> index_column;

							if (l == '1') value = a(index_row, index_column);
							else  value = b(index_row, index_column);

							cout << "Value of index(" << index_row << "," << index_column << ") = " << value;
							if (_getch()) z = 0;
						}
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (_getch()) z = 0;
					}
					system("cls");
				}
				if (z == '3')
				{
					try
					{
						matrix<double> d = a + b;
						cout << "Matrix addition" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '4')
				{
					try
					{
						matrix<double> d = a - b;
						cout << "Matrix subtraction" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '5')
				{
					try
					{
						matrix<double> d = a * b;
						cout << "Matrix multiplication" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '6')
				{
					matrix<double> d;
					matrix<double> f;
					cout << "Multiplication matrix by scalar" << endl;
					if (l == '1') d = a * n;
					else d = b * n;
					cout << d;
					cout << "\nMultiplication scalar by matrix" << endl;
					if (l == '1') f = n * a;
					else f = n * b;
					cout << f;
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '7')
				{
					try
					{
						matrix<double> d;
						if (l == '1') d = a / n;
						else d = b / n;
						cout << "Division matrix by scalar" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '8')
				{
					double trace_ = 0;
					try
					{
						if (l == '1') trace_ = a.trace();
						else trace_ = b.trace();
						cout << "Matrix trace" << endl;
						cout << trace_;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '9')
				{
					try
					{
						if (l == '1') {
							if (complanarns(a) == 1) cout << "Vectors are coplanar";
							else cout << "Vectors are not coplanar";
						}

						if (l == '2') {
							if (complanarns(b) == 1) cout << "Vectors are coplanar";
							else cout << "Vectors are not coplanar";
						}

					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '0')
				{
					cout << "Enter epsilon" << endl;
					cin >> epsilon;
					if (a == b) cout << "Matrices are the same";
					if (a != b) cout << "Matrices are different";
					if (_getch()) z = 0;
				}
				if (z == 27)
				{
					return 0;
				}
			}
		}

		if (num_of_menu == '3') {

			matrix<complex<double>> a;
			matrix<complex<double>> b;
			double n = 0;
			complex<double> nn = (0, 0);
			while (true) {

				system("cls");
				cout << "first matrix\n\n";
				cout << a << endl;
				cout << "second matrix\n\n";
				cout << b << endl;
				cout << "1 - Create matrix" << endl;
				cout << "2 - Enter value by index" << endl;
				cout << "3 - Matrix addition" << endl;
				cout << "4 - Matrix subtraction" << endl;
				cout << "5 - Matrix multiplication" << endl;
				cout << "6 - Multiplication matrix by scalar" << endl;
				cout << "7 - Division matrix by complex scalar" << endl;
				cout << "8 - Matrix trace calculation" << endl;
				cout << "9 - Task" << endl;
				cout << "0 - Matrix comparison" << endl;
				cout << "esc - Exit" << endl;
				int z = _getch();
				int l = '3';
				system("cls");
				if (z == '1' || z == '2' || (z > '5' && z <= '9'))
				{
					cout << "1 - Work with first matrix" << endl;
					cout << "2 - Work with second matrix" << endl;
					while (l != '1' && l != '2') l = _getch();
					system("cls");
				}
				if (z == '6')
				{
					cout << "Enter  scalar value" << endl;
					cin >> n;
					system("cls");
				}
				if (z == '1')
				{
					try
					{
						int x = '3';
						cout << "1 - Create random matrix" << endl;
						cout << "2 - Create matrix manually" << endl;
						while (x != '1' && x != '2') x = _getch();

						if (x == '1')
						{
							matrix<complex<double>> d;
							if (l == '1') a = d;
							else b = d;
						}
						else
						{
							int rows, columns;
							cout << "Enter number of rows" << endl;
							cin >> rows;
							cout << "Enter number of columns" << endl;
							cin >> columns;
							matrix<complex<double>> d(rows, columns);
							if (l == '1') a = d;
							else b = d;
						}
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (_getch()) z = 0;
					}
					system("cls");
				}
				if (z == '2')
				{
					try
					{
						int x = '3';
						int index_row, index_column;
						complex<double> value;
						cout << "1 - Change value" << endl;
						cout << "2 - Get value" << endl;
						while (x != '1' && x != '2') x = _getch();
						if (x == '1') {
							cout << "Enter index_row" << endl;
							cin >> index_row;
							cout << "Enter index_column" << endl;
							cin >> index_column;
							cout << "Enter value" << endl;
							cin >> value;

							if (l == '1') a(index_row, index_column) = value;
							else b(index_row, index_column) = value;
						}

						if (x == '2') {
							cout << "Enter index_row" << endl;
							cin >> index_row;
							cout << "Enter index_column" << endl;
							cin >> index_column;

							if (l == '1') value = a(index_row, index_column);
							else  value = b(index_row, index_column);

							cout << "Value of index(" << index_row << "," << index_column << ") = " << value;
							if (_getch()) z = 0;
						}
					}
					catch (const char* msg)
					{
						cout << msg << endl;
						if (_getch()) z = 0;
					}
					system("cls");
				}
				if (z == '3')
				{
					try
					{
						matrix<complex<double>> d = a + b;
						cout << "Matrix addition" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '4')
				{
					try
					{
						matrix<complex<double>> d = a - b;
						cout << "Matrix subtraction" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '5')
				{
					try
					{
						matrix<complex<double>> d = a * b;
						cout << "Matrix multiplication" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '6')
				{
					matrix<complex<double>> d;
					matrix<complex<double>> f;
					cout << "Multiplication matrix by scalar" << endl;
					if (l == '1') d = a * n;
					else d = b * n;
					cout << d;
					cout << "\nMultiplication scalar by matrix" << endl;
					if (l == '1') f = n * a;
					else f = n * b;
					cout << f;
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '7')
				{
					try
					{
						matrix<complex<double>> d;
						double temp_real, temp_imag;
						cout << "Enter the value: " << endl;
						cout << "Real: " << endl;
						cin >> temp_real;
						cout << "Imag: " << endl;
						cin >> temp_imag;
						nn = complex<double>(temp_real, temp_imag);
						if (l == '1') d = a / nn;
						else d = b / nn;
						cout << "Division matrix by scalar" << endl;
						cout << d;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '8')
				{
					complex<double> trace_ = 0;
					try
					{
						if (l == '1') trace_ = a.trace();
						else trace_ = b.trace();
						cout << "Matrix trace" << endl;
						cout << trace_;
					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '9')
				{
					try
					{
						if (l == '1') {
							if (complanarns(a) == 1) cout << "vectors are coplanar";
							else cout << "vectors are not coplanar" << endl;
						}

						if (l == '2') {
							if (complanarns(b) == 1) cout << "vectors are coplanar";
							else cout << "vectors are not coplanar" << endl;
						}

					}
					catch (const char* msg)
					{
						cout << msg << endl;
					}
					if (_getch()) z = 0;
					system("cls");
				}
				if (z == '0')
				{
					cout << "Enter epsilon" << endl;
					cin >> epsilon;
					if (a == b) cout << "Matrices are the same";
					if (a != b) cout << "Matrices are different";
					if (_getch()) z = 0;
				}
				if (z == 27)
				{
					return 0;
				}
			}
		}
	}
}