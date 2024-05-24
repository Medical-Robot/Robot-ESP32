#ifndef __MOVING_AVERAGE_H__
#define __MOVING_AVERAGE_H__

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

class MovingAverage {
  int size, head = 0, count = 0;
  float windowSum = 0.0f;
  float* queue;

public:
MovingAverage(int size = 10) {
    this->size = size;
    queue = new float[size];
    for (int i = 0; i < size; i++) {
      queue[i] = 0.0f;
    }
    
  }

float next(float val) {
    ++count;
    // calculate the new sum by shifting the window
    int tail = (head + 1) % size;
    windowSum = windowSum - queue[tail] + val;
    // move on to the next head
    head = (head + 1) % size;
    queue[head] = val;
    return ((float)windowSum) / ((float)MIN(size, count));
  }

  ~MovingAverage(){
    delete this->queue;
  }
};


#endif