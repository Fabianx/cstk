/***************************************************************************
                              kprof.h  -  v1.00
                             -------------------
    begin                : Fri Nov 14 2003
    copyright            : (C) 2003 by Kristof Van Laerhoven
    email                :  
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/**Profiler for CSTK tools
  *@author Kristof Van Laerhoven
  
 KProf parses the CSTK Settings Files. It uses the folling classes that make 
 up the main structures in such a file: 
 
 + InputSettings (parameters that specify how to get the sensor data) 
           -> IChannelSettings (list of channels & parameters)
           -> IColumnSettings (list of columns & parameters)
 + OutputSettings (parameters that specify what should be produced)
           -> OChannelSettings (list of channels & parameters)
 + WinSettings (parameters for the window)
           -> SubPlotSettings (list of window's subplots) 
 + ParamSettings (parameters for the CSTK tool's internal algorithms) 
*/ 
 
#include <string.h>
#include <stdio.h>
#include "sensordata/sensordata.h"
#include "sensordata/rs232parser/rs232parser.h"    // settings

#include <termios.h>  // for the baudrate ids

#define PTYPE_NULL  0x00 // rtplot
#define PTYPE_HISTO 0x01
#define PTYPE_TMSER 0x02
#define PTYPE_PEAKS 0x03
#define PTYPE_IMPLS 0x04
#define PTYPE_SCATT 0x05   
#define PTYPE_TEXT  0x06
#define PTYPE_TRAIN 0x07
#define PTYPE_BARS  0x10 // topoplot
#define PTYPE_LINES 0x11

#define SEC_INPUT  0x10
#define SEC_OUTPUT 0x11
#define SEC_WINDOW 0x12
#define SEC_PARAMS 0x13

#define IMODE_NADA   0x00
#define IMODE_FILE   0x01
#define IMODE_UDP    0x02
#define IMODE_RS232  0x03
#define IMODE_INT    0x04
#define IMODE_PIPE   0x05
#define IMODE_SIM    0x06

#define OMODE_NADA   0x00
#define OMODE_FILE   0x01
#define OMODE_UDP    0x02
#define OMODE_RS232  0x03
#define OMODE_INT    0x04
#define OMODE_PIPE   0x05

#define OTYPE_MIN    0x00
#define OTYPE_MAX    0x01
#define OTYPE_AVG    0x02
#define OTYPE_VAR    0x03
#define OTYPE_STD    0x04
#define OTYPE_VAL    0x05
                           
#define SETTP_NON    0x00 // none
#define SETTP_ALL    0x01 // any
#define SETTP_RTP    0x02 // rtplot
#define SETTP_TPP    0x03 // topoplot
#define SETTP_DTP    0x04 // dataplot

// quick&dirty functional programming:

#define _IFCOMMAND(str) if (strcasecmp(last_command,(str))==0 )
#define _IFATTRIB(str) if (strcasecmp(last_item,(str))==0 )
#define _IFSTR(str) if (strcasecmp(tempstr,(str))==0 )

#define _CONF_DOCTYPE(string,doctype) _IFCOMMAND(string) settings_type = (doctype); 
#define _CONF_INT_ATTRIB(string,var) _IFATTRIB(string) fscanf(fp,"%i\"",(var)); else
#define _CONF_FLT_ATTRIB(string,var) _IFATTRIB(string) fscanf(fp,"%f\"",(var)); else
#define _CONF_STR_ATTRIB(string,var) _IFATTRIB(string) fscanf(fp,"%[^\"]",var); else
#define _CONF_INT_ADD_ATTRIB(string,val,fun) _IFATTRIB(string) { fscanf(fp,"%i\"",&val); fun(val);} else
#define _CONF_STR_ADD_ATTRIB(string,val,fun) _IFATTRIB(string) { fscanf(fp,"%[^\"]",val); fun(val);} 

#define _SET_IMODE(val) for (i=0; i<NUMIMODES; i++) _IFCOMMAND(imodes[i]) {val=imodesmap[i]; break;}
#define _SET_OMODE(val) for (i=0; i<NUMOMODES; i++) _IFCOMMAND(omodes[i]) {val=omodesmap[i]; break;}

#define _SET_BR(val) for (i=0; i<NUMBAUDRATES; i++) _IFSTR(baudrates[i]) {val=baudratemap[i]; break;}
#define _SET_OTYPE(fun) for (i=0; i<NUMOTYPES; i++) _IFSTR(otypes[i]) {fun(otypesmap[i]); break;}

#define NUMBAUDRATES 7
const char baudrates[NUMBAUDRATES][10] = {"2400","4800", "9600", "19200", "38400", "57600", "115200"};
const int  baudratemap[NUMBAUDRATES]   = {B2400 ,B4800 , B9600, B19200 , B38400 , B57600, B115200 };

#define NUMOTYPES 6
const char otypes[NUMOTYPES][4] = {"min",     "max",    "avg",    "var",    "std",    "val"};
const int  otypesmap[NUMOTYPES] = {OTYPE_MIN,OTYPE_MAX,OTYPE_AVG,OTYPE_VAR,OTYPE_STD,OTYPE_VAL };

#define NUMOMODES 6
const char omodes[NUMOMODES][10] = {"none",    "file",    "udp",    "rs232",    "int",    "pipe"};
const int  omodesmap[NUMOMODES]  = {OMODE_NADA,OMODE_FILE,OMODE_UDP,OMODE_RS232,OMODE_INT,OMODE_PIPE};

