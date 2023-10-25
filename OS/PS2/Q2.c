/* this is the print queue buffer; each slot stores a request of type request_t */
request_t pqReqBuff[N];
/* this is for counting the number of requests in the print queue buffer */
int pqReqCount = 0;
/* this is the data type of the print buffer for temporary storing of the output file */
buffer_t pBuffer;
/* enqueue() - for adding a new request to the print queue buffer */
/* It updates the pqReqBuff array and pqReqCount variable, so MUST be called after getting the guard
lock, i.e., should be called inside the critical section */
void enqueue(request_t req);
/* dequeue() - for getting (& removing) a request from the print queue buffer */
/* It updates the pqReqBuff array and pqReqCount variable, so MUST be called after getting the guard
lock, i.e., should be called inside the critical section */
void dequeue(request_t *req);
/* process() - for processing the print request to generate the output file of type buffer_t; should be called
outside the critical section */
void process(request_t req, buffer_t *output);
/* output() - for adding the output file to the printer buffer */
/* It updates the pBuffer variable, so MUST be called after getting the guard lock, i.e., should be called
inside the critical section */
void output(buffer_t output);
/* retrieve() - for getting (& removing) the output file from the print buffer */
/* It updates the pBuffer variable, so MUST be called after getting the guard lock, i.e., should be called
inside the critical section */
void retrieve(buffer_t *output);
/* printing() - for sending the output file to the hardware (printer); should be called outside the critical
section */
void printing(buffer_t output); 