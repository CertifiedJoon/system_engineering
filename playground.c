typedef struct __lock_t {
  int flag;
} lock_t;

void lock(lock_t *lock) {
  while (compareAndSwap(lock->flag, 0, 1) == 1);
}

int compareAndSwap(int *flag, int expected, int newVal) {
  int oldVal = *flag;
  if (*flag == expected) {
    *flag = newVal;
  }

  return oldVal;
}

void unlock(lock_t *lock) {
  int flag = 0;
}