#pragma once
#include <functional>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>

/*
FMTS - Fucntional Multi-Threading System

This is a multi-purpose task-based multithreading library.
*/
class FMTS {
public:
	 FMTS(size_t num_threads) : _running(true) {
		  // allocate threads based on described amount
		  for (int i = 0; i < num_threads; i++) {

				_worker_threads.emplace_back([this]() {
					 // begin work
					 worker();
				});
		  }
	 }

	 // submit the task
	 void submit_task(std::function<void()> task) {
		  // lock the thread, then add the task 
		  {
				std::unique_lock<std::mutex> lock(_mtx);
				_tasks.push(std::move(task));
		  }
		  // notify the threads that another has been added
		  _cv.notify_one();
	 }

	 void terminate() {
		  // join all threads to main	  
		  {
				std::unique_lock<std::mutex> lock(_mtx);
				_running = false;
		  }

		  // wake up all threads
		  _cv.notify_all();
		  for (auto& thread : _worker_threads) {
				thread.join();
		  }
	 }

	 bool is_empty() {
		  std::unique_lock<std::mutex> lock(_mtx);
		  return _tasks.empty();
	 }

private:
	 // working thread
	 void worker() {
		  // working
		  while (_running) {
				std::function<void()> task;
				
				{
					 // lock the thread
					 std::unique_lock<std::mutex> lock(_mtx);

					 // wait for the condition variable
					 _cv.wait(lock, [this]{
						  return !_tasks.empty() || !_running;
					 });

					 if (!_running) break;

					 // there is a task, get it then pop it
					 task = std::move(_tasks.front());
					 _tasks.pop();
				}

				// run the task
				task();
		  }
	 }

	 // thread related
	 std::mutex _mtx;
	 std::mutex _lock;
	 std::atomic<bool> _running;
	 std::vector<std::thread> _worker_threads;
	 std::queue<std::function<void()>> _tasks;
	 std::condition_variable _cv;
};
