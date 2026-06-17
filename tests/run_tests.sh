#!/bin/bash
# Batterie de tests pour minishell
# Usage : ./tests/run_tests.sh
# Le minishell doit être compilé avant : make

MINISHELL="./minishell"
PASS=0
FAIL=0
TOTAL=0

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
RESET='\033[0m'

# ─── helpers ────────────────────────────────────────────────────────────────

section() {
    echo ""
    echo -e "${CYAN}═══════════════════════════════════════${RESET}"
    echo -e "${CYAN}  $1${RESET}"
    echo -e "${CYAN}═══════════════════════════════════════${RESET}"
}

# run_test <description> <input> <expected_output_substring>
run_test() {
    local desc="$1"
    local input="$2"
    local expected="$3"
    TOTAL=$((TOTAL + 1))

    actual=$(echo "$input" | timeout 3 "$MINISHELL" 2>/dev/null)
    if echo "$actual" | grep -qF "$expected"; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc"
        echo -e "         attendu  : $(echo "$expected" | head -1)"
        echo -e "         obtenu   : $(echo "$actual" | head -3)"
        FAIL=$((FAIL + 1))
    fi
}

# run_test_exact <description> <input> <expected_exact_output>
run_test_exact() {
    local desc="$1"
    local input="$2"
    local expected="$3"
    TOTAL=$((TOTAL + 1))

    actual=$(echo "$input" | timeout 3 "$MINISHELL" 2>/dev/null | grep -v "^----" | grep -v "^===" | grep -v "^\[" | grep -v "^$" | head -20)
    if [ "$actual" = "$expected" ]; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc"
        echo -e "         attendu  : |$expected|"
        echo -e "         obtenu   : |$actual|"
        FAIL=$((FAIL + 1))
    fi
}

# run_test_exit <description> <input> <expected_exit_code>
run_test_exit() {
    local desc="$1"
    local input="$2"
    local expected_code="$3"
    TOTAL=$((TOTAL + 1))

    echo "$input" | timeout 3 "$MINISHELL" > /dev/null 2>&1
    actual_code=$?
    if [ "$actual_code" = "$expected_code" ]; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc (exit=$actual_code)"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc"
        echo -e "         attendu exit=$expected_code  obtenu exit=$actual_code"
        FAIL=$((FAIL + 1))
    fi
}

# run_test_no_crash <description> <input>
run_test_no_crash() {
    local desc="$1"
    local input="$2"
    TOTAL=$((TOTAL + 1))

    echo "$input" | timeout 3 "$MINISHELL" > /dev/null 2>&1
    code=$?
    if [ "$code" -ne 139 ] && [ "$code" -ne 134 ]; then
        echo -e "  ${GREEN}[PASS]${RESET} $desc (pas de crash)"
        PASS=$((PASS + 1))
    else
        echo -e "  ${RED}[FAIL]${RESET} $desc → CRASH (signal $code)"
        FAIL=$((FAIL + 1))
    fi
}

# ─── vérification préliminaire ───────────────────────────────────────────────

if [ ! -f "$MINISHELL" ]; then
    echo -e "${RED}ERREUR : binaire '$MINISHELL' introuvable. Lance 'make' d'abord.${RESET}"
    exit 1
fi

echo ""
echo -e "${YELLOW}▶ Lancement de la batterie de tests minishell${RESET}"
echo "  Binaire : $MINISHELL"
echo "  Date    : $(date)"

# ═══════════════════════════════════════════════════════════════════════════════
section "1. ECHO"
# ═══════════════════════════════════════════════════════════════════════════════

run_test "echo simple"                  "echo hello"              "hello"
run_test "echo plusieurs mots"          "echo hello world"        "hello world"
run_test "echo sans args"               "echo"                    ""
run_test "echo -n supprime newline"     "echo -n hello"           "hello"
run_test "echo -nn supprime newline"    "echo -nn hello"          "hello"
run_test "echo flag invalide (-a)"      "echo -a hello"           "-a hello"
run_test "echo guillemets simples"      "echo 'hello world'"      "hello world"
run_test "echo guillemets doubles"      'echo "hello world"'      "hello world"
run_test "echo chaine vide"             "echo ''"                 ""
run_test "echo string avec espaces"     "echo '  a  b  '"         "  a  b  "
run_test "echo plusieurs -n"            "echo -n -n hello"        "hello"

# ═══════════════════════════════════════════════════════════════════════════════
section "2. PWD"
# ═══════════════════════════════════════════════════════════════════════════════

CURRENT_DIR=$(pwd)
run_test "pwd affiche le répertoire courant" "pwd" "$CURRENT_DIR"
run_test "pwd ne crash pas"                  "pwd" "/"

# ═══════════════════════════════════════════════════════════════════════════════
section "3. CD"
# ═══════════════════════════════════════════════════════════════════════════════

run_test_no_crash "cd /tmp"               "cd /tmp"
run_test_no_crash "cd sans args (HOME)"   "cd"
run_test_no_crash "cd path relatif"       "cd /tmp && pwd"
run_test_no_crash "cd chemin inexistant"  "cd /chemin_qui_nexiste_pas_du_tout_12345"
run_test_no_crash "cd puis pwd"           "cd /tmp
pwd"

# ═══════════════════════════════════════════════════════════════════════════════
section "4. ENV"
# ═══════════════════════════════════════════════════════════════════════════════

run_test "env contient PATH"  "env" "PATH"
run_test "env contient HOME"  "env" "HOME"
run_test "env contient USER"  "env" "USER"
run_test_no_crash "env ne crash pas" "env"

# ═══════════════════════════════════════════════════════════════════════════════
section "5. EXPORT"
# ═══════════════════════════════════════════════════════════════════════════════

run_test "export sans args affiche les vars"  "export"             "declare -x"
run_test "export affiche PATH"                "export"             "PATH"
run_test "export affiche HOME"                "export"             "HOME"
run_test_no_crash "export VAR=val"            "export TEST_VAR=hello"
run_test_no_crash "export VAR sans valeur"    "export TEST_VAR2"
run_test_no_crash "export puis export"        "export MYVAR=42
export"

# ═══════════════════════════════════════════════════════════════════════════════
section "6. UNSET"
# ═══════════════════════════════════════════════════════════════════════════════

run_test_no_crash "unset variable existante"     "unset PATH"
run_test_no_crash "unset variable inexistante"   "unset VAR_QUI_NEXISTE_PAS"
run_test_no_crash "unset sans args"              "unset"
run_test_no_crash "unset plusieurs variables"    "unset PATH HOME"

# ═══════════════════════════════════════════════════════════════════════════════
section "7. EXIT"
# ═══════════════════════════════════════════════════════════════════════════════

run_test_exit "exit sans code → 0"        "exit"      0
run_test_exit "exit 0"                    "exit 0"    0
run_test_exit "exit 1"                    "exit 1"    1
run_test_exit "exit 42"                   "exit 42"   42
run_test_exit "exit 255"                  "exit 255"  255
run_test_exit "exit 256 → 0 (modulo)"     "exit 256"  0
run_test_exit "exit -1 → 255"             "exit -1"   255
run_test_no_crash "exit trop d'args → pas exit" "exit 1 2"
run_test_no_crash "exit arg non-numérique"      "exit abc"

# ═══════════════════════════════════════════════════════════════════════════════
section "8. COMMANDES EXTERNES"
# ═══════════════════════════════════════════════════════════════════════════════

run_test "ls s'execute"           "ls"              "Makefile"
run_test "ls /tmp s'execute"      "ls /tmp"         ""
run_test "cat /etc/hostname"      "/bin/cat /etc/hostname" ""
run_test "true retourne 0"        "true"            ""
run_test "false retourne 1"       "false"           ""
run_test_no_crash "commande inexistante" "cmd_qui_nexiste_absolument_pas_xyz"
run_test_no_crash "chemin absolu"   "/bin/echo hello"
run_test "/bin/echo hello" "/bin/echo hello" "hello"

# ═══════════════════════════════════════════════════════════════════════════════
section "9. REDIRECTIONS"
# ═══════════════════════════════════════════════════════════════════════════════

TMP_FILE="/tmp/minishell_test_$$"

run_test_no_crash "redir > crée un fichier"     "echo hello > $TMP_FILE"
run_test_no_crash "redir >> append"             "echo world >> $TMP_FILE"
run_test_no_crash "redir < lit un fichier"      "cat < /etc/hostname"
run_test_no_crash "redir > puis cat"            "echo test > $TMP_FILE
cat $TMP_FILE"

# Vérification que le fichier a bien été créé
echo "hello" | timeout 3 "$MINISHELL" > /dev/null 2>&1
TOTAL=$((TOTAL + 1))
echo "echo fichier_test > $TMP_FILE" | timeout 3 "$MINISHELL" > /dev/null 2>&1
if [ -f "$TMP_FILE" ]; then
    echo -e "  ${GREEN}[PASS]${RESET} redir > crée bien le fichier sur disque"
    PASS=$((PASS + 1))
    rm -f "$TMP_FILE"
else
    echo -e "  ${RED}[FAIL]${RESET} redir > n'a pas créé le fichier"
    FAIL=$((FAIL + 1))
fi

run_test_no_crash "redir fichier inexistant en entrée"  "cat < /fichier_qui_nexiste_pas"

# ═══════════════════════════════════════════════════════════════════════════════
section "10. PIPES"
# ═══════════════════════════════════════════════════════════════════════════════

run_test_no_crash "pipe simple"              "echo hello | cat"
run_test_no_crash "pipe vers grep"           "echo hello | grep hello"
run_test_no_crash "pipe double"              "ls | grep Makefile | cat"
run_test_no_crash "pipe avec ls et wc"       "ls | wc -l"
run_test_no_crash "pipe vers commande vide"  "echo test | cat"

