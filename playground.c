typedef struct __lock_t {
  int turn;
  int ticket;
} lock_t;

int fetchTicket(lock_t *lock){
  return (lock->ticket)++;
}

int isCorrectTurn(lock_t *lock, int ticket) {
  return lock->turn == ticket;
}

