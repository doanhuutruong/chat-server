# Chat Server (C++17, Linux)

A multi-client TCP chat server built to practice Linux system programming and object-oriented design.
Clients log in with a name and exchange messages; chat history is written to a file by a separate child process.

## Techniques

| Area | Implementation |
|---|---|
| Socket programming | `socket`, `bind`, `listen`, `accept`, `recv`, `send` |
| Multi-threading | One thread per client; shared client list protected by `std::mutex` |
| Multi-process / IPC | Child process writes chat history, receiving data via `fork` + `pipe` |
| Design patterns | Singleton (`Logger`), Command, Factory (`CommandFactory`) |
| Modern C++ | `std::unique_ptr`, `std::function`, lambdas, STL containers |

## Build and run

Requirements: Linux or WSL2, g++ (C++17), CMake 3.16+

```bash
cmake -S . -B build
cmake --build build
./build/chat_server          # listens on port 8080
```

Connect from other terminals:

```bash
nc localhost 8080
```

Commands: `/login <name>`, `/help`, `/quit`. Any other line is sent as a message to all logged-in users.

## Author

Doan Huu Truong — [github.com/doanhuutruong](https://github.com/doanhuutruong)
