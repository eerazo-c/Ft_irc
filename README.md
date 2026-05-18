# Ft_irc

*This project has been created as part of the 42 curriculum by <nalesso>, <arhea>*

## Description

This project aims to implement a fully functional Internet Relay Chat (IRC) server acting as a central hub for multiple clients to communicate in real-time. Built entirely in C++ 98, it strictly adheres to non-blocking I/O operations using a single `epoll` instance to multiplex client connections. This implementation mirrors the functionality of standard IRC servers, allowing users to connect using a reference IRC client, authenticate, join channels, and communicate via private or channel-wide messages. It also fully handles channel operator privileges and specific modes (`i`, `t`, `k`, `o`, `l`).

### Bonus Features Included
A bot has been developed and is provided in the `bot_bonus/` directory.

## Instructions

### Compilation and Execution

1. Build the server:
   ```bash
   make
   ```
2. Run the server, providing a listening port and a connection password:
   ```bash
   ./ircserv <port> <password>
   # Example: ./ircserv 6667 my_pass
   ```

### Connecting with an IRC Client

You can use standard IRC clients like `irssi`, `weechat`, or `hexchat` to connect:
```bash
/connect 127.0.0.1 6667 my_pass
```

## Resources

*   [RFC 1459 - Internet Relay Chat Protocol](https://datatracker.ietf.org/doc/html/rfc1459)
*   [RFC 2812 - Internet Relay Chat: Client Protocol](https://datatracker.ietf.org/doc/html/rfc2812)
*   [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/)
*   [Linux manual page for epoll(7)](https://man7.org/linux/man-pages/man7/epoll.7.html)

**Usage of AI:**
Artificial Intelligence (LLMs) was used strictly as a support tool during development. Specifically, AI was used to draft and structure this `README.md` file to ensure it aligns perfectly with the 42 curriculum's mandatory formatting criteria, and to provide guidance on Git branch workflows.
