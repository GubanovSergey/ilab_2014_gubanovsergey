#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>

#define DBG_mode

#ifdef DBG_mode
#define DBG if(1)
#else
#define DBG if(0)
#endif

class UnsBigInteger
{
public:
	UnsBigInteger();
	UnsBigInteger(const UnsBigInteger & A);
	friend std::ostream & operator << (std::ostream & os, const UnsBigInteger & A);
	~UnsBigInteger();

	const unsigned short Init_size = 10;
	const unsigned long long Init_max = 9999999999;

	UnsBigInteger & operator = (const UnsBigInteger & a);
	UnsBigInteger & operator += (const UnsBigInteger & a);

	friend const UnsBigInteger operator + (const UnsBigInteger & a, const UnsBigInteger & b);
	friend bool operator > (const UnsBigInteger & a, const UnsBigInteger & b);
	friend bool operator < (const UnsBigInteger & a, const UnsBigInteger & b);
	friend bool operator == (const UnsBigInteger & a, const UnsBigInteger & b);
	friend bool operator >= (const UnsBigInteger & a, const UnsBigInteger & b);
	friend bool operator <= (const UnsBigInteger & a, const UnsBigInteger & b);

	friend const UnsBigInteger operator + (const UnsBigInteger & a, unsigned long long b);
	friend const UnsBigInteger operator + (unsigned long long b, const UnsBigInteger & a);
	friend bool operator > (const UnsBigInteger & a, unsigned long long b);
	friend bool operator < (const UnsBigInteger & a, unsigned long long b);
	friend bool operator == (const UnsBigInteger & a, unsigned long long b);
	friend bool operator >= (const UnsBigInteger & a, unsigned long long b);
	friend bool operator <= (const UnsBigInteger & a, unsigned long long b);
	friend bool operator > (unsigned long long a, const UnsBigInteger & b);
	friend bool operator < (unsigned long long a, const UnsBigInteger & b);
	friend bool operator == (unsigned long long a, const UnsBigInteger & b);
	friend bool operator >= (unsigned long long a, const UnsBigInteger & b);
	friend bool operator <= (unsigned long long a, const UnsBigInteger & b);

	UnsBigInteger & operator = (unsigned long long a);
	UnsBigInteger & operator += (unsigned long long a);
	const UnsBigInteger operator ++ (int);
	/*TODO
	UnsBigInteger & operator -= (const UnsBigInteger & A);					    		0
	//UnsBigInteger & operator -= (unsigned long long a);								0
	//UnsBigInteger & operator -- (int);												0
	//friend const UnsBigInteger operator - (UnsBigInteger & A, UnsBigInteger & B);		1
	//friend const UnsBigInteger operator - (UnsBigInteger & A, unsigned long long b);	2
	//friend const UnsBigInteger operator - (unsigned long long a, UnsBigInteger & B);	2
	//UnsBigInteger & operator *= (const UnsBigInteger & A);							0
	//UnsBigInteger & operator *= (unsigned long long a);								0
	//friend const UnsBigInteger operator * (UnsBigInteger & A, UnsBigInteger & B);		1
	//friend const UnsBigInteger operator * (UnsBigInteger & A, unsigned long long b);	2
	//friend const UnsBigInteger operator * (unsigned long long a, UnsBigInteger & B);	2
	//UnsBigInteger & operator /= (const UnsBigInteger & A);							0
	//UnsBigInteger & operator /= (unsigned long long a);								0
	//friend const UnsBigInteger operator / (UnsBigInteger & A, UnsBigInteger & B);		1
	//friend const UnsBigInteger operator / (UnsBigInteger & A, unsigned long long b);	2
	//friend const UnsBigInteger operator / (unsigned long long a, UnsBigInteger & B);	2
	*/
private:
	unsigned long long Size_;      //!!!The problem is often about this
	unsigned long long Mem_size_;
	unsigned short * Data_;
	void UBInt_resize(unsigned long long New_size);
	static short int UBInt_comp(const UnsBigInteger & a, const UnsBigInteger & b);
	static short int UBInt_comp(const UnsBigInteger & a, unsigned long long b);
	unsigned short Base_;
};

