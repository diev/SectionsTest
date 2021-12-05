/* Dmitrii Evdokimov, 2021-12-03 */

#include <bitset>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

const int WIDTH_LIMIT = 210; // mm of A4
const int SECTION_LIMIT = WIDTH_LIMIT / 10; // max size of one section for tests
const int SECTIONS_TEST = 100000; // number of tests (limited by stack size)

struct Section
{
	int start;
	int length;
	bitset<WIDTH_LIMIT> bits;
	unsigned long bits32;
	unsigned long long bits64;
};

Section sections[SECTIONS_TEST];

unsigned long scaleTo32(int start, int length = 0);
unsigned long long scaleTo64(int start, int length = 0);

void setSections();
bool compareSections(Section a, Section b);
int binsearchStart(int point);

void test1(int point);
void test2(int point);
void test3(int point);
void test4(int point);
void test5(int point);
void test6(int point);
void test7(int point);

int main()
{
	int point;

	setSections();

	cout
		<< "Enter a test point [0.."
		<< WIDTH_LIMIT-1 
		<< "]:"
		<< endl;

	cin >> point;
	//point = WIDTH_LIMIT / 2;

	//cout
	//	<< point
	//	<< endl;

	test1(point);
	test2(point);
	test3(point);
	test4(point);
	test5(point);
	test6(point);
	test7(point);

	return 0;
}

int MulDiv(int a, int b, int c) //TODO: replace with WinBase.h later
{
	return a * b / c;
}

unsigned long scaleTo32(int start, int length)
{
	int stt = start;
	int end = start + length;

	if (WIDTH_LIMIT > 32)
	{
		stt = MulDiv(stt, 32, WIDTH_LIMIT);
		end = MulDiv(end, 32, WIDTH_LIMIT);
	}

	end = end < 32 ? end : 31;
	bitset<32> bits;

	for (int j = stt; j <= end; j++)
	{
		bits.set(j);
	}

	return bits.to_ulong();
}

unsigned long long scaleTo64(int start, int length)
{
	int stt = start;
	int end = start + length;

	if (WIDTH_LIMIT > 64)
	{
		stt = MulDiv(stt, 64, WIDTH_LIMIT);
		end = MulDiv(end, 64, WIDTH_LIMIT);
	}

	end = end < 64 ? end : 63;
	bitset<64> bits;

	for (int j = stt; j <= end; j++)
	{
		bits.set(j);
	}

	return bits.to_ullong();
}

void setSections()
{
	default_random_engine generator;
	uniform_int_distribution<int> start_distribution(0, WIDTH_LIMIT);
	uniform_int_distribution<int> length_distribution(1, SECTION_LIMIT);

	int start, length;

	for (int i = 0; i < SECTIONS_TEST; i++)
	{
		do
		{
			start = start_distribution(generator);
			length = length_distribution(generator);
		} 
		while (start + length > WIDTH_LIMIT); // subject to discuse

		sections[i].start = start;
		sections[i].length = length;

		for (int j = start; j < start + length; j++)
		{
			sections[i].bits.set(j);
		}

		sections[i].bits32 = scaleTo32(start, length);
		sections[i].bits64 = scaleTo64(start, length);
	}
}

bool compareSections(Section a, Section b)
{
	return a.start < b.start;
}

int binsearchStart(int point)
{
	int l = 0, r = SECTIONS_TEST - 1;
	//int scale1 = WIDTH_LIMIT / 64;

	while (r > l)
	//while (r - l > scale1)
	{
		int m = (l + r) >> 1; // / 2

		if (sections[m].start < point)
		{
			l = m + 1;
		}
		else if (sections[m].start > point) 
		{
			r = m - 1;
		}
		else 
		{
			return m;
		}
	}
}

