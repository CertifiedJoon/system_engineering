#include <stdio.h>
#include <stdlib.h>

typedef struct __lock_t {
  int lock;
} lock_t;

int testAndSet(int *old, int new){
  int old_val = *old;
  *old = new;
  return old_val;
}

int unlock(lock_t *lock){
  lock->lock = 0;
}


int lock(lock_t *lock){
  while(testAndSet(lock->lock, 1) == 1);

  unlock();
}
