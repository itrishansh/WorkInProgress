#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
	int rc;
	char *args[] = {"/bin/cat", __FILE__, NULL};
	printf("Hello from execv test app\n");
	rc = execv("/bin/cat", args);
	printf("Failed in execv rc %d, errno %d\n", rc, errno);
	return 0;
}
