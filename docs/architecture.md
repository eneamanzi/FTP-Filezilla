---
title: Architecture
layout: default
---

# Architecture and Main Files

| File | Description |
|------|-------------|
| **ftpServer.c** | Main, socket init, accept loop, command handling |
| **handlers.c / handlers.h** | FTP commands logic: LIST, RETR, STOR, PASV, etc. |
| **myLib.c / myLib.h** | Utility library: I/O, parsing, socket wrappers |
| **colors.h** | Macros for colored terminal output |
| **Makefile** | Compilation automation |
| **Progetto reti.pdf** | Project documentation |

The server follows a modular design: command handling is separated from utilities.
