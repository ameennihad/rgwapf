# Rioch GW Arabic Print Filter (rgwapf)

A Unix filter in c larguage to support arabic text printing on Ricoh MFPs/Printers with GW controller
## Supported input file encoding
ISO1256, can support other encodings by simply creating a new charactor map table (charmap[]).
## Supported Printers
Any Ricoh printer with GW controller, you need to change following default PCL settings
	- Font Number: 56
	- Symbol Set: PC-864