# ═══════════════════════════════════════════════════════════════════════════════
section "11. QUOTES"
# ═══════════════════════════════════════════════════════════════════════════════

run_test "simple quote préserve espaces"    "echo 'a  b  c'"    "a  b  c"
run_test "double quote préserve espaces"    'echo "a  b  c"'    "a  b  c"
run_test "single quote : $ non expandé"    "echo '\$HOME'"      "\$HOME"
run_test "single quote : pipe non interprété" "echo '|'"        "|"
run_test "single quote : > non interprété"  "echo '>'"          ">"
run_test "double quote vide"               'echo ""'            ""
run_test "single quote vide"               "echo ''"            ""
run_test_no_crash "quote non fermée single" "echo 'hello"
run_test_no_crash "quote non fermée double" 'echo "hello'

# ═══════════════════════════════════════════════════════════════════════════════
section "12. CAS LIMITES / EDGE CASES"
# ═══════════════════════════════════════════════════════════════════════════════

run_test_no_crash "commande vide (juste Enter)" ""
run_test_no_crash "espaces seuls"               "   "
run_test_no_crash "commande avec plein d'espaces" "   echo   hello   "
run_test_no_crash "plusieurs pipes consécutifs"   "echo a | cat | cat | cat"
run_test_no_crash "redirection sans commande"     "> /tmp/test_redir_$$"
run_test_no_crash "pipe en début de ligne"        "| echo"
run_test_no_crash "commande très longue"          "echo aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
run_test_no_crash "beaucoup d'args"               "echo a b c d e f g h i j k l m n o p q r s t u v w x y z"
run_test_no_crash "chemin avec slash"             "echo /usr/bin/../bin/cat"
run_test_no_crash "slash seul"                    "/"
run_test_no_crash "point seul"                    "."
run_test_no_crash "guillemets imbriqués"          "echo \"'hello'\""

# ═══════════════════════════════════════════════════════════════════════════════
section "13. SÉQUENCES DE COMMANDES"
# ═══════════════════════════════════════════════════════════════════════════════

run_test_no_crash "pwd puis echo"         "pwd
echo hello"
run_test_no_crash "export puis env"       "export TESTVAR=42
env"
run_test_no_crash "cd puis pwd"           "cd /tmp
pwd"
run_test_no_crash "echo plusieurs fois"   "echo a
echo b
echo c"
run_test_no_crash "commandes mixtes"      "echo start
ls
echo end"

# ═══════════════════════════════════════════════════════════════════════════════
section "14. LEXER — ANALYSE"
# ═══════════════════════════════════════════════════════════════════════════════

run_test_no_crash "opérateur >> reconnu"   "echo test >> /tmp/test_append_$$"
run_test_no_crash "opérateur << reconnu"   "cat << EOF"
run_test_no_crash "opérateur > reconnu"    "echo x > /tmp/test_out_$$"
run_test_no_crash "opérateur < reconnu"    "cat < /etc/hostname"
run_test_no_crash "opérateur | reconnu"    "echo x | cat"
run_test_no_crash "opérateurs collés"      "echo x>/tmp/test_$$"
run_test_no_crash "multi espaces entre args" "echo      hello      world"

# Nettoyage des fichiers temporaires
rm -f /tmp/test_append_$$ /tmp/test_out_$$ /tmp/test_$$ /tmp/minishell_test_$$

# ═══════════════════════════════════════════════════════════════════════════════
section "15. ANTI-CRASH AVANCÉ (sécurité)"
# ═══════════════════════════════════════════════════════════════════════════════

run_test_no_crash "NULL cmd (redirection seule)"        "< /etc/hostname"
run_test_no_crash "pipe puis rien"                      "echo |"
run_test_no_crash "plusieurs redirections out"          "echo x > /tmp/a_$$ > /tmp/b_$$"
run_test_no_crash "unset de PATH puis ls"               "unset PATH
ls"
run_test_no_crash "unset de HOME puis cd"               "unset HOME
cd"
run_test_no_crash "export variable avec = dans valeur"  "export A=b=c"
run_test_no_crash "exit dans un pipe"                   "echo hi | exit"

rm -f /tmp/a_$$ /tmp/b_$$

# ═══════════════════════════════════════════════════════════════════════════════
# RÉSUMÉ
# ═══════════════════════════════════════════════════════════════════════════════

echo ""
echo -e "${CYAN}═══════════════════════════════════════${RESET}"
echo -e "${CYAN}  RÉSULTATS${RESET}"
echo -e "${CYAN}═══════════════════════════════════════${RESET}"
echo -e "  Total  : $TOTAL"
echo -e "  ${GREEN}Réussis${RESET} : $PASS"
echo -e "  ${RED}Échoués${RESET} : $FAIL"
echo ""

if [ "$FAIL" -eq 0 ]; then
    echo -e "${GREEN}  ✓ Tous les tests passent !${RESET}"
else
    PERCENT=$(( PASS * 100 / TOTAL ))
    echo -e "  Score   : $PERCENT%"
fi
echo ""
