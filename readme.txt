(4BPP SMS/GG/WSC) Neo-Geo Sprite Converter (Version 1.0)
by AJ/freem (http://www.ajworld.net/ and http://freem.co/)
==========================================================
[Information]
This program converts 4BPP Sega Master System/Game Gear/WonderSwan Color tiles
to Neo-Geo sprite tile format. It should be noted that the output format is not
suited for immediate use with either cart or CD.

"Why SMS/GG/WSC", you may ask? The format is close to the Neo-Geo sprite format,
unlike the SNES (for which tools exist already).

(I wrote this tool since I was tired of decode16/recode16's interactive mode UI.
No disrespect intended; I just prefer to pass parameters in on the command line.)

================================================================================
[Usage]
neosprconv (infile) [outfile]

outfile is optional; if omitted, the default filename is "outfile.c0".

================================================================================
[Conversion]
I noted above that "the output format is not suited for immediate use with
either cart or CD". Depending on what system you're targeting, you're going to
need to perform a different task.

(Converting to Cart)
Cart systems have the .c# ROMs, where each odd-numbered file has the first two
bitplanes, and the even-numbered files have the second two bitplanes.

To convert a "outfile.c0" to the numbered C ROMs, you'll need a program that can
split files by words (each two bytes). ROMWak can do this with the /w option.

(Converting to CD/.SPR)
CD systems use a .SPR file, so you're halfway there.
All you need to do is byteswap the output. Yes, it's that simple.

================================================================================
The latest version can be found at http://www.ajworld.net/neogeodev/
