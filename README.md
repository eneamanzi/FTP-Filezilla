# FTP-Filezilla

**FTP Server compatible with the FileZilla client**

---

## Description

This project implements a simple **FTP server** written in **C**, developed as part of the *Computer Networks* course project.  
The goal is to provide a server fully compatible with the **FileZilla client**, supporting key FTP protocol operations such as user authentication, file transfer (active and passive modes), and directory management.

> The repository contains C source code, a `Makefile` for compilation, and the project documentation file `Progetto reti.pdf`.

---

## Table of Contents

- [Description](#description)
- [Supported Features](#supported-features)
- [Requirements](#requirements)
- [Compilation](#compilation)
- [Execution](#execution)
- [Testing with FileZilla](#testing-with-filezilla)
- [Architecture and Main Files](#architecture-and-main-files)
- [Technical Details](#technical-details)
- [References](#references)
- [Authors](#authors)


---

## Supported Features

The FTP server implements the main standard FTP commands:

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
| `QUIT` | Terminate the FTP session |

> For implementation details, see `ftpServer.c`, `handlers.c`, and `myLib.c`.

---

## Requirements

- Unix-like environment (Linux, macOS)
- `gcc` or any compatible C compiler
- `make` (Makefile included)
- FTP client for testing (recommended: **FileZilla**)

---

## Compilation

1. Clone the repository:
   ```bash
   git clone https://github.com/eneamanzi/FTP-Filezilla.git
   cd FTP-Filezilla
   ```

2. Compile using the Makefile:
   ```bash
   make
   ```

   For a clean build:
   ```bash
   make clean && make
   ```

> The generated executable (usually named `ftpServer`) depends on the targets defined in the `Makefile`.

---

## Execution

Run the server executable as follows:
```bash
./ftpServer <port>
```

Example:
```bash
./ftpServer 2121
```

- The server will start listening on the specified port.  
- Connect using FileZilla or any FTP client via the server’s IP and port.

> Refer to the source code or the document `Progetto reti.pdf` for any additional command-line options.

---

## Testing with FileZilla

1. Start the FTP server on the test machine.  
2. Open **FileZilla** → *Site Manager* → *New Site*  
3. Configure the connection:

   | Field | Value |
   |--------|--------|
   | Host | `localhost` or server IP |
   | Port | e.g. `2121` |
   | Protocol | FTP |
   | Encryption | None |
   | Logon Type | Normal |
   | Username / Password | As implemented |

4. Connect and test commands such as: `LIST`, `RETR`, `STOR`, `PWD`, `CWD`, `DELE`, etc.

---

## Architecture and Main Files

| File | Description |
|------|--------------|
| **ftpServer.c** | Contains `main`, socket initialization, `accept` loop, and command handling |
| **handlers.c / handlers.h** | Implements the logic for FTP commands (LIST, RETR, STOR, PASV, etc.) |
| **myLib.c / myLib.h** | Utility library (I/O, parsing, socket wrappers) |
| **colors.h** | Macros for colored terminal output |
| **Makefile** | Automates compilation of the project |
| **Progetto reti.pdf** | Project documentation and design specifications |

---

## Technical Details

- Supports both **active (PORT)** and **passive (PASV)** data transfer modes  
- Commands are parsed through the control socket  
- Basic authentication (`USER` / `PASS`) implemented
- Standard FTP reply codes used (`2xx`, `3xx`, `4xx`, `5xx`)  
- Modularized architecture separating command logic and utility functions  
- Intended for **educational and experimental purposes**

---

## References

- Project report: `Progetto reti.pdf`  
- FTP protocol specification: [RFC 959](https://www.rfc-editor.org/rfc/rfc959)

---

## Authors

[**Enea Manzi**](https://github.com/eneamanzi)

[**Riccardo Barone**](https://github.com/Riiccardob)

---

