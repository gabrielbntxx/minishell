#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
char	**ft_split(char const *s, char c);


//
// void which_arg(char **cmd){
//   if (!cmd[1])
// }


void cd(char *args, char **env) {
  char **cmd;
  char *home;
  char *path;
  char *pwd;
  path = getenv("PWD");
  pwd = path;
  home = getenv("HOME");
  if (!home) {
    perror("cd: Home not set\n");
    return;
  }
  cmd = ft_split(args, ' ');
  if (!cmd[1]) {
    path = home;
    printf("%s\n", path);
    chdir(home);

    printf("%s\n", path);
}
}

int main(int argc, char *argv[], char **envp) {
  if (argv[1])
    cd(argv[1], envp);
  return 0;
}
