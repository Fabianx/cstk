/***************************************************************************
                          kprof.cpp  -  v1.00
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

#include "kprof.h"

//////////////////////////////////////////////////////////////////////////////

InputSettings::InputSettings() {
  chlist = NULL;
  collist= NULL;
  sps = NULL;
  csps = NULL;  
  numcols = 0;
  numchs = 0;
  filename[0] = '\0';
  port = 0;
  rs232 = new Rs232ParserSettings;
}

InputSettings::~InputSettings() {
  ChannelSettings *spstemp;
  ColumnSettings *cspstemp;  
  // wipe out the input channel list:
    sps = chlist;
    while (sps!=NULL) {
         spstemp = sps;
         sps = sps->next;
         delete spstemp;
    }        
  // wipe out the input column list:
  csps = collist;
  while (csps!=NULL) {
         cspstemp = csps;
         csps = csps->next;
         delete cspstemp;
  }
  if (rs232!=NULL) delete rs232;
}

char InputSettings::addch() {
  ChannelSettings *spstemp;
  spstemp = new ChannelSettings;
  spstemp->id = -1;
  spstemp->name[0]='\0';
  spstemp->bits=-1;
  spstemp->next = NULL;
  if (sps==NULL) {
      sps = spstemp;
      chlist = sps;
  } else {
      // make sure to go to the end of the list:
      while (sps->next!=NULL) {
        sps = sps->next;
      }
      sps->next = spstemp;
      sps = sps->next;
  }
  numchs++;
  return numchs;    
}

void InputSettings::add_id(int newid) {
  sps->id = newid;
}

void InputSettings::add_name(char* newname) {
  int i=0;
  do  {
    sps->name[i] = newname[i]; i++;
  } while (newname[i-1]!='\0');
}

void InputSettings::add_bits(int newbits) {
  sps->bits = newbits;
}

void InputSettings::add_sign(bool newsign) {
  sps->sign = newsign;
}

void InputSettings::add_format(char newformat) {
  sps->format = newformat;
}

char InputSettings::firstch() {
  sps = chlist;
  return ((sps==NULL)?0:1);
}

char InputSettings::nextch() {
  sps = sps->next;
  return ((sps==NULL)?0:1); 
}

int InputSettings::get_id() {
  return sps->id;  
}

void InputSettings::get_name(char* newname) {
  int i=0;
  do  {
     newname[i] = sps->name[i]; i++;
  } while (newname[i-1]!='\0');
}

int InputSettings::get_bits() {
  return sps->bits;      
}

bool InputSettings::get_sign() {
  return sps->sign;      
}

char InputSettings::get_format() {
  return sps->format;
}

char InputSettings::addcol() {
  ColumnSettings *spstemp;

  spstemp = new ColumnSettings;
  spstemp->id = -1;
  spstemp->name[0]='\0';
  spstemp->channel=-1;
  spstemp->bits=-1;
  spstemp->next = NULL;

  if (csps==NULL) {
      csps = spstemp;
      collist = csps;
  } else {
      // make sure to go to the end of the list:
      while (csps->next!=NULL) {
        csps = csps->next;
      }
      csps->next = spstemp;
      csps = csps->next;
  }

  numcols++;

  return numcols;   
}

void InputSettings::add_col_id(int newid) {
  csps->id = newid;
}

void InputSettings::add_col_channel(int newchannel) {
  csps->channel = newchannel;
}

void InputSettings::add_col_name(char* newname) {
  int i=0;
  do  {
    csps->name[i] = newname[i]; i++;
  } while (newname[i-1]!='\0');
}

void InputSettings::add_col_bits(int newbits) {
  csps->bits = newbits;
}

void InputSettings::add_col_sign(bool newsign) {
  csps->sign = newsign;
}

void InputSettings::add_col_format(char newformat) {
  csps->format = newformat;
}

char InputSettings::firstcol() {
  csps = collist;
  return ((csps==NULL)?0:1);
}

char InputSettings::nextcol() {
  csps = csps->next;
  return ((csps==NULL)?0:1);   
}

int  InputSettings::get_col_id() {
  return csps->id;    
}

int  InputSettings::get_col_channel() {
  return csps->channel;    
}

void InputSettings::get_col_name(char* newname) {
  int i=0;
  do  {
     newname[i] = csps->name[i]; i++;
  } while (newname[i-1]!='\0');
}

int  InputSettings::get_col_bits() {
  return csps->bits;  
}

bool InputSettings::get_col_sign() {
  return csps->sign;  
}

char InputSettings::get_col_format() {
  return csps->format;
}

int InputSettings::get_col_res() {
  return csps->res;
}

//////////////////////////////////////////////////////////////////////////////

OutputSettings::OutputSettings() {
  chlist = NULL;
  sps = NULL;
  
  numchs = 0;
  filename[0] = '\0';
  host[0] = '\0';
  port = 0;
  bits = 0;
  baudrate = 0;
}

OutputSettings::~OutputSettings() {

  ChannelSettings *spstemp;

  sps = chlist;
  while (sps!=NULL) {
         spstemp = sps;
         sps = sps->next;
         delete spstemp;
  }

}

char OutputSettings::addch() {
  
  ChannelSettings *spstemp;

  spstemp = new ChannelSettings;
  spstemp->id = -1;
  spstemp->name[0]='\0';
  spstemp->bits=-1;
  spstemp->src=-1;
  spstemp->type= -1;
  spstemp->next = NULL;

  if (sps==NULL) {
      sps = spstemp;
      chlist = sps;
  } else {
      // make sure to go to the end of the list:
      while (sps->next!=NULL) {
        sps = sps->next;
      }
      sps->next = spstemp;
      sps = sps->next;
  }

  numchs++;

  return numchs;  
}

void OutputSettings::add_id(int newid) {
  sps->id = newid;
}

void OutputSettings::add_name(char* newname) {
  int i=0;
  do  {
    sps->name[i] = newname[i]; i++;
  } while (newname[i-1]!='\0');
}

void OutputSettings::add_bits(int newbits) {
  sps->bits = newbits;
}

void OutputSettings::add_sign(bool newsign) {
  sps->sign = newsign;
}

void OutputSettings::add_format(char newformat) {
  sps->format = newformat;
}

void OutputSettings::add_src(int newsrc) {
  sps->src = newsrc;
}

void OutputSettings::add_type(int newtype) {
  sps->type = newtype;
}

char OutputSettings::firstch() {
  sps = chlist;
  return ((sps==NULL)?0:1);
}

char OutputSettings::nextch() {
  sps = sps->next;
  return ((sps==NULL)?0:1); 
}

int OutputSettings::get_id() {
  return sps->id;
}

char OutputSettings::get_name(char* newname) {
  int i=0;
  do  {
     newname[i] = sps->name[i]; i++;
  } while (newname[i-1]!='\0');
  return 0;  
}    

int OutputSettings::get_bits() {
  return sps->bits;    
}

bool OutputSettings::get_sign() {
  return sps->sign;    
}

char OutputSettings::get_format() {
  return sps->format;
}

int OutputSettings::get_src() {
  return sps->src;
}

int OutputSettings::get_type() {
  return sps->type;  
}

//////////////////////////////////////////////////////////////////////////////

WinSettings::WinSettings() {
  
  plotlist=NULL;
  sps = NULL;
  numplots=0;
  numsrcs=0;
  numpeaks=0;
  numids=0;
  peakiter=0;
  skip = 3;
}

WinSettings::~WinSettings() {

  SubPlotSettings *spstemp;

  sps = plotlist;
  while (sps!=NULL) {
         spstemp = sps;
         sps = sps->next;         
         delete spstemp;
  }
             
}

char WinSettings::addplot() {

  SubPlotSettings *spstemp;

  spstemp = new SubPlotSettings;
  spstemp->id = 0;
  spstemp->type= PTYPE_NULL;
  spstemp->src= 0;
  spstemp->res= 100;
  spstemp->scale=1;
  spstemp->title[0]='\0';
  spstemp->xtitle[0]='\0';
  spstemp->ytitle[0]='\0';
  spstemp->colour=0;
  spstemp->next = NULL;

  if (sps==NULL) {
      sps = spstemp;
      plotlist = sps;          
  } else {
      // make sure to go to the end of the list:
      while (sps->next!=NULL) {
        sps = sps->next;
      }
      sps->next = spstemp;
      sps = sps->next;
  }

  numplots++;

  return numplots;
}

char WinSettings::add_id(unsigned int newid){
  sps->id = newid;
  if (numids<newid+1) numids=newid+1;
  return 0;  
}

char WinSettings::add_type(int newtype){
  sps->type = newtype;
  if (newtype==PTYPE_PEAKS) // = peakplot
    numpeaks++;
  return 0;  
}

char WinSettings::add_src(unsigned int newsrc){
  sps->src = newsrc;
  if (numsrcs<newsrc+1) numsrcs=newsrc+1;  // mainly for sanity check
  return 0;  
}

char WinSettings::add_res(int newres){
  sps->res = newres;
  return 0;
}

char WinSettings::add_scale(char newscale){
  sps->scale = newscale;
  return 0; 
}

char WinSettings::add_title(char* newtitle){
  int i=0;
  do  {
    sps->title[i] = newtitle[i]; i++;
  } while (newtitle[i-1]!='\0');
  return 0; 
}

char WinSettings::add_xtitle(char* newtitle){
  int i=0;
  do  {
    sps->xtitle[i] = newtitle[i]; i++;
  } while (newtitle[i-1]!='\0');
  return 0;
}

char WinSettings::add_ytitle(char* newtitle){
  int i=0;
  do  {
    sps->ytitle[i] = newtitle[i]; i++;
  } while (newtitle[i-1]!='\0');
  return 0;
}

char WinSettings::add_colour(int newcolour){
  sps->colour = newcolour;
  return 0;  
}

char WinSettings::firstplot() {
  sps = plotlist;
  peakiter=0;  
  return ((sps==NULL)?0:1);
}

char WinSettings::nextplot() {
  sps = sps->next;
  if (sps==NULL)
       return 0;
  else {
    if (sps->type==PTYPE_PEAKS) peakiter++;
    return 1;
  }
}

unsigned int WinSettings::get_id(){
  return sps->id;
}

unsigned int WinSettings::get_type(){
  return sps->type;
}

unsigned int WinSettings::get_src(){
  return sps->src;
}

unsigned int WinSettings::get_res(){
  return sps->res;
}

unsigned int WinSettings::get_res(unsigned int id, unsigned int type){
  firstplot();
  do {
    if ( (get_src()==id) && (get_type()==type) )
           return get_res(); 
  } while ( nextplot() );

  return 0;
}

char WinSettings::get_scale(){
  return sps->scale;
}

char WinSettings::get_title(char* newtitle){
  int i=0;
  do  {
     newtitle[i] = sps->title[i]; i++;
  } while (newtitle[i-1]!='\0');
  return 0;
}

char WinSettings::get_xtitle(char* newtitle){
  int i=0;
  do  {                              
     newtitle[i] = sps->xtitle[i]; i++;
  } while (newtitle[i-1]!='\0');
  return 0;
}

char WinSettings::get_ytitle(char* newtitle){
  int i=0;
  do  {
     newtitle[i] = sps->ytitle[i]; i++;
  } while (newtitle[i-1]!='\0');
  return 0;
}

int WinSettings::get_colour(){
  return sps->colour;
}

//////////////////////////////////////////////////////////////////////////////
/* a quick & dirty parser, but hey: it's FAST! */
char KProf::parse(char *filename, const unsigned int validtype){

  FILE* fp;
  unsigned int i;
  char command = false;
  char plot_opened = false;
  char col_opened = false;
  char och_opened = false;
  char ich_opened = false;
  char ch;
  char last_command[256];
  char last_item[256];
  unsigned int  tempvalue;
  char tempstr[255];
  unsigned int  settings_type = SETTP_NON; // what's the intended tool ?
  int line = 1;
  
  last_command[0]='\0';
  last_item[0]='\0';

  is.numcols = 0;
  par.size_iter = 0;
  fp = fopen(filename, "r");
  try {  
   if (fp) {
    while (!feof(fp)) {
       ch = fgetc(fp);
       switch (ch) { 
          case '<':  command = true; last_command[0]='\0'; 
		     // boolean per section:
                     plot_opened=false; 
		     col_opened=false; 
		     och_opened=false; 
		     ich_opened=false; 
		     break;
          case '>':  command = false; last_item[0]='\0';      break;      
          case ' ':  command = false; last_item[0]='\0';      break;
          case '\t': command = false; last_item[0]='\0';      break;
	  case '\n': line++;
          default: 
                if (command) {
                    for (i=0; i<strlen(last_command)+1;i++) 
                        if (last_command[i]=='\0') {  last_command[i+1] = '\0'; last_command[i] = ch; break;}
                } else { 
                    for (i=0; i<strlen(last_item)+1;i++) 
                        if (last_item[i]=='\0') { last_item[i+1] = '\0'; last_item[i] = ch; break; }
                    for (i=0; i<NUMDOCTYPES; i++) _CONF_DOCTYPE(dtypes[i],dtypesmap[i]) 
		    if ((validtype==SETTP_ALL) || (settings_type==validtype)) { 
			_IFCOMMAND("input") last_section = SEC_INPUT;
			_IFCOMMAND("output") last_section = SEC_OUTPUT;
			_IFCOMMAND("window") last_section = SEC_WINDOW;
			_IFCOMMAND("params") last_section = SEC_PARAMS;
			switch (last_section) {
			   case SEC_INPUT: _IFCOMMAND("column") {  
					        if (!col_opened) { col_opened = true; is.addcol(); }
						_CONF_INT_ADD_ATTRIB("id=\"",tempvalue,is.add_col_id)
						_CONF_INT_ADD_ATTRIB("channel=\"",tempvalue,is.add_col_channel)
						_CONF_STR_ADD_ATTRIB("name=\"",tempstr,is.add_col_name)
						_CONF_INT_ADD_ATTRIB("bits=\"",tempvalue,is.add_col_bits)
						_IFATTRIB("format=\"") {
							fscanf(fp, "%[^\"]", tempstr);
							_IFSTR("integer")	is.add_col_format(DF_INTEGER);
							else _IFSTR("float")	is.add_col_format(DF_FLOAT);
							else throw line; //attribute format isn't set
						}

					        _IFATTRIB("sign=\"") {
							fscanf(fp,"%[^\"]", tempstr);
							_IFSTR("signed")        is.add_col_sign(SIGN_SIGNED);
							else _IFSTR("unsigned") is.add_col_sign(SIGN_UNSIGNED);
							else throw line; //attribute sign isn't signed or unsigned         
						}
					    } else
					    _IFCOMMAND("channel") { 
						if (!ich_opened) { ich_opened = true; is.addch(); }
						_CONF_INT_ADD_ATTRIB("id=\"",tempvalue,is.add_id)
						_CONF_STR_ADD_ATTRIB("name=\"",tempstr,is.add_name)
						_CONF_INT_ADD_ATTRIB("bits=\"",is.bits,is.add_bits)
						_IFATTRIB("format=\"") {
							fscanf(fp, "%[^\"]", tempstr);
							_IFSTR("integer")		is.add_format(DF_INTEGER);
							else _IFSTR("float")	is.add_format(DF_FLOAT);
							else throw line; //attribute format isn't set
						}
						_IFATTRIB("sign=\"") {
							fscanf(fp,"%[^\"]", tempstr);
							_IFSTR("signed")        is.add_sign(SIGN_SIGNED);
							else _IFSTR("unsigned") is.add_sign(SIGN_UNSIGNED);
							else throw line; //attribute sign isn't signed or unsigned    
						}
					    } else {
					        _SET_IMODE(input_mode); // it has to be one of the input modes
						switch(input_mode) {
						   case IMODE_RS232:    _IFATTRIB("baudrate=\"") {
										fscanf(fp,"%[^\"]", tempstr);
										_SET_BR(is.rs232->baudrate);
									} else
									_IFATTRIB("mode=\"") {
										fscanf(fp,"%[^\"]", tempstr);
										_IFSTR("ascii")   is.rs232->mode=ASC_MODE;
										_IFSTR("binary")  is.rs232->mode=BIN_MODE;
									} else
									_CONF_STR_ATTRIB("port=\"",is.rs232->device)
									_CONF_INT_ATTRIB("buffsize=\"",&is.rs232->buff_size)
									_IFCOMMAND("poll") {
										_CONF_STR_ATTRIB("command=\"", is.rs232->poll_char);
									} 
						   			break;
						   case IMODE_FILE:	_CONF_STR_ATTRIB("filename=\"",is.filename);   
						   			break;
						   case IMODE_PIPE:     
						   			break;
						   case IMODE_UDP:	_CONF_INT_ATTRIB("port=\"",&is.port);
						   			break;
						   case IMODE_SIM:      
						   			break;
						   case IMODE_INT:      _CONF_STR_ATTRIB("filename=\"",is.filename);   
						   			break;
						}					    	
					    } 					      
					    break;
			   case SEC_OUTPUT: _SET_OMODE(output_mode); // it has to be one of the output modes
					    _IFCOMMAND("channel") {
						if (!och_opened) { och_opened = true; os.addch(); }
						_CONF_INT_ADD_ATTRIB("id=\"",tempvalue,os.add_id)
						_CONF_INT_ADD_ATTRIB("src=\"",tempvalue,os.add_src)
						_CONF_STR_ADD_ATTRIB("name=\"",tempstr,os.add_name)
						_CONF_INT_ADD_ATTRIB("bits=\"",os.bits,os.add_bits)
						_IFATTRIB("sign=\"") {
							fscanf(fp,"%[^\"]", tempstr);
							_IFSTR("signed")        os.add_sign(SIGN_SIGNED);
							else _IFSTR("unsigned") os.add_sign(SIGN_UNSIGNED);
							else throw line; //attribute sign isn't signed or unsigned
						}
						_IFATTRIB("type=\"") {
							fscanf(fp,"%[^\"]", tempstr);
							_SET_OTYPE(os.add_type)
						} 
					    } else {
					        _SET_OMODE(output_mode); // it has to be one of the input modes
						switch(output_mode) {
						   case OMODE_RS232:    _IFATTRIB("baudrate=\"") {
										fscanf(fp,"%[^\"]", tempstr);
										_SET_BR(os.baudrate);
									} else
									_CONF_STR_ATTRIB("port=\"",os.serport);
									break;
						   case OMODE_FILE:	_CONF_STR_ATTRIB("filename=\"",os.filename);   
						   			break;
						   case OMODE_PIPE:
						   			break;
						   case OMODE_UDP:	_CONF_STR_ATTRIB("host=\"",os.host)
						   			_CONF_INT_ATTRIB("port=\"",&os.port);
						   			break;
						   case OMODE_INT:      _CONF_STR_ATTRIB("filename=\"",os.filename);
						   			break;
						}
					    }
					    break;
			   case SEC_WINDOW: _IFCOMMAND("window") {
						_CONF_INT_ATTRIB("xpos=\"",&win.xpos)
						_CONF_INT_ATTRIB("ypos=\"",&win.ypos)
						_CONF_INT_ATTRIB("width=\"",&win.width)
						_CONF_INT_ATTRIB("height=\"",&win.height)
						_CONF_INT_ATTRIB("border=\"",(int*)&win.border)
						_CONF_INT_ATTRIB("framespersec=\"",&win.framespersec)
						_CONF_INT_ATTRIB("skip=\"",&win.skip);
			                    } else
					    _IFCOMMAND("plot") {
						if (!plot_opened) { plot_opened = true; win.addplot(); } 
						_CONF_INT_ADD_ATTRIB("id=\"",tempvalue,win.add_id)
						if (strcasecmp(last_item,"type=\"")==0) {
							fscanf(fp,"%[^\"]", tempstr);
							for (i=0; i<NUMSPLOTTYPES; i++) 
								_IFSTR(sptypes[i]) win.add_type(sptypesmap[i]);
						} else
						_CONF_INT_ADD_ATTRIB("src=\"",tempvalue,win.add_src)
						_CONF_INT_ADD_ATTRIB("res=\"",tempvalue,win.add_res)
						_IFATTRIB("scale=\"") {
							fscanf(fp,"%[^\"]", tempstr);
							_IFSTR("yes")         win.add_scale(true);
							else _IFSTR("no")  win.add_scale(false);
							else throw line; //attribute scale needs either 'yes' or 'no'
						} else
						_CONF_STR_ADD_ATTRIB("title=\"",tempstr,win.add_title)
						_CONF_INT_ADD_ATTRIB("color=\"",tempvalue,win.add_colour)
						_CONF_STR_ADD_ATTRIB("xtitle=\"",tempstr,win.add_xtitle)
						_CONF_STR_ADD_ATTRIB("ytitle=\"",tempstr,win.add_ytitle);
					    }
			                    break;
			   case SEC_PARAMS: _IFCOMMAND("param") {
					        _CONF_FLT_ATTRIB("lr=\"",&par.lr)
						_CONF_FLT_ATTRIB("nbr=\"",&par.nbr);
					        _CONF_INT_ADD_ATTRIB("dim=\"",tempvalue,par.add_dim);
					    }
			                    break;
			} // switch last section
		    } // if valid setting
                } // if command.. else                
       } //switch
    } //while  
    fclose(fp);
    
   } //if
  } //try 
  catch(int position) {
    return position;
  }
  
  ///// additional sanity checks here: //////////////
  switch(validtype) {
    	case SETTP_ALL:  
	                 break;
    	case SETTP_RTP:  if ((is.numcols==0)||(is.numchs==0)) {  // no input!
	                       return -1;
	                 } 
			 if (win.numplots==0) {  // no input!
	                       return -1;
	                 } 
			 break;
    	case SETTP_TPP:  if ((is.numcols==0)||(is.numchs==0)) {  // no input!
	                       return -1;
	                 } 
			 if (win.numplots==0) {  // no input!
	                       return -1;
	                 } 
	                 break;
    	case SETTP_DTP:  if ((is.numcols==0)||(is.numchs==0)) {  // no input!
	                       return -1;
	                 } 
			 if (win.numplots==0) {  // no input!
	                       return -1;
	                 } 
	                 break;	
  }
  /////////////////////////////////////////////

  return 0;
}


