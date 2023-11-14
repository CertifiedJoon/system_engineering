/*
PLEASE WRITE DOWN FOLLOWING INFO BEFORE SUBMISSION
* FILE NAME:
* NAME:
* UID :
* Development Platform:
* Remark: (How much you implemented?)
* How to compile: (gcc -o llama2_[UID] llama2_[UID].c utilities.c -O2 -pthread -lm)

Please download the model and tokenizer to the same folder:
$ wget -O model.bin https://huggingface.co/huangs0/llama2.c/resolve/main/model.bin
$ wget -O tokenizer.bin https://huggingface.co/huangs0/llama2.c/resolve/main/tokenizer.bin

In compile, remember to add `-pthred` to link library:
$ gcc -o llama2_[UID] llama2_[UID].c utilities.c -O2 -pthread -lm

Then Run with:
$ ./llama2_[UID] <seed> <thr_count>
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "utilities.h"

/**
 * ----------------------------------------------------------------------------
 * TASK - Optimize Matrix-Vector Multiplication by Multi-Threading
 * 
 * Matrix-Vector Multiplication, used in Attention and Feed-Forward Network
 * is the most time-consuming part of GPT. Luckily, most of computation is 
 * independent of each row, so we can use Multi-Threading for acceleration.
 * 
 * Please use <pthread.h> and your favorite synchronization method,
 * semaphore / mutex lock + conditional variable
 * 
 * A sequential version is provided in seq.c, please modify it to parallel version.
*/

// YOUR CODE STARTS HERE

// Addtional Header File Here
#include <pthread.h>
#include <semaphore.h>

// Global Variables
struct rusage main_usage; // get usage for main thread
struct rusage* child_usages;

typedef struct __mult_vec_param {
    float* vec;
    float* mat;
    int col;
    int row;
    int rows_per_thread;
    float* val;
} mult_vec_param;

pthread_t* threads;
int n = 0;
mult_vec_param* params;

sem_t* child;
sem_t parent;
sem_t sem_param;
sem_t sem_term;

int terminated = 0;


void *thr_func(void *arg) {
    int thread_index = *((int*) arg);
    sem_wait(&sem_param);
    mult_vec_param* p = &(params[thread_index]);
    sem_post(&sem_param);

    while (terminated == 0) {
        sem_wait(&child[thread_index]);

        sem_wait(&sem_term);
        if (terminated == 1){
            sem_post(&sem_term);
            break;
        }
        sem_post(&sem_term);

        sem_wait(&sem_param);
        float *vec = p->vec;
        float *mat = p->mat;
        int row = p->row;
        int col = p->col;
        int rows_per_thread = p->rows_per_thread;
        sem_post(&sem_param);

        for (int i = 0; i < rows_per_thread; i++) {
            float sum = 0.0f;
            int row_index = thread_index * rows_per_thread + i;
            
            if (row_index < row){
                for (int j = 0; j < col; j++){
                    int mat_index = row_index * col + j;
                    sum += mat[mat_index] * vec[j];
                }
                p->val[row_index] = sum;
            }
        }
        
        sem_post(&parent);
    }
    getrusage(RUSAGE_THREAD, &child_usages[thread_index]);
    free(arg);
}

int init_mat_vec_mul(int thr_count) {
    sem_init(&parent, 0, 0);
    sem_init(&sem_param, 0, 1);
    sem_init(&sem_term, 0, 1);

    n = thr_count;

    child = (sem_t *) malloc (sizeof(sem_t) * thr_count);
    for (int i = 0; i < thr_count; i++) {
        sem_init(&child[i], 0, 0);
    }

    threads = (pthread_t*) malloc (sizeof(pthread_t) * thr_count);
    params = (mult_vec_param*) malloc (sizeof(mult_vec_param) * thr_count);
    child_usages = (struct rusage*) malloc (sizeof(struct rusage) * thr_count);

    for (int i = 0; i < thr_count; i++) {
        int *j = (int *) malloc (sizeof(int));
        *j = i;
        pthread_create(&threads[i], NULL, thr_func, (void *) j);
    }

    return 0;
}

void mat_vec_mul(float* out, float* vec, float* mat, int col, int row) {
    int rows_per_thread;

    if (row % n == 0) {
        rows_per_thread = row / n;
    } else {
        rows_per_thread = (row / n) + 1;
    }

    for (int i = 0; i < n; i++) {
        params[i].vec = vec;
        params[i].mat = mat;
        params[i].row = row;
        params[i].col = col;
        params[i].rows_per_thread = rows_per_thread;
        params[i].val = out;
    }    

    for (int i = 0; i < n; i++) {
        sem_post(&child[i]);
    }

    for (int i = 0; i < n; i++) {
        sem_wait(&parent);
    }
}

