#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

std::string split(const std::string &str, char delimiter);

int main(int argc, char **argv) {
  std::string GIT_SERVER;
  std::string REPO_NAME;
  if (argc == 2) {
    char buffer[1 << 10];
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
      std::cerr << "ERROR: could not deduce current directory\n";
      return 1;
    }
    GIT_SERVER = argv[1];
    REPO_NAME = split(buffer, '/');
    return 0;
  } else if (argc < 3) {
    std::cerr << "Usage: create-repo {REMOTE_SERVER} {REPO_NAME}\n";
    return 1;
  }
  GIT_SERVER = argv[1];
  REPO_NAME = argv[2];
  auto ssh_cmd = "ssh git@" + GIT_SERVER;
  std::string mkdir_command = ssh_cmd + " \"mkdir " + REPO_NAME + ".git\"";
  std::string init_command =
      ssh_cmd + " \"cd " + REPO_NAME + ".git; git init --bare\"";
  auto status_mkdir = system(mkdir_command.c_str());
  if (status_mkdir != 0) {
    std::cerr << "repository by that name already exists\n";
    return 1;
  }
  system(init_command.c_str());
}

std::string split(const std::string &str, char delimiter) {
  std::vector<std::string> result;
  std::string current;

  for (char ch : str) {
    if (ch == delimiter) {
      if (!current.empty()) {
        result.push_back(current);
        current.clear();
      }
    } else {
      current += ch;
    }
  }

  if (!current.empty()) {
    result.push_back(current);
  }

  return result[result.size()];
}
