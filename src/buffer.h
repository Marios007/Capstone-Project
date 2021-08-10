#include <mutex>
#include <deque>
#include <atomic>
#include <condition_variable>

// template class to use a buffer with any type
template <typename T> class Buffer
{
public:
    void addData(T &&data);
    T pop();
    void SetEndOfFile(bool status);
    bool EndOfBuffer();

private:
    std::deque<T> _dequeue;
    std::mutex _mtx;
    std::condition_variable _condition;
    std::atomic<bool> _endOfFile{false};
    std::atomic<bool> _endOfBuffer{false};
};

// add Data to the buffer
template <typename T> void Buffer<T>::addData(T &&data)
{
    std::lock_guard<std::mutex> lock(_mtx);
    
    _dequeue.emplace_back(std::move(data));
    _condition.notify_one();
}

// pop off data from the buffer
template <typename T> T Buffer<T>::pop()
{
    std::unique_lock<std::mutex> lock(_mtx);
    _condition.wait(lock, [this]() { return !_dequeue.empty(); });

    auto dataToReturn = std::move(_dequeue.front());
    _dequeue.pop_front();
    _endOfBuffer = _dequeue.empty() && _endOfFile;

    return dataToReturn;
}

template <typename T> void Buffer<T>::SetEndOfFile(bool status)
{
    _endOfFile = status;
}

template <typename T>bool Buffer<T>::EndOfBuffer()
{
    return _endOfBuffer;
}
