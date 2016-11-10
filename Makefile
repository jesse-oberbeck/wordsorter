.DEFAULT_GOAL := hangman #found at stackoverflow.com/questions/2057689/how-make-app-knows-default-target-to-build-if-no-target-is-specified

TARGET=hangman

CPPFLAGS+=-Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline

CFLAGS+=-std=c11

LDLIBS+=-lm

.PHONY: clean

clean:
	rm $(TARGET)
