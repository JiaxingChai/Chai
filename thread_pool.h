#pragma once
#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include<queue>
#include<future>
#include<functional>
#include<condition_variable>


namespace chai
{
	class ThreadPool
	{
	public:
		ThreadPool(int thread_num): stop(false)
		{
			if (thread_num <= 0) thread_num = 1;
			else if (thread_num > std::thread::hardware_concurrency()) thread_num = std::thread::hardware_concurrency();

			for (int i = 0; i < thread_num; ++i) AddThread();
		}

		~ThreadPool()
		{
			{
				std::unique_lock<std::mutex> lock(mtx);
				stop = true;

			}
			cv.notify_all();

			for (auto& worker : worker_threads_) worker.join();
		}

		void AddThread() 
		{
			worker_threads_.emplace_back(
				[this]()
				{
					while (true)
					{
						std::function<void()> task;
						{
							std::unique_lock<std::mutex> lock(this->mtx);
							this->cv.wait(lock, [this]() {return this->stop || !this->tasks.empty(); });
							if (this->stop && this->tasks.empty()) return;

							task = std::move(this->tasks.front());
							this->tasks.pop();
						}
						task();
					}
				}
			);
		}

		template<typename F, typename... Args>
		auto Enqueue(F&& f, Args &&...args)->std::future<typename std::result_of<F(Args...)>::type>
		{
			using return_type = typename std::result_of<F(Args...)>::type;

			auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

			std::future<return_type> res = task->get_future();
			{
				std::unique_lock<std::mutex> lock(mtx);
				if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");

				tasks.emplace([task]() {(*task)(); });
			}
			cv.notify_one();
			return res;
		}

		ThreadPool(const ThreadPool&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;
	private:
		std::vector<std::thread> worker_threads_;
		std::queue<std::function<void()>> tasks;
		std::condition_variable cv;
		std::mutex mtx;
		std::atomic<bool> stop;
	};

	void thread_test()
	{
		ThreadPool pool(4);
		std::vector<std::future<int>> results;

		std::function<int(int, int)> f = [](int i, int j) { return i * j;};

		for (int i = 0; i < 10; i++)
		{
			results.emplace_back(
				pool.Enqueue(f, i, i)
			);
		}

		for (auto&& result : results)
		{
			std::cout << result.get() << std::endl;
		}
	}

	void matrix_transpose(int row_num, int col_num, int thread_num)
	{
		time_t start = time(nullptr);
		std::cout << "Matrix Transpose Start!" << std::endl;

		ThreadPool pool(thread_num);
		std::vector<std::future<void>> results;
		std::vector<std::vector<int>> read_matrix(row_num, std::vector<int>(col_num));
		std::vector<std::vector<int>> matrix(col_num, std::vector<int>(row_num));

		auto f_init = [&read_matrix](int row_min, int row_max, int col_min, int col_max)
			{
				for (int i = row_min; i < row_max; ++i)
				{
					for (int j = col_min; j < col_max; ++j)
					{
						read_matrix[i][j] = i;
					}
				}
			};

		for (int i = 0; i < thread_num; ++i)
		{
			int row_min = i * (row_num / thread_num);
			int row_max = (i + 1) * (row_num / thread_num);
			if (i == thread_num - 1) row_max = row_num;
			results.emplace_back(
				pool.Enqueue(f_init, row_min, row_max, 0, col_num)
			);
		}

		auto f_transpose = [read_matrix, &matrix](int row_min, int row_max, int col_min, int col_max)
			{
				for (int i = row_min; i < row_max; ++i)
				{
					for (int j = col_min; j < col_max; ++j)
					{
						matrix[j][i] = read_matrix[i][j];
					}
				}
			};

		for (int i = 0; i < thread_num; ++i)
		{
			int row_min = i * (row_num / thread_num);
			int row_max = (i + 1) * (row_num / thread_num);
			if (i == thread_num - 1) row_max = row_num;
			results.emplace_back(
				pool.Enqueue(f_transpose, row_min, row_max, 0, col_num)
			);
		}

		for (auto&& result : results)
		{
			result.get();
		}

		time_t end = time(nullptr);
		std::cout << "Matrix Transpose Completed!" << std::endl;
		std::cout << "Time: " << end - start << " seconds" << std::endl;
	}
}