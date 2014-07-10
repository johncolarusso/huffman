huffman - C++
=======
Encodes plain text files, compressing their file size.
Included: Makefile, source code, sample input text file

Use: huffman [-d] < inputFileName > outputFileName
	-d specifies whether the input will be decompressed from an encoded file to a plain text file.
	Not specifying -d will compress the input plain text file to an encoded file.

Sample use/input files:
	huffman < hugeinput.txt > compressed1
	huffman -d < compressed1 > output
