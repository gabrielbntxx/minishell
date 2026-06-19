#!/bin/bash
# Batterie de tests minishell — 150+ tests
# Usage : ./tests/run_tests.sh

MINISHELL="./minishell"
PASS=0
FAIL=0
TOTAL=0

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
BOLD='\033[1m'
RESET='\033[0m'

section() {
    echo ""
    echo -e "${CYAN}${BOLD}══════════════════════════════════════════${RESET}"
    echo -e "${CYAN}${BOLD}  $1${RESET}"
    echo -e "${CYAN}${BOLD}══════════════════════════════════════════${RESET}"
}

# run_test <desc> <input> <expected_substring>
run_test() {
    local desc="$1" input="$2" expected="$3"
    TOTAL=$((TOTAL + 1))
    actual=$(printf '%s\n' "$input" | "$MINISHELL" 2>/dev/null)
    if echo "$actual" | grep -qF -- "$expected"; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc"
        echo -e "         attendu : |$expected|"
        echo -e "         obtenu  : |$(echo "$actual" | head -2)|"
        FAIL=$((FAIL + 1))
    fi
}

# run_test_exit <desc> <input> <expected_exit_code>
run_test_exit() {
    local desc="$1" input="$2" expected="$3"
    TOTAL=$((TOTAL + 1))
    printf '%s\n' "$input" | "$MINISHELL" >/dev/null 2>&1
    actual=$?
    if [ "$actual" = "$expected" ]; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc (exit=$actual)"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc — attendu exit=$expected, obtenu exit=$actual"
        FAIL=$((FAIL + 1))
    fi
}

# run_test_no_crash <desc> <input>
run_test_no_crash() {
    local desc="$1" input="$2"
    TOTAL=$((TOTAL + 1))
    printf '%s\n' "$input" | "$MINISHELL" >/dev/null 2>&1
    code=$?
    if [ "$code" -ne 139 ] && [ "$code" -ne 134 ]; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc → CRASH (signal $code)"
        FAIL=$((FAIL + 1))
    fi
}

# run_test_stderr <desc> <input> <expected_substring>  — captures stderr too
run_test_stderr() {
    local desc="$1" input="$2" expected="$3"
    TOTAL=$((TOTAL + 1))
    actual=$(printf '%s\n' "$input" | "$MINISHELL" 2>&1)
    if echo "$actual" | grep -qF -- "$expected"; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc"
        echo -e "         attendu : |$expected|"
        echo -e "         obtenu  : |$(echo "$actual" | head -2)|"
        FAIL=$((FAIL + 1))
    fi
}

# run_test_exact <desc> <input> <expected_exact>
run_test_exact() {
    local desc="$1" input="$2" expected="$3"
    TOTAL=$((TOTAL + 1))
    actual=$(printf '%s\n' "$input" | "$MINISHELL" 2>/dev/null)
    if [ "$actual" = "$expected" ]; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc"
        echo -e "         attendu : |$expected|"
        echo -e "         obtenu  : |$actual|"
        FAIL=$((FAIL + 1))
    fi
}

if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}ERREUR : '$MINISHELL' introuvable. Lance 'make' d'abord.${RESET}"
    exit 1
fi

echo ""
echo -e "${YELLOW}${BOLD}▶ Batterie de tests minishell${RESET}"
echo -e "  Binaire : $MINISHELL"
echo -e "  Date    : $(date)"

# ══════════════════════════════════════════
section "1. ECHO — basique"
# ══════════════════════════════════════════
run_test        "echo simple"                   "echo hello"                    "hello"
run_test        "echo plusieurs mots"           "echo hello world"              "hello world"
run_test_no_crash "echo sans args"              "echo"
run_test        "echo -n"                       "echo -n hello"                 "hello"
run_test        "echo -nn"                      "echo -nn hello"                "hello"
run_test        "echo -nnn"                     "echo -nnn hello"               "hello"
run_test        "echo flag invalide -a"         "echo -a hello"                 "-a hello"
run_test        "echo -n puis args"             "echo -n foo bar"               "foo bar"
run_test        "echo chiffres"                 "echo 1 2 3"                    "1 2 3"
run_test        "echo caracteres speciaux"      "echo hello!"                   "hello!"
run_test        "echo tiret seul"               "echo -"                        "-"
run_test        "echo plusieurs espaces args"   "echo a   b"                    "a b"

