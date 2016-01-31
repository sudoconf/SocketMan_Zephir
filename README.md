# SocketMan
SocketMan是一款Zephir + Libev 开发的高性能的PHP Socket服务器扩展框架。

#环境要求
* *nux 
* PHP >= 5.5
* Zephir最新版
* re2c
* gcc

# 安装Zephir

<a href="http://docs.zephir-lang.com/en/latest/install.html" target="_blank">参考</a>

# 安装Libev

* 下载Libev最新版 <a href="http://dist.schmorp.de/libev/" target="_blank">下载</a>

* 安装
```
sh autogen.sh

./configure && make

sudo make install

```
# 编译扩展
* 生成C API
```
zephir generate

```

* 编译
```
phpize

make 

make install

```
