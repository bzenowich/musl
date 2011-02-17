#include <fcntl.h>
#include <unistd.h>

int daemon(int nochdir, int noclose)
{
	int fd;

	switch(fork()) {
	case 0: break;
	case -1: return -1;
	default: _exit(0);
	}

	if (setsid() < 0) return -1;

	switch(fork()) {
	case 0: break;
	case -1: return -1;
	default: _exit(0);
	}

	if (!nochdir && chdir("/"))
		return -1;
	if (!noclose) {
		int failed = 0;
		if ((fd = open("/dev/null", O_RDWR)) < 0) return -1;
		if (dup2(fd, 0) < 0 || dup2(fd, 1) < 0 || dup2(fd, 2) < 0)
			failed++;
		if (fd > 2) close(fd);
		if (failed) return -1;
	}

	return 0;
}
