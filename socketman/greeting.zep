%{
// top statement before namespace, add to after headers
#define MAX_FACTOR 40
}%

namespace Socketman;

%{
// top statement before class, add to after headers
// test include .h
#include "kernel/require.h"
#include <ev.h>
#include "lib/include/test.h"
#include <pthread.h>
}%

%{
    //define variable
    ev_io stdin_watcher;
    ev_timer timeout_watcher;
}%

%{

// c implementation of fibonacci
static long fibonacci(long n) {
	if (n < 2) return n;
	else return fibonacci(n - 2) + fibonacci(n - 1);
}

/*static long sum(int a , int b){
	return a + b;
}*/

static void *run(void *arg){
    sleep(20);
    printf("thread id = %ld\n" , pthread_self());
}

static void start(){
    int i = 0;
    pthread_t threads[5];
    for(i = 0 ; i<5; i++){
        pthread_create(&threads[i] , NULL , run , NULL);
    }
    for(i =0 ;i < 5 ;i++){
        pthread_join(threads[i] , NULL);
    }
}

static void stdin_cb(EV_P_ ev_io *w , int revents){
    puts("stdin ready");
    ev_io_stop(EV_A_ w);
    ev_break(EV_A_ EVBREAK_ALL);
}

static void timeout_cb (EV_P_ ev_timer *w, int revents){
     puts ("timeout");
     // this causes the innermost ev_run to stop iterating
     ev_break (EV_A_ EVBREAK_ONE);
   }

   int start_ev (void)
   {
     // use the default event loop unless you have special needs
     struct ev_loop *loop = EV_DEFAULT;

     // initialise an io watcher, then start it
     // this one will watch for stdin to become readable
     ev_io_init (&stdin_watcher, stdin_cb, /*STDIN_FILENO*/ 0, EV_READ);
     ev_io_start (loop, &stdin_watcher);

     // initialise a timer watcher, then start it
     // simple non-repeating 5.5 second timeout
     ev_timer_init (&timeout_watcher, timeout_cb, 5.5, 0.);
     ev_timer_start (loop, &timeout_watcher);

     // now wait for events to arrive
     ev_run (loop, 0);

     // break was called, so exit
     return 0;
   }

}%

class Greeting
{
    public static function say()
    {
        echo "hello,hellohello,hellohellohellohello,hello world!\n";
    }

    public static function start(string ip , int port)
    {
        %{
            start_chat(ip , port);
        }%
        return 10;
    }

    public static function fibonacci(int a){
    	print_r(a);
    	long result = 0;
    	%{
    		result = fibonacci(a);
    	}%
    	 return result;
    }

    public static function sum(int a , int b){
    	long result = 0;
    	%{
    		result = sum(a , b);
    	}%
    	 return result;
    }
}