docdir := $(dir $(lastword $(MAKEFILE_LIST)))

# Stripped slash directory
docsdir := $(patsubst %/,%,$(docdir))

pdf := $(docdir)report.pdf

.PHONY: doc
all: doc
doc: $(pdf)

$(pdf): %.pdf : %.tex
	cd $(docdir);\
	latexmk -pdf $(notdir $<) -pdflatex="pdflatex --shell-escape %O %S"

.PHONY: doc-mostlyclean
mostlyclean: doc-mostlyclean
doc-mostlyclean:
	cd $(docdir) && latexmk -c

.PHONY: doc-clean
clean: doc-clean
doc-clean:
	cd $(docdir) && latexmk -C