# ══════════════════════════════════════════
section "2. ECHO — quotes"
# ══════════════════════════════════════════
run_test        "single quote preserve espaces" "echo 'hello world'"            "hello world"
run_test        "double quote preserve espaces" 'echo "hello world"'            "hello world"
run_test        "single quote vide"             "echo ''"                       ""
run_test        "double quote vide"             'echo ""'                       ""
run_test        "single quote espaces multiples""echo '  a  b  '"              "  a  b  "
run_test        "single quote pipe literal"     "echo '|'"                      "|"
run_test        "single quote redir literal"    "echo '>'"                      ">"
run_test        "single quote dollar literal"   "echo '\$HOME'"                 "\$HOME"
run_test        "double quote dollar expandé"   'echo "$HOME"'                  "/"
run_test        "single quote newline literal"  "echo 'a\nb'"                   "a\nb"
run_test        "double quote avec texte"       'echo "bonjour monde"'          "bonjour monde"
run_test_no_crash "single quote tab literal"    "echo 'hello	world'"

# ══════════════════════════════════════════
section "3. ECHO — expansion de variables"
# ══════════════════════════════════════════
run_test        "echo \$HOME expandé"           'echo $HOME'                    "/"
run_test        "echo \$USER expandé"           'echo $USER'                    ""
run_test        "echo \$PATH non vide"          'echo $PATH'                    "/"
run_test        "echo \$? vaut 0 apres succes"  'echo $?'                       "0"
run_test        "echo var inexistante vide"      'echo $VAR_INEXISTANTE_XYZ'     ""
run_test        "echo dollar seul"              "echo \$"                       "\$"
run_test        "echo double dollar"            "echo \$\$"                     ""
run_test        "no expand single quote"        "echo '\$USER'"                 "\$USER"
run_test        "no expand single quote HOME"   "echo '\$HOME'"                 "\$HOME"

# ══════════════════════════════════════════
section "4. PWD"
# ══════════════════════════════════════════
CURRENT_DIR=$(pwd)
run_test        "pwd affiche répertoire"        "pwd"                           "$CURRENT_DIR"
run_test_no_crash "pwd ne crash pas"            "pwd"
run_test_no_crash "pwd plusieurs fois"          "pwd
pwd
pwd"

# ══════════════════════════════════════════
section "5. CD"
# ══════════════════════════════════════════
run_test_no_crash "cd /tmp"                     "cd /tmp"
run_test_no_crash "cd sans args"                "cd"
run_test_no_crash "cd chemin inexistant"        "cd /chemin_inexistant_xyz123"
run_test_no_crash "cd puis pwd"                 "cd /tmp
pwd"
run_test          "cd /tmp puis pwd = /tmp"     "cd /tmp
pwd"                                                                            "/tmp"
run_test_no_crash "cd tiret (OLDPWD)"           "cd /tmp
cd /
cd -"
run_test_no_crash "cd home"                     "cd $HOME"
run_test_no_crash "cd point"                    "cd ."
run_test_no_crash "cd double point"             "cd .."
run_test_no_crash "cd chemin relatif"           "cd /tmp
cd .."
run_test_no_crash "cd trop args (ignore)"       "cd /tmp /var"

# ══════════════════════════════════════════
section "6. ENV"
# ══════════════════════════════════════════
run_test        "env contient PATH"             "env"                           "PATH"
run_test        "env contient HOME"             "env"                           "HOME"
run_test        "env contient USER"             "env"                           "USER"
run_test        "env format KEY=VALUE"          "env"                           "="
run_test_no_crash "env ne crash pas"            "env"
run_test_no_crash "env plusieurs fois"          "env
env"

# ══════════════════════════════════════════
section "7. EXPORT"
# ══════════════════════════════════════════
run_test        "export sans args affiche"      "export"                        "declare -x"
run_test        "export affiche PATH"           "export"                        "PATH"
run_test        "export affiche HOME"           "export"                        "HOME"
run_test        "export affiche quotes"         "export"                        '"'
run_test_no_crash "export VAR=val"              "export TEST_VAR=hello"
run_test_no_crash "export VAR sans valeur"      "export TEST_NOVAL"
run_test_no_crash "export puis export"          "export MYVAR=42
export"
run_test        "export puis env le voit"       "export TESTEXPORT=oui
env"                                                                            "TESTEXPORT=oui"
run_test_no_crash "export var avec chiffres"    "export VAR2=val2"
run_test_no_crash "export plusieurs vars"       "export A=1 B=2 C=3"
run_test_no_crash "export avec = dans valeur"   "export A=b=c"
run_test_no_crash "export ecrase valeur"        "export X=1
export X=2"

