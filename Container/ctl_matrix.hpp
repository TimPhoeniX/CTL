/**
 * Conceptual Template Library by Piotr Grudzień
 * Numerical Matrix 
 */
#ifndef _CTL_MATRIX_HPP_
#define _CTL_MATRIX_HPP_

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>

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
				tmp += (R[i] * m[i][c]);
			}
			return  tmp;
		}
		
	public:
		constexpr T Epsilon(unsigned int e) const
		{
			return e==0 ? 1 : this->Epsilon(e-1)*T(0.1);
		}
		
		Matrix( const unsigned int dim)
		: Row(dim), Col(dim), Total(Row*Col), Val(new T[this->Total]{})
		{
			for(unsigned int i = 0; i < dim; ++i)
				this->operator[](i)[i] = T(1);
		}
		
		Matrix( const unsigned int r, const unsigned int c)
		: Row(r), Col(c), Total(Row*Col), Val(new T[Total]{})
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
		
		Matrix<T>& operator=(Matrix<T>&& m)
		{
			delete[] this->Val;
			this->Val=m.Val;
			m.Val=nullptr;
			this->Row=m.Row;
			this->Col=m.Col;
			this->Total=m.Total;
			return *this;
		}
		
		Matrix<T>& operator=(const Matrix<T>& m)
		{
			if(this->Row != m.Row || this->Col != m.Col)
			{
				this->Row = m.Row;
				this->Col = m.Col;
				this->Total = m.Total;
				delete[] this->Val;
				this->Val = new T[Total];
			}
			auto end = this->Val + this->Total;
			auto tp = this->Val;
			auto mp = m.Val;
			while(tp!=end)
			{
				*(tp++)=*(mp++);
			}
			return *this;
		}
		
		~Matrix()
		{
			delete[] this->Val;
		}
		
		T* operator[](unsigned int row)
		{
			return this->Val+(row*this->Col);
		}
		
		const T* operator[](unsigned int row) const
		{
			return this->Val+(row*this->Col);
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
		
		Matrix<T> operator-(const Matrix& m) const
		{
			if(this->Row == m.Row && this->Col == m.Col)
			{
				Matrix<T> tmp(this->Row, this->Col);
				auto p1 = tmp.Val;
				auto p2 = this->Val;
				auto p3 = m.Val;
				for(unsigned int i = 0; i< tmp.Total; ++i)
				{
					*(p1++)=(*(p2++)-*(p3++));
				}
				return tmp;
			}
			else throw std::range_error("Adding incompatibile matrices");
		}
		
		Matrix<T>& operator+=(const Matrix& m)
		{
			if(this->Row == m.Row && this->Col == m.Col)
			{
				auto p1 = this->Val;
				auto p2 = m.Val;
				for(unsigned int i = 0; i< this->Total; ++i)
				{
					*(p1++)+=*(p2++);
				}
				return *this;
			}
			else throw std::range_error("Adding incompatibile matrices");
		}
		
		Matrix<T>& operator-=(const Matrix& m)
		{
			if(this->Row == m.Row && this->Col == m.Col)
			{
				auto p1 = this->Val;
				auto p2 = m.Val;
				for(unsigned int i = 0; i< this->Total; ++i)
				{
					*(p1++)-=*(p2++);
				}
				return *this;
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
						*(Cell++)=this->SumRC(m,r,c);
					}
				}
				return tmp;
			}
			else throw std::range_error("Multiplying incompatibile matrices");
		}
		
		explicit operator T() const
		{
			return *this->Val;
		}
		
		Matrix<T>& operator*=(T val)
		{
			for(unsigned int i = 0; i < this->Total; ++i)
			{
				this->Val[i]*=val;
			}
			return *this;
		}
		
		Matrix<T>& operator/=(T val)
		{
			for(unsigned int i = 0; i < this->Total; ++i)
			{
				this->Val[i]/=val;
			}
			return *this;
		}
		
		Matrix<T> Transpose() const
		{
			Matrix<T> tmp(this->Col,this->Row);
			auto cell = this->Val;
			for(unsigned int r = 0; r < this->Row; ++r)
			{
				for(unsigned int c = 0; c < this->Col; ++c)
				{
					tmp[c][r]=*(cell++);
				}
			}
			return tmp;
		}
		
		void Print(std::ostream& out = std::cout) const
		{
			auto Cell = this->Val;
			out << std::setprecision(6) << std::fixed;
			for(unsigned int r = 0; r < this->Row; ++r)
			{
				for(unsigned int c = 0; c < this->Col; ++c)
				{
					out << *(Cell++) << '\t';
				}
				out << '\n';
			}
		}
		
		void PrintDiagonal(std::ostream& out = std::cout) const
		{
			for(unsigned int i = 0; i < this->Col; ++i)
			{
				out.precision(6);
				out << this->operator[](i)[i] << '\t';
			}
		}
		
		friend std::ostream& operator<<(std::ostream& out, const Matrix<T>& m)
		{
			m.Print(out);
			return out;
		}
		
		void SwapRows(unsigned int a, unsigned int b)
		{
			auto ra = this->operator[](a);
			auto rb = this->operator[](b);
			for(unsigned int i = 0; i < this->Col; ++i)
			{
				std::swap(*(ra++),*(rb++));
			}
		}
		
		Matrix<T> GivensRotation(const unsigned int i, const unsigned int j, const unsigned int c) const
		{
			Matrix<T> givens(this->Row);
			auto ai = this->operator[](i)[c];
			auto aj = this->operator[](j)[c];
			//Speeding up a little bit
			auto l = 1./std::sqrt(ai*ai + aj*aj);
			givens[i][i]=( ai*l );
			givens[j][j]=( ai*l );
			givens[i][j]=( aj*l );
			givens[j][i]=(-aj*l );
			return givens;
		}
		
		//Tranposes in place;
		Matrix<T>& InplaceTransposeGivensRotation(const unsigned int i, const unsigned int j)
		{
			std::swap(this->operator[](i)[j],this->operator[](j)[i]);
			return *this;
		}
		
		bool CheckQRConvergnece() const
		{
			for(unsigned int i = 0; i < this->Col-1; ++i)
			{
				if(this->operator[](i+1)[i] > this->Epsilon(15)) return false;
			}
			return true;
		}
		
		void QRAlgorithmSymmetricalTridiagonal()
		{
			unsigned int iterationLimit = 6400;
			while(!this->CheckQRConvergnece() && iterationLimit--)
			{
				for(unsigned int i = 0; i < this->Col - 1; ++i)
				{
					auto givens = this->GivensRotation(i,i+1,i);
					(*this) = givens*(*this); 
					givens.InplaceTransposeGivensRotation(i,i+1);
					*this = this->operator*(givens);
				}
			}
		}
		
		//Unsafe
		void FillWith( const unsigned int rbegin, const unsigned int cbegin, const Matrix<T>& m )
		{
			for(unsigned int i = 0; i < m.Row; ++i)
				for(unsigned int j = 0; j < m.Col; ++j)
				{
					this->operator[](rbegin+i)[cbegin+j]=m[i][j];
				}
		}
		
		void FillWith( std::istream& in = std::cin )
		{
			auto ptr = this->Val;
			for(unsigned int i = 0; i < this->Total; ++i)
			{
				in >> *(ptr++);
			}
		}
		
		//Unsafe
		T VectorNorm() const
		{
			T norm = 0;
			if(this->Col == 1 || this->Col == 1)
			{
				auto cell = this->Val;
				for( unsigned int i = 0; i < this->Total; ++i)
				{
					norm+=( (*cell)*(*cell) );
					++cell;
				}
				norm = sqrt(norm);
			}
			return norm;
		}
		
		CTL::Matrix<T> TridiagonalizingHouseholder(const unsigned int c) const
		{
			CTL::Matrix<T> vec(this->Row-c-1,1);
			for( unsigned int i = 0; i < vec.Row; ++i)
			{
				vec[i][0]=this->operator[](i+c+1)[c];
			}
			vec[0][0]-=(std::signbit(vec[0][0])?(-1):(1))*vec.VectorNorm();
			auto vet = vec.Transpose();
			Matrix<T> subm(vec.Row);
			auto norm = 2/T(vet*vec);
			subm-=( (vec*vet)*=norm);
			CTL::Matrix<T> householder(this->Col);
			householder.FillWith(c+1,c+1,subm);
			return householder;
		}
		
		void TridiagonalizeSymmetricHouseholder()
		{
			const unsigned int count = this->Row-2;
			for(unsigned int i = 0; i < count; ++i)
			{
				auto h = this->TridiagonalizingHouseholder(i);
				*this = h*(*this)*h;
			}
		}
		
		Matrix<T>& InplacePermutateRows(unsigned int* permutation)
		{
			Matrix<T> pMatrix(this->Row,this->Col);
			for(unsigned int i = 0; i < this->Row; ++i)
			{
				pMatrix[i][permutation[i]]=1;
			}
			this->operator=(pMatrix*(*this));
			return *this;
		}
		
		Matrix<T>& InplaceReversePermutateRows(unsigned int* permutation)
		{
			Matrix<T> pMatrix(this->Row,this->Col);
			for(unsigned int i = 0; i < this->Row; ++i)
			{
				pMatrix[permutation[i]][i]=1;
			}
			this->operator=(pMatrix*(*this));
			return *this;
		}
		
		Matrix<T>& InplacePermutateCols(unsigned int* permutation)
		{
			Matrix<T> pMatrix(this->Row,this->Col);
			for(unsigned int i = 0; i < this->Row; ++i)
			{
				pMatrix[permutation[i]][i]=1;
			}
			this->operator=(this->operator*(pMatrix));//*(*this));
			return *this;
		}
		
		//If permutation is insufficient in size errors may occur;
		void InplaceLU(unsigned int* permutation = nullptr)
		{
			if(permutation)
			{
				for(unsigned int i = 0; i < this->Row;++i)
				{
					permutation[i]=i;
				}
			}
			for(unsigned int c = 0; c < this->Col; ++c)
			{
				//Above Diagonal Elements;
				for(unsigned int r = 0; r < c; ++r)
				{
					for(unsigned int i = 0; i < r; ++i)
					{
						this->operator[](r)[c]-=this->operator[](r)[i]*this->operator[](i)[c];
					}
				}
				//PrePivot
				for(unsigned int r = c; r < this->Row; ++r)
				{
					for(unsigned int i = 0; i < c; ++i)
					{
						this->operator[](r)[c]-=this->operator[](r)[i]*this->operator[](i)[c];
					}
				}
				//Pivot
				unsigned int row = c;
				auto basic = T(0);
				for(unsigned int r = c; r < this->Row; ++r)
				{
					auto candidate = abs(this->operator[](r)[c]);
					if(candidate > basic)
					{
						row = r;
						basic = candidate;
					}
				}
				if(row!=c)
				{
					if(permutation) std::swap(permutation[c],permutation[row]);
					this->SwapRows(row,c);
				}
				//PostPivot
				if(basic != T(0))
				{
					basic = this->operator[](c)[c];
					for(unsigned int r = c+1; r < this->Row; ++r)
					{
						this->operator[](r)[c]/=basic;
					}
				}
			}
		}
		
		Matrix<T> ReassembleFromLU() const
		{
			Matrix<T> L(this->Col);
			Matrix<T> U(this->Row,this->Col);
			for(unsigned int r = 1; r < this->Row; ++r)
			{
				for(unsigned int c = 0; c < r; ++c)
				{
					L[r][c]=this->operator[](r)[c];
				}
			}
			for(unsigned int r = 0; r < this->Row; ++r)
			{
				for(unsigned int c = r; c < this->Col; ++c)
				{
					U[r][c]=this->operator[](r)[c];
				}
			}
			std::cout << "L:\n" <<  L << "\nU:\n" << U << std::endl;
			return L*U;
		}
		
		//Undefined result when used on non vector. Specialized form required for complex
		T VectorDot(const Matrix<T>& m) const
		{
			unsigned int lim = (this->Col > this->Row ? this->Col : this->Row);
			auto i1 = this->Val;
			auto i2 = m.Val;
			T value = T(0);
			for(unsigned int i = 0; i < lim; ++i)
			{
				value+=*(i1++)**(i2++);
			}
			return value;
		}
		
		Matrix<T> ProjectionVector(const Matrix<T>& v) const
		{
			auto u = *this;
			u*=v.VectorDot(*this);
			u/=this->VectorDot(*this);
			return u;
		}
		
		Matrix<T> GramSchmidt(const Matrix<T> v) const
		{
			return this->operator-(v.ProjectionVector(*this));
		}
		
		Matrix<T> GramSchmidt(const std::initializer_list<const Matrix<T>>& vec) const
		{
			auto u = *this;
			for( auto v : vec )
			{
				u-=v.ProjectionVector(*this);
			}
			return u;
		}
		
		bool CheckPowerMethodConvergence(const Matrix<T>& v)
		{
			for(unsigned int i = 0; i < this->Row; ++i)
			{
				if(std::abs(std::abs(this->Val[i])-std::abs(v.Val[i])) > this->Epsilon(16)) return false;
			}
			return true;
		}
		
		Matrix<T> LeadingEigenvector() const
		{
			Matrix<T> V(this->Row,1);
			for(unsigned int i = 0; i < V.Row; i++)
			{
				V[i][0]=i;
			}
			V/=V.VectorNorm();
			auto Vk = this->operator*(V);
			Vk/=Vk.VectorNorm();
			while(!V.CheckPowerMethodConvergence(Vk))
			{
				V = Vk;
				Vk = this->operator*(V);
				Vk/=Vk.VectorNorm();
			}
			return Vk;
		}
		
		Matrix<T> SubleadingEigenvector(const Matrix<T>& l) const
		{
			Matrix<T> V(this->Row,1);
			for(unsigned int i = 0; i < V.Row; i++)
			{
				V[i][0]=i;
			}
			V/=V.VectorNorm();
			V=V.GramSchmidt(l);
			auto Vk = this->operator*(V);
			Vk/=Vk.VectorNorm();
			while(!V.CheckPowerMethodConvergence(Vk))
			{
				V = Vk;
				Vk = this->operator*(V);
				Vk/=Vk.VectorNorm();
				Vk=Vk.GramSchmidt(l);
			}
			return Vk;
		}
	};
}
template class CTL::Matrix<double>;
#endif // _CTL_MATRIX_HPP_