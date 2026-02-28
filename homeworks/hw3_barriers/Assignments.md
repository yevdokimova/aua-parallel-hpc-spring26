# Homework 3: Barriers

Submit a PDF document that includes the following:
- Git Repository Link: Include the link to your Git repository containing all source code files at the top of the PDF.
- Program Outputs: Add clear, labeled screenshots showing the output of each program. Ensure each screenshot is associated with the corresponding exercise or program name for easy reference.


## Assignment 1: Dice Game with POSIX Barriers

**Objective: Implement a multi-threaded dice game, where multiple players roll a dice in each round. The game is synchronized using POSIX barriers, ensuring all players finish rolling before determining the winner of each round.**

_Task_:

1. The game consists of P players (threads).
2. Each player rolls a six-sided dice (`rand() % 6 + 1`).
3. All players **must wait at a POSIX barrier** before proceeding to determine the round winner.
4. After all players have rolled their dice, the **winner of the round** is displayed.
5. The game runs for **R rounds**.
6. At the end of all rounds, the **player with the most wins** is declared the overall winner.

_Requirements_:
1. Use `pthread_barrier_t` to ensure all players finish rolling before finding the winner.
2. If needed use `pthread_mutex_t` to protect shared data.
3. Use `rand()` to generate dice rolls.


## Assignment 2: Multiplayer Game Lobby Synchronization

**Objective: Simulate a multiplayer game lobby, where all players must be ready before the game starts.**

_Task_:

1. Create **N** player threads.
2. Each player simulates **"getting ready"** (sleeping for a random time).
3. Players must **wait at a barrier** before the game starts.
4. Once all players are ready, they print **"Game Started!"**.


## Assignment 3: Weather Data Collection System

**Objective: Implement a distributed weather station where sensors synchronize data collection before processing it.**

_Task_:

1. Create **S** sensor threads.
2. Each sensor:
    - Collects temperature data. Simulate some values.
    - Waits at a **barrier** for all sensors to finish.
    - Once all sensors collect data, they proceed to processing.
    - Calculate the average temperature for the given period.
3. The main thread initializes and destroys the barriers.


## Assignment 4: Multi-Stage Pipeline Simulation

**Objective: Implement a 3-stage pipeline where worker threads must synchronize at each stage.**

_Task_:

1. Create **M** threads.
2. Each thread performs **three stages** of execution.
3. Synchronize threads at **each stage** using **POSIX barriers**.
4. Either use **one barrier** and reset it between stages, or **three separate barriers**.