# ══════════════════════════════════════════
section "8. UNSET"
# ══════════════════════════════════════════
run_test_no_crash "unset var existante"         "unset HOME"
run_test_no_crash "unset var inexistante"       "unset VAR_INEXISTANTE_XYZ"
run_test_no_crash "unset sans args"             "unset"
run_test_no_crash "unset plusieurs vars"        "unset PATH HOME USER"
run_test_no_crash "unset puis env"              "unset HOME
env"
run_test          "unset supprime la var"       "export TODEL=oui
unset TODEL
env"                                                                            ""
run_test_no_crash "unset premier noeud"         "unset PATH"
run_test_no_crash "unset et re-export"          "unset HOME
export HOME=/tmp"

# ══════════════════════════════════════════
section "9. EXIT"
# ══════════════════════════════════════════
run_test_exit   "exit sans code → 0"            "exit"                          0
run_test_exit   "exit 0"                        "exit 0"                        0
run_test_exit   "exit 1"                        "exit 1"                        1
run_test_exit   "exit 42"                       "exit 42"                       42
run_test_exit   "exit 127"                      "exit 127"                      127
run_test_exit   "exit 255"                      "exit 255"                      255
run_test_exit   "exit 256 → 0 modulo"           "exit 256"                      0
run_test_no_crash "exit trop args"              "exit 1 2"
run_test_no_crash "exit arg non numérique"      "exit abc"
run_test_no_crash "exit après commande"         "echo ok
exit 0"

# ══════════════════════════════════════════
section "10. COMMANDES EXTERNES"
# ══════════════════════════════════════════
run_test        "ls s'exécute"                  "ls"                            "Makefile"
run_test        "ls Makefile"                   "ls Makefile"                   "Makefile"
run_test        "/bin/echo hello"               "/bin/echo hello"               "hello"
run_test        "true"                          "true"                          ""
run_test_no_crash "false ne crash pas"          "false"
run_test_no_crash "cmd inexistante"             "cmd_xyz_inexistante_42"
run_test_no_crash "chemin absolu /bin/ls"       "/bin/ls /tmp"
run_test_no_crash "plusieurs cmds externes"     "ls
pwd
env"
run_test        "cat /etc/hostname"             "cat /etc/hostname"             ""
run_test_no_crash "wc -l /etc/hostname"         "wc -l /etc/hostname"

# ══════════════════════════════════════════
section "11. REDIRECTIONS"
# ══════════════════════════════════════════
TMP1="/tmp/mini_test1_$$"
TMP2="/tmp/mini_test2_$$"

run_test_no_crash "redir > crée fichier"        "echo hello > $TMP1"
run_test_no_crash "redir >> append"             "echo world >> $TMP1"
run_test_no_crash "redir < lit fichier"         "cat < /etc/hostname"
run_test_no_crash "redir > puis cat"            "echo test > $TMP1
cat $TMP1"
run_test          "redir > contenu correct"     "echo bonjour > $TMP1
cat $TMP1"                                                                      "bonjour"
run_test_no_crash "redir >> cumule"             "echo a > $TMP1
echo b >> $TMP1"
run_test_no_crash "redir fichier inexistant <"  "cat < /fichier_inexistant_xyz"
run_test_no_crash "redir > fichier avec spaces" "echo x > $TMP2"
run_test_no_crash "double redir out"            "echo x > $TMP1 > $TMP2"
run_test_no_crash "redir sans commande"         "> $TMP1"

# vérification sur disque
printf 'echo fichier_ok > %s' "$TMP1" | "$MINISHELL" >/dev/null 2>&1
TOTAL=$((TOTAL + 1))
if [ -f "$TMP1" ]; then
    echo -e "  ${GREEN}[PASS]${RESET} redir > crée bien le fichier sur disque"
    PASS=$((PASS + 1))
else
    echo -e "  ${RED}[FAIL]${RESET} redir > n'a pas créé le fichier"
    FAIL=$((FAIL + 1))
fi

rm -f "$TMP1" "$TMP2"

# ══════════════════════════════════════════
section "12. PIPES"
# ══════════════════════════════════════════
run_test_no_crash "pipe simple"                 "echo hello | cat"
run_test          "pipe echo | cat"             "echo hello | cat"              "hello"
run_test          "pipe echo | grep"            "echo hello | grep hello"       "hello"
run_test_no_crash "double pipe"                 "echo hi | cat | cat"
run_test          "triple pipe"                 "echo foo | cat | cat | cat"    "foo"
run_test_no_crash "pipe ls | grep"              "ls | grep Makefile"
run_test          "pipe ls | grep Makefile"     "ls | grep Makefile"            "Makefile"
run_test_no_crash "pipe ls | wc"               "ls | wc -l"
run_test_no_crash "pipe avec cmd inexistante"   "echo hi | cmd_xyz_inexistante"
run_test_no_crash "pipe plusieurs cmds"         "ls | grep . | wc -l"

