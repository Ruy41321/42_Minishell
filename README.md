# Minishell  

**Minishell** is a simple Unix shell developed in C, designed to replicate the basic behavior of **Bash** by implementing essential features such as command execution, environment variable handling, redirections, pipes, and built-in commands.  

![Minishell Demo](extra/minishell.gif)

---

### Project Overview  

This project was developed as part of a team effort with **Federico Lo Dolce** ([GitHub](https://github.com/flodolce92)).  
The goal was to create a functional shell that executes commands, handles process management, and implements fundamental shell behaviors, while maintaining modularity and avoiding unnecessary global variables.  

---

### Responsibilities  

As part of the development team, I was responsible for:  

- **Command Execution**: Managing external commands and built-in commands.  
- **Input Parsing**: Handling user input, tokenizing commands, and managing quotes.  
- **Redirections & Pipes**: Implementing `>`, `<`, `>>`, `<<`, and `|` for process communication.  
- **Memory Management**: Preventing memory leaks and ensuring proper allocation.  
- **Signal Handling**: Managing interrupts (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`).  

---

### Technologies Used  

- **Programming Language**: C  

---

### Challenges Faced  

- **Command-Line Interface Implementation**: Creating an interactive shell prompt.  
- **Process Management**: Handling child processes and system calls.  
- **Handling Special Characters**: Implementing correct parsing and behavior for pipes and redirections.  

---

### Outcome  

The project resulted in a fully functional shell capable of:  

- **Executing commands** via absolute path, relative path, or using `$PATH`.  
- **Handling environment variables**, special characters, and exit status expansion (`$?`).  
- **Managing input redirections (`<`), output redirections (`>`), and appends (`>>`)**.  
- **Supporting pipelines (`|`)** for connecting commands.  
- **Providing built-in commands** such as `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.  

---

Minishell was an excellent opportunity to deepen my understanding of **system calls, process management, and shell behavior**, while working on a structured and modular C project. 🚀  
