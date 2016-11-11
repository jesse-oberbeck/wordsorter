.DEFAULT_GOAL := ws #found at stackoverflow.com/questions/2057689/how-make-app-knows-default-target-to-build-if-no-target-is-specified

TARGET=ws

CPPFLAGS+=-Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline

CFLAGS+=-std=c11

LDLIBS+=-lm

.PHONY: clean debug profile

clean: 
	rm $(TARGET)
debug: 
	CFLAGS+=-g
debug: 
	cc -std=c99 -g ws.c -o ws
profile: 
	FLAGS+=-pg
profile: 
	LDFLAGS+=-pg
profile: 
	$(BIN)
