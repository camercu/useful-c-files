##########################################################
# Example test script to run unit tests (see minunit.h)
# Assumes test binaries are in a "tests/" subfolder
# Adapted from Zed Shaw's "Learn C the Hard Way"
# url: http://c.learncodethehardway.org/book/ex28.html
##########################################################

ATTR_BOLD=$(tput bold)
ATTR_RESET=$(tput sgr0)
COLOR_RED=$(tput setaf 1)

echo "${ATTR_BOLD}>>> Running unit tests:${ATTR_RESET}"

for test in *_tests; do
	if [ -f "$test" ]; then
		VALGRIND=
		# VALGRIND="valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind-${test}.log"
		if $VALGRIND "./$test" 2>> tests.log; then
			echo "${ATTR_BOLD}+++ $test PASS${ATTR_RESET}"
		else
			echo "${COLOR_RED}${ATTR_BOLD}>>> ERROR in test '$test':${ATTR_RESET} here's the tests.log"
			echo "--------------"
			tail tests.log
			exit 1
		fi
	fi
done

echo