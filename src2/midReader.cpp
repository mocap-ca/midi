// midReader.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <string.h>
#include <vector>

#include <stdio.h>
#include "MidiFileIn.h"
#include "Stk.h"

using namespace std;

int main(int argc, char* argv[])
{
	std::vector<unsigned char> e;
	unsigned long tick;
	try
	{
		MidiFileIn f("c:/test.mid");

		printf("Tracks: %i\n", f.getNumberOfTracks());

		
		tick = f.getNextEvent(&e, 1);

		while(e.size())
		{
			if(e[0]==0x90)
			{
				unsigned char note = e[1];
				unsigned char vel = e[2];

				printf("%04i: note: %02x   vel:%02x\n", tick, note, vel);
			}
			else
			{

				printf("%i:", tick);

				for(size_t i=0; i<e.size();i++)
				{
					printf("%x", e[i]);
				}
				printf("\n");
			}

			tick = f.getNextEvent(&e, 1);
		}
		

	}
	catch(StkError e)
	{
		printf("Whoopsie");
	}


	return 0;
}


int myfunc(int argc, char* argv[])
{

	int M, T, h, d, r, k;
	int hl, t, tn;
	int div0, div1, div2;
	int com = 0;
	int pos;
	int it;
	FILE *notesfile;
	int len;
	int i;
	char ch;

	if(argc!=2)
	{
		fprintf(stderr, "usage readmid file.mid\n");
		return 0;
	}

	char *title;
	title=NULL;

	if (!(notesfile = fopen (argv[1], "r")))
	{
		return -1;
	}

	M = getc (notesfile);
	T = getc (notesfile);
	h = getc (notesfile);
	d = getc (notesfile);
	hl = getc (notesfile) << 24;
	hl += getc (notesfile) << 16;
	hl += getc (notesfile) << 8;
	hl += getc (notesfile);
	t = getc (notesfile) << 8;
	t += getc (notesfile);
	tn = getc (notesfile) << 8;
	tn += getc (notesfile);
	/* how much MIDI time makes up one quarter note */
	div1 = getc (notesfile);
	div2 = getc (notesfile);
	/* Alternatively, if the first byte has highest bit set, the time is given
	 * in another format: The first byte is the negative FPS, normally -24, -25,
	 * -29 or -30; the second byte then is the sub-resolution, usually 4, 8, 10,
	 * 80 or 1000. */
	if (div1 < 128) /* i.e., first byte is positive */
	{
                /* assume one second = quarter note */
		div0 = div1 * 256 + div2;
	}
	else
	{
		div0 = (div1 - 256) * div2;
	}

	if (M != 'M' || T != 'T' || h != 'h' || d != 'd' || hl != 6)
	{
		return -1;
	}

	if (t != 0 && t != 1)
	{
		return - 1;
	}

	if (t == 0)
		tn = 1;

	/* Read all tracks. */
	for (t = 0; t < tn; t++)
	{
		pos = 0;
		M = getc (notesfile);
		T = getc (notesfile);
		r = getc (notesfile);
		k = getc (notesfile);
		if (M != 'M' || T != 'T' || r != 'r' || k != 'k')
		{
			return -1;
		}

		len = getc (notesfile) << 24;
		len += getc (notesfile) << 16;
		len += getc (notesfile) << 8;
		len += getc (notesfile);

		/* Read the track. */
		for (it = 0; it < len; it++)
		{
			int c, tt, next;
			int vl;
			if (feof (notesfile))
				break;

			tt = 0;
			for (vl = 0; vl < 4; vl++)
			{
				int b;
				b = getc (notesfile);
				tt += b & 127;
				if (b > 127)
					tt *= 128;
				else
					break;
			}
			pos += tt;
			printf("tt: %i\n", tt);

			c = getc (notesfile);
			if (c & 0x80)
			{
				com = c;
				next = getc (notesfile);
			}
			else	/* running status */
			{
				next = c;
				c = com;
			}

			if (c >= 0x80 && c <= 0x8f)	/* note off */
			{
				ch = getc (notesfile);
				printf("Note Off %i\n", ch);
			}
			else if (c >= 0x90 && c <= 0x9f)	/* note on */
			{
				ch = getc (notesfile);
				printf("Node on %i\n", ch);
			}
			else if (c >= 0xa0 && c <= 0xaf)	/* aftertouch */
			{
				ch = getc (notesfile);
				printf("Aftertouch %i\n", ch);
			}
			else if (c >= 0xb0 && c <= 0xbf)	/* controller */
			{
				ch = getc (notesfile);
				printf("controller %i\n", ch);
			}
			else if (c >= 0xc0 && c <= 0xcf)	/* program change */
			{
				printf("Program Change %i\n", c);	
			}
			else if (c >= 0xd0 && c <= 0xdf)	/* channel pressure */
			{
				printf("Channel Pressure 5c\n" , c);
			}
			else if (c >= 0xe0 && c <= 0xef)	/* pitch wheel */
			{
				getc (notesfile);
			}
			else if (c == 0xf0 || c == 0xf7)	/* sysex */
			{
				int l;

				l = 0;
				for (vl = 0; vl < 4; vl++)
				{
					int b;
					b = vl ? getc (notesfile) : next;
					l += b & 127;
					if (b > 127)
						l *= 128;
					else
						break;
				}

				for (i = 0; i < l; i++)
				{
					getc (notesfile);
				}
			}
			else if (c == 0xff)	/* meta */
			{
				int t2, l;
				t2 = next;

				l = 0;
				for (vl = 0; vl < 4; vl++)
				{
					int b;
					b = getc (notesfile);
					l += (b & 127);
					if (b > 127)
						l *= 128;
					else
						break;
				}

				if (t2 == 0x2f)
					break;
				else if (t2 >= 1 && t2 <= 9)
				{
					int title_len;
					if(title==NULL)
						title_len = 0;
					else
						title_len = strlen (title) + 1;

					title = (char*)realloc (title, title_len + l + 1);
					if (title_len)
					{
						(title)[title_len - 1] = ' ';
					}
					for (i = 0; i < l; i++)
					{
						int c2 = getc (notesfile);
						(title)[title_len + i] = c2;
					}
					(title)[title_len + i] = '\0';
					printf("Title: %s\n", title);
				}
				else
				{
					printf("Something: ");
					for (i = 0; i < l; i++)
					{
						ch = getc (notesfile);
						printf("%i", ch);
					}
					printf("\n");

				}

			}
		}
	}
	fclose (notesfile);
	return 0;
}


