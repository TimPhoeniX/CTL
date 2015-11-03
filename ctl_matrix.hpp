#ifndef _CTL_MATRIX_HPP_
#define _CTL_MATRIX_HPP_

namespace CTL
{
	template<typename T>
	class Matrix
	{
	private:
		T* MatrixPtr = nullptr;
		unsigned int Row = 1;
		unsigned int Col = 1;
	public:
		CTLMatrix( const unsigned int row = 0, const unsigned int col = 0) : Row(row), Col(col)
		{
			Matrix = new T[Row*Col];
		}
		T* operator[](unsigned int row)
		{
			return Matrix+(row*Row);
		}
	};
}
#endif // _CTL_MATRIX_HPP_