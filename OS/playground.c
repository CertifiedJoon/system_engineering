int loadLinked(int *ptr) {
  return *ptr;
}

int StoreConditional(int *ptr, int value) {
  if (no update to *ptr since loadlinked) {
    *ptr  = value;
    return 1
  } else {
    return 0;
  }
}

void lock(lock_t *lock) {
  while (Loadlinked(&lock->flag) || !StoreConditional(&lock->flag, 1))
  ;
}