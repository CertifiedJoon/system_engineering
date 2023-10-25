pthread_create(&thread, NULL, mythread, (void *)(100))
pthread_join(thread, (void **)rval)