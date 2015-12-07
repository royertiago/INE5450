TESTDIR := $(dir $(lastword $(MAKEFILE_LIST)))

TEST := $(TESTDIR)test
TESTSRC := $(shell find $(TESTDIR) -name "*.cpp")

prog += $(TEST)
src += $(TESTSRC)
dep += $(TESTSRC:.cpp=.dep.mk)

$(TEST): $(TESTSRC:.cpp=.o)

all : $(TEST)

.PHONY: test
test: $(TEST)
	$(TEST)


.PHONY: test-clean test-mostlyclean

mostlyclean: test-mostlyclean

# We will not remove test/test.o because it is very costly to rebuild.
test-mostlyclean:
	find $(TESTDIR) -name "*.o" -! -wholename $(TEST).o -exec rm {} +

clean: test-clean
test-clean: test-mostlyclean
	rm -f $(TEST) $(TEST).o
