#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <queue>
#include <condition_variable>
#include <iostream>
#include <chrono>

using namespace std;

#define MAX_THREADS std::thread::hardware_concurrency() - 1;

class Th_pool {
public:
    void resize(int newCount) {
        int tmp = MAX_THREADS;
        if (newCount <= 0 || newCount > tmp) {


        }
        else {           
            if (newCount > numThreads) {
                uint8_t tmp = numThreads;
                numThreads = newCount;
                Pool.resize(numThreads);
                for (int i = tmp; i != numThreads; ++i) {
                    Pool.emplace_back(thread(&Th_pool::thread_while, this));
                    Pool.back().detach();
                }
            }
            else {
                numThreads = newCount;
                Pool.resize(newCount);
            }
        }
    }

private:

    class task {
    private:
        function<void(int)> func;
        int info;
        string stat;
      
    public:
        ~task(){}
        task(function<void(int)> ff, int inf) {
            func = ff;
            info = inf;
            stat = "in the queue";
        }
        void performed_f() {
            stat = "performed";

        }
        void completed_f() {
             stat = "completed";

        }
        void execute() {
            
            func(info);     
           
           
           
        }
        string statys() {

            return stat;

        }
    };
    int numThreads; 
    vector<thread> Pool; 
    vector<shared_ptr<task>> taskQueue;
    condition_variable thread_c;
    mutex taskMutex; 
    
    void thread_while() {
        while (true) {

            


            if (taskQueue.size() < 1)
                continue;

            for (int i = 0; i < taskQueue.size(); i++) {
              

                if ((*taskQueue[i]).statys() == "in the queue") {
                    
                    taskMutex.lock();
                    (*taskQueue[i]).performed_f();
                    taskMutex.unlock();
                    shared_ptr<task> cc = taskQueue[i];
                    (*taskQueue[i]).execute();
                                                     

                    taskMutex.lock();
                    for (int j = 0; j < taskQueue.size(); j++) {

                        if (taskQueue[j] == cc) {
                            (*taskQueue[j]).completed_f();
                            taskQueue.erase(taskQueue.begin() + j);
                        }
                    }
                    
                    taskMutex.unlock();
                }

            }

            unique_lock<mutex> lock(taskMutex);
            thread_c.wait(lock, [this] {return !taskQueue.empty(); });
        }
    }
public:
    Th_pool(int th_size) : numThreads(th_size) {
        int tmp = MAX_THREADS;
        if (numThreads > tmp) {
            numThreads = tmp;
        }
        Pool.reserve(numThreads);
        for (int i = 0; i != numThreads; ++i) {

            Pool.emplace_back(thread(&Th_pool::thread_while, this));
            Pool.back().detach();
        }



    }
    shared_ptr<task> push_task(function<void(int)> func, int info) {
        unique_lock<mutex> lock(taskMutex);
        task tt(func, info);
        
        taskQueue.push_back(std::make_shared<task>(std::move(tt)));
        thread_c.notify_one();
        return taskQueue.back();
    }
    string statys(shared_ptr<task> tas) {

        string rr = (*tas).statys();

        return rr;
    }


};


