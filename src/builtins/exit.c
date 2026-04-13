void built_exit(char **cmd) {
  if (cmd[1]) {
    if (is_numeric(cmd[1]))  {
      write(1, "needs numeric arguments\n", 24);
      return;
    }
    if (cmd[2]) {
      write(1, "too many arguments\n", 19);
      return();
    }
    exit(ft_atoi(args[1]) % 256);
  }
}
