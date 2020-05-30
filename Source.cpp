#include "Th_pool.h"

void rrr(int r) {
    this_thread::sleep_for(chrono::milliseconds(2000));
	cout << "rrr funk" << endl;

}


int main() {
    Th_pool pool(6);

    auto tt = pool.push_task(rrr, 1);
    cout << pool.statys(tt) << endl;
    pool.push_task(rrr, 2);
    pool.push_task(rrr, 3);
    pool.push_task(rrr, 4);
    pool.push_task(rrr, 5);
    
    this_thread::sleep_for(chrono::milliseconds(100));
    cout << pool.statys(tt) << endl;
    this_thread::sleep_for(chrono::seconds(3));
    cout << pool.statys(tt) << endl;
    this_thread::sleep_for(chrono::milliseconds(6000));
    pool.resize(2);
    auto ttt = pool.push_task(rrr, 1);
    cout << pool.statys(ttt) << endl;
    pool.push_task(rrr, 2);
    pool.push_task(rrr, 3);
    pool.push_task(rrr, 4);
    pool.push_task(rrr, 5);
    
    this_thread::sleep_for(chrono::milliseconds(100));
    cout << pool.statys(ttt) << endl;
    this_thread::sleep_for(chrono::seconds(2));
    cout << pool.statys(ttt) << endl;
    this_thread::sleep_for(chrono::milliseconds(6000));

	return 0;
}