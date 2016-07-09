#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
 
using namespace std;

template <typename T>
class Queue{
 public:
 
  T pop(){
    unique_lock<mutex> mlock(m_mutex);
    while (m_queue.empty()){
      m_cond.wait(mlock);
    }
    auto item = m_queue.front();
    m_queue.pop();
    return item;
  }
 
  void pop(T& item){
    unique_lock<mutex> mlock(m_mutex);
    while (m_queue.empty()){
      m_cond.wait(mlock);
    }
    item = m_queue.front();
    m_queue.pop();
  }
 
  void push(const T& item){
    unique_lock<mutex> mlock(m_mutex);
    m_queue.push(item);
    mlock.unlock();
    m_cond.notify_one();
  }
 
  void push(T&& item){
    unique_lock<mutex> mlock(m_mutex);
    m_queue.push(move(item));
    mlock.unlock();
    m_cond.notify_one();
  }

  int size(){
    unique_lock<mutex> mlock(m_mutex);
    int s = m_queue.size();
    mlock.unlock();
    return s;
  }
 
 private:
  queue<T> m_queue;
  mutex m_mutex;
  condition_variable m_cond;
};
