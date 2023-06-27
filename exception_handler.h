//
// Created by Sancho on 2023/6/27.
//

#ifndef TRY_CATCH_BLOCK_EXCEPTION_HANDLER_H
#define TRY_CATCH_BLOCK_EXCEPTION_HANDLER_H


#include <stdio.h>
#include <setjmp.h>
#include <pthread.h>

typedef struct exception_s{
    const char* name;

}exception;

exception runtime_exception={.name= "runtime exception"};
exception biz_exception={.name= "biz exception"};
exception illegal_argument_exception={.name= "illegal arguments exception"};
exception arithmetic_exception={.name= "arithmetic exception"};


typedef   struct exception_node_s{
    jmp_buf env;
    int line ;
    const  char * file;
    const char * func;
    exception* ex;
    struct exception_node_s * prev;
} exception_node;

enum {
    enter_try=0,
    after_thrown,
    enter_catch,
    finalize
};
//isolate exception stack of each thread
pthread_key_t  key;
#define get_ctx_exception(key) (exception_node *)pthread_getspecific((key))
#define set_ctx_exception(key,value)  pthread_setspecific((key),(value))
#define create_ctx_exception(key) pthread_key_create(&(key),NULL)
#define pop_ctx_exception_node set_ctx_exception(key,(get_ctx_exception(key))->prev)







#define THROW(e) throw_exception(&(e),__FILE__,__func__,__LINE__)
#define RETHROW throw_exception(node.ex,node.file,node.func,node.line)




#define TRY \
do{   \
          volatile int flag;  \
          exception_node node; \
          node.prev= get_ctx_exception(key); \
          set_ctx_exception(key,&node);       \
          flag= setjmp(node.env);              \
          if (flag==enter_try){                 \

#define CATCH(e) \
              if (flag==enter_try) pop_ctx_exception_node; \
          }   \
          else if (&(e)==node.ex){                   \

#define FINALLY  \
             if (flag==enter_try) pop_ctx_exception_node;      \
           }                                                    \
           {                  \
              if (flag==enter_try) flag=finalize;   \



#define ENDTRY \
               if (flag==enter_try) pop_ctx_exception_node;    \
           } \
           if (flag==after_thrown) RETHROW; \
}while(0)  \


void throw_exception(exception * ex,const char * file ,const char * func,int line ){
    exception_node*  node=get_ctx_exception(key);

    if (node){
        node->file=file;
        node->func=func;
        node->line=line;
        node->ex=ex;

        pop_ctx_exception_node;

        longjmp(node->env,after_thrown);

    } else{
        printf("%s,  raised in %s at %s:%d\n", ex->name,  func ? func : "?", file ? file : "?", line);
    }
}

static pthread_once_t once_control = PTHREAD_ONCE_INIT;

static void init_once() {
    create_ctx_exception(key);
}


void thread_init() {
    pthread_once(&once_control, init_once);
}

#define __enable_try_catch__ thread_init();


#endif //TRY_CATCH_BLOCK_EXCEPTION_HANDLER_H
