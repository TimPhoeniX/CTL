#ifndef _CTL_MATRIX_HPP_
#define _CTL_MATRIX_HPP_

#include <stdexcept>
#include <iostream>

namespace CTL
{
	template<typename T>
	class Matrix
	{
	private:
		unsigned int Row = 1;
		unsigned int Col = 1;
		unsigned int Total = 1;
		T* Val = nullptr;
		
		T SumRC(const Matrix<T>& m, unsigned int r, unsigned int c) const
		{
			T tmp = 0;
			auto R = this->operator[](r);
			for(unsigned int i = 0; i < this->Col; ++i)
			{
				//std::cerr << R[i] << ' ' << m[i][c] << '\n';
				tmp += (R[i] * m[i][c]);
			}
			//std::cout << tmp << std::endl;
			return  tmp;
		}
		
	public:
		Matrix( const unsigned int row, const unsigned int col)
		: Row(row), Col(col), Total(Row*Col), Val(new T[this->Total])
		{
		}
		
		Matrix( const Matrix<T>& m )
		: Row(m.Row), Col(m.Col), Total(m.Total), Val(new T[this->Total])
		{
			auto tp = this->Val;
			auto mp = m.Val;
			for(unsigned int i = 0; i < this->Total; ++i)
				*(tp++) = *(mp++);
		}
		
		Matrix( Matrix<T>&& m )
		: Row(m.Row), Col(m.Col), Total(m.Total), Val(m.Val)
		{
			m.Val = nullptr;
		}
		
		Matrix( std::istream& in = std::cin )
		{
			in >> this->Row >> this->Col;
			this->Total = this->Row*this->Col;
			this->Val = new T[this->Total];
			auto ptr = this->Val;
			for(unsigned int i = 0; i < this->Total; ++i)
			{
				in >> *(ptr++);
			}
		}
		
		Matrix<T> operator=(Matrix&& m)
		{
			delete[] this->Val;
			this->Val=m.Val;
			m.Val=nullptr;
			this->Row=m.Row;
			this->Col=m.Col;
			this->Total=m.Total;
		}
		
		~Matrix()
		{
			delete[] this->Val;
		}
		
		T* operator[](unsigned int row)
		{
			return this->Val+(row*Row);
		}
		
		const T* operator[](unsigned int row) const
		{
			return this->Val+(row*Row);
		}
		
		Matrix<T> operator+(const Matrix& m) const
		{
			if(this->Row == m.Row && this->Col == m.Col)
			{
				Matrix<T> tmp(this->Row, this->Col);
				auto p1 = tmp.Val;
				auto p2 = this->Val;
				auto p3 = m.Val;
				for(unsigned int i = 0; i< tmp.Total; ++i)
				{
					*(p1++)=(*(p2++)+*(p3++));
				}
				return tmp;
			}
			else throw std::range_error("Adding incompatibile matrices");
		}
		
		Matrix<T> operator*(const Matrix<T>& m) const
		{
			if(this->Col == m.Row)
			{
				Matrix<T> tmp(this->Row,m.Col);
				auto Cell = tmp.Val;
				for(unsigned int r = 0; r < tmp.Row; ++r)
				{
					for(unsigned int c = 0; c < tmp.Col; ++c)
					{
						std::cout << r << ' ' << c << std::endl;
						*(Cell++)=this->SumRC(m,r,c);
					}
				}
				return tmp;
			}
			else throw std::range_error("Multiplying incompatibile matrices");
		}
		
		void Print(std::ostream& out = std::cout) const
		{
			auto Cell = this->Val;
			for(unsigned int r = 0; r < this->Row; ++r)
			{
				for(unsigned int c = 0; c < this->Col; ++c)
				{
					out << *(Cell++) << ' ';
				}
				out << '\n';
			}
		}
		
		friend std::ostream& operator<<(std::ostream& out, const Matrix<T>& m)
		{
			m.Print(out);
			return out;
		}
	};
}
#endif // _CTL_MATRIX_HPP_