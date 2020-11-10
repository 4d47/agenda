
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
#define USAGE "usage: %s [-e] [-t date] [-A num] [-B num]\n"

static time_t	midnight(struct tm *);
static void		die(const char *);


int
main(int argc, char *argv[])
{
	int				ch;
	char*			s;
	time_t			t;
	time_t			today;
	struct tm*		tminfo;
	int				year;
	int				backward = 0;
	int				advance = DAYS(14);
	int				eflag = 0;

	char			path[PATH_SIZE];
	const char*		editor = NULL;
	const char*		agendafile = NULL;

	FILE*			stream;
	FILE*			sortout;
	char*			line = NULL;
	size_t			linesize = 0;
	ssize_t			linelen;
	struct tm		day;

	/* initialize today parameter */
	t = time(NULL);
	tminfo = localtime(&t);
	year = tminfo->tm_year;
	today = midnight(tminfo);

	/* initialize env parameters */
	if ((editor = getenv("EDITOR")) == NULL)
		editor = "vi";
	if ((agendafile = getenv("AGENDA_FILE")) == NULL
			&& (s = getenv("HOME")) != NULL
			&& snprintf(path, PATH_SIZE, "%s/%s", s, "AGENDA") > 0)
		agendafile = path;

	/* parse options */
	while ((ch = getopt(argc, argv, "A:B:t:f:eh")) != -1) {
		switch (ch) {
			case 'e':
				eflag = 1;
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

	if (agendafile == NULL)
		die("agendafile");

	if (eflag == 1) 
		(void) execlp(editor, editor, agendafile, NULL);

	stream = fopen(agendafile, "r");
	sortout = popen("sort", "w");

	if (errno > 0)
		die("error");

	/* output lines from stream starting with a date within range */
	while ((linelen = getline(&line, &linesize, stream)) != -1) {

		if ((s = strptime(line, "%F", &day)))
			;
		else if ((s = strptime(line, "%m-%d", &day)))
			day.tm_year = year;

		if (s != NULL) {
			t = midnight(&day);
			if (t >= today - backward && t <= today + advance)
				fputs(line, sortout);
		}
	}

	pclose(sortout);
	fclose(stream);
	return EXIT_SUCCESS;
}


static time_t
midnight(struct tm *tm)
{
	time_t t;
	tm->tm_hour = tm->tm_min = tm->tm_sec = tm->tm_isdst = 0;
	if ((t = mktime(tm)) < 0)
		die("mktime");
	return t;
}


static void
die(const char *string)
{
	perror(string);
	exit(EXIT_FAILURE);
}
