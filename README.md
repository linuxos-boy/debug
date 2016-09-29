# EDEBUG
edebug 可以跟踪PHP在运行时的函数调用、请求信息、执行流程 

能很好的定位阻塞问题，定位问题，主要用于线上环境

页面右下角展示bug 在线调

1. PHP扩展 - 编译安装
   ```
   {php_dir}/phpize
   ./configure --with-php-config={php_dir}/php-config
   make && make install
   ```
2. PHP扩展 - 配置
    ```
    extension=EDebug.so
    ```
#展示

![image](https://raw.githubusercontent.com/linuxos-boy/debug/master/code.png)
![image](https://raw.githubusercontent.com/linuxos-boy/debug/master/code2.png)