#define NUMIMODES 7
const char imodes[NUMIMODES][10] = {"none",    "file",    "udp",    "rs232",    "int",    "pipe",    "sim"};
const int  imodesmap[NUMIMODES]  = {IMODE_NADA,IMODE_FILE,IMODE_UDP,IMODE_RS232,IMODE_INT,IMODE_PIPE,IMODE_SIM};

#define NUMDOCTYPES 5
const char dtypes[NUMDOCTYPES][32] = { "none",    "any",     "rtplotsettings", 
					"topoplotsettings", "dataplotsettings"};
const int  dtypesmap[NUMDOCTYPES]  = {SETTP_NON, SETTP_ALL, SETTP_RTP, SETTP_TPP, SETTP_DTP};

#define NUMSPLOTTYPES 10
const char sptypes[NUMSPLOTTYPES][32] = {"none",        "histogram",   "timeseries", "peakplot", 
					"impulseplot", "scatterplot", "textplot", "spiketrain",
					"barplot", "linesplot" };
const int  sptypesmap[NUMSPLOTTYPES]  = {PTYPE_NULL, PTYPE_HISTO, PTYPE_TMSER, PTYPE_PEAKS, 
					PTYPE_IMPLS, PTYPE_SCATT, PTYPE_TEXT, PTYPE_TRAIN,
					PTYPE_BARS, PTYPE_LINES};

/////////////////////////////////////////////////

// channel  
struct ChannelSettings{
   int id;
   char name[255];
   int src;
   int bits;
   bool sign;
   char format;
   int type;      // see (I/O)TYPE_ defines..
   ChannelSettings *next;
};

// (input) column
struct ColumnSettings{
   int id;
   int channel;
   char name[255];
   int bits;
   bool sign;
   char format;
   int res;
   ColumnSettings *next;
};

// main input settings class
class InputSettings{
  public:
    InputSettings();
    ~InputSettings();
    
    char addch();
    void add_id(int newid);
    void add_name(char* newname);
    void add_bits(int newbits);    
    void add_sign(bool newsign);
    void add_format(char newformat);
    char firstch();
    char nextch();
    int  get_id();
    void get_name(char* newname);
    int  get_bits();
    bool get_sign();
    char get_format();

    char addcol();
    void add_col_id(int newid);
    void add_col_channel(int newchannel);
    void add_col_name(char* newname);
    void add_col_bits(int newbits);
    void add_col_sign(bool newsign);
    void add_col_format(char newformat);
    void add_col_res(int newres);
    char firstcol();
    char nextcol();
    int  get_col_id();
    int  get_col_channel();
    void get_col_name(char* newname);
    int  get_col_bits();
    bool get_col_sign();
    char get_col_format();
    int  get_col_res();
    
    unsigned int numcols;
    unsigned int numchs;
    
    char filename[255];
    int port;
    int bits;
    Rs232ParserSettings *rs232;
    
  private:
    ColumnSettings  *collist, *csps;
    ChannelSettings *chlist, *sps;
};

// main output class
class OutputSettings{
  public:
    OutputSettings();
    ~OutputSettings();

   char addch();
   void add_id(int newid);
   void add_name(char* newname);
   void add_bits(int newbits);
   void add_sign(bool newsign);
   void add_format(char newformat);
   void add_src(int newsrc);
   void add_type(int newtype);
   
   char firstch();
   char nextch();
   int  get_id();
   char get_name(char* newname);
   int  get_bits();
   bool get_sign();
   char get_format();
   int  get_src();
   int  get_type();

   unsigned int numchs;
   char filename[255];
   char host[255];
   int port;
   char serport[255];
   int bits;
   int baudrate;
   
  private:
    ChannelSettings *chlist, *sps;    
};

// plot
struct SubPlotSettings{
   unsigned int id;
   unsigned int type;
   unsigned int src;
   unsigned int res;
   char scale;
   char title[255];
   char xtitle[255];
   char ytitle[255];
   int colour;
   SubPlotSettings *next;
};

// plots list
class WinSettings {
  
 public:
   WinSettings();
   ~WinSettings();

   char addplot();
   char add_id(unsigned int newid);
   char add_type(int newtype);
   char add_src(unsigned int newsrc);
   char add_res(int newres);
   char add_scale(char newscale);
   char add_title(char* newtitle);
   char add_xtitle(char* newtitle);
   char add_ytitle(char* newtitle);
   char add_colour(int newcolour);

   char firstplot();
   char nextplot();
   unsigned int get_id();
   unsigned int get_type();
   unsigned int get_src();
   unsigned int get_res();
   unsigned int get_res(unsigned int id, unsigned int type);
   char get_scale();
   char get_title(char* newtitle);
   char get_xtitle(char* newtitle);
   char get_ytitle(char* newtitle);
   int get_colour(); 

   unsigned int numplots;
   unsigned int numsrcs;
   unsigned int numpeaks;
   unsigned int numids;
   unsigned int peakiter;

   int height;
   int width;
   int xpos;
   int ypos;
   char border;
   int framespersec;
   int skip;

 private:
   SubPlotSettings *plotlist, *sps;  // root pointer, and last iterater
};
  
class ParamSettings{
 public:
   ParamSettings() {size_iter=0;}
   ~ParamSettings() {}
 
   void add_dim(unsigned int dim) { gridsize[size_iter++]=dim;} 
    
   float lr;
   float nbr;
   unsigned int gridsize[2];
   unsigned int size_iter;
};
     
class KProf {
 
 public:
   KProf() {input_mode=0;output_mode=0;};
   ~KProf() {};
   
   char parse(char *filename, const unsigned int validtype); 
   
   InputSettings is;
   OutputSettings os;
   WinSettings win; 
   ParamSettings par;  
             
   int last_section;

   int input_mode;
   int output_mode;   
};
