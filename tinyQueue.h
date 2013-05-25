/*
	Copyright (c) Gareth Scott 2011, 2013

*/

#ifndef __TINYQUEUE_H__
#define __TINYQUEUE_H__

// Can be used as a stack by only calling push_front(), pop_front(), preventing the _back index from changing.
template<typename T>
class tinyQueue
{
public:
#define TINY_QUEUE_SIZE (256)
    tinyQueue(): _front(TINY_QUEUE_SIZE-1), _back(TINY_QUEUE_SIZE-1) {}
    tinyQueue(int startIndex): _front(startIndex), _back(startIndex) {}
    ~tinyQueue() {}
    T& operator=(const T& p) {if (*this != p) {_front = p._front; _back = p._back; for (int i = 0; i < TINY_QUEUE_SIZE; ++i) {_tq[i] = p._tq[i];}} return *this;}
    unsigned int size(void) const {return TINY_QUEUE_SIZE - spaceAvail() - 1;} // Buffer can hold up to (TINY_QUEUE_SIZE - 1) elements
    bool empty(void) const {if (size() == 0) {return true;} return false;}
    // Removed 'push()' to make sure nobody is using this as a queue
    void push(T p) {if (spaceAvail() == 0) {return;} _tq[_back] = p; _back += 1; if (_back >= TINY_QUEUE_SIZE) {_back = 0;}}
    void push_front(T p) {if (spaceAvail() == 0) {return;} if (_front == 0) {_front = TINY_QUEUE_SIZE - 1;} else {_front -= 1;} _tq[_front] = p;}
    void pop_front() {if (empty()) {return;} _front += 1; if (_front >= TINY_QUEUE_SIZE) {_front = 0;}}
    
    void clear(void) {_front = _back = 0;} 
    unsigned int spaceAvail(void) const {if (_back >= _front) {return TINY_QUEUE_SIZE - 1 - _back + _front;} {return _front - _back - 1;}}
    T front(void) const {return _tq[_front];} 

    // Allows explicit set/get of stack frame pointer (i.e. _front). Used to set up stack frames for function calls.
    unsigned int stackFrameIndex(void) const {return _front;}
    void stackFrameIndex(const unsigned int setFront) {_front = setFront;}
    int peekAtIndex(const unsigned int index) const {return _tq[index];}
    T* getBuffer(void) {return _tq;}
    
private:
    T _tq[TINY_QUEUE_SIZE];
    unsigned int _front, _back; // front and back indices
};

#endif /* __TINYQUEUE_H__ */
