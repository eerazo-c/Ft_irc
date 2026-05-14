`<ft_irc>`

This project has been created as part
of the 42 curriculum by `<elerazo>` `<israetor>`

`<Description>`

ft_irc is a network programming project whose goal is to recreate
an IRC (Internet Relay Chat) server using C++.

The goal of this project is to explore the fundamentals of socket programming,
TCP/IP communication, multiplexing, and real-time client/server interactions.

Through this project, you learn how multiple clients can connect to the same server,
how messages are exchanged through sockets, how channels work,
and how communication protocols are implemented at a low level.

You also work with non-blocking file descriptors, event-driven programming,
client management, command parsing, and multiplexing using `epoll()`.

ft_irc is an introduction to network and systems programming
and helps build intuition about how real-time communication systems,
chat servers, and IRC applications work internally.

`<Instructions>`

`#Compiling the project`

1. Clone the repository.
2. Run `make` to compile the program.
3. An executable called `ircserv` will be generated.

`#Running the server`

1. Run the server with:
   `./ircserv <port> <password>`
2. Example:
   `./ircserv 6667 password42`
3. The server will start listening for client connections.

#Connecting with HexChat

1. Open HexChat 
2. Create a new network.
3. Connect using:
   `/server localhost 6667 password42`
4. Set a nickname:
   `/nick usuario`
5. Join a channel:
   `/join #42`

#Connecting with Irssi

1. Open Irssi
2. Start the client using:
   `irssi`
3. Connect to the server:
   `/connect localhost 6667 password42`
4. Set a nickname:
   `/nick eli`
5. Join a channel:
   `/join #42`

#Testing with:
-HexChat
-Irssi

`<Submission requirements>`

1. The project must compile without errors or warnings.
2. All mandatory features described in the subject must be implemented.
3. The program must respect the Norm of 42.
4. Memory leaks are not allowed.
5. The server must handle multiple simultaneous client connections.
6. Communication must follow IRC protocol behavior.
7. The server must use non-blocking file descriptors.

`<Resources>`

#Concepts studied
This project covers the following concepts:

1. Socket programming
2. TCP/IP communication
3. Client/server architecture
4. IRC protocol basics
5. Non-blocking I/O
6. Multiplexing with `epoll()`
7. Event-driven programming
8. Message parsing
9. Channels and private messaging
10. Connection and client management
11. Real-time communication systems
12. Memory management in C++

#References

1. 42 ft_irc subject documentation
2. IRC RFC documentation
3. Beej's Guide to Network Programming
4. Linux socket and epoll man pages
5. Basic networking tutorials
6. Peer to peer learning
7. [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/?utm_source=chatgpt.com)
8. [Linux man pages](https://man7.org/linux/man-pages/?utm_source=chatgpt.com)
9. [IRC RFC 2812](https://datatracker.ietf.org/doc/html/rfc2812?utm_source=chatgpt.com)
10. [HexChat Official Website](https://hexchat.github.io/?utm_source=chatgpt.com)
11. [Irssi Official Website](https://irssi.org/?utm_source=chatgpt.com)

`<Use of AI >`

AI was used as a learning support tool to:
1. Clarify networking and socket programming concepts in simple terms
2. Help understand IRC protocol behavior
3. Assist in understanding `epoll()` and multiplexing
4. Help debug segmentation faults, memory leaks, and connection issues
5. Assist in structuring the project documentation
6. Help understand edge cases and command behavior

AI was not used to implement the core logic automatically,
but to better understand the theory and debug errors.

`<This project is part of the learning path at 42 and aims to introduce
students to network programming, socket communication,
and real-time communication systems concepts>`
