
typedef struct __lock_t {
  int flag;
} lock_t;

void init (lock_t *lock) {
  lock->flag = 0;
}

int StoreConditional(int *ptr, int value) {
  if (*ptr == 0) {
    *ptr = value;
    return 1
  } else {
    return 0;
  }
}

void lock(lock_t *lock) {
  while (LoadLinked(lock->flag) || !StoreConditional(lock->flag, 1));
}

void unlock(lock_t *lock) {
  lock->flag = 0;
}