
typedef struct __lock_t {
  int flag;
} lock_t;

void init (lock_t *lock) {
  lock->flag = 0;
}

int TestAndSet(int *ptr, int new){
  int original = *ptr;
  *ptr = new;
  return original;
}

void lock (lock_t *lock) {
  while (TestAndSet(lock->flag, 1) == 1);
}

void unlock(lock_t *lock) {
  lock->flag = 0;
}