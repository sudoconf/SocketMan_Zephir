<?php
Socketman\Greeting::say() . "\n";
echo Socketman\Greeting::sum(20, 80) . "\n";
//echo Socketman\Greeting::start(array("ip" => "127.0.0.1", 'port' => 9090)) . "\n";
$test = new Socketman\Test();
$test->say(" kkkk") . "\n";
$test->on("aaaa" , function(){
    echo "aaaaaaaaa";
},null);
