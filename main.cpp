#include <cstdlib>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>
#define PATH_MAX 4096
#define MAX_PATH_EXPECTED_DEPTH 20
using std::string;

string split(const string &str, char delimiter);
void case_1();
string case_2(char **argv);

int main(int argc, char **argv) {
  if (argc == 1)
    case_1();
  string GIT_SERVER{argv[1]};
  string REPO_NAME{argc == 2 ? case_2(argv) : argv[2]};

  string ssh_cmd = "ssh git@" + GIT_SERVER;
  string mkdir_command = ssh_cmd + " \"mkdir " + REPO_NAME + ".git\"";
  string init_command =
      ssh_cmd + " \"cd " + REPO_NAME + ".git; git init --bare\"";
  int status_mkdir = system(mkdir_command.c_str());
  if (status_mkdir != 0) {
    std::cerr << "repository by that name already exists\n";
    return 1;
  }
  system(init_command.c_str());
}

string split(const string &str, char delimiter) {
  std::vector<string> result;
  // shouldn't be more than this I pray
  result.reserve(MAX_PATH_EXPECTED_DEPTH);
  string current;

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

string case_2(char **argv) {
  char buffer[PATH_MAX];
  if (getcwd(buffer, sizeof(buffer)) == nullptr) {
    std::cerr << "ERROR: could not deduce current directory\n";
    exit(1);
  }
  return split(buffer, '/');
}
