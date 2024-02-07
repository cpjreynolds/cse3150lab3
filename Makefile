CXXFLAGS=-Wall -g --std=c++17

# testing target
TESTTARGET=lab3test.out
# runnable target
RUNTARGET=lab3.out

# all source files including test
SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(SOURCES:.cpp=.o)

.PHONY: all clean check run leaks

all: $(RUNTARGET) $(TESTTARGET)

check: $(TESTTARGET)
	./$(TESTTARGET)

run: $(RUNTARGET)
	./$(RUNTARGET)

$(TESTTARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) -DTESTING $(CXXFLAGS) $^ -o $@

$(RUNTARGET): $(SOURCES)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

# macos specific leak checker (macos valgrind is sketchy)
leaks: $(TESTTARGET)
	leaks -atExit -quiet -- ./$(TESTTARGET)
	leaks -atExit -quiet -- ./$(RUNTARGET)

clean:
	rm -rf \
		$(OBJECTS)					\
		$(RUNTARGET)				\
		$(RUNTARGET:.out=.out.dSYM)	\
		$(TESTTARGET)				\
		$(TESTTARGET:.out=.out.dSYM)
