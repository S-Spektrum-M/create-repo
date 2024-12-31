#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#define PATH_MAX 4096
#define MAX_PATH_EXPECTED_DEPTH 20

std::string split(const std::string &str, char delimiter);
void case_1();
std::pair<std::string, std::string> case_2(char **argv);
std::pair<std::string, std::string> case_3(char **argv);

int main(int argc, char **argv) {
  if (argc == 1)
    case_1();
  auto [GIT_SERVER, REPO_NAME] = argc == 2 ? case_2(argv) : case_3(argv);

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
  // shouldn't be more than this I pray
  result.reserve(MAX_PATH_EXPECTED_DEPTH);
  std::string current;

  for (char ch : str) {
    if (ch == delimiter && !current.empty()) {
      result.push_back(current);
      current.clear();
    } else {
      current += ch;
    }
  }

  if (!current.empty()) {
    result.push_back(current);
  }

  return result[result.size() - 1];
}

void case_1() {
  std::cerr << "Usage: create-repo {REMOTE_SERVER} {REPO_NAME}\n";
  exit(1);
}

std::pair<std::string, std::string> case_2(char **argv) {
  char buffer[PATH_MAX];
  if (getcwd(buffer, sizeof(buffer)) == nullptr) {
    std::cerr << "ERROR: could not deduce current directory\n";
    exit(1);
  }
  return {argv[1], split(buffer, '/')};
}

std::pair<std::string, std::string> case_3(char **argv) {
  return {argv[1], argv[2]};
}
