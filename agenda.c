#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>

#define DAYS(n) 60*60*24*n
#define BUF_SIZE 11

struct tm*
now()
{
  time_t t = time(NULL);
  return localtime(&t);
}

time_t
midnight()
{
  struct tm *t = now();
  t->tm_hour = 0;
  t->tm_min = 0;
  t->tm_sec = 0;
  return mktime(t);
}

const char*
env(const char *name, const char *alt)
{
  const char *value = getenv(name);
  return value ? value : alt;
}

int
main(int argc, char *argv[])
{
  int ch, advance = 0, backward = DAYS(14);
  const char *editor = env("EDITOR", "vi");
  const char *calendarfile = env("CALENDAR_FILE", "/home/mat/CALENDAR");
  char *line = NULL;
  size_t linelen = 0;
  ssize_t nread;
  FILE *stream;
  time_t today = midnight();
  int thisyear = now()->tm_year;
  struct tm day = {};
  time_t date;
  char *n;

  while ((ch = getopt(argc, argv, "A:B:t:f:eh")) != -1) {
    switch (ch) {
      case 't':
        if (strptime(optarg, "%Y-%m-%d", &day) != 0) {
          today = mktime(&day);
        }
        break;
      case 'A':
        advance = DAYS(strtoimax(optarg, NULL, 10));
        break;
      case 'B':
        backward = DAYS(strtoimax(optarg, NULL, 10));
        break;
      case 'f':
        calendarfile = optarg;
        break;
      case 'e':
        execlp(editor, editor, calendarfile, NULL);
        break;
      default:
        fputs("usage: agenda [-A num] [-B num] [-t date] [-f file]\n", stderr);
        return EXIT_FAILURE;
    }
  }

  stream = fopen(calendarfile, "r");

  if (errno != 0) {
    perror("error");
    return EXIT_FAILURE;
  }

  while ((nread = getline(&line, &linelen, stream)) != -1) {
    if ((n = strptime(line, "%m-%d", &day)) != NULL) {
      day.tm_year = thisyear;
    } else if ((n = strptime(line, "%F", &day)) != NULL) {
      ;
    }
    if (n) {
      date = mktime(&day);
      if (today - backward < date && date < today + advance) {
        fputs(line, stdout);
      }
    }
  }
  free(line);
  fclose(stream);
  return EXIT_SUCCESS;
}

