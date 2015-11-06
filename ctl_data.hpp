/* 
	Conceptual Template Library by Piotr Grudzień
	A simple array for use with streams.
*/
#ifndef _CTL_DATA_HPP_
#define _CTL_DATA_HPP_

#include "ctl_sort.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>


namespace CTL
{
	template<typename T>
	class Data
	{
	private:
		T* DataPtr = nullptr;
		int Size = 0;
		
		void ReadStream(std::istream& stream)
		{
			for(int i = 0; i<this->Size; ++i)
			{
				stream >> this->DataPtr[i];
			}
		}
		
		
	public:
		int GetSize()
		{
			return this->Size;
		}
		
		std::ostream& operator<<(std::ostream& stream) const
		{
			this->Print(stream);
			return stream;
		}
		
		friend std::ostream& operator<<(std::ostream& stream,const CTL::Data<T>& data)
		{
			return data.operator<<(stream);
		}
		
		void Clear()
		{
			delete[] this->DataPtr;
			this->DataPtr=nullptr;
			this->Size = 0;
		}
		
		void Load(std::istream& stream = std::cin)
		{
			if(this->DataPtr==nullptr) this->Clear();
			stream >> this->Size;
			this->DataPtr = new T[Size];
			this->ReadStream(stream);
		}
		
		void Load(const int size, std::istream& stream = std::cin)
		{
			if(this->DataPtr==nullptr) this->Clear();
			this->Size = size;
			this->DataPtr = new T[Size];
			this->ReadStream(stream);
		}
		
		void Print(std::ostream& stream = std::cout) const
		{
			for(int i = 0; i<this->Size; ++i)
			{
				stream << this->DataPtr[i] << " ";
			}
		}
		
		void ReadFile(const char* filename)
		{
			std::ifstream file(filename);
			this->Load(file);
			file.close();
		}
		
		void ReadFile(const int size, const char* filename)
		{
			std::ifstream file(filename);
			this->Load(size,file);
			file.close();
		}
		
		void WriteFile(const std::string& filename)
		{
			std::ofstream file (filename);
			this->operator<<(file);
			file.close();
		}
		
		Data() {}
		
		Data(const CTL::Data<T>& data)
		: DataPtr(new T[data.Size]), Size(data.Size)
		{
			for(int i = 0; i < Size; ++i)
				this->DataPtr[i]=data.DataPtr[i];
		}
		
		Data(std::istream& stream)
		{
			this->Load(stream);
		}
		
		Data(const int size, std::istream& stream = std::cin)
		: Size(size), DataPtr(new T[size])
		{
			this->ReadStream(stream);
		}
		
		Data(const char* filename)
		{
			std::ifstream file(filename);
			this->Load(file);
		}
		
		T& operator[](const int poz)
		{
			return this->DataPtr[poz];
		}
		
		const T& operator[](const int poz) const
		{
			return this->DataPtr[poz];
		}
		
		T& at(const int poz)
		{
			if(poz < 0 || this->Size <= poz)
				throw std::out_of_range("CTL::Data range check");
			else
				return this->DataPtr[poz];
		}
		
		const T& at(const int poz) const
		{
			if(poz < 0 || this->Size <= poz)
				throw std::out_of_range("CTL::Data range check");
			else
				return this->DataPtr[poz];
		}
		
		T* begin()
		{
			return this->DataPtr;
		}
		
		const T* begin() const
		{
			return this->DataPtr;
		}
		
		T* end()
		{
			return this->DataPtr+this->Size;
		}
		
		const T* end() const
		{
			return this->DataPtr+this->Size;
		}
		
		const T* cbegin() const
		{
			return this->DataPtr;
		}
		
		const T* cend() const
		{
			return this->DataPtr+this->Size;
		}
		
		~Data()
		{
			delete[] this->DataPtr;
		}
		
		template<typename Compar>
		void Sort(void (*sort)(T*,T*,Compar)=CTL::MergeSort,Compar comp=std::less<T>())
		{
			sort(this->begin(),this->end(),comp);
		}
	};
	
}

#endif