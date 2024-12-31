#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage: create-repo {REMOTE_SERVER} {REPO_NAME}\n";
        return 1;
    }
    const std::string GIT_SERVER = argv[1];
    const std::string REPO_NAME = argv[2];
    auto ssh_c = "ssh git@" + GIT_SERVER;
    std::string mkdir_command = ssh_c + " \"mkdir " + REPO_NAME + ".git\"";
    std::string init_command =
        ssh_c + " \"cd " + REPO_NAME + ".git; git init --bare\"";
    auto status_mkdir = system(mkdir_command.c_str());
    if (status_mkdir != 0) {
        std::cerr << "repository by that name already exists\n";
        return 1;
    }
    system(init_command.c_str());
}
