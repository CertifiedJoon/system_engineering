int FetchAndAdd(int *ptr) {
  int old = *ptr;
  *ptr = old + 1;
  return old;
}

typedef struct __lock_t {
  int *turn;
  int *ticket;
} lock_t;

void lock(lock_t *lock) {
  int myturn = FetchAndAdd(&lock->ticket);
  while (lock->turn != myturn);
}

void unlock(lock_t *lock) {
  lock->turn = lock->turn + 1;
}