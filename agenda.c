
#define _XOPEN_SOURCE 700

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DAYS(n) 60*60*24*n
#define DAYS_MAX INT_MAX/60/60/24
#define PATH_SIZE 1024
#define LINE_LEN 2048 + 1
#define USAGE "usage: %s [-e] [-f file] [-t date] [-A num] [-B num]\n"

static const char*	env(const char *, const char *);
static time_t		midnight(struct tm *);
static struct tm*	now();

int
main(int argc, char *argv[])
{
	int ch;
	time_t today;
	int backward = 0;
	int advance = DAYS(14);

	char path[PATH_SIZE];
	const char *editor;
	const char *agendafile;

	FILE *stream;
	char line[LINE_LEN]; /* *p; */
	struct tm day = { 0 };
	int thisyear = now()->tm_year;
	time_t date;
	char *n;

	today = midnight(now());
	editor = env("EDITOR", "vi");
	if (snprintf(path, PATH_SIZE, "%s/%s", getenv("HOME"), "AGENDA"))
		agendafile = env("AGENDA_FILE", path);

	while ((ch = getopt(argc, argv, "A:B:t:f:eh")) != -1) {
		switch (ch) {
			case 'e':
				execlp(editor, editor, agendafile, NULL);
				break;
			case 'f':
				agendafile = optarg;
				break;
			case 't':
				if (strptime(optarg, "%F", &day))
					today = midnight(&day);
				break;
			case 'A':
				advance = DAYS(atoi(optarg));
				break;
			case 'B':
				backward = DAYS(atoi(optarg));
				break;
			default:
				fprintf(stderr, USAGE, argv[0]);
				return EXIT_FAILURE;
		}
	}

	stream = fopen(agendafile, "r");

	if (errno != 0) {
		perror("error");
		return EXIT_FAILURE;
	}

	while (fgets(line, LINE_LEN, stream) != NULL) {
		if ((n = strptime(line, "%F", &day)))
			;
		else if ((n = strptime(line, "%m-%d", &day)))
			day.tm_year = thisyear;

		if (n) {
			date = midnight(&day);
			if (today - backward <= date && date <= today + advance)
				fputs(line, stdout);

			/* if ((p = strchr(line, '\n')) == NULL) { */
			/*   while ((ch = getw(stream)) != '\n' && ch != EOF) { */
			/*     // ignore rest of line */
			/*     ; */
			/*   } */
			/* } */
		}
	}

	fclose(stream);
	return EXIT_SUCCESS;
}


const char*
env(const char *name, const char *alt)
{
	const char *value = getenv(name);
	return value ? value : alt;
}


time_t
midnight(struct tm *t)
{
	t->tm_hour = 0;
	t->tm_min = 0;
	t->tm_sec = 0;
	t->tm_isdst = 0;
	return mktime(t);
}


struct tm*
now()
{
	time_t t = time(NULL);
	return localtime(&t);
}