int close_mat_vec_mul() {
    sem_wait(&sem_term);
    terminated = 1;
    sem_post(&sem_term);

    for (int i = 0; i < n; i++)
        sem_post(&child[i]);

    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    getrusage(RUSAGE_SELF, &main_usage);
    
    for (int i = 0; i < n; i++) {
        float child_u_time = child_usages[i].ru_utime.tv_sec + child_usages[i].ru_utime.tv_usec / 1000000.0;
        float child_s_time = child_usages[i].ru_stime.tv_sec + child_usages[i].ru_stime.tv_usec / 1000000.0;
        printf("Thread %d has completed - user: %f s, system : %f s\n", i, child_u_time, child_s_time);
    }

    free(threads);
    free(params);

    sem_wait(&sem_term);
    terminated = 1;
    sem_post(&sem_term);

    float main_u_time = main_usage.ru_utime.tv_sec + main_usage.ru_utime.tv_usec / 1000000.0;
    float main_s_time = main_usage.ru_stime.tv_sec + main_usage.ru_stime.tv_usec / 1000000.0;
    printf("main thread - user: %f s, system : %f s\n", main_u_time, main_s_time);

    for (int i = 0; i < n; i++) {
        sem_destroy(&child[i]);
    }
    free(child);
    sem_destroy(&parent);
    sem_destroy(&sem_param);
    sem_destroy(&sem_term);
    return 0;
}


// YOUR CODE ENDS HERE
int transformer(int token, int pos, LLMConfig* p, LLMRuntime* s, LLMWeight* w) {
    
    // a few convenience variables
    int dim = p->dim, hidden_dim =  p->hidden_dim, head_size = p->dim / p->n_heads;

    // copy the token embedding into x
    memcpy(s->x, &(w->token_embedding_table[token * dim]), dim*sizeof(float));

    // forward all the layers
    for(int l = 0; l < p->n_layers; l++) {

        // Attention
        {
            // attention normalization
            normalize(s->xb, s->x, w->rms_att_weight + l*dim, dim);

            // q, k, v = w_q @ x, w_k @ x, w_v @ x, respectively
            mat_vec_mul(s->q, s->xb, w->wq + l*dim*dim, dim, dim);
            mat_vec_mul(s->k, s->xb, w->wk + l*dim*dim, dim, dim);
            mat_vec_mul(s->v, s->xb, w->wv + l*dim*dim, dim, dim);

            // apply positional embedding
            position_embedding(s->q, s->k, w, pos, p->dim, p->n_heads);

            // save intermediate result for later reference
            key_value_cache(l, pos, p, s);
            
            // attention calculation
            attention(l, pos, p, s, w);

            // wo @ x to get final result
            mat_vec_mul(s->xb2, s->xb, w->wo + l*dim*dim, dim, dim);

            // residual connection back into x
            accum(s->x, s->xb2, dim);
        }
    
        // Feed-Forward Network: w2 @ (silu(w1 @ x) * (w3 @ x)), * is element-wise multiply
        {
            // FFN Normalization
            normalize(s->xb, s->x, w->rms_ffn_weight + l*dim, dim);

            // w1 @ x
            mat_vec_mul(s->h1, s->xb, w->w1 + l*dim*hidden_dim, dim, hidden_dim);
            // silu(w1 @ x)
            silu(s->h1, hidden_dim);
            // w3 @ x
            mat_vec_mul(s->h2, s->xb, w->w3 + l*dim*hidden_dim, dim, hidden_dim);
            // silu(w1 @ x) * (w3 @ x)
            element_wise_mul(s->h1, s->h2, hidden_dim);
            // w2 @ (silu(w1 @ x) * (w3 @ x))
            mat_vec_mul(s->xb, s->h1, w->w2 + l*dim*hidden_dim, hidden_dim, dim);

            // residual connection
            accum(s->x, s->xb, dim);
        }
    }
    
    // final normalization
    normalize(s->x, s->x, w->rms_final_weight, dim);
    // classifier into logits
    mat_vec_mul(s->logits, s->x, w->token_embedding_table, p->dim, p->vocab_size);
    // apply the temperature to the logits
    for (int q=0; q<p->vocab_size; q++) { s->logits[q] /= 0.9f; }
    // apply softmax to the logits to get the probabilities for next token
    softmax(s->logits, p->vocab_size);
    // now sample from this distribution to get the next token
    return sample(s->logits, p->vocab_size);
}

int main(int argc, char* argv[]) {

    unsigned int seed;
    int thr_count;

    if (argc == 3) {
        seed = atoi(argv[1]);
        thr_count = atoi(argv[2]);
    } else {
        printf("Usage: ./compiled <seed> <thr_count>\n");
        return 1;
    }

    // Initialize
    srand(seed);
    init_mat_vec_mul(thr_count);

    // load model
    LLMConfig config;
    LLMWeight weights;
    if (load_LLM_Config_Weight(&config, &weights) == 1) { return 1; }

    // load tokenizer
    char** vocab = malloc(config.vocab_size * sizeof(char*));
    if (load_tokenizer(vocab, config.vocab_size) == 1) { return 1; }

    // create and init the application LLMRuntime
    LLMRuntime state;
    malloc_LLMRuntime(&state, &config);
    
    // the current position we are in
    long start = time_in_ms();

    int next, token = 1, pos = 0; // token = 1 -> <START>
    while (pos < config.seq_len) {

        // forward the transformer to get logits for the next token
        next = transformer(token, pos, &config, &state, &weights);

        printf("%s", vocab[next]);
        fflush(stdout); // force print

        token = next;
        pos++;
    }

    long end = time_in_ms();
    printf("\n\nlength: %d, time: %f s, achieved tok/s: %f\n", config.seq_len, (double)(end-start)/1000, config.seq_len / (double)(end-start)*1000);

    // cleanup
    close_mat_vec_mul();
    free_LLMRuntime(&state);
    free_LLMWeight(&weights);
    for (int i = 0; i < config.vocab_size; i++) { free(vocab[i]); }
    free(vocab);
    return 0;
}