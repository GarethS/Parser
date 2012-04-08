/*
	Copyright (c) Gareth Scott 2011

*/

#ifndef __TINYQUEUE_H__
#define __TINYQUEUE_H__


template<typename T>
class tinyQueue
{
public:
#define TINY_QUEUE_SIZE (64)
    tinyQueue(): _front(0), _back(0) {}
    ~tinyQueue() {}
    T& operator=(const T& p) {if (*this != p) {_front = p._front; _back = p._back; for (int i = 0; i < TINY_QUEUE_SIZE; ++i) {_tq[i] = p._tq[i];}} return *this;}
    unsigned int size(void) {return TINY_QUEUE_SIZE - spaceAvail() - 1;} // Buffer can hold up to (TINY_QUEUE_SIZE - 1) elements
    bool empty(void) {if (size() == 0) {return true;} return false;}
    void push(T p) {if (spaceAvail() == 0) {return;} _tq[_back] = p; _back += 1; if (_back >= TINY_QUEUE_SIZE) {_back = 0;}}
    void push_front(T p) {if (spaceAvail() == 0) {return;} if (_front == 0) {_front = TINY_QUEUE_SIZE - 1;} else {_front -= 1;} _tq[_front] = p;}
    void pop_front() {if (empty()) {return;} _front += 1; if (_front >= TINY_QUEUE_SIZE) {_front = 0;}}
    
    void clear() {_front = _back;} 
    unsigned int spaceAvail(void) {if (_back >= _front) {return TINY_QUEUE_SIZE - 1 - _back + _front;} {return _front - _back - 1;}}
    T front() const {return _tq[_front];}         
    
private:
    T _tq[TINY_QUEUE_SIZE];
    unsigned int _front, _back; // front and back indices
};

#endif /* __TINYQUEUE_H__ */
