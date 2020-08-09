#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include <linux/input.h>

/* Macros for loging info */
#define LOG(file, fmt, args...)	\
	fprintf(file, "%s#%d: " fmt "\n", __func__, __LINE__, ##args);

#define LOGI(fmt, args...) \
	LOG(stdout, fmt, ##args);

#define LOGE(fmt, args...) \
	LOG(stderr, fmt, ##args);

#define INPUT_EVENT_BIN_SIZE 100

int *to_notify = NULL;
void signal_handler(int signo) {
	LOGI("Received signal %d, notify %p", signo, to_notify);
	if (to_notify)
		*to_notify = 1;
}
void setup_signal_handler (void (*hndlr)(int), int* notify) {
	to_notify = notify;
	LOGI("notify %p", notify);
	signal(SIGINT, hndlr);
	signal(SIGHUP, hndlr);
	signal(SIGPIPE, hndlr);
}

void do_record(const char *finname, const char * foutname) {
	FILE *fin = fopen(finname, "r");
	if (!fin) {
		LOGE("Failed to open file \"%s\" err %d", finname, errno);
		return;
	}

	FILE *fout = fopen(foutname, "w");
	if (!fout) {
		LOGE("Failed to open file \"%s\" err %s", foutname, strerror(errno));
		return;
	}

	int shouldexit = 0;
	setup_signal_handler(signal_handler, &shouldexit);

	struct input_event *ev = malloc(sizeof(struct input_event) * INPUT_EVENT_BIN_SIZE);
	LOGI("sizeof(input_event) %zd", sizeof(ev[0]));
	int bucket = 0;
	while (1) {
		if (shouldexit)
			return;
		int bytesread = read(fileno(fin), ev, sizeof(struct input_event) * INPUT_EVENT_BIN_SIZE);
		int numevent = bytesread / sizeof(struct input_event);
		LOGI("Read %d bytes, nevent %d", bytesread, numevent);

		if (bytesread == 0) {
			LOGI("errno %d, shouldexit %d", errno, shouldexit);
			return;
		}
		for (int i = 0; i < numevent; i++) {
			LOGI("%d %ld.%ld %hd %hd %d", i, ev[i].time.tv_sec, ev[i].time.tv_usec, ev[i].type, ev[i].code, ev[i].value);
			int byteswritten = fwrite(ev, 1, bytesread, fout);
			if (bytesread != byteswritten) {
				LOGI("bytesread != byteswritten : %d != %d", bytesread, byteswritten);
			}

		}
	}
}

void do_replay() {
	LOGE("Not Implemented");
}

int main(int argc, char *argv[]) {
	LOGI("argc %d", argc);
	for (int i = 0; i < argc; i++) {
		LOGI("argv[%d] %s", i, argv[i]);
	}

	if (argc < 4) {
		LOGE("Not enough arguments");
	}

	if (!strcmp("record", argv[1])) {
		LOGI("Initiate record");
		do_record(argv[2], argv[3]);
	} else if (!strcmp("replay", argv[1])) {
		LOGI("Initiate replay");
		do_replay();
	} else {
		LOGI("Unknown operation \"%s\"", argv[1]);
	}
	LOGI("Exiting...");
	return 0;
}
