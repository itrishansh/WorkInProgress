#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int main() {
	int rc;
	printf("Hello from execlp test app\n");
	rc = execlp("cat", "cat", __FILE__, NULL);
	printf("Failed in execlp rc %d, errno %d\n", rc, errno);
	return 0;
}
