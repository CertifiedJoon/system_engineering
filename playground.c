typedef struct __lock_t{
  int turn;
  int ticket;
} lock_t;

int fetchTicket(lock_t *lock) {
  int t = lock->ticket;
  lock->ticket += 1;
  return t;
}

int checkMyTurn(lock_t *lock, int ticket){
  if (lock->turn == ticket){
    return 1;
  }
  return 0;
}
