# This clears all the implicit rules.
.SUFFIXES:

# Utility defines
define compile_obj
$(CXX) -c $< -o $@ $(ALL_CXXFLAGS)
endef

define link_main
$(CXX) -o $@ $^ $(ALL_CXXFLAGS) $(ALL_LDFLAGS)
endef

define generate_dependency
$(CXX) $*.cpp -MM -MP -MF $*.dep.mk -MT '$*.o $*.dep.mk' $(ALL_CXXFLAGS)
endef


# This makefile handles multiple programs in the same directory
# that include several files.
CXXFLAGS ?= -g
ALL_CXXFLAGS := $(CXXFLAGS) -std=c++1y -iquote./ $$(pkg-config --cflags opencv)
ALL_LDFLAGS += $$(pkg-config --libs opencv) -laruco

# Subdirecories thad need to be ignored by find.
findignore := ./.git
findignore := $(patsubst %, -path '%/*' -prune -o, $(findignore))

# Programs generated by this makefile
prog := $(shell find . $(findignore) -name "*.cpp" -exec grep -l "^int main" {} +)
prog := $(prog:.cpp=)

# cpp files whose compilation will be managed by this makefile.
src := $(shell find . $(findignore) -name "*.cpp" -print)

# .dep.mk files that should be automatically generated from .cpp files.
dep := $(src:.cpp=.dep.mk)

# Object files that contains definitions needed by other programs
obj := $(shell find . $(findignore) -name "*.cpp" -exec grep -L "^int main" {} +)
obj := $(obj:.cpp=.o)


-include $(dep)


.DEFAULT_GOAL := all

.PHONY: all
all: $(prog)

# Suboptimal dependency management.
# Resolving this without such intrusion would need to somehow
# inspect the object files and tell make which symbols are defined
# in each object. This inspection could be done by using by somehow parsing
# 'nm -u' (to get the dependecies) and 'nm | grep " *[TtRr]"' (to get the
# provided symbols), but I have no idea of how to dynamically update
# the make's DAG without invoking make repeatedly
# --- which should be make's work.
$(prog): %: %.o $(obj)
	$(link_main)

$(src:.cpp=.o): %.o : %.cpp
	$(compile_obj)

$(dep): %.dep.mk: %.cpp
	$(generate_dependency)

.PHONY: mostlyclean clean
mostlyclean:
	-find $(findignore) -name "*.o" -exec rm '{}' \;
	-find -name "*.dep.mk" -exec rm '{}' \;

clean: mostlyclean
	-rm -f $(prog)
