#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include "unistd.h"

using std::string;

void printUsage();
string get_working_dir();
bool isValidInput(const string &input);

int main(int argc, char **argv) {
  string repo_name;

  switch (argc) {
      case 1:           // failing state with too many or not enough arguments
      default:
          printUsage();
          return 1;
      case 2:           // use the current directory name as the repo name
          repo_name = get_working_dir();
      case 3:           // use user specified name as the repo name
          repo_name = argv[2];
  }

  string remote_server = argv[1];

  if (!isValidInput(remote_server) || !isValidInput(repo_name)) {
    std::cerr << "Error: Invalid characters in server or repository name.\n"
              << "Only letters, numbers, hyphens, underscores, and periods are "
                 "allowed.\n";
    return 1;
  }

  string ssh_host = "git@" + remote_server;
  string ssh_command = "'mkdir " + repo_name + ".git && cd " + repo_name + ".git && git init --bare";
  std::cout << "Executing on remote: ssh" << ssh_host << " " << ssh_command << std::endl;

  if (!execl("ssh", ssh_host.c_str(), ssh_command.c_str())) {
    std::cerr << "Error: Failed to create repository on the remote.\n"
              << "Please check if:\n"
              << "1. The repository already exists.\n"
              << "2. You have the correct SSH permissions.\n"
              << "3. 'git' is installed and in the remote user's PATH.\n";
  }

  std::cout << "\nSuccessfully initialized bare repository: " << repo_name
            << "\n"
            << "\n"
            << "Add it as a remote with:\n"
            << "  git remote add " << remote_server << " git@" << remote_server
            << ":~/" << repo_name << ".git\n";

  return 0;
}

/**
 * @brief Prints the usage message for the program.
 */
void printUsage() {
  std::cerr << "Usage: create-repo {REMOTE_SERVER} [REPO_NAME]\n"
            << "If REPO_NAME is not provided, it's derived from the current "
               "directory.\n";
}

/**
 * @brief Gets the name of the current working directory.
 * @return The name of the directory as a string.
 */
string get_working_dir() {
  try {
    return std::filesystem::current_path().filename().string();
  } catch (const std::filesystem::filesystem_error &e) {
    std::cerr << "Error: Could not deduce current directory name: " << e.what()
              << '\n';
    exit(1);
  }
}

/**
 * @brief Validates input to prevent command injection.
 * @param input The string to validate.
 * @return if the string is safe or not.
 */
bool isValidInput(const string &input) {
  if (input.empty()) {
    return false;
  }
  for (char ch : input) {
    // Allow alphanumeric characters, hyphen, underscore, and period.
    if (!std::isalnum(static_cast<unsigned char>(ch)) && ch != '-' &&
        ch != '_' && ch != '.') {
      return false;
    }
  }
  return true;
}
