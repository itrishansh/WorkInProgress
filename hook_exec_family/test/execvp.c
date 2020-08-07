#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
	int rc;
	char *args[] = {"cat", __FILE__, NULL};
	printf("Hello from execvp test app.\n");
	rc = execvp("cat", args);
	printf("Failed in execv rc %d, errno %d\n", rc, errno);
	return 0;
}
