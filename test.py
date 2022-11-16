# -*- coding: shift-jis

import xsecsPyPort

x = xsecsPyPort.openLogFromOffline("MULTI_PACKET_2005110413433340.blog")
for rr in x:
	waitstr = ""
	if rr.wait == 1:
		waitstr = "W"
	print rr.srcHostString(), rr.dstHostString()
	print '<S%(stream)sF%(#)d %(wait)s' % \
          {'stream': rr.stream, "#": rr.function, "wait": waitstr}
	print ">\n" 
	
