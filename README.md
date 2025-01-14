# Minishell

Minishell is a simple Unix-like shell created as part of the Hive Helsinki Coding School curriculum. The program replicates the behavior of a basic bash shell, allowing users to execute commands, manage environment variables, and handle redirections and pipes.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Shell Commands](#shell-commands)
- [Bonus Features](#bonus-features)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
- [License](#license)

## Overview

Minishell is designed to mimic the behavior of a standard shell, offering support for executing commands, handling pipes and redirections, and managing environment variables. We implemented key built-in commands such as `cd`, `echo`, `export`, `unset`, `env`, `exit`, and `pwd`, ensuring that the shell can function independently without relying on external binaries.

## Features

- **Command Parsing**: Our shell parses and executes commands entered by the user, supporting multiple arguments, pipes, and redirections.
- **Builtin Commands**: We implemented essential built-in commands like `cd`, `echo`, `export`, `unset`, `env`, `exit`, and `pwd`.
- **Environment Variables**: The shell handles environment variables, allowing users to set, unset, and display them.
- **Exit Expansion**: We implemented support for `$?` expansion, where commands expand to the exit status of the most recently executed foreground pipeline.
- **Pipes and Redirections**: We support piping the output of one command to the input of another and handling input/output redirections.
- **Signal Handling**: The shell responds appropriately to signals such as Ctrl+C, Ctrl+D, and Ctrl+\, ensuring smooth user interaction.

## Installation

To install and run Minishell, follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/Lauri-Litovuo/MiniShell.git
    ```

2. Navigate to the project directory:

    ```bash
    cd minishell
    ```

3. Build the project with Docker:

    ```bash
    docker build . -t minishell
    ```

4. Run the container and start the shell:

    ```bash
    docker run -it minishell
    ./minishell
    ```

## Usage

Once the shell is running, you can use it just like a typical bash. Here are some examples of the commands and functionalities supported by our Minishell:

- Navigate the filesystem:

    ```bash
    cd /path/to/directory
    ```

- List environment variables:

    ```bash
    env
    ```

- Set an environment variable:

    ```bash
    export VAR_NAME=value
    ```

- Unset an environment variable:

    ```bash
    unset VAR_NAME
    ```

- Print working directory:

    ```bash
    pwd
    ```

- Echo a string:

    ```bash
    echo "Hello, world!"
    ```

- Run an external command:

    ```bash
    ls -la
    ```

- Redirect output to a file:

    ```bash
    ls > output.txt
    ```

- Redirect input from a file:

    ```bash
    cat < input.txt
    ```

- Pipe commands:

    ```bash
    ls -la | grep minishell
    ```

- Exit the shell:

    ```bash
    exit
    ```

## Shell Commands

Our Minishell supports the following built-in commands:

- `cd`: Changes the current directory.
- `echo`: Outputs the specified string to the terminal.
- `export`: Sets or updates environment variables.
- `unset`: Unsets or removes environment variables.
- `env`: Displays all environment variables.
- `pwd`: Prints the current working directory.
- `exit`: Exits the shell.

These built-in commands are implemented directly within the shell and do not rely on external binaries, ensuring that the shell remains functional even in minimal environments.

## Bonus Features

In addition to the core functionality, we implemented several advanced features to enhance the user experience:

- **Advanced Signal Handling**: Our shell gracefully handles signals like Ctrl+C (interrupt) and Ctrl+D (end-of-file) without terminating unexpectedly.
- **Command History**: We added a simple command history feature that allows users to scroll through previous commands using the arrow keys.



