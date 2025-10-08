all:
	gcc src/main.c src/expresso.c src/mapa.c -o ./saida/expresso
clear:
	rm -f ./saida/expresso
run:
	./saida/expresso