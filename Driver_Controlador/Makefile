.PHONY: all clean kernel user

all: kernel user

kernel:
	$(MAKE) -C src

user:
	gcc -o src/user_app src/user_app.c

clean:
	$(MAKE) -C src clean
	rm -f src/user_app