UnsBigInteger::UnsBigInteger() :
	Data_(new unsigned short[Init_size]),
	Size_(0),
	Mem_size_(Init_size),
	Base_(10)
{
	DBG std::cout << "#Constructor is creating " << this << std::endl;
	memset(Data_, 0, sizeof(*Data_) * Init_size);
}

UnsBigInteger::UnsBigInteger(const UnsBigInteger & A) :
	Data_(new unsigned short[Init_size]),
	Mem_size_(Init_size),
	Base_(10)
{
	DBG std::cout << "#Constructor is creating " << this << std::endl;
	memset(Data_, 0, sizeof(*Data_) * Init_size);
	unsigned long long len = A.Size_, i = 0;
	UBInt_resize(len + 1);
	for (i = 0; i < len; i++)
		this->Data_[i] = A.Data_[i];
	this->Size_ = i;
}

void UnsBigInteger::UBInt_resize(unsigned long long New_size)
{
	if (Mem_size_ < New_size)
	{
		DBG std::cout << "#Resize " << this << std::endl;
		Data_ = (unsigned short *)realloc(Data_, New_size * sizeof(unsigned short));
		memset(Data_ + Mem_size_, 0, (New_size - Mem_size_) * sizeof(unsigned short));
		Mem_size_ = New_size;
	}
}

short int UnsBigInteger::UBInt_comp(const UnsBigInteger & A, const UnsBigInteger & B)
{
	unsigned long long i = A.Size_;
	if (A.Size_ > B.Size_)
		return 1;
	else if (B.Size_ > A.Size_)
		return -1;
	else
		for (; i > 0; i--)
		{
		if (A.Data_[i - 1] > B.Data_[i - 1])
			return 1;
		else if (B.Data_[i - 1] > A.Data_[i - 1])
			return -1;
		}
	return 0;
}

short int UnsBigInteger::UBInt_comp(const UnsBigInteger & A, unsigned long long b)
{
	unsigned long long i = 0;
	unsigned short digit = 0, base = A.Base_;
	for (; i < A.Size_; i++)
	{
		if (b == 0 || A.Data_[i] > (digit = b % base))
			return 1;
		else if (digit > A.Data_[i])
			return -1;
		b /= base;
	}
	if (b != 0)
		return -1;
	else
		return 0;
}

UnsBigInteger & UnsBigInteger::operator = (const UnsBigInteger & a)
{
	unsigned long long len = a.Size_, i = 0;
	if (this == &a)
		return *this;
	UBInt_resize(len + 1);
	for (i = 0; i < len; i++)
		this->Data_[i] = a.Data_[i];
	this->Size_ = i;
	return *this;
}

UnsBigInteger & UnsBigInteger::operator = (unsigned long long a)
{
	unsigned long long i = 0;
	if (a > Init_max)
		UBInt_resize(20);
	do
	{
		this->Data_[i] = a % Base_;
		a = a / Base_;
		i++;
	} while (a != 0);
	Size_ = i;
	return *this;
}

UnsBigInteger & UnsBigInteger::operator += (const UnsBigInteger & a)
{
	unsigned short fornext = 0, sum = 0;
	unsigned long long i = 0, len = a.Size_, n_size = 0;
	if (len > Size_)
		UBInt_resize(len + 1);
	for (i = 0; i < len; i++)
	{
		fornext = !!((sum = Data_[i] + fornext + a.Data_[i]) >= Base_);
		Data_[i] = sum % Base_;
	}
	n_size = (Size_ > i ? Size_ : i);
	if (fornext)
	{
		UBInt_resize(len + 1);  //won't be done if it has been already done at the beg of the func (see the text of UBInt_resize())
		Data_[i] = fornext;
		this->Size_ = Size_ + 1;
	}
	else this->Size_ = Size_;
	return *this;
}

UnsBigInteger & UnsBigInteger::operator += (unsigned long long a)
{
	unsigned short fornext = 0, sum = 0;
	unsigned long long i = 0, n_size = 0;
	if (a > Init_max)
		UBInt_resize(21);
	do
	{
		fornext = !!((sum = Data_[i] + fornext + a % Base_) >= Base_);
		Data_[i] = sum % Base_;
		a = a / Base_;
		i++;
	} while (a != 0);
	n_size = (Size_ > i ? Size_ : i);
	if (fornext)
	{
		UBInt_resize(21);  //won't be done if it has been already done at the beg of the func (see the text of UBInt_resize())
		Data_[i] = fornext;
		this->Size_ = n_size + 1;
	}
	else this->Size_ = n_size;
	return *this;
}

const UnsBigInteger UnsBigInteger::operator ++ (int)
{
	operator += (1);
	return *this;
}

const UnsBigInteger operator + (const UnsBigInteger & a, const UnsBigInteger & b)
{
	UnsBigInteger res = b;
	return (res += a);
}

const UnsBigInteger operator + (const UnsBigInteger & a, unsigned long long b)
{
	UnsBigInteger res = a;
	return (res += b);
}

const UnsBigInteger operator + (unsigned long long a, const UnsBigInteger & b)
{
	UnsBigInteger res = b;
	return (res += a);
}

bool operator > (const UnsBigInteger & A, const UnsBigInteger & B) {
	return (UnsBigInteger::UBInt_comp(A, B) == 1);
}

bool operator < (const UnsBigInteger & A, const UnsBigInteger & B) {
	return (UnsBigInteger::UBInt_comp(A, B) == -1);
}

bool operator == (const UnsBigInteger & A, const UnsBigInteger & B) {
	return (UnsBigInteger::UBInt_comp(A, B) == 0);
}

bool operator >= (const UnsBigInteger & A, const UnsBigInteger & B) {
	return (UnsBigInteger::UBInt_comp(A, B) >= 0);
}

bool operator <= (const UnsBigInteger & A, const UnsBigInteger & B) {
	return (UnsBigInteger::UBInt_comp(A, B) <= 0);
}

bool operator > (const UnsBigInteger & A, unsigned long long b) {
	return (UnsBigInteger::UBInt_comp(A, b) == 1);
}

bool operator < (const UnsBigInteger & A, unsigned long long b) {
	return (UnsBigInteger::UBInt_comp(A, b) == -1);
}

bool operator == (const UnsBigInteger & A, unsigned long long b) {
	return (UnsBigInteger::UBInt_comp(A, b) == 0);
}

bool operator >= (const UnsBigInteger & A, unsigned long long b) {
	return (UnsBigInteger::UBInt_comp(A, b) >= 0);
}

bool operator <= (const UnsBigInteger & A, unsigned long long b) {
	return (UnsBigInteger::UBInt_comp(A, b) <= 0);
}

bool operator > (unsigned long long a, const UnsBigInteger & b) {
	return b < a;
}

bool operator < (unsigned long long a, const UnsBigInteger & b) {
	return b > a;
}

bool operator == (unsigned long long a, const UnsBigInteger & b) {
	return b == a;
}

bool operator >= (unsigned long long a, const UnsBigInteger & b) {
	return b <= a;
}

bool operator <= (unsigned long long a, const UnsBigInteger & b) {
	return b >= a;
}

std::ostream & operator << (std::ostream & os, const UnsBigInteger & A)
{
	unsigned long long i = A.Size_ - 1;
	for (i; i > 0; i--)
		std::cout << A.Data_[i] << " ";
	std::cout << A.Data_[0] << std::endl;
	return os;
}

//UnsBigInteger & operator -- (int)

//UnsBigInteger & operator -= (const UnsBigInteger & a);
//UnsBigInteger & operator -= (unsigned long long a);

UnsBigInteger::~UnsBigInteger()
{
	DBG std::cout << "#Destructor is killing " << this << std::endl;
	delete [] Data_;
	Size_ = 0;
}



int main()
{
	UnsBigInteger A, B;
	A = 1;
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	std::cout << (A += A += A += A += A += A += A += A += A += A += A);
	system("pause");
}
