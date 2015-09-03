/*
 * (4BPP SMS/GG/WSC) Neo-Geo Sprite Converter
 * Converts from 4BPP Master System/Game Gear/Wonderswan Color tile format
 * to Neo-Geo Sprite tile format
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OUTBUF_SIZE 128

unsigned char NybbleReverse[16] = {
	0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
	0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF
};

void Usage(){
	puts("Converts 4BPP SMS/GG/WSC tiles to Neo-Geo Sprite tiles.");
	puts("Usage: neosprconv (infile) [outfile]");
}

int main(int argc, char *argv[]){
	FILE *inFile;
	FILE *outFile;
	long inFileSize = 0;
	long inBufPos = 0;
	char *outFilename = "outfile.c0";
	unsigned char *inBuf;
	unsigned char outBuf[OUTBUF_SIZE];	/* 128 bytes; one 16x16 tile at a time */
	unsigned char halfBuf[OUTBUF_SIZE/2]; /* 64 bytes; used for swapping */
	size_t result = 0;
	int i, j;

	puts("(4BPP SMS/GG/WSC) Neo-Geo Sprite Converter");
	/* check for arguments */
	if(argc < 2){
		Usage();
		exit(EXIT_SUCCESS);
	}
	if(argv[2] != NULL){
		outFilename = argv[2];
	}

	/**************************************************************************/
	/* try opening inFile */
	inFile = fopen(argv[1],"rb");
	if(inFile == NULL){
		printf("Failed to open input file '%s'",argv[1]);
		perror(" ");
		exit(EXIT_FAILURE);
	}

	/* The input file is open. We want to see how big the file is. */
	fseek(inFile,0,SEEK_END);
	inFileSize = ftell(inFile);

	/* prepare input file buffer */
	inBuf = malloc(inFileSize);
	if(inBuf == NULL){
		perror("Unable to allocate memory for input file buffer");
		exit(EXIT_FAILURE);
	}

	/* read input file into buffer */
	fseek(inFile,0,SEEK_SET);
	fread(inBuf,sizeof(unsigned char),inFileSize,inFile);
	fclose(inFile);

	/**************************************************************************/
	/* try creating outFile */
	outFile = fopen(outFilename,"wb");
	if(outFile == NULL){
		printf("Failed to create output file '%s'",argv[2]);
		perror(" ");
		exit(EXIT_FAILURE);
	}

	/*
	 * The output file is going to have the same length as the input file.
	 * However, we need to perform some surgery and modification on the data
	 * before we can call our task complete.
	 */

	if(inFileSize % OUTBUF_SIZE != 0){
		puts("Input file size is not a multiple of 128 bytes!");
		puts("I _really_ don't want to deal with this file, so pad it!");
		exit(EXIT_FAILURE);
	}

	printf("Converting %s...\n",argv[1]);

	for(i = 0; i < inFileSize/OUTBUF_SIZE; i++){
		/* read 128 bytes from the input buffer */
		memmove(outBuf,inBuf+inBufPos,OUTBUF_SIZE);

		/* perform the 64 byte swap */
		/* step 1: get the bottom 64 bytes. */
		memcpy(halfBuf,outBuf+(OUTBUF_SIZE/2),(OUTBUF_SIZE/2));
		/* step 2: move the top 64 bytes. */
		memmove(outBuf+(OUTBUF_SIZE/2),outBuf,(OUTBUF_SIZE/2));
		/* step 3: write the top 64 bytes. */
		memmove(outBuf,halfBuf,(OUTBUF_SIZE/2));

		/* do the reversal */
		for(j = 0; j < OUTBUF_SIZE; j++){
			unsigned char upper = (outBuf[j]&0xF0)>>4;
			unsigned char lower = (outBuf[j]&0x0F);
			outBuf[j] = NybbleReverse[lower] << 4 | NybbleReverse[upper];
		}

		/* write output buffer to file */
		result = fwrite(outBuf,sizeof(unsigned char),OUTBUF_SIZE,outFile);
		if(result != OUTBUF_SIZE){
			/* error */
			perror("Error writing to output file");
			exit(EXIT_FAILURE);
		}

		/* end of loop logic */
		inBufPos += OUTBUF_SIZE;
	}

	/* finishing up */
	free(inBuf);
	fclose(outFile);

	printf("Successfully converted '%s' to '%s'\n",argv[1],outFilename);
	exit(EXIT_SUCCESS);
}
