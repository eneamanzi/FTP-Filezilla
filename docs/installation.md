---
title: Installation
layout: default
---

# Installation

## Requirements
- Unix-like OS (Linux, macOS)
- `gcc` or compatible compiler
- `make` (Makefile included)
- FTP client (FileZilla recommended)

## Compilation
1. Clone the repository:
```bash
git clone https://github.com/eneamanzi/FTP-Filezilla.git
cd FTP-Filezilla
```

2. Build project:
```bash
make
```

For a clean build:
```bash
make clean && make
```

## Execution
Run the server:
```bash
./ftpServer <port>
```
Example:
```bash
./ftpServer 2121
```

Connect using FileZilla or another FTP client with server IP and port.
