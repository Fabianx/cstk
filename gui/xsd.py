#***************************************************************************
#                              xsd.py - v.1.00
#                             -----------------
#    begin                : Jul 01 2004
#    copyright            : (C) 2004 by Kristof Van Laerhoven
#    email                :
#***************************************************************************

#***************************************************************************
#                                                                          *
#    This program is free software; you can redistribute it and/or modify  *
#    it under the terms of the GNU General Public License as published by  *
#    the Free Software Foundation; either version 2 of the License, or     *
#    (at your option) any later version.                                   *
#                                                                          *
#***************************************************************************

#
#     Constants for CSTK's Xml Settings Document
#

dtd = ''' 
<!DOCTYPE RTPlotSettings [                                                
                                                                        
   <!ENTITY AUTHOR "The CommonSense Toolkit Team">                             
   <!ENTITY SOFTWARE "CommonSense ToolKit">                             
                                                                        
   <!ELEMENT RTPlotSettings  (input?,output?,window?,params?)>           
                                                                         
   <!ELEMENT input ((file|udp|rs232|int|sim|pipe),column*)>               
   <!ATTLIST    input id           CDATA           #IMPLIED>             
   <!ELEMENT file (channel*)>                                            
   <!ATTLIST   file filename       CDATA           #REQUIRED>            
   <!ATTLIST   file mode           (ascii|binary)  "ascii">              
   <!ELEMENT udp (channel*)>                                             
   <!ATTLIST   udp port            CDATA           "1515">               
   <!ATTLIST   udp host            CDATA           "localhost">          
   <!ATTLIST   udp mode            (ascii|binary)  "ascii">              
   <!ELEMENT rs232 (poll?,channel*)>                                     
   <!ATTLIST   rs232 port          CDATA           "/dev/ttyS0">         
   <!ATTLIST   rs232 baudrate      CDATA           "115200">             
   <!ATTLIST   rs232 buffsize      CDATA           "512">               
   <!ATTLIST   rs232 databits      CDATA           "8">                  
   <!ATTLIST   rs232 stopbits      CDATA           "1">                  
   <!ATTLIST   rs232 parity        (yes|no)        "no">                 
   <!ATTLIST   rs232 mode          (ascii|binary)  "ascii">              
   <!ELEMENT int (channel*)>                                             
   <!ATTLIST   int filename        CDATA           "/proc/hal/light">    
   <!ATTLIST   int mode            (ascii|binary)  "">                   
   <!ELEMENT sim (column*)>
   <!ATTLIST   sim mode            CDATA           "">
   <!ELEMENT pipe (channel*)>
   <!ATTLIST   pipe filename       CDATA           "">
   <!ATTLIST   pipe mode           (ascii|binary)  "ascii">

   <!ELEMENT channel EMPTY>
   <!ATTLIST    channel id         CDATA           #REQUIRED>
   <!ATTLIST    channel src        CDATA           "0">
   <!ATTLIST    channel name       CDATA             #IMPLIED>
   <!ATTLIST    channel bits       CDATA           "8">
   <!ATTLIST    channel sign       (signed|unsigned) "unsigned">
   <!ATTLIST    channel type       (val|min|max|avg|std|var) "val">
   <!ATTLIST    channel format     (integer|float)   "integer">

   <!ELEMENT poll (#PCDATA)>
   <!ATTLIST    poll mode          (ascii|binary)  "ascii">
   <!ATTLIST    poll command       CDATA           #IMPLIED>
   <!ELEMENT column EMPTY>
   <!ATTLIST    column id          CDATA           #REQUIRED>
   <!ATTLIST    column channel     CDATA           #REQUIRED>
   <!ATTLIST    column name        CDATA           #IMPLIED>
   <!ATTLIST    column bits        CDATA           "8">
   <!ATTLIST    column sign        (signed|unsigned) "unsigned">
   <!ATTLIST    column format      (integer|float)   "integer">

   <!ELEMENT output ((file|udp|rs232|int|pipe),channel*)>
   <!ATTLIST    output id          CDATA           #IMPLIED>
   
   <!ELEMENT window (plot*)>
   <!ATTLIST    window xpos        CDATA           "0">
   <!ATTLIST    window ypos        CDATA           "0">
   <!ATTLIST    window width       CDATA           "240">
   <!ATTLIST    window height      CDATA           "320">
   <!ATTLIST    window border      CDATA           "2">
   <!ATTLIST    window framespersec  CDATA         "10">
   <!ATTLIST    window skip        CDATA           "3">
   <!ELEMENT plot EMPTY>
   <!ATTLIST    plot id            CDATA           #REQUIRED>
   <!ATTLIST    plot src           CDATA           #REQUIRED>
   <!ATTLIST    plot type          (histogram|timeseries|peakplot|textplot) #REQUIRED>
   <!ATTLIST    plot title         CDATA           "title">
   <!ATTLIST    plot res           CDATA           "100">
   <!ATTLIST    plot scale         (yes|no)        "yes">
   <!ATTLIST    plot color         CDATA           "0">

   <!ELEMENT params (param*)>
   <!ATTLIST    params id          CDATA           #IMPLIED>
   <!ELEMENT param EMPTY>
   <!ATTLIST    param peak_minl    CDATA           #IMPLIED>
   <!ATTLIST    param peak_maxl    CDATA           #IMPLIED>
   <!ATTLIST    param lr           CDATA           #IMPLIED>
   <!ATTLIST    param nbr          CDATA           #IMPLIED>
   <!ATTLIST    param dim          CDATA           #IMPLIED>

]>
'''

# xml header:
head = '''
<!-- RTPLOTGUI xml settings document: Please don't change
 these lines, as they were automatically generated. See
 http://ubicomp.lancs.ac.uk/cstk/manual/quickstart.html
 for more details.
-->
'''

# mapping between type and ids:
vtype = { 'none':0, 'val':1, 'min':2, 'max':3, 'avg':4, 'std':5, 'var':6 }

# mapping between plottype and ids:
ptype = { 'none':0, 'histogram':1, 'timeseries':2, 'peakplot':3, 
          'impulse':4, 'scatter':5, 'textplot':6 }

# mapping between sign and booleans:
sign = { 'signed':True, 'unsigned':False }

# mapping between format and ids:
form = { 'none':0, 'integer':1, 'float':2 }

# mapping between scale and bools:
scale = { 'yes':True, 'no':False }

# mapping between parity and bools:
par = { 'yes':True, 'no':False }

# mapping between sections and ids:
sect = { 'none':0, 'input':1, 'output':2, 'window':3  }

inp = ['file', 'udp', 'rs232', 'int', 'sim', 'pipe']
outp = ['none', 'file', 'udp', 'rs232', 'int', 'pipe']
