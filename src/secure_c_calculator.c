#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STR_LEN 100
#define MAX_REPEAT_LEN 1024
#define INPUT_BUF 256

/* ---------- Utility Functions ---------- */

bool has_whitespace(const char *s) {
    while (*s) {
        if (isspace((unsigned char)*s)) return true;
        s++;
    }
    return false;
}

bool valid_characters(const char *s) {
    while (*s) {
        if (!(isalnum((unsigned char)*s) ||
              *s == '+' || *s == '-' || *s == '*' ||
              *s == '/' || *s == '%')) {
            return false;
        }
        s++;
    }
    return true;
}

bool is_all_letters(const char *s) {
    if (*s == '\0') return false;
    while (*s) {
        if (!isalpha((unsigned char)*s)) return false;
        s++;
    }
    return true;
}

bool is_all_digits(const char *s) {
    if (*s == '\0') return false;
    while (*s) {
        if (!isdigit((unsigned char)*s)) return false;
        s++;
    }
    return true;
}

bool parse_int_safe(const char *s, int *out) {
    long val = 0;
    while (*s) {
        val = val * 10 + (*s - '0');
        if (val > INT_MAX) return false;
        s++;
    }
    *out = (int)val;
    return true;
}

/* ---------- String Operations ---------- */

void shift_string(char *s, int n, bool right) {
    n = n % 26;
    for (int i = 0; s[i]; i++) {
        char base = islower(s[i]) ? 'a' : 'A';
        int offset = s[i] - base;
        offset = right ? (offset + n) % 26 : (offset - n + 26) % 26;
        s[i] = base + offset;
    }
}

void repeat_string(const char *s, int n) {
    char result[MAX_REPEAT_LEN + 1] = {0};
    int len = strlen(s);
    int pos = 0;

    for (int i = 0; i < n && pos + len <= MAX_REPEAT_LEN; i++) {
        memcpy(result + pos, s, len);
        pos += len;
    }

    result[pos] = '\0';
    printf("%s\n", result);
}

void cut_string(char *s, int n) {
    int len = strlen(s);
    if (n >= len) {
        s[0] = '\0';
    } else {
        s[len - n] = '\0';
    }
}

/* ---------- Main ---------- */

int main(void) {
    char input[INPUT_BUF];

    while (1) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        /* Basic validation */
        if (has_whitespace(input)) {
            printf("Error: whitespace not allowed\n");
            continue;
        }

        if (!valid_characters(input)) {
            printf("Error: invalid characters\n");
            continue;
        }

        /* Find operator */
        char *op_ptr = NULL;
        char op = 0;
        for (int i = 0; input[i]; i++) {
            if (strchr("+-*/%", input[i])) {
                op = input[i];
                op_ptr = &input[i];
                break;
            }
        }

        if (!op_ptr) {
            printf("Error: no operator found\n");
            continue;
        }

        char left[INPUT_BUF], right[INPUT_BUF];
        strncpy(left, input, op_ptr - input);
        left[op_ptr - input] = '\0';
        strcpy(right, op_ptr + 1);

        if (*left == '\0' || *right == '\0') {
            printf("Error: missing operand\n");
            continue;
        }

        bool left_is_str = is_all_letters(left);
        bool right_is_str = is_all_letters(right);
        bool left_is_int = is_all_digits(left);
        bool right_is_int = is_all_digits(right);

        if (left_is_str && right_is_str) {
            printf("Error: both operands cannot be strings\n");
            continue;
        }

        /* ---------- Integer + Integer ---------- */
        if (left_is_int && right_is_int) {
            int a, b;
            if (!parse_int_safe(left, &a) || !parse_int_safe(right, &b)) {
                printf("Error: integer out of range\n");
                continue;
            }

            long result;
            switch (op) {
                case '+':
                    result = (long)a + b;
                    break;
                case '-':
                    result = (long)a - b;
                    break;
                case '*':
                    result = (long)a * b;
                    break;
                case '/':
                    if (b == 0) {
                        printf("Error: divide by zero\n");
                        continue;
                    }
                    result = a / b;
                    break;
                case '%':
                    if (b == 0) {
                        printf("Error: divide by zero\n");
                        continue;
                    }
                    result = a % b;
                    break;
                default:
                    printf("Error: invalid operator\n");
                    continue;
            }

            if (result > INT_MAX || result < INT_MIN) {
                printf("Error: integer overflow/underflow\n");
                continue;
            }

            printf("%ld\n", result);
            continue;
        }

        /* ---------- Integer + String ---------- */
        char *str;
        int n;

        if (left_is_str && right_is_int) {
            str = left;
            if (!parse_int_safe(right, &n)) {
                printf("Error: integer out of range\n");
                continue;
            }
        } else if (left_is_int && right_is_str) {
            str = right;
            if (!parse_int_safe(left, &n)) {
                printf("Error: integer out of range\n");
                continue;
            }
        } else {
            printf("Error: invalid operand combination\n");
            continue;
        }

        if ((int)strlen(str) > MAX_STR_LEN) {
            printf("Error: string too long\n");
            continue;
        }

        if (op == '%') {
            printf("Error: %% not allowed with strings\n");
            continue;
        }

        char buffer[MAX_STR_LEN + 1];
        strcpy(buffer, str);

        switch (op) {
            case '+':
                shift_string(buffer, n, true);
                printf("%s\n", buffer);
                break;
            case '-':
                shift_string(buffer, n, false);
                printf("%s\n", buffer);
                break;
            case '*':
                repeat_string(buffer, n);
                break;
            case '/':
                cut_string(buffer, n);
                printf("%s\n", buffer);
                break;
            default:
                printf("Error: invalid operator\n");
        }
    }

    return 0;
}
