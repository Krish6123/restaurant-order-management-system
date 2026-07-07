# 🍽️ Restaurant Order Management System

A Linux-based restaurant simulation developed in **C** to demonstrate core **Computer Systems and Operating System concepts** including process communication, synchronization, multithreading, semaphores, signals, and file handling.

---

## 📌 Overview

The project simulates a restaurant where multiple customers place food orders simultaneously.

Customer processes communicate with the restaurant using a **Named Pipe (FIFO)**. The restaurant manager receives incoming orders and stores them in a synchronized queue. Multiple cook threads process orders concurrently. Once an order is prepared, the corresponding customer is notified using a **SIGUSR1 signal**. A semaphore limits the number of customers occupying tables simultaneously. Restaurant activities and customer bills are stored using file I/O.

---

## ✨ Features

- 🍕 Multiple customers placing orders simultaneously
- 👨‍🍳 Multiple cook threads processing orders concurrently
- 📨 Inter-Process Communication (Named Pipe / FIFO)
- 🔔 Signal-based notification when food is ready
- 🪑 Table allocation using POSIX Semaphores
- 🔒 Producer-Consumer synchronization using Mutexes and Condition Variables
- 📝 Restaurant activity logging
- 💳 Automatic billing generation
- ⚡ Concurrent order processing

---

## 🛠️ Technologies Used

- C Programming
- POSIX Threads (pthreads)
- POSIX Semaphores
- Named Pipes (FIFO)
- Unix Signals
- File I/O
- Mutexes
- Condition Variables
- Linux System Programming

---

## 📂 Project Structure

```
restaurant-order-management-system/
│── common.h
│── restaurant.c
│── customer.c
│── README.md
│── Makefile
```

---

## ⚙️ Compilation

```bash
gcc restaurant.c -o restaurant -pthread
gcc customer.c -o customer -pthread
```

---

## ▶️ Execution

Start the restaurant server:

```bash
./restaurant
```

Open additional terminals and run:

```bash
./customer
```

Each customer acts as an independent process communicating with the restaurant.

---

## 📄 Output Files

After execution, the following files are generated:

- `restaurant_log.txt` – Restaurant activity log
- `billing.txt` – Customer billing information
- `customer_id_counter.txt` – Customer ID management

---

## 🧠 Computer Systems Concepts Demonstrated

- Process Management
- Inter-Process Communication (IPC)
- Named Pipes (FIFO)
- Multithreading
- Producer-Consumer Problem
- Mutex Synchronization
- Condition Variables
- POSIX Semaphores
- Unix Signals
- File I/O

---

## 👨‍💻 Author

Developed as a Computer Systems Programming course project.
