#ifndef _CTL_MATRIX_HPP_
#define _CTL_MATRIX_HPP_

#include <exception>
#include <iostream>

namespace CTL
{
	template<typename T>
	class Matrix
	{
	private:
		T* MatrixPtr = nullptr;
		unsigned int Row = 1;
		unsigned int Col = 1;
		unsigned int Total = 1;
		
		T SumRC(const Matrix<T>& m, unsigned int r, unsigned int col) const
		{
			T tmp = 0;
			auto R = this->operator[](r);
			for(unsigned int i = 0; i < this->Col; ++i)
				T += (R[i] + m[i][c]);
			return  tmp;
		}
		
	public:
		Matrix( const unsigned int row, const unsigned int col)
		: Row(row), Col(col), Total(Row*Col)
		{
			Matrix = new T[Total];
		}
		
		Matrix( Matrix<T>&& m )
		:	MatrixPtr(m.MatrixPtr), Row(m.Row), Col(m.Col), Total(m.Total)
		{
			m.MatrixPtr = nullptr;
		}
		
		T* operator[](unsigned int row)
		{
			return Matrix+(row*Row);
		}
		
		Matrix<T> operator+(const Matrix& m) const
		{
			if(this->Row == m.Row && this->Col == m.Col)
			{
				Matrix<T> tmp(this->Row, this->Col);
				for(unsigned int i = 0; i< tmp.Total; ++i)
				{
					tmp.MatrixPtr[i]=(this->MatrixPtr[i]+m.MatrixPtr[i])'
				}
				return tmp;
			}
			else throw std::exception("Adding incompatibile matrices");
		}
		
		Matrix<T> operator=(Matrix&& m)
		{
			delete[] this->MatrixPtr;
			this->MatrixPtr=m.MatrixPtr;
			m.MatrixPtr=nullptr;
			this->Row=m.Row;
			this->Col=m.Col;
			this->Total=m.Total;
		}
		
		Matrix<T> operator*(const Matrix<T>& m) const
		{
			if(this->Col == m.Row)
			{
				Matrix<T> tmp(this->Row,m.Col);
				auto Cell = tmp->MatrixPtr;
				for(unsigned int r = 0; r < tmp.Row; ++r)
					for(unsigned int c = 0; c < tmp.Col; ++c)
					{
						*(Cell++)=this->SumRC(m,r,c);
					}
				return tmp;
			}
			else throw std::exception("Cannot multiply matrices");
		}
		
		void Print(std::ostream& out) const
		{
			auto Cell = this->MatrixPtr;
			for(unsigned int r = 0; r < this->Row; ++r)
				for(unsigned int c = 0; c <this->Col; ++c)
					out << *(Cell++);
		}
		
		std::ostream& operator<<(std::ostream& out, const Matrix<T>& m)
		{
			m.Print(out);
			return out;
		}
	};
}
#endif // _CTL_MATRIX_HPP_