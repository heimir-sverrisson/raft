.PHONY: all test lint clean

all:
	$(MAKE) -C src

test:
	$(MAKE) -C test

lint:
	$(MAKE) -C src lint

clean:
	rm -f *.log
	$(MAKE) -C src clean
