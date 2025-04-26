CXX = g++
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++17

GCOVFLAGS = --coverage
SRC = $(wildcard model/*.cpp controller/*.cpp)
OBJS = $(SRC:.cpp=.o)

LIBS= -lgtest -lm

all: 
	make uninstall
	make clean 
	make install
	make dvi 
	make dist
	make gcov_report
	make run

install: app_folder
	cd build && cmake ../cmake && make #&& make clean && rm -f Makefile && rm -f .qmake.stash

run:
	cd build && ./3D_Viewer

uninstall:
	rm -rf build

dvi:
	doxygen Doxyfile
	cd docs/latex && latex refman.tex
	cd docs/latex && texi2pdf refman.tex
	cp docs/latex/refman.pdf docs/docs.pdf

dist:
	mkdir dist
	cp /build/3D_Viewer /dist/3D_Viewer
	cp /docs/docs.pdf /dist/docs.pdf
	tar -cf dist.tar dist && rm -rf dist

tests: $(OBJS)
	$(CXX) $(CXXFLAGS) test/s21_tests.cpp $^ -o viewer_test $(LIBS)
	./viewer_test

gcov_report: clean
	$(CXX) $(CXXFLAGS) $(GCOVFLAGS) model/*.cpp test/s21_tests.cpp -o viewer_test $(LIBS)
	./viewer_test
	lcov -t "Report" -o report.info --no-external -c -d . --ignore-errors mismatch,mismatch
	genhtml -o report/ report.info
	rm -rf *.gcno *.gcov *.gcda *.info *.a tests/*.o
	open ./report/index.html

memcheck: $(OBJS)
	$(CXX) $(CXXFLAGS) test/s21_tests.cpp $^ -o viewer_test $(LIBS)
ifeq ($(shell uname), Linux)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./viewer_test
else
	leaks -atExit -- ./viewer_test | grep LEAK:
endif

clean:
	rm -rf viewer_test *.gcno *.gcda **/*.o report docs

app_folder:
	mkdir -p build

clang:
	clang-format -style=Google -n ./**/*.cpp