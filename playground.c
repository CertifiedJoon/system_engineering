typedef struct __lock_t {
  int flag;
} lock_t;

int linkedloaded(lock_t *lock){
  return lock->flag;
}

int storeConditional(int* ptr, int value) {
  if (/*No change to linked address*/){
    *ptr = value;
    return 1;
  } else {
    return 0;
  }
  
}

void lock (lock_t *lock) {
  while (LoadLinked(&lock->flag) || !StoreConditional(&lock->flag, 1));
}
