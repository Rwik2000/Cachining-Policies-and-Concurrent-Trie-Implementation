# Complete This!
all: build
build:
	gcc -Wall main.c policy.c workload.c -lm -o out
	python3 policy_plot.py
	./out
# TO run checker.c
checker:
	gcc -Wall checker.c policy.c workload.c -lm -o out
	./out