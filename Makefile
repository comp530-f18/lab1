
all: thsh

thsh: thsh.c
	gcc thsh.c -o thsh -g

handin: clean
	@if [ `git status --porcelain| wc -l` != 0 ] ; then echo "\n\n\n\n\t\tWARNING: YOU HAVE UNCOMMITTED CHANGES\n\n    Consider committing any pending changes and rerunning make handin.\n\n\n\n"; fi
	git tag -f -a handin -m "Lab 0 Handin"
	git push --tags handin


clean:
	rm thsh
