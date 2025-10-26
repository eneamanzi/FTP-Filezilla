---
title: FTP Commands
layout: default
nav_order: 4
---

# FTP Commands

| Command | Description |
|----------|-------------|
| `USER` | Username input for authentication |
| `PASS` | Password input |
| `PORT` | Active mode (client opens data port) |
| `PASV` | Passive mode (server opens data port) |
| `LIST` | List files and directories |
| `PWD`  | Show current working directory |
| `CWD`  | Change directory |
| `DELE` | Delete file |
| `RETR` | Download file from server |
| `STOR` | Upload file to server |
| `MKD`  | Create directory |
| `RMD`  | Remove directory |
| `TYPE` | Set transfer type (ASCII/Binary) |
| `QUIT` | Terminate the session |

> For implementation details, check `ftpServer.c`, `handlers.c`, `myLib.c`.
