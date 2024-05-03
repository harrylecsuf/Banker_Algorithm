CC = g++
CFLAGS = -std=c++11 -Wall
TARGET = banker
TEST_TARGET = test_code

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(filter-out $(OBJDIR)/main.o, $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS)))

$(BINDIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJDIR) $(BINDIR)

run: $(BINDIR)/$(TARGET)
	@./$(BINDIR)/$(TARGET)

$(OBJDIR)/$(TEST_TARGET).o: $(SRCDIR)/$(TEST_TARGET).cpp $(SRCDIR)/BankerAlgorithm.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/$(TEST_TARGET): $(OBJDIR)/$(TEST_TARGET).o $(OBJDIR)/BankerAlgorithm.o
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

test: $(BINDIR)/$(TEST_TARGET)
	@./$(BINDIR)/$(TEST_TARGET)

.PHONY: clean run test
