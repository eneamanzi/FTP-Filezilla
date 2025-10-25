# FTP-Filezilla

**FTP Server compatible with FileZilla client**

*Repository source: eneamanzi/FTP-Filezilla*.

## Panoramica

Questo progetto implementa un semplice **FTP server** scritto in **C** realizzato come progetto per l'esame di *Reti di Calcolatori*. L'obiettivo è fornire un server compatibile con il client FileZilla che supporti le principali operazioni del protocollo FTP (login, trasferimento file in active/passive, gestione directory, ecc.).

> Il repository contiene codice sorgente in C e un `Makefile` che facilita la compilazione del server; è incluso anche un PDF con la descrizione del progetto (`Progetto reti.pdf`). 

---

## Funzionalità supportate

Il server supporta (almeno) i seguenti comandi FTP:

* `USER` — specifica l'username per il login
* `PASS` — specifica la password per il login
* `PORT` — connessione dati in active mode
* `PASV` — attivazione passive mode
* `LIST` — lista file e directory
* `PWD` — mostra la working directory
* `CWD` — cambia directory
* `DELE` — cancella file
* `RETR` — scarica un file dal server
* `STOR` — carica un file sul server
* `MKD` — crea directory
* `RMD` — rimuove directory
* `TYPE` — imposta il tipo di trasferimento (A/I)
* `QUIT` — termina la sessione

Per dettagli implementativi vedi i sorgenti `ftpServer.c`, `handlers.c`, `myLib.c` e gli header associati.

---

## Requisiti

* Ambiente Unix-like (Linux, macOS). Il codice è in C e usa socket di basso livello.
* `gcc` o altro compilatore C compatibile
* `make` (Makefile incluso)
* Client FTP per test (FileZilla raccomandato)

---

## Compilazione

1. Clona il repository:

```bash
git clone https://github.com/eneamanzi/FTP-Filezilla.git
cd FTP-Filezilla
```

2. Compila usando Makefile:

```bash
make
```

Dovrebbe generare l'eseguibile (tipicamente `ftpServer` o nome analogo) se il Makefile è configurato per questo. Se vuoi una build pulita:

```bash
make clean && make
```

(Controlla il contenuto del `Makefile` per i target e le variabili di build).

---

## Esecuzione

Lo script principale è `ftpServer.c` — lancia l'eseguibile risultante come:

```bash
./ftpServer <porta>  # esempio: ./ftpServer 2121
```

* Avvia il server in ascolto sulla porta specificata.
* Connetti FileZilla (o un altro client FTP) usando indirizzo IP della macchina e la porta scelta.

> Nota: i dettagli esatti della riga di comando (argomenti, opzioni) sono documentati eventualmente nei sorgenti o nel PDF `Progetto reti.pdf`.

---

## Struttura del repository (file principali)

* `.vscode/` — configurazioni editor (se presenti)
* `Makefile` — istruzioni di build. 
* `Progetto reti.pdf` — documento del progetto e specifiche.
* `README.md` — (questo file).
* `colors.h` — definizioni di colore per output testuale/console.
* `ftpServer.c` — entry-point del server, gestione connessioni e loop principale.
* `handlers.c`, `handlers.h` — implementazione e prototipi delle funzioni che gestiscono i comandi FTP.
* `myLib.c`, `myLib.h` — libreria di utilità interna (funzioni ausiliarie, file I/O, parsing ecc.).


---

## Dettagli tecnici e note implementative

* Il server implementa sia modalità attiva (`PORT`) sia passiva (`PASV`) per i trasferimenti dati.
* La gestione dei comandi è modularizzata: `handlers.*` contiene la logica per `USER`, `PASS`, `LIST`, `RETR`, `STOR`, ecc.
* `myLib.*` fornisce funzioni helper (probabilmente per parsing, logging, manipolazione stringhe, gestione file).
* Non è chiaro se il server includa meccanismi di autenticazione persistente (file di utenti) oppure usa credenziali hard-coded o anonime — controllare `handlers.c` per i dettagli sul flusso di login.

Per sicurezza: prima di esporre il server in una rete pubblica, rivedere il codice per validazione dei percorsi, controllo degli accessi e prevention di directory traversal. Il repository è pensato per scopi didattici.

---

## Testing con FileZilla

1. Avvia il server sulla macchina di test.
2. Apri FileZilla → Nuova connessione:

   * Host: `localhost` (o IP del server)
   * Porta: la porta scelta (es. 2121)
   * Protocol: FTP
   * Encryption: No encryption (o come implementato dal server)
   * Logon Type: Normal (poi user/password secondo implementazione)
3. Verifica i comandi: LIST, RETR, STOR, CWD, PWD, DELE ecc.

---

## Limitazioni note & possibili miglioramenti

* Mancanza di TLS/FTPS (se è necessario cifrare le credenziali/dati).
* Gestione utenti/permessi potenzialmente basica — valutare integrazione con file di configurazione o sistema di utenti.
* Hardening contro path traversal, controllo dei permessi di file e rate-limiting per connessioni multiple.
* Logging più completo e gestione di più client concorrenti (threading o fork).

---

## Contributi

Contributi e miglioramenti sono i benvenuti. Se vuoi aprire una PR, segui questi passi:

1. Fork del repository
2. Crea un branch feature: `git checkout -b feature/descrizione`
3. Implementa le modifiche e aggiungi test quando possibile
4. Apri una Pull Request descrivendo i cambiamenti

---

## Riferimenti

* Documento di progetto incluso `Progetto reti.pdf` per specifiche progettuali. citeturn0view0

---

## Autore

Repository: `eneamanzi` (vedi pagina GitHub). 

---
