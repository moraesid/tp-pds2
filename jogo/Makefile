all: merge-sources

merge-sources: merge-sources.o
	g++ merge-sources.o -o merge-sources.out -lsfml-graphics -lsfml-window -lsfml-system

merge-sources.o: merge-sources.cpp
	 g++ -c merge-sources.cpp -o merge-sources.o

.PHONY: clean run
clean:
	rm -r merge-sources.o
run:
	./merge-sources.out

