namespace Socketman;
%{
// top statement before class, add to after headers
// test include .h
#include "kernel/require.h"
#include <ev.h>
#include "lib/include/test.h"
#include <pthread.h>
}%

class Greeting {
    public static function say() {
      echo "hello,hellohello,hellohellohellohello,hello world!\n";
    }

    public  static function start(array !descriptor) {
        var ip, port;
        if fetch ip, descriptor["ip"] { 
          unset descriptor["ip"]; 
        } else {
          let ip = null;
        }
        /**
         * Check for a password or use null as default
         */
        if fetch port, descriptor["port"] {
          unset descriptor["port"]; 
        } else {
          let port = null;
        }
      %{ start_chat(Z_STRVAL_P(ip), Z_LVAL_P(port)); }% 
        return 10;
      }

    public static function sum(int a, int b) {
        long result = 0;
        %{ result = sum(a, b); }% 
        return result;
    }
}