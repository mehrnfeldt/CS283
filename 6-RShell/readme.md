1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines when a command's output is fully received by detecting when the server closes the socket after sending all data. Since recv() returns 0 when the connection is closed, the client simply reads until it gets this signal. This method is simple and ensures the client doesn't need special delimiters or length headers. A new connection must be created for each command. To handle partial reads, the client should use a loop to keep calling recv() until it gets all data.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol should define and detect command boundaries using explicit delimiters or length-prefixing. These methods help the receiver know exactly where a command starts and ends, preventing partial reads or unintended merging of multiple commands. If boundaries are not properly handled, the client or server may misinterpret fragmented or combined commands, leading to errors, unexpected behavior, or hanging connections. Without clear separation, the shell might process incomplete commands, causing execution failures.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain session information between requests, meaning the server keeps track of past interactions with a client. Stateless protocols treat each request as independent, with no memory of previous interactions. This simplifies server design, reduces resource usage, and improves scalability but may require clients to resend necessary context with each request. Stateful protocols offer continuity and context retention, while stateless protocols prioritize simplicity and scalability at the cost of needing extra data in each request.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is considered "unreliable" because it does not guarantee packet delivery, order, or error correction like TCP does. However, it is faster and more efficient because it avoids the overhead of connection setup, acknowledgments, and retransmissions. UDP is ideal for applications where speed is more important than reliability, such as live video streaming and online gaming. These applications can tolerate some data loss but require low latency and real-time performance.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides Sockets API as an interface for applications to use network communication. This abstraction allows programs to send and receive data over networks using TCP, UDP, and other protocols without needing to manage low-level details like packet routing or error handling. Applications can create socket descriptors, bind them to network addresses, listen for connections, and send or receive data. Functions like socket(), bind(), listen(), accept(), connect(), send(), and recv() enable network communication.
