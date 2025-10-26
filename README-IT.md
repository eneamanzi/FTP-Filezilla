# FTP-Filezilla

**FTP Server compatible with FileZilla client**

---

## Descrizione

Questo progetto implementa un semplice **FTP server** scritto in **C**, realizzato come progetto per l’esame di *Reti di Calcolatori*.  
L’obiettivo è fornire un server compatibile con il client **FileZilla**, che supporti le principali funzionalità del protocollo FTP: autenticazione utente, trasferimento file in modalità **attiva/passiva**, e gestione delle directory.

> Il repository contiene codice sorgente in C, un `Makefile` per la compilazione automatica e il documento di progetto `Progetto reti.pdf`.

---

## Sommario
- [Descrizione](#-descrizione)
- [Funzionalità supportate](#-funzionalità-supportate)
- [Requisiti](#-requisiti)
- [Compilazione](#-compilazione)
- [Esecuzione](#-esecuzione)
- [Testing con FileZilla](#-testing-con-filezilla)
- [Architettura e file principali](#-architettura-e-file-principali)
- [Dettagli tecnici e note implementative](#-dettagli-tecnici-e-note-implementative)
- [Riferimenti](#-riferimenti)
- [Autore](#-autore)

---

## Funzionalità supportate

Il server implementa i principali **comandi FTP**:

| Comando | Descrizione |
|----------|-------------|
| `USER` | Inserimento username per autenticazione |
| `PASS` | Inserimento password |
| `PORT` | Modalità attiva (client apre porta dati) |
| `PASV` | Modalità passiva (server apre porta dati) |
| `LIST` | Elenco di file e directory |
| `PWD`  | Mostra la working directory corrente |
| `CWD`  | Cambia directory |
| `DELE` | Elimina un file |
| `RETR` | Scarica un file dal server |
| `STOR` | Carica un file sul server |
| `MKD`  | Crea directory |
| `RMD`  | Rimuove directory |
| `TYPE` | Imposta tipo di trasferimento (ASCII/BINARY) |
| `QUIT` | Chiude la sessione FTP |

> Per maggiori dettagli consultare i sorgenti: `ftpServer.c`, `handlers.c`, `myLib.c` e relativi header.

---

##  Requisiti

- Sistema operativo **Unix-like** (Linux o macOS)
- Compilatore **gcc** o compatibile
- **make** (per utilizzare il `Makefile`)
- Client FTP per i test — **FileZilla** consigliato

---

## 🛠️ Compilazione

1. Clona il repository:
   ```bash
   git clone https://github.com/eneamanzi/FTP-Filezilla.git
   cd FTP-Filezilla
   ```

2. Compila il progetto tramite Makefile:
   ```bash
   make
   ```

   Per una build pulita:
   ```bash
   make clean && make
   ```

> Controlla il `Makefile` per verificare nome e posizione dell’eseguibile generato (solitamente `ftpServer`).

---

## Esecuzione

Avvia il server con:
```bash
./ftpServer <porta>
```

Esempio:
```bash
./ftpServer 2121
```

- Il server rimane in ascolto sulla porta specificata.  
- Per porte inferiori a 1024 (es. 21) può essere necessario usare `sudo`.

> Consulta il file `Progetto reti.pdf` o i sorgenti per eventuali opzioni aggiuntive a riga di comando.

---

## Testing con FileZilla

1. Avvia il server sulla macchina di test.  
2. Apri **FileZilla** → *Site Manager* → *New Site*  
3. Configura la connessione:

   | Campo | Valore |
   |--------|--------|
   | Host | `localhost` o IP del server |
   | Porta | `2121` (o quella configurata) |
   | Protocollo | FTP |
   | Encryption | None |
   | Logon Type | Normal |
   | User / Password | Come implementato nel server |

4. Connetti e verifica i comandi: `LIST`, `RETR`, `STOR`, `PWD`, `CWD`, `DELE`, ecc.

---

## Architettura e file principali

| File | Descrizione |
|------|--------------|
| **ftpServer.c** | Contiene `main`, inizializzazione socket, ciclo `accept`, e gestione dei comandi |
| **handlers.c / handlers.h** | Implementano la logica per i comandi FTP (LIST, STOR, RETR, PASV, ecc.) |
| **myLib.c / myLib.h** | Libreria di utilità: parsing, I/O, socket wrapper |
| **colors.h** | Macro per stampa colorata in console |
| **Makefile** | Script di build per compilare automaticamente il progetto |
| **Progetto reti.pdf** | Documento con specifiche e descrizione progettuale |

---

##  Dettagli tecnici e note implementative

- Supporta **modalità attiva (PORT)** e **passiva (PASV)**  
- Parsing comandi FTP tramite socket di controllo  
- Gestione potenzialmente sequenziale o multi-processo (verificare nel codice)  
- Autenticazione base (`USER` / `PASS`) — probabilmente con credenziali fisse o anonime  
- Risposte standard FTP: `2xx`, `3xx`, `4xx`, `5xx`  
- `handlers.*` gestisce la logica dei comandi, `myLib.*` fornisce funzioni helper


Questo repository è destinato a scopi **didattici**, non alla produzione.

---

## Riferimenti

- Documento di progetto: `Progetto reti.pdf`  
- Specifiche del protocollo FTP (RFC 959)

---

## Autori

[**Enea Manzi**](https://github.com/eneamanzi)

[**Riccardo Barone**](https://github.com/Riiccardob)


---
