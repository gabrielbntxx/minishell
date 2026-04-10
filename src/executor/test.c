#include "executor.h"

int main(int argc, char *argv[], char **envp) {
  char *cmd = argv[1];
  execute_cmd(cmd, envp);

  return 0;
}
