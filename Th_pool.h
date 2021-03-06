#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <queue>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <time.h>
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
                Th_id.resize(numThreads);
                for (int i = tmp; i != numThreads; ++i) {
                    
                    Pool.emplace_back(move(thread(&Th_pool::thread_while, this)));
                    Th_id.emplace_back(Pool.back().get_id());
                    Pool.back().detach();
                }
            }
            else {
                


                
               numThreads = newCount;
               Pool.resize(newCount);
               Th_id.resize(newCount);
            }
        }
    }

private:

    class task {
    private:
        function<void(int)> func;
        int info;
        string stat;
        bool trig = 1;
    public:
        ~task(){}
        task(function<void(int)> ff, int inf) {
            func = ff;
            info = inf;
            stat = "in the queue";
            
        }
        void performed_f() {
            trig = 0;
            stat = "performed";
            trig = 1;
        }
        void completed_f() {
             trig = 0;
             stat = "completed";
             trig = 1;
        }
        void execute() {
            
            func(info);     
           
           
           
        }
        string statys() {
            while (!trig) {
                
            }
            return stat;
        }
    };
    int numThreads; 
    vector<thread> Pool;
    vector<thread::id> Th_id;
    vector<shared_ptr<task>> taskQueue;
    condition_variable thread_c;
    mutex taskMutex; 
    
    void thread_while() {
        srand(0);
        while (true) {

            


            

            for (int i = 0; i < taskQueue.size(); i++) {
                shared_ptr<task> cc = taskQueue[i];
                string rr = this->statys(cc);
                
                    if (rr == "in the queue") {
                        //shared_ptr<task> cc = taskQueue[i];
                        taskMutex.lock();
                        (*cc).performed_f();
                        taskMutex.unlock();
                        (*cc).execute();
                        
                        (*cc).completed_f();

                        //for (int j = 0; j < taskQueue.size(); j++) {

                        //    if (taskQueue[j] == cc) {
                        //        taskQueue.erase(taskQueue.begin() + j);
                        //    }
                        //}

                      
                    }
               
            }

            unique_lock<mutex> lock(taskMutex);
            thread_c.wait(lock, [this] {return !taskQueue.empty(); });

            bool tr = 1;
            
            for (int i = 0; i < Th_id.size(); i++) {
                
                if (Th_id[i] == this_thread::get_id()) {
                    tr = 0;
                    break;
                }

            }
            if (tr) {
                break;
            }
        }
    }
public:
    Th_pool(int th_size) : numThreads(th_size) {
        int tmp = MAX_THREADS;
        if (numThreads > tmp) {
            numThreads = tmp;
        }
        Pool.reserve(numThreads);
        Th_id.reserve(numThreads);
        for (int i = 0; i != numThreads; ++i) {

            Pool.emplace_back(thread(&Th_pool::thread_while, this));
            Th_id.emplace_back(Pool.back().get_id());
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

    ~Th_pool() {
        this_thread::sleep_for(chrono::milliseconds(1000));
        while (this->statys(taskQueue[taskQueue.size()-1]) != "completed") {

        }
        Th_id.resize(0);
        this_thread::sleep_for(chrono::milliseconds(1000));

    }
};


