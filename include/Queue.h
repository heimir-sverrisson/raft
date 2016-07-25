#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
 
using namespace std;

template <typename T>
class Queue{
 public:
 
  T pop(){
    unique_lock<mutex> mlock(mutex_);
    while (queue_.empty()){
      cond_.wait(mlock);
    }
    auto item = queue_.front();
    queue_.pop();
    return item;
  }
 
  void pop(T& item){
    unique_lock<mutex> mlock(mutex_);
    while (queue_.empty()){
      cond_.wait(mlock);
    }
    item = queue_.front();
    queue_.pop();
  }
 
  void push(const T& item){
    unique_lock<mutex> mlock(mutex_);
    queue_.push(item);
    mlock.unlock();
    cond_.notify_one();
  }
 
  void push(T&& item){
    unique_lock<mutex> mlock(mutex_);
    queue_.push(move(item));
    mlock.unlock();
    cond_.notify_one();
  }

  int size(){
    unique_lock<mutex> mlock(mutex_);
    int s = queue_.size();
    mlock.unlock();
    return s;
  }
 
 private:
  queue<T> queue_;
  mutex mutex_;
  condition_variable cond_;
};