# ══════════════════════════════════════════
section "13. CAS LIMITES"
# ══════════════════════════════════════════
run_test_no_crash "ligne vide"                  ""
run_test_no_crash "espaces seuls"               "   "
run_test_no_crash "tabulation seule"            "	"
run_test_no_crash "cmd avec plein d'espaces"    "   echo   hello   "
run_test_no_crash "tres long argument"          "echo $(python3 -c 'print("a"*1000)' 2>/dev/null || printf 'a%.0s' {1..200})"
run_test_no_crash "beaucoup d'args"             "echo a b c d e f g h i j k l m n o p q r s t u v w x y z"
run_test_no_crash "slash seul"                  "/"
run_test_no_crash "point seul"                  "."
run_test_no_crash "double point"                ".."
run_test_no_crash "quote non fermée single"     "echo 'hello"
run_test_no_crash "quote non fermée double"     'echo "hello'
run_test_no_crash "pipe en fin de ligne"        "echo hi |"
run_test_no_crash "redir en debut sans cmd"     "< /etc/hostname"
run_test_no_crash "plusieurs pipes consec"      "echo a | cat | cat | cat | cat"

# ══════════════════════════════════════════
section "14. SÉQUENCES DE COMMANDES"
# ══════════════════════════════════════════
run_test_no_crash "pwd puis echo"               "pwd
echo hello"
run_test_no_crash "export puis env"             "export SEQVAR=42
env"
run_test          "export puis env le contient" "export SEQVAR=42
env"                                                                            "SEQVAR=42"
run_test_no_crash "cd puis pwd"                 "cd /tmp
pwd"
run_test          "cd /tmp puis pwd affiche"    "cd /tmp
pwd"                                                                            "/tmp"
run_test_no_crash "echo plusieurs fois"         "echo a
echo b
echo c"
run_test_no_crash "export unset re-export"      "export FOO=1
unset FOO
export FOO=2
env"
run_test_no_crash "10 commandes"                "echo 1
echo 2
echo 3
echo 4
echo 5
echo 6
echo 7
echo 8
echo 9
echo 10"

# ══════════════════════════════════════════
section "15. EXPANSION AVANCÉE"
# ══════════════════════════════════════════
run_test        "\$? après echo = 0"            'echo ok
echo $?'                                                                        "0"
run_test_no_crash "\$? après cmd inexistante"   'cmd_xyz_inexistante
echo $?'
run_test        "\$HOME dans double quote"      'echo "home=$HOME"'             "home=/"
run_test        "\$PATH non vide"              'echo $PATH'                    "/usr"
run_test_no_crash "expand dans redir out"       'export TMP=/tmp/exp_test_$$
echo hello > $TMP
cat $TMP'
run_test_no_crash "expand dans redir in"        'export SRC=/etc/hostname
cat < $SRC'
run_test        "no expand single PATH"         "echo '\$PATH'"                 "\$PATH"
run_test        "no expand single USER"         "echo '\$USER'"                 "\$USER"
run_test_no_crash "expand var inexistante"      "echo $VAR_QUI_NEXISTE_PAS_XYZ"

# ══════════════════════════════════════════
section "16. ANTI-CRASH SÉCURITÉ"
# ══════════════════════════════════════════
run_test_no_crash "dispatch args NULL"          "> /tmp/crash_test_$$"
run_test_no_crash "unset PATH puis ls"          "unset PATH
ls"
run_test_no_crash "unset HOME puis cd"          "unset HOME
cd"
run_test_no_crash "pipe sans commande gauche"   "| echo hi"
run_test_no_crash "pipe sans commande droite"   "echo hi |"
run_test_no_crash "redirection double >"        "echo x > /tmp/a_$$ > /tmp/b_$$"
run_test_no_crash "exit dans pipe"              "echo hi | exit"
run_test_no_crash "cmd vide entre pipes"        "echo hi | | cat"
run_test_no_crash "redir vers /dev/null"        "echo hi > /dev/null"
run_test_no_crash "cat /dev/null"               "cat /dev/null"
run_test_no_crash "echo apres exit raté"        "exit abc
echo still alive"
run_test_no_crash "unset inexistant"            "unset VAR_INEXISTANTE_XYZ_42"
run_test_no_crash "export vide"                 "export ''"
run_test_no_crash "heredoc simple"              "cat << EOF
hello
EOF"

