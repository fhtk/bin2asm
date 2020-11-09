/* -*- coding: utf-8 -*- */

#include <stdio.h>
#include <uni/err.h>
#include <uni/futils.h>
#include <uni/log.h>
#include <uni/str.h>

const char* const help_txt = "\nBinary to assembly converter\n\nUsage:\n    \n    bin2asm [options] <input> [output]\n    Takes the binary file <input> and converts it to its GNU assembly equivalent.\n    If <input> is - then standard input is read.\n\nOptions:\n    -s <sym>  Specify symbol name to assign the data. Useful when reading from stdin.\n";

char* convert( u8* in, ptri in_sz, char* sym )
{
	struct uni_str* str;
	ptri i, li;

	if(!in || !in_sz)
	{
		uni_die( );
	}

	str = uni_str_init( "\n.section .rodata\n.balign 4, 0\n.globl " );
	uni_str_app( str, (const char*)sym );
	uni_str_app( str, "\n.type " );
	uni_str_app( str, (const char*)sym );
	uni_str_app( str, ", %object\n" );
	uni_str_app( str, (const char*)sym );
	uni_str_app( str, ":\n\t.byte " );

	for(i = 0, li = 0; i < in_sz; ++i)
	{
		u8 msn = (in[i] & 0xF0) >> 4;
		u8 lsn = in[i] & 0x0F;

		if(li == 10)
		{
			uni_str_app( str, "\n\t.byte " );
			li = 1;
		}
		else
		{
			li++;
		}

		uni_str_app( str, "0x" );
		uni_str_appch( str, msn >= 10 ? msn + 0x37 : msn + 0x30 );
		uni_str_appch( str, lsn >= 10 ? lsn + 0x37 : lsn + 0x30 );

		if(i < in_sz - 1 && li < 10)
		{
			uni_str_app( str, ", " );
		}
	}

	uni_str_appch( str, '\n' );
	uni_str_app( str, ".size " );
	uni_str_app( str, (const char*)sym );
	uni_str_app( str, ", .-" );
	uni_str_app( str, (const char*)sym );
	uni_str_app( str, "\n\n" );

	{
		char* ret;

		ret = uni_str_make( str );
		uni_str_fini( str );

		return ret;
	}
}

int main( int ac, char* av[] )
{
	ptri sym_i, in_i, out_i;

	if(ac <= 1 || (ac == 2 && (uni_strequ( av[1], "--help" ) || uni_strequ( av[1], "-h"))))
	{
		uni_print( "%s\n", help_txt );

		return 0;
	}

	{
		int i;

		for(i = 1, sym_i = 0, in_i = 0, out_i = 0; i < ac; ++i)
		{
			if(!sym_i && i < ac - 1 && uni_strequ( av[i], "-s" ))
			{
				sym_i = i + 1;
			}
			else if(!in_i && (av[i][0] != '-' || uni_strequ( av[i], "-"))
			&& i != sym_i)
			{
				in_i = i;
			}
			else if(!out_i && av[i][0] != '-' && i != sym_i)
			{
				out_i = i;
			}
		}

		if(!sym_i)
		{
			uni_perror( "No symbol name specified. Exiting...\n" );

			return 127;
		}
	}

	{
		char* sym = av[sym_i];
		const char* in = uni_strequ( av[in_i], "-" ) ? NULL : av[in_i];
		u8* buf;
		ptri buf_sz;
		char* out;
		FILE* outf;

		if(!in_i)
		{
			uni_perror( "No input file specified. Exiting...\n" );

			return 127;
		}

		uni_loadfile( in, &buf, &buf_sz );
		out = convert( buf, buf_sz, sym );

		if(!out_i)
		{
			outf = stdout;
		}
		else
		{
			outf = fopen( av[out_i], "w" );
		}

		fwrite( out, sizeof(u8), uni_strlen( out ), outf );
		fflush( outf );

		if(out_i)
		{
			fclose( outf );
		}

		return 0;
	}
}
