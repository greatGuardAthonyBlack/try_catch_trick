#include <stdio.h>
#include "exception_handler.h"

#define TEST_THREAD_NUMS 4

exception exception_vectors [TEST_THREAD_NUMS]={
        runtime_exception,
       biz_exception,
        illegal_argument_exception,
        arithmetic_exception
};


void* threads_runtime(void * args){
    __enable_try_catch__
    exception ex=*((exception*)args);
    // use macro as c/c++ keyword
    TRY{
        THROW(ex);
    }
    CATCH(ex){
        printf("exception: %s\n",ex.name);

    }FINALLY{

    }ENDTRY;
}

int main() {

    int i = 0;
    pthread_t threads[TEST_THREAD_NUMS];

    for (i = 0;i < TEST_THREAD_NUMS;i ++) {
        pthread_create(&threads[i], NULL, threads_runtime, &exception_vectors[i]);
    }

    for (i = 0;i < TEST_THREAD_NUMS;i ++) {
        pthread_join(threads[i], NULL);
    }

}
