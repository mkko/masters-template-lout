SRC:= $(wildcard *.lout)
SRC += references.ld

book: 
	iconv -t ISO-8859-15 < book.lout | lout -U > book.ps

book.ps: $(SRC)
	iconv -t ISO-8859-15 < book.lout | lout -U > book.ps

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
