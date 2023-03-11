#include "utils.h"
#define LUNGCUV 35
#define MAX 1000
void SolveTask3() {
  char s[MAX], text[MAX][LUNGCUV], gasit[MAX][LUNGCUV], gram[MAX][LUNGCUV];
  char *token, *rest = NULL;
  int i = 0, aparitii[MAX] = {0};
  while (fgets(s, MAX, stdin)) {
    s[strcspn(s, "\n")] = '\0';
    token = strtok_r(s, " .,!;\n", & rest);
    while (token != NULL) {
      snprintf(text[i], "%s", token);
      i++;
      token = strtok_r(NULL, " .,!;\n", & rest);
    }
  }
  int n = i, j = 0, c = 0, k = 0, cn = 1, ok = 0, nr_n_grame = 0;
  for (i = 0; i <= n - 1; i++) {
    c = 1;
    for (j = i + 1; j <= n - 2; j++) {
if (strcmp(text[i], text[j]) == 0 && strcmp(text[i + 1], text[j + 1]) == 0) {
        c++;
      }
    }
    cn++;
    ok = 1;
    for (k = 0; k <= cn; k++) {
if (strcmp(text[i], gasit[k]) == 0 && strcmp(text[i + 1], gasit[k + 1]) == 0) {
        ok = 0;
        k = cn;
      }
    }
    if (ok == 1) {
       snprintf(gram[nr_n_grame], "%s", text[i]);
       strncat(gram[nr_n_grame], " ", 1);
       strncat(gram[nr_n_grame], text[i+1], strlen(text[i+1]));
       aparitii[nr_n_grame] = c;
      nr_n_grame++;
    }
    snprintf(gasit[i], "%s", text[i]);
    snprintf(gasit[i + 1], "%s", text[i + 1]);
  }
  printf("%d\n", nr_n_grame);
  cn = 1;
  for (i = 0; i < nr_n_grame; i++) {
    printf("%s %d\n", gram[i], aparitii[i]);
  }
}
