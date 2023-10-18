#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unitsd.h>

typedef struct __lock_t {
  int turn;
  int ticket;
} lock_t;


int fetchTicket(lock_t *lock) {
  return lock->ticket++;
}

int isMyTurn(lock_t *lock, int ticket) {
  if (ticket == lock->turn){
    lock->turn++;
    return true;
  } else {
    return false;
  }
}


int thread(){
  
  
  int ticket = fetchTicket(lock);
  while (isMyTurn);
  
}