<?php
Socketman\Greeting::say() . "\n";
echo Socketman\Greeting::fibonacci(2) . "\n";
echo Socketman\Greeting::sum(20, 80) . "\n";
echo Socketman\Greeting::start("127.0.0.1", 8080) . "\n";

sleep(10);