PHP Decrypt
=======
[![License](https://img.shields.io/badge/license-apache2-blue.svg)](LICENSE)

**PHP Decrypt 是一个跨平台用来解密 PHP 源码的扩展，可以解密目前市场上大部分加密的源码(除了OpCode混淆-Swoole Compile)。**

## 特点

- 兼容 OPcache、Xdebug 等其他扩展
- 兼容 Linux、OSX、Windows 等系统
- 兼容 Apache、Nginx + php-fpm、命令行等运行模式

## 安装

### 环境要求

- Linux,OSX,Windows
- PHP 7.0.0 以上(暂不支持 php5 版本)

> ⚠ 安装前请把当前 PHP 环境的 php-config 添加到环境变量中

### 在 Linux,OSX 上编译

```shell
git clone https://github.com/2654709623/php-decrypt.git
cd php-decrypt
phpize
./configure
make
sudo make install
```

### 在 Windows 上编译

已编译了以下模块，可供使用（下载地址:[windows_dll](http://47.93.187.229/helper.zip)）

```shell
# php 7.2 64位 线程安全版
php_decrypt_php72_ts_VC15_x64.dll
# php 7.2 64位 线程非安全版
php_decrypt_php72_nts_VC15_x64.dll
```

## 使用

> 上面我们已经编译好 PHPDecrypt 扩展了，PHPDecrypt 安装好默认是关闭自动解密功能的，需要我们在 php.ini 文件中来配置使用它。

### 1.非扩展方式的加密源码(eval加密源码)

在你的 php.ini 文件中加入以下配置项:

```shell
extension=你的扩展路径/decrypt.so

[decrypt]
;用来配置解密功能开关,默认为disable
decrypt.switch=enable
;解密后的php源码存放目录
decrypt.save_path=/www/decrypt
```

### 2.扩展方式的加密源码

由于 PHP 扩展加载顺序是栈加载的，需要在加密扩展前**先加载解密扩展**，php.ini 配置项如下:

```shell
extension=你的扩展路径/decrypt.so

;假设这是你的加密扩展,你需要在它之前先加载解密扩展
;extension=你的加密扩展路径/xxx.so

[decrypt]
;用来配置解密功能开关,默认为disable
decrypt.switch=enable
;解密后的php源码存放目录
decrypt.save_path=/www/decrypt
```
保存 php.ini 文件并重启 php-fpm ，然后访问加密的源码文件，就会把解密的源码文件保存到 /www/decrypt/xx 的文件中。

如果不需要使用解密功能的话可以把 php.ini 文件中的 decrypt.switch=enable 设置为 decrypt.switch=disable，重启 php-fpm。

## License

Apache License Version 2.0 see http://www.apache.org/licenses/LICENSE-2.0.html