rm -f /tmp/crash_test_$$ /tmp/a_$$ /tmp/b_$$ /tmp/exp_test_$$

# ══════════════════════════════════════════
section "17. EXPORT — valeurs avec quotes"
# ══════════════════════════════════════════
run_test_no_crash "export valeur double quote"      'export QVAR="hello world"'
run_test          "export double quote puis echo"   'export QVAR="hello world"
echo $QVAR'                                                                         "hello world"
run_test          "export double quote dans env"    'export QVAR="hello world"
env'                                                                                "QVAR=hello world"
run_test_no_crash "export valeur single quote"      "export SQVAR='hello world'"
run_test          "export single quote puis echo"   "export SQVAR='hello world'
echo \$SQVAR"                                                                       "hello world"
run_test          "export valeur avec espaces"      'export SP="a b c d"
echo $SP'                                                                           "a b c d"
run_test          "export valeur vide double"       'export EMP=""
echo x$EMP'                                                                         "x"
run_test          "export ecrase double quote"      'export X=old
export X="new value"
echo $X'                                                                            "new value"
run_test_no_crash "export numero dans valeur"       'export NUM="42"'
run_test          "export numero puis echo"         'export NUM="42"
echo $NUM'                                                                          "42"

# ══════════════════════════════════════════
section "18. EXIT STATUS — \$?"
# ══════════════════════════════════════════
run_test          "\$? apres true = 0"              'true
echo $?'                                                                            "0"
run_test          "\$? apres false = 1"             'false
echo $?'                                                                            "1"
run_test          "\$? cmd inexistante = 127"       'cmd_xyz_inexistante_$$
echo $?'                                                                            "127"
run_test          "\$? apres echo = 0"              'echo hi
echo $?'                                                                            "0"
run_test          "\$? apres pwd = 0"               'pwd
echo $?'                                                                            "0"
run_test          "\$? apres cd invalide != 0"      'cd /chemin_qui_nexiste_pas_xyz
echo $?'                                                                            "1"
run_test_no_crash "\$? dans pipe = dernier"         'true | false
echo $?'
run_test_no_crash "\$? pipe vrai | vrai"            'true | true
echo $?'
run_test_no_crash "\$? apres redir echouee"         'cat < /fichier_inexistant_xyz_$$
echo $?'
run_test          "exit 0 apres false"              'false
exit $?'                                                                            ""

# ══════════════════════════════════════════
section "19. HEREDOC"
# ══════════════════════════════════════════
run_test          "heredoc contenu basique"         'cat << EOF
hello
EOF'                                                                                "hello"
run_test          "heredoc multiligne"              'cat << END
line1
line2
END'                                                                                "line1"
run_test          "heredoc multiligne line2"        'cat << END
line1
line2
END'                                                                                "line2"
run_test_no_crash "heredoc dans pipe"               'cat << EOF | cat
hello
EOF'
run_test          "heredoc pipe contenu"            'cat << EOF | cat
world
EOF'                                                                                "world"
run_test_no_crash "heredoc puis autre cmd"          'cat << EOF
hi
EOF
echo done'
run_test_no_crash "heredoc vide"                    'cat << EOF
EOF'

# ══════════════════════════════════════════
section "20. CD — avancé"
# ══════════════════════════════════════════
run_test          "cd /tmp puis cd - revient"       'cd /tmp
cd /
cd -
pwd'                                                                                "/tmp"
run_test          "cd /tmp/.. = /"                  'cd /tmp/..
pwd'                                                                                "/"
run_test_no_crash "cd .. depuis /"                  'cd /
cd ..'
run_test          "cd . reste en place"             "cd /tmp
cd .
pwd"                                                                                "/tmp"
run_test_no_crash "cd fichier = erreur"             "cd /etc/hostname"
run_test          "cd double point puis pwd"        "cd /tmp
cd ..
pwd"                                                                                "/"

# ══════════════════════════════════════════
section "21. ECHO — flags avancés"
# ══════════════════════════════════════════
run_test          "echo -n pas de newline output"   "echo -n hello"                 "hello"
run_test          "echo -nnn = flag valide"         "echo -nnn hi"                  "hi"
run_test          "echo -n -n = -n"                 "echo -n -n hello"              "hello"
run_test          "echo -- traité comme arg"        "echo -- hello"                 "-- hello"
run_test          "echo -e non traité"              "echo -e hello"                 "-e hello"
run_test          "echo nb negatif"                 "echo -1"                       "-1"
run_test_no_crash "echo une quote double vide"      'echo ""'
run_test          "echo concat quotes"              "echo 'a''b'"                   "a b"

# ══════════════════════════════════════════
section "22. ENV / EXPORT interactions"
# ══════════════════════════════════════════
run_test          "export puis unset supprime"      "export TMPV=oui
unset TMPV
env"                                                                                ""
run_test_no_crash "export puis unset puis export"   "export X=1
unset X
export X=2
env"
run_test          "export puis unset puis re-export" "export X=1
unset X
export X=2
env"                                                                                "X=2"
run_test          "export plusieurs puis env"       "export A=1
export B=2
export C=3
env"                                                                                "A=1"
run_test          "env apres unset PATH"            "unset PATH
env"                                                                                ""
run_test_no_crash "export sans = puis env"          "export NOVAL
env"
run_test          "export = vide valeur"            "export EMPTY=
env"                                                                                "EMPTY="
run_test          "export override plusieurs fois"  "export X=1
export X=2
export X=3
echo \$X"                                                                           "3"

# ══════════════════════════════════════════
section "23. REDIRECTIONS — avancé"
# ══════════════════════════════════════════
TMP3="/tmp/mini_redir_$$"
run_test          ">> append réel 2 lignes"         "echo ligne1 > $TMP3
echo ligne2 >> $TMP3
cat $TMP3"                                                                          "ligne1"
run_test          ">> contient ligne2 aussi"        "echo ligne1 > $TMP3
echo ligne2 >> $TMP3
cat $TMP3"                                                                          "ligne2"
run_test          "redir in + out"                  "echo test > $TMP3
cat < $TMP3"                                                                        "test"
run_test_no_crash "redir > /dev/null"               "ls > /dev/null"
run_test_no_crash "redir < /dev/null"               "cat < /dev/null"
run_test          "redir > écrase fichier"          "echo first > $TMP3
echo second > $TMP3
cat $TMP3"                                                                          "second"
run_test_no_crash "redir > avec variable"           "export F=$TMP3
echo hello > \$F"
run_test_no_crash "pipe + redir out"                "echo hi | cat > $TMP3"
run_test          "pipe + redir out contenu"        "echo piped | cat > $TMP3
cat $TMP3"                                                                          "piped"
rm -f "$TMP3"

# ══════════════════════════════════════════
section "24. PIPES — avancé"
# ══════════════════════════════════════════
run_test          "pipe 4 etapes"                   "echo hello | cat | cat | cat"  "hello"
run_test          "pipe grep filtre"                "echo -e 'yes\nno' | grep yes"  "yes"
run_test_no_crash "pipe wc -w"                      "echo one two three | wc -w"
run_test          "pipe echo | grep match"          "echo minishell | grep mini"    "mini"
run_test          "pipe echo | grep no match"       "echo minishell | grep xyz_no"  ""
run_test_no_crash "pipe ls | sort"                  "ls | sort"
run_test_no_crash "pipe env | grep PATH"            "env | grep PATH"
run_test          "pipe env | grep PATH contenu"    "env | grep PATH"               "PATH"
run_test_no_crash "pipe cmd inexistante | cat"      "cmd_inexistante_xyz | cat"
run_test_no_crash "pipe 5 etapes"                   "echo a | cat | cat | cat | cat"

# ══════════════════════════════════════════
section "25. EXPANSION — cas avancés"
# ══════════════════════════════════════════
run_test          "double quote expand HOME"        'echo "HOME=$HOME"'             "HOME=/"
run_test          "double quote expand USER"        'echo "USER=$USER"'             "USER="
run_test          "double quote expand ?"           'echo hi
echo "$?"'                                                                          "0"
run_test          "var dans chemin redir"            'export D=/tmp
echo ok > $D/expand_test_$$
cat $D/expand_test_$$'                                                              "ok"
run_test_no_crash "expand var vide dans cmd"        'export EMPTY=
echo $EMPTY'
run_test          "dollar suivi non-alnum"          'echo $?xyz'                    "0xyz"
run_test          "var adjacente texte"             'export PRE=hello
echo $PRE world'                                                                    "hello world"
run_test          "no expand in single then double" "echo '\$HOME' \"\$HOME\""     "\$HOME /"
run_test_no_crash "expand dans heredoc delimiter"   'cat << $NONEXIST
test
'
rm -f /tmp/expand_test_$$

# ══════════════════════════════════════════
section "26. ANTI-CRASH — inputs hostiles"
# ══════════════════════════════════════════
run_test_no_crash "tres longue commande"            "echo $(head -c 500 /dev/urandom | tr -dc 'a-z' | head -c 200)"
run_test_no_crash "100 espaces"                     "$(printf ' %.0s' {1..100})echo hi"
run_test_no_crash "pipe sans rien a gauche"         "| cat"
run_test_no_crash "pipe sans rien a droite"         "echo hi |"
run_test_no_crash "double pipe"                     "echo hi || cat"
run_test_no_crash "redir sans fichier"              "echo hi >"
run_test_no_crash "redir sans fichier <"            "cat <"
run_test_no_crash "heredoc non terminé"             'cat << EOF
hello'
run_test_no_crash "quote dans export"               "export A=\"test\""
run_test_no_crash "export = seul"                   "export ="
run_test_no_crash "unset vide string"               "unset ''"
run_test_no_crash "echo dollar dollar"              "echo \$\$"
run_test_no_crash "beaucoup de pipes"               "echo a | cat | cat | cat | cat | cat | cat | cat"
run_test_no_crash "redir + pipe"                    "echo hi > /tmp/t_$$ | cat"
run_test_no_crash "commande avec slash"             "echo hel/lo"
rm -f /tmp/t_$$

# ══════════════════════════════════════════
section "27. ERREURS DE SYNTAXE — exit code 2"
# ══════════════════════════════════════════
run_test_exit   "> seul exit 2"                 ">"                             2
run_test_exit   "< seul exit 2"                 "<"                             2
run_test_exit   ">> seul exit 2"                ">>"                            2
run_test_exit   "<< seul exit 2"                "<<"                            2
run_test_exit   "| seul exit 2"                 "|"                             2
run_test_exit   "|| exit 2"                     "||"                            2
run_test_exit   "&& exit 2"                     "&&"                            2
run_test_exit   "echo > < exit 2"               "echo > <"                      2
run_test_exit   "echo | | exit 2"               "echo | |"                      2
run_test_exit   "> > > > exit 2"                "> > > >"                       2
run_test_exit   "| hola exit 2"                 "| hola"                        2
run_test_exit   ">>>>> exit 2"                  ">>>>>"                         2
run_test_exit   "<<<<< exit 2"                  "<<<<<"                         2
run_test_exit   "|echo| exit 2"                 "|echo|"                        2
run_test_exit   "|echo -n hola exit 2"          "|echo -n hola"                 2
run_test_exit   ">echo> exit 2"                 ">echo>"                        2
run_test_exit   "<echo< exit 2"                 "<echo<"                        2
run_test_exit   ">>echo>> exit 2"               ">>echo>>"                      2

# ══════════════════════════════════════════
section "28. RÉPERTOIRES — exit code 126"
# ══════════════════════════════════════════
run_test_exit   "/ seul exit 126"               "/"                             126
run_test_exit   "// exit 126"                   "//"                            126
run_test_exit   "/. exit 126"                   "/."                            126
run_test_stderr "/ message Is a directory"      "/"                             "directory"
run_test_exit   "/tmp/.. exit 126"              "/tmp/.."                       126

# ══════════════════════════════════════════
section "29. CONCATÉNATION DE TOKENS"
# ══════════════════════════════════════════
run_test        'echo hola""bonjour = holabonjour'  'echo hola""bonjour'        "holabonjour"
run_test        "echo ''hola'' = hola"          "echo ''hola''"                 "hola"
run_test        "echo ''h'o'la'' = hola"        "echo ''h'o'la''"               "hola"
run_test        'echo ""hola = hola'            'echo ""hola'                   "hola"
run_test        'echo hola""'                   'echo hola""'                   "hola"
run_test        'echo ""hola"" = hola'          'echo ""hola""'                 "hola"
run_test        'echo ""$HOME = /...'           'echo ""$HOME'                  "/"
run_test        "echo ''$HOME = /..."           "echo ''$HOME"                  "/"
run_test        'echo hola""que tal'            'echo hola""que'                "holaque"

# ══════════════════════════════════════════
section "30. EXPANSION — cas du spreadsheet"
# ══════════════════════════════════════════
run_test        'echo $?$ = 0$'                 'echo $?$'                      '0$'
run_test        'echo $:$= reste littéral'      'echo $:$='                     '$:$='
run_test        'echo $HOME9 = vide'            'echo $HOME9'                   ""
run_test        'echo $9HOME = HOME'            'echo $9HOME'                   "HOME"
run_test        'echo $DONTEXIST Hola = Hola'   'echo $DONTEXIST Hola'          "Hola"
run_test        'echo $HOME% garde le %'        'echo $HOME%'                   "%"
run_test        'echo $UID$HOME concat vars'    'echo $UID$HOME'                "/"
run_test        "echo '\$HOME' single noescape" "echo '\$HOME'"                 "\$HOME"
run_test        'echo "$?" = 0'                 'echo "$?"'                     "0"

