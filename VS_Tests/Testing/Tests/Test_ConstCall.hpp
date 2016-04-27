#ifndef TEST_CONSTCALL_HPP
#define TEST_CONSTCALL_HPP

class TEST_CONSTCALL
{
private:
	static int ret(const TEST_CONSTCALL* a)
	{
		return 2;
	}

	static int ret(TEST_CONSTCALL* a)
	{
		return 1;
	}
public:
	int trycall() const
	{
		return ret(this);
	}

	int trycall()
	{
		return ret(this);
	}
};

void test_constcall()
{
	const TEST_CONSTCALL* a = nullptr;
	TEST_CONSTCALL* b = nullptr;

	std::cout << a->trycall() << b->trycall() << std::endl;
}

#endif // !TEST_CONSTCALL_HPP