void test1(int point)
{
	int matches = 0;
	auto t1 = chrono::high_resolution_clock::now();

	for (int i = 0; i < SECTIONS_TEST; i++)
	{
		int start = sections[i].start;
		int end = start + sections[i].length;

		if (start <= point && point < end)
		{
			matches++;
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = t2 - t1;

	cout << "Test1,    matches: "
		 << matches
		 << ", time: " 
		 << elapsed.count()
		 << "s"
		 << endl;
}

void test2(int point)
{
	sort(sections, sections + SECTIONS_TEST, compareSections);

	int matches = 0;
	auto t1 = chrono::high_resolution_clock::now();

	for (int i = 0; i < SECTIONS_TEST; i++)
	{
		int start = sections[i].start;

		if (start <= point)
		{
			int end = start + sections[i].length;

			if (point < end)
			{
				matches++;
			}
		}
		else
		{
			break;
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = t2 - t1;

	cout << "Test2,    matches: "
		<< matches
		<< ", time: "
		<< elapsed.count()
		<< "s"
		<< endl;
}

void test3(int point)
{
	int matches = 0;
	auto t1 = chrono::high_resolution_clock::now();

	for (int i = 0; i < SECTIONS_TEST; i++)
	{
		if (sections[i].bits.test(point))
		{
			matches++;
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = t2 - t1;

	cout << "Test3,    matches: "
		 << matches 
		 << ", time: " 
		 << elapsed.count()
		 << "s"
		 << endl;
}

void test4(int point)
{
	unsigned long point32 = scaleTo32(point);
	{
		int matches = 0;
		auto t1 = chrono::high_resolution_clock::now();

		for (int i = 0; i < SECTIONS_TEST; i++)
		{
			if (point32 & sections[i].bits32)
			{
				int start = sections[i].start;
				int end = start + sections[i].length;

				if (start <= point && point < end)
				{
					matches++;
				}
			}
		}

		auto t2 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = t2 - t1;

		cout
			<< "Test4x32, matches: "
			<< matches 
			<< ", time: " 
			<< elapsed.count()
			<< "s"
			<< endl;
	}

	unsigned long long point64 = scaleTo64(point);
	{
		int matches = 0;
		auto t1 = chrono::high_resolution_clock::now();

		for (int i = 0; i < SECTIONS_TEST; i++)
		{
			if (point64 & sections[i].bits64)
			{
				int start = sections[i].start;
				int end = start + sections[i].length;

				if (start <= point && point < end)
				{
					matches++;
				}
			}
		}

		auto t2 = chrono::high_resolution_clock::now();
		chrono::duration<double> elapsed = t2 - t1;

		cout
			<< "Test4x64, matches: "
			<< matches 
			<< ", time: " 
			<< elapsed.count()
			<< "s"
			<< endl;
	}
}

void test5(int point)
{
	unsigned long long point64 = scaleTo64(point);
	unsigned long long index[SECTIONS_TEST];

	for (int i = 0; i < SECTIONS_TEST; i++)
	{
		index[i] = sections[i].bits64;
	}

	int matches = 0;
	auto t1 = chrono::high_resolution_clock::now();

	for (int i = 0; i < SECTIONS_TEST; i++)
	{
		if (point64 & index[i])
		{
			int start = sections[i].start;
			int end = start + sections[i].length;

			if (start <= point && point < end)
			{
				matches++;
			}
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = t2 - t1;

	cout
		<< "Test5,    matches: "
		<< matches 
		<< ", time: " 
		<< elapsed.count()
		<< "s"
		<< endl;
}

void test6(int point)
{
	unsigned long long point64 = scaleTo64(point);

	sort(sections, sections + SECTIONS_TEST, compareSections);

	unsigned long long index[SECTIONS_TEST];

	for (int i = 0; i < SECTIONS_TEST; i++)
	{
		index[i] = sections[i].bits64;
	}

	int matches = 0;
	auto t1 = chrono::high_resolution_clock::now();

	int max = SECTIONS_TEST - 1;
	int mid = max;

	//while (index[mid] > point64)
	while (sections[mid].start > point)
	{
		max = mid;
		mid -= max / 10; //TODO: binary search?
	}

	for (int i = 0; i <= max; i++)
	{
		if (point64 & index[i])
		{
			int start = sections[i].start;
			int end = start + sections[i].length;

			if (start <= point && point < end)
			{
				matches++;
			}
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = t2 - t1;

	cout
		<< "Test6,    matches: " 
		<< matches 
		<< ", time: " 
		<< elapsed.count()
		<< "s"
		<< endl;
}

void test7(int point)
{
	unsigned long long point64 = scaleTo64(point);
	unsigned long long index[SECTIONS_TEST];

	sort(sections, sections + SECTIONS_TEST, compareSections);

	int max_length = 0;
	for (int i = 0; i < SECTIONS_TEST; i++)
	{
		index[i] = sections[i].bits64;

		if (sections[i].length > max_length)
		{
			max_length = sections[i].length;
		}
	}

	int matches = 0;
	auto t1 = chrono::high_resolution_clock::now();

	int max = binsearchStart(point + 1);
	int min = binsearchStart(point - max_length);

	for (int i = min; i <= max; i++)
	{
		if (point64 & index[i])
		{
			int start = sections[i].start;
			int end = start + sections[i].length;

			if (start <= point && point < end)
			{
				matches++;
			}
		}
	}

	auto t2 = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed = t2 - t1;

	cout
		<< "Test7,    matches: "
		<< matches
		<< ", time: "
		<< elapsed.count()
		<< "s"
		<< endl;
}
