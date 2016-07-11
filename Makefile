.PHONY: all test lint clean

all:
	$(MAKE) -C src

test:
	$(MAKE) -C src
	$(MAKE) -C test

testrun:
	$(MAKE) -C test run

lint:
	$(MAKE) -C src lint

clean:
	rm -f *.log
	$(MAKE) -C src clean
	$(MAKE) -C test clean

