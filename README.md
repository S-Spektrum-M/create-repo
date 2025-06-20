# create-repo

A C++ utility for creating bare Git repositories on remote servers via SSH.

## Description

`create-repo` simplifies the process of setting up bare Git repositories on remote servers. It connects via SSH, creates a directory with the `.git` extension, and initializes it as a bare repository ready to receive pushes.

## Features

- Creates bare Git repositories on remote servers
- Automatic repository naming from current directory
- Input validation to prevent command injection
- Clear error messages and troubleshooting guidance
- Shows the exact `git remote add` command to use

## Prerequisites

- C++17+ compiler
- SSH access to the target server
- Git installed on the remote server
- SSH key authentication configured (recommended)
- `git` user on the remote server

## Compilation

```bash
make # this will also automatically move it to /usr/bin/create-repo
```

## Usage

```bash
create-repo {REMOTE_SERVER} [REPO_NAME]
```

### Parameters

- `REMOTE_SERVER`: The hostname or IP address of the remote server
- `REPO_NAME` (optional): Name for the repository. If not provided, uses the current directory name

### Examples

```bash
# Create a repo named after the current directory
create-repo myserver.com

# Create a repo with a specific name
create-repo myserver.com my-project

# Using IP address
create-repo 192.168.1.100 web-app
```

## What it does

1. Validates input parameters for security
2. Connects to the remote server via SSH as the `git` user
3. Creates a directory named `{REPO_NAME}.git`
4. Initializes it as a bare Git repository
5. Provides the command to add it as a remote

## Security

The tool includes input validation that only allows:
- Alphanumeric characters (a-z, A-Z, 0-9)
- Hyphens (-)
- Underscores (_)
- Periods (.)

This helps prevent command injection attacks.

## Error Handling

If the command fails, check:
1. The repository doesn't already exist
2. You have proper SSH permissions to the server
3. Git is installed and accessible on the remote server
4. The SSH connection is working

## Contributing

Feel free to submit issues or pull requests to improve the tool.

## License

This project is provided as-is under the MIT License.
