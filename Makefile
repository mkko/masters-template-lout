SRC:= $(wildcard *.lout)
SRC += references.ld

book: 
	lout -U book.lout > book.ps

book.ps: $(SRC)
	lout -U book.lout > book.ps

txt:    $(SRC)
	lout -p -U book.lout > book.txt

view: book.ps
	gv book.ps

pdf: book.ps
	ps2pdf14 -dPDFSETTINGS=/prepress -dConvertCMYKImagesToRGB=false -dEncodeColorImages=false -dEncodeGrayImages=false -dEncodeMonoImages=false book.ps book.pdf

all:
	make clean
	make new
	make pdf

new:
	make;make;make;make
newtxt:
	make txt;make txt;make txt;make txt
clean:
	rm *.li *.lout.ld
