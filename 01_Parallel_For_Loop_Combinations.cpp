// compile with /EHsc
// Mostly the compiler optimizations of serial loops will make them seem faster, but as the amount of data increases parallel_for 
// will be a viable option
#include <windows.h>
#include <ppl.h>
#include <vector>
#include <concurrent_vector.h>
#include <string>
#include <iostream>

using namespace concurrency;

// Calls the provided work function and returns the number of milliseconds
// that it takes to call that function.
template <class Function>
__int64 time_call(Function&& f)
{
	__int64 begin = GetTickCount();
	f();
	return GetTickCount() - begin;
}

int main()
{
	char key;

	std::vector<std::string>* result1 = new std::vector<std::string>();
	concurrent_vector<std::string>* result2 = new concurrent_vector<std::string>();

	__int64 elapsed_serial = time_call([&]()
	{
		for (char i{ 'A' }; i <= 'Z'; i++)
		{
			std::string combination{"0000"};
			combination[0] = i;

			for (char j{ 'A' }; j <= 'Z'; j++)
			{
				combination[1] = j;

				for (char k{ 'A' }; k <= 'Z'; k++)
				{
					combination[2] = k;

					for (char l{ 'A' }; l <= 'Z'; l++)
					{
						combination[3] = l;
						// deep copy needed
						result1->push_back(std::string(combination));
					}
				}
			}
		}

	});

	std::cout << "----Serial Execution----" << std::endl;
	std::cout << (*result1)[0] << " .... " << (*result1)[result1->size() - 1] << std::endl;
	std::cout << "Items stored : " << result1->size() << std::endl;
	std::cout << "Time taken : " << elapsed_serial << "ms" << std::endl;
	std::cout << "------------------------" << std::endl;


	
	__int64 elapsed_parallel = time_call([&]()
	{
		parallel_for (char{ 'A' }, '[', [&](char i) // need to give '[' as it is one more than 'Z'
		{
			std::string combination{ "0000" };
			combination[0] = i;

			for (char j{ 'A' }; j <= 'Z'; j++)
			{
				combination[1] = j;

				for (char k{ 'A' }; k <= 'Z'; k++)
				{
					combination[2] = k;

					for (char l{ 'A' }; l <= 'Z'; l++)
					{
						combination[3] = l;
						// deep copy needed
						result2->push_back(std::string(combination));
					}
				}
			}
		});
	});

	std::cout << "----Parallel Execution----" << std::endl;
	std::cout << (*result2)[0] << " .... " << (*result2)[result2->size() - 1] << std::endl;
	std::cout << "Items stored : " << result2->size() << std::endl;
	std::cout << "Time taken : " << elapsed_parallel << "ms" << std::endl;
	std::cout << "------------------------" << std::endl;
	

	std::cin >> key;
}
