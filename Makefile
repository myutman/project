CXX = g++
CXXFLAGS = -O2 -Wall -std=c++11 -Iinclude
LDFLAGS = 

EXE = yc
SRCDIR = src
BINDIR = bin

all: $(EXE)

$(EXE): $(BINDIR) $(BINDIR)/main.o $(BINDIR)/model.o $(BINDIR)/ypl.tab.o $(BINDIR)/lex.yy.o
	$(CXX) $(BINDIR)/main.o $(BINDIR)/model.o $(BINDIR)/ypl.tab.o $(BINDIR)/lex.yy.o -o $(EXE) $(LDFLAGS)
	
$(BINDIR)/main.o: $(SRCDIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

$(BINDIR)/model.o: $(SRCDIR)/model.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

$(BINDIR)/ypl.tab.o: ypl.tab.c
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

$(BINDIR)/lex.yy.o: lex.yy.c
	$(CXX) $(CXXFLAGS) -c -MMD -o $@ $<

$(SRCDIR)/main.cpp: ypl.tab.c

ypl.tab.c: $(SRCDIR)/ypl.y
	bison -d $(SRCDIR)/ypl.y

lex.yy.c: $(SRCDIR)/ypl.lex
	lex $(SRCDIR)/ypl.lex

include $(wildcard $(BINDIR)/*.d)

$(BINDIR):
	mkdir -p $(BINDIR)

clean:
	rm -rf $(BINDIR) $(EXE)

.PHONY: clean all