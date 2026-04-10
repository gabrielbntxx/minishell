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
  home = getenv("home");
  if (!home)
    perror("cd: Home not set\n");
    return;
  cmd = ft_split(args, ' ');
  if (!args[1]) {
    path = home;
    chdir(home);
}
}

int main(int argc, char *argv[], char **envp) {
  cd(argv[1], envp);
  return 0;
}
