TEX = pdflatex  -halt-on-error #-interaction=batchmode
BIB= bibtex
FIGURES = 
NAME = Weave-Examples

.PHONY:all clean remote-start remote-stop mainbuild

all: remote-stop


remote-start:
	echo "Starting Build"
	#put any prebuild set-up here


remote-stop:mainbuild
	echo "Finished Build"
	#put any post-build tear down steps here


mainbuild:remote-start
	-$(MAKE) -k -s --no-print-directory $(NAME).pdf 


$(NAME).pdf:$(NAME).tex $(FIGURES)
	@$(TEX) $< 
	@$(TEX) $<
	@$(TEX) $< 

