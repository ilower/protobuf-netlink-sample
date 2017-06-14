# protobuf-netlink-sample

这是一个展示 google protocol buffer 和 netlink 使用的简单项目。

netlink 目录是 netlink 源代码，用来编译静态库 libnetlink.a。

Server.cc 和 Client.cc 是基于 netlink 实现的一个服务端和客户端源码。

message.proto 是服务端和客户端通信的数据协议定义，用 genpb.sh 来生成解析类的源码文件。

测试环境要求:

1.下载 google protocal buffer 的源码，编译 protoc 并安装到系统

2.需要将 protobuf 编译出的静态库 libprotobuf.a 复制到项目目录下

3.测试步骤:

1）进入项目根目录

2）先执行 genpb.sh 生成解析类的源文件: ./sh genpb.sh

3）编译出 netlink/libnetlink.a 静态库，以及 server 和 client 两个可执行程序: ./make

4）打开一个新终端窗口，启动服务端: ./server 9527

5）打开一个新终端窗口，启动客户端: ./client 9527

6）在客户端按提示发送信息和查看服务端返回的信息。还可以打开第三个终端窗口，启动第二个客户端和服务端通信