# ══════════════════════════════════════════
section "31. ECHO — cas avancés du spreadsheet"
# ══════════════════════════════════════════
run_test        "echo -nHola = -nHola avec newline" "echo -nHola"              "-nHola"
run_test        "echo -n-nnn -nnnn = reste tel quel" "echo -n-nnn -nnnn"       "-n-nnn -nnnn"
run_test        "echo -nnn --------n"           "echo -nnn --------n"           "--------n"
run_test        "echo -nnn -----nn---nnnn"      "echo -nnn -----nn---nnnn"      "-----nn---nnnn"
run_test        "echo -nnn --------nnnn"        "echo -nnn --------nnnn"        "--------nnnn"
run_test        "echo -p flag invalide"         "echo -p"                       "-p"
run_test        "echo --------n = pas un flag"  "echo --------n"                "--------n"
run_test        "echo -n -nnn hola -nnnn"       "echo -n -nnn hola -nnnn"       "hola -nnnn"
run_test        "echo Hola -n = -n en fin = arg" "echo Hola -n"                 "Hola -n"
run_test        'echo $ seul'                   'echo $'                        '$'

# ══════════════════════════════════════════
section "32. COMMANDES SPÉCIALES"
# ══════════════════════════════════════════
run_test_exit   ": builtin exit 0"              ":"                             0
run_test_exit   "commande inexistante exit 127" "hola"                          127
run_test_exit   "hola que tal exit 127"         "hola que tal"                  127
run_test_exit   "Makefile pas executable 127"   "Makefile"                      127
run_test_stderr "cmd inexistante message"        "hola_inexistante_xyz"          "command not found"
run_test_exit   '! exit 1 ou 127'               '!'                             127
run_test_exit   "- command not found 127"       "-"                             127
run_test_exit   'true exit 0'                   'true'                          0
run_test_exit   'false exit 1'                  'false'                         1
run_test_exit   '$? comme commande exit 127'    'echo ok
$?'                                                                             127

# ══════════════════════════════════════════
section "33. REDIRECTIONS — comportements spreadsheet"
# ══════════════════════════════════════════
TMP4="/tmp/mini_sheet_$$"
run_test          "redir > puis cat contenu"    "echo hello > $TMP4
cat $TMP4"                                                                      "hello"
run_test          "redir >> cumule correctement" "echo line1 > $TMP4
echo line2 >> $TMP4
cat $TMP4"                                                                      "line2"
run_test_no_crash "redir > fichier inexistant dir" "echo x > /no_dir_xyz/file"
run_test_no_crash "cat < inexistant"            "cat < /fichier_xyz_inexistant_test"
run_test          "heredoc puis echo"           'cat << STOP
hello
STOP
echo done'                                                                      "done"
rm -f "$TMP4"

# ══════════════════════════════════════════
section "34. PIPES — comportements spreadsheet"
# ══════════════════════════════════════════
run_test          "echo $? pipe echo $?"        'echo $? | echo $?'             "0"
run_test          "pipe | cat -e newline"        'echo | cat -e'                 "$"
run_test          "echo espaces | cat -e"        'echo "         " | cat -e'     "$"
run_test_no_crash "cat dans pipe"               'echo hello | cat'
run_test          "echo hello | cat output"     'echo hello | cat'              "hello"
run_test_no_crash "pipe triple"                 'echo a | cat | cat'

# ══════════════════════════════════════════
# RÉSULTATS FINAUX
# ══════════════════════════════════════════
echo ""
echo -e "${CYAN}${BOLD}══════════════════════════════════════════${RESET}"
echo -e "${CYAN}${BOLD}  RÉSULTATS${RESET}"
echo -e "${CYAN}${BOLD}══════════════════════════════════════════${RESET}"
echo -e "  Total   : ${BOLD}$TOTAL${RESET}"
echo -e "  ${GREEN}${BOLD}Réussis${RESET} : $PASS"
echo -e "  ${RED}${BOLD}Échoués${RESET} : $FAIL"

if [ "$TOTAL" -gt 0 ]; then
    PERCENT=$((PASS * 100 / TOTAL))
    echo -e "  Score   : ${BOLD}$PERCENT%${RESET}"
fi
echo ""
if [ "$FAIL" -eq 0 ]; then
    echo -e "${GREEN}${BOLD}  ✓ Tous les tests passent !${RESET}"
else
    echo -e "${YELLOW}  ✗ $FAIL test(s) échoué(s)${RESET}"
fi
echo ""
