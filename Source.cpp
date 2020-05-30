#include "Th_pool.h"

void rrr(int r) {
	this_thread::sleep_for(chrono::milliseconds(100));
	cout << r;

}


int main() {
    Th_pool pool(6);

	auto tt = pool.push_task(rrr, 1);
	cout << pool.statys(tt) << endl;

	thread th([&] {
		for (int i = 0; i < 100; i++) {
			pool.push_task(rrr, 2);
			pool.push_task(rrr, 3);
			pool.push_task(rrr, 4);
			pool.push_task(rrr, 5);
		}
		this_thread::sleep_for(chrono::milliseconds(100));
		for (int i = 0; i < 100; i++) {
			pool.push_task(rrr, 2);
			pool.push_task(rrr, 3);
			pool.push_task(rrr, 4);
			pool.push_task(rrr, 5);
		}
		});
	th.detach();
	
	
	auto ttt = pool.push_task(rrr, 1);

	auto data = pool.push_task(rrr, 2);
	while (pool.statys(data) != "completed") {


	}
	cout << pool.statys(data) << endl;
	
	return 0;
}