#include <iostream>
#include <amp.h>
#include <time.h>
#include <fstream>
using namespace concurrency;

const long long size = 50000001;

int A[size];
int B[size];
int sum[size];
int  main() {
	std::ofstream fout("syncedGPUon1try2.txt");

	srand(time(NULL));

	for (long long i = 0; i < size; ++i)
	{
		A[i] = rand() % 10000;
		B[i] = rand() % 10000;
	}
	//auto t = clock();
	for (long long i = 100; i < size; i += 10000) {
		int sizeq = i;
		auto t = clock();
		array_view<const int, 1> a(sizeq, A);
		array_view<const int, 1> b(sizeq, B);
		array_view<int, 1> s(sizeq, sum);
		s.discard_data();
//now it's faster (no)
		parallel_for_each(extent < 1 >(1), [&](index < 1 > j) restrict(amp)
		{
			for (int j = 0; j < sizeq; ++j)
				s[j] = a[j] + b[j];
		}
		);

		s.synchronize();

		fout << i << " " << clock() - t << std::endl;

	}
	/*for (long long i = 100; i < size; i += 10000)
	{
		auto t = clock();
		for (long long j = 0; j < i; ++j)
			sum[j] = A[j] + B[j];
		fout << i << " " << double(clock() - t) << std::endl;
	}*/


	return 0;
}
