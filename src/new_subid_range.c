#include <stdio.h>
#include <unistd.h>
#include "subid.h"
#include "stdlib.h"
#include "prototypes.h"

/* Test program for the subid creation routine */

const char *Prog;

void usage(void)
{
	fprintf(stderr, "Usage: %s [-g] [-n] user count\n", Prog);
	fprintf(stderr, "    Find a subuid (or with -g, subgid) range for user\n");
	fprintf(stderr, "    If -n is given, a new range will be created even if one exists\n");
	fprintf(stderr, "    count defaults to 65536\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int c;
	struct subordinate_range range;
	bool makenew = false; // reuse existing by default
	bool group = false;   // get subuids by default
	bool ok;

	Prog = Basename (argv[0]);
	while ((c = getopt(argc, argv, "gn")) != EOF) {
		switch(c) {
		case 'n': makenew = true; break;
		case 'g': group = true; break;
		default: usage();
		}
	}
	argv = &argv[optind];
	argc = argc - optind;
	if (argc == 0)
		usage();
	range.owner = argv[0];
	range.start = 0;
	range.count = 65536;
	if (argc > 1)
		range.count = atoi(argv[1]);
	if (group)
		ok = grant_subgid_range(&range, !makenew);
	else
		ok = grant_subuid_range(&range, !makenew);

	if (!ok) {
		fprintf(stderr, "Failed creating new id range\n");
		exit(EXIT_FAILURE);
	}
	printf("Subuid range %lu:%lu\n", range.start, range.count);

	return 0;
}
