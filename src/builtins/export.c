

#include <string.h>
#include <string.h>

char **sort_array(char **env) {
    if (!env) return (env);

    int n = 0;
    while (env[n]) 
        n++;

    int pass = 0;
    while (pass < n - 1) {
        int i = 0;
        int swapped = 0;
        while (i < n - 1 - pass) {
            if (strcmp(env[i], env[i + 1]) > 0) {
                char *tmp = env[i];
                env[i] = env[i + 1];
                env[i + 1] = tmp;
                swapped = 1;
            }
            i++;
        }
        if (!swapped) break;
        pass++;
    }
    return env;
}


void export(char **env) {
    char **sorted;
    int i = 0;
    //sorted = env_to_array(env);
    sorted = sort_array(sorted);
}

int main(int ac, char **av, char **envp) {
    int i = 0;
    char **env;
    //while (envp[])
    env = sort_array(envp);
    while (envp[i]) {
        printf("%s\n", envp[i]);
        i++;
    }
}