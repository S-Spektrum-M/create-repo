#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

void printUsage();
std::string get_working_dir();
bool isValidInput(const std::string &input);

int main(int argc, char **argv) {
  if (argc < 2 || argc > 3) {
    printUsage();
    return 1;
  }

  std::string remote_server = argv[1];
  std::string repo_name;

  if (argc == 2) {
    // Automatically get repo name from the current directory
    repo_name = get_working_dir();
  } else {
    // Get repo name from the command-line argument
    repo_name = argv[2];
  }

  if (!isValidInput(remote_server) || !isValidInput(repo_name)) {
    std::cerr << "Error: Invalid characters in server or repository name.\n"
              << "Only letters, numbers, hyphens, underscores, and periods are "
                 "allowed.\n";
    return 1;
  }

  std::string command = "ssh git@" + remote_server + " 'mkdir " + repo_name +
                        ".git && cd " + repo_name + ".git && git init --bare'";

  std::cout << "Executing on remote: " << command << std::endl;

  if (system(command.c_str()) != 0) {
    std::cerr << "Error: Failed to create repository on the remote.\n"
              << "Please check if:\n"
              << "1. The repository already exists.\n"
              << "2. You have the correct SSH permissions.\n"
              << "3. 'git' is installed and in the remote user's PATH.\n";
    return 1;
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
std::string get_working_dir() {
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
bool isValidInput(const std::string &input) {
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
