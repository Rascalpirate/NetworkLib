关于 IO 通信库的设计，考虑本地文件和TCP socket，设计一个通用的c++网络库。
通信的两端分别是提供数据的 server 和 获取数据的 client；
抽象出 IServer 和 IClient 的类，由 IEndpoint 提供 server 和 client 的信息；
IServer 提供 bind，listen, accept;
IClient 提供 connect;
IClient 一侧，成功 connect 之后，获得一个 ISession，代表与 IServer 之间的会话；
IServer 一侧，成功 accept 之后，获得一个 ISession，代表与 IClient 之间的会话；
一个 ISession 包含一个 IChannel，会话数据通过 IChannel 传输，表现为一侧发送 send，一侧接收 recv;
一个会话的数据基于一种消息格式封装，定义为 MessageType,
常见消息类型如：
    帧格式的消息 - frame message，
        定义：每一条消息包含一个固定长度的消息头 message header 和 长度被指定的消息体 message body，消息头中有消息类型、消息体的长度等信息，消息体的长度应该与消息头中的长度一致。
        发送消息的时候，一般先根据消息体的类型和长度，创建固定长度的消息头，然后接上消息体，组成一条消息发送出去;
        接收消息的时候，一般先接收固定长度的消息头，然后根据消息头中解析出的消息体的长度，按长度接收消息体；
    固定分隔符格式的消息 - delimited message，
        定义：相邻消息之间用固定的指定分隔符分割。
        发送消息的时候，每条消息末尾新增一个分隔符，再发送出去;
        接收消息的时候，判断数据流中是否有分隔符，遇到分隔符，即为一条消息接收完成;
消息内容是 server 和 client 之间的协议了，提供注册协议和处理器的接口。

开发的时候，需要注意几个重要的抽象：IEndpoint, IServer, IClient, ISession, IChannel, MessageType, MessageHandler.
开发要求，要考虑职责的封装，
    IEndpoint 只负责代表通信双方；
    IServer 只负责 bind, listen, accept, 并生成 ISession;
    IClient 只负责 connect, 并生成 ISession;
    ISession 中包含一个只负责收发的 IChannel；
    ISession 还要负责协议的注册和消息的协调；
要考虑灵活扩展性，比如消息类型可以作为类型参数，提供给通信双方。

比如 TCP socket 的案例中：
    IEndpoint 由 ip 加 port 表示；
    IServer bind port；
    IClient connect 到 IServer 的 ip 加 port；
    双方的 ISession 通过 IChannel 收发消息、驱动逻辑；

比如 本地文件 的案例中：
    IEndpoint 由本地文件路径表示；
    IClient connect 到一个本地文件；
    IServer 根据这个本地文件，打开本地文件；
    IClient 的 ISession 发送一条消息给 IServer，IServer 处理消息后，处理本地文件；
    IServer 实际上是本地文件的代理，IClient 的操作是通过 IServer 直接穿透给本地文件的。

Linux 中是将 IO 都当作本地文件来表达的，而当前设计的网络库将 IO 都当作网络通信的双方来表达的。
