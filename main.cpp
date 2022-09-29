#include <thread>
#include <mutex>
#include <vector>
#include <iostream>
#include <random>

void transpose(const int A, const int B, std::vector<std::vector<int>> &old_x, std::vector<std::vector<int>> &new_x, int thread_num){
	std::thread thr[thread_num];
	//std::mutex write_lock;
	for(int i = 0; i < thread_num; i++){
		auto f = [&](int A, int B, std::vector<std::vector<int>> &old_x, std::vector<std::vector<int>> &new_x, int current_thread, int thread_num){
			std::vector<int> new_v(A);
			for(int i = current_thread; i < B; i += thread_num){
				for(int j = 0; j < A; j++){
					new_v[j] = old_x[j][i];
				}
				//std::lock_guard<std::mutex> lock(write_lock);
				new_x[i] = new_v;
			}
		};
		thr[i] = std::thread(f, A, B, std::ref(old_x), std::ref(new_x), i, thread_num);
	}
	for(int i = 0; i < thread_num; i++)
		thr[i].join();
}

int main(){
	std::random_device r;
	std::mt19937 e(r());
	std::uniform_int_distribution<int> uniform_dist(2000, 3000);
	std::chrono::time_point<std::chrono::system_clock> start, end;
	int A, B;
	int thr[] = {1, 2, 4, 10};
	for(auto t : thr){
		std::cout << t << " : ";
		double duration_ms = 0;
		for(int i = 0; i < 10; i++){
			A = uniform_dist(e);
			B = uniform_dist(e);
			std::vector<std::vector<int>> arr1(A, std::vector<int>(B)), arr2(B);
			for(int i = 0; i < A; i++)
				for(int j = 0; j < B; j++)
					arr1[i][j] = i * i + j;
			start = std::chrono::system_clock::now();
			transpose(A, B, arr1, arr2, t);
			end = std::chrono::system_clock::now();
			std::chrono::duration<double> elapsed_time = end - start;
			duration_ms += std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();
		}
		std::cout << duration_ms / 10 << "ms" << std::endl;
	}
	return 0;
}
