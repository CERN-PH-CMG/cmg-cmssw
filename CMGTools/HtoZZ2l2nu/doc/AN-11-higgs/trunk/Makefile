#
# MAKEFILE FOR CMS-AN 2011/higgs
#

.DEFAULT: all

#ANALYSIS NOTE
all: pdf

pdf:
	tdr --style=an b AN-11-higgs
	cp ../../tmp/*.pdf ./AN-11-higgs.pdf

clean:
	rm -v ../tmp/*

