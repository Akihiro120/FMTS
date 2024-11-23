# Functional Multi-Threading System (FMTS)
FMTS is a simple and functional task-based multithreading system.

> [!WARNING]
> This is a project created for my personal uses, if something isn't included please make a **Pull Request** or open an Issue.

# Example Usage

``` c++
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
 // expected result, 100t, 100000
```
