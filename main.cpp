#include "src/fmts.h"
#include <iostream>

int main() {
	 // create threads
	 FMTS fmts(4);

	 int counter = 0;
	 std::mutex mtx;

	 // create tasks
	 for (int i = 0; i < 10; i++) {
		  fmts.submit_task([&mtx, &counter] {

				// lock the counter variable
				std::lock_guard<std::mutex> lock(mtx);
				for (int i = 0; i < 10000; i++) {
					 counter++;
				}
		  });
	 }

	 // ensure that tasks are all completed
	 while (!fmts.is_empty()) {
	 }


	 // stop all threads
	 fmts.terminate();
	 std::cout << counter << std::endl;
	 // expected result, 100000
}
