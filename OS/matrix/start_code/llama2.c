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
    float val;
} mult_vec_param;

pthread_t* threads;
int n = 0;
mult_vec_param* params;

sem_t child;
sem_t parent;

int terminated = 1;

void *thr_func(void *arg) {
    int k = 0;
    while (terminated == 1) {
        sem_wait(&child);
        if (terminated == 1){
            mult_vec_param* p = (mult_vec_param*) arg;
            float *vec = p->vec;
            float *mat = p->mat;
            int i = p->row;
            k = i;
            int col = p->col;

            for (int j = 0; j < col; j++) {
                p->val += mat[i * col + j] * vec[j];
            }
        }
        sem_post(&parent);
    }

    getrusage(RUSAGE_THREAD, &child_usages[k]);
}

int init_mat_vec_mul(int thr_count) {
    printf("Init\n");
    sem_init(&child, 0, 0);
    sem_init(&parent, 0, 0);
    n = thr_count;
    threads = (pthread_t*) malloc (sizeof(pthread_t) * n);
    params = (mult_vec_param*) malloc (sizeof(mult_vec_param) * n);
    child_usages = (struct rusage*) malloc (sizeof(struct rusage) * n);

    for (int i = 0; i < n; i++) {
        pthread_create(&threads[i], NULL, thr_func, (void *) &params[i]);
    }

    return 0;
}

void mat_vec_mul(float* out, float* vec, float* mat, int col, int row) {
    int k = row / n;
   
    for (int i = 0; i <= k; i++) {
        for (int j = 0; j < n && i*n + j < row; j++) {
            params[j].vec = vec;
            params[j].mat = mat;
            params[j].row = i;
            params[j].col = col;
            params[j].val = 0.0f;
        }

        for (int j = 0; j < n; j++) {
            sem_post(&child);
        }

        for (int j = 0; j < n; j++) {
            sem_wait(&parent);
        }

        for (int j = 0; j < row; j++) {
            out[i * n + j] = params[j].val;
        }
    }    

    getrusage(RUSAGE_THREAD, &main_usage);
}

int close_mat_vec_mul() {
    terminated = 0;
    for (int i = 0; i < n; i++) {
        sem_post(&child);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < n; i++) {
        float child_u_time = child_usages[i].ru_utime.tv_sec + child_usages[i].ru_utime.tv_usec / 1000000;
        float child_s_time = child_usages[i].ru_stime.tv_sec + child_usages[i].ru_stime.tv_usec / 1000000;
        printf("Thread %d has completed - user: %f s, system : %f s\n", i, child_u_time, child_s_time);
        terminated = 1;
    }
    free(threads);
    free(child_usages);
    free(params);
    sem_destroy(&child);
    sem_destroy(&parent);

    float main_u_time = main_usage.ru_utime.tv_sec + main_usage.ru_utime.tv_usec / 1000000;
    float main_s_time = main_usage.ru_stime.tv_sec + main_usage.ru_stime.tv_usec / 1000000;
    printf("main thread - user: %f s, system : %f s\n", main_u_time, main_s_time);

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