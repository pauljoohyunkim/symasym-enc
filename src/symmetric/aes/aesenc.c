#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include "aes.h"

int main(int argc, char** argv)
{
	int opt;


	// Options
	/*

	Mandatory options are starred.
	t: AES type*
	i: Input*
	k: Key file*
	b: Block mode*
	o: Output file
	h: Help
	s: Silent
	f: File integrity check skip
	*/
	bool optT = false, optI = false, optK = false, optB = false, optO = false, optH = false, optS = false, optF = false;
	while((opt = getopt(argc, argv, ":t:i:k:b:o:hsf")) != -1)
	{
		switch(opt)
		{
			case 't':
				optT = true;

				break;
			case 'i':
				optI = true;

				break;
			case 'k':
				optK = true;
			

				break;
			case 'b':
				optB = true;


				break;
			case 'o':
				optO = true;



				break;
			case 'h':
				optH = true;



				break;
			case 's':
				optS = true;


				break;
			case 'f':
				optF = true;



				break;
			case ':':


			case '?':

		}
	}
	
	return 0;
}
