/***************************************************************************
                          wparse.cpp  -  v1.00
                          --------------------
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

#include "wparse.h" 

WParse::WParse() 
{
	err = 0;	errline = 0;
	sp      	= NULL; sp_size=0;
	kp      	= NULL;
	cp      	= NULL;
	wn		= NULL;
	winset  	= NULL;
	plotset 	= NULL;
	kvect_hist	= NULL;
	kpeak_hist	= NULL;
	num_icols	= 0;
}

WParse::~WParse()
{
	// delete all elements separately from sp:
	for (unsigned int i=0; i<sp_size; i++) delete sp[i];
	if (sp!=NULL) delete []sp;
	if (kp!=NULL) delete kp;
	if (cp!=NULL) delete cp;
	if (wn!=NULL) delete wn;
	if (kvect_hist!=NULL) delete []kvect_hist;
	if (kpeak_hist!=NULL) delete []kpeak_hist;
}

void WParse::parse_window(FILE* fp, char* tmpstr, unsigned int line, 
                        bool valid_sub_tag, bool valid_att_tag)
{
	if (strcasecmp(tmpstr,"window")==0) {
		winset = new WindowSettings;
		sp[sp_size] = new WindowSetParse(winset);
	}
	else if (strcasecmp(tmpstr,"plot")==0) {
		if (plotset==NULL) { // first addition
			plotset = new PlotSettings; plotpset = plotset;
		}
		else { 
			PlotSettings *plotnset = new PlotSettings;
			plotpset->next = plotnset; plotpset = plotnset;
		}
		sp[sp_size] = new PlotSetParse(plotpset);
	}
	else if (!valid_sub_tag) {
		err = ERR_INVTAG; errline = line;
	}
	if (valid_att_tag || (strcasecmp(tmpstr,"window")==0)){
		if (sp[sp_size]->read_set(fp)!=0) { 
			err = ERR_INVATTR; errline = line;
		}
		if (sp[sp_size]->update_set()!=0) { 
			err = ERR_UPDATE; errline = line;
		}
		if (strcasecmp(tmpstr,"plot")==0){
			if (plotpset->id > (int)num_plots) 
				num_plots = (unsigned int) plotpset->id;
		}
		sp_size++;
	}
}

int WParse::parse(FILE* fp) 
{
	unsigned int line = 1;
	unsigned int max_tags=1024;
	err=0;
	num_plots=0;
	sp = new SetParse*[max_tags];
	bool valid_section_tag = false;
	if (!fp) 
		return ERR_NOFILE;
	else while (!feof(fp))
	{
		int t=0;
		char ch;
		bool valid_att_tag = false, valid_sub_tag = false;
		char tmpstr[MAX_TAG_LENGTH];
		do {
			do { ch=getc(fp); if (ch=='\n') line++;
			} while ((ch!='<')&&(!feof(fp))); // wait for a '<'
			t=0;		   
			do { ch=getc(fp); tmpstr[t++]=ch; if (ch=='\n') line++;
			} while ((ch!=' ')&&(ch!='\t')&&
			         (ch!='\n')&&(ch!='\r')&&(ch!='>')&&(!feof(fp)));
			tmpstr[t-1]='\0'; // extract the word following the '<'
			int i;
			for (i=0; i<NUM_SUBTAGS; i++) // sub tag ?
				if (strcasecmp(tmpstr,sub_tags[i])==0) 
					valid_sub_tag=true;
			if (valid_section_tag) // we're in <window> 
			{
				for (i=0; i<NUM_A_WTAGS; i++) // window tag? 
					if (strcasecmp(tmpstr,window_att_tags[i])==0) 
						valid_att_tag=true;
			}
			if (strcasecmp(tmpstr,"window")==0) {
				valid_section_tag=true; 
				valid_sub_tag=true;
			}
		} while (!feof(fp)&&(!valid_att_tag)&&(!valid_sub_tag));
		if (feof(fp)) return err;
		if (sp_size<max_tags) {
			if (valid_section_tag) 
				parse_window(fp,tmpstr,line,valid_sub_tag,valid_att_tag);
		}
		else { // too many tags: array is overflowing!setparse
			err = ERR_TAGOVERFLOW; errline = line;
		}
	}
	fclose(fp);
	return err; // got error?
}

int WParse::export_dtd(char* buffer) 
{
// 	char tmpstr[MAX_TAG_LENGTH];
// 	char tmpbuf[MAX_DTD_LENGTH];
	char last_tag[MAX_TAG_LENGTH];
	last_tag[0]=0;
	buffer[0]=0;
	char *authorstr="The CSTK Team";
	char *titlestr="CommonSense ToolKit";
//	unsigned int i;
	sprintf(buffer,"<!DOCTYPE CSTK [\n\t");
	sprintf(buffer,"%s<!ENTITY AUTHOR \"%s\">\n\t",buffer,authorstr);
	sprintf(buffer,"%s<!ENTITY SOFTWARE \"%s\">\n",buffer,titlestr);
/*	sprintf(buffer,"%s\t<!ELEMENT output ((%s",buffer,output_att_tags[3]);
	for (i=4; i<NUM_A_ITAGS; i++)
		sprintf(buffer,"%s|%s",buffer,output_att_tags[i]);
	sprintf(buffer,"%s),outputcolumn*)>\n",buffer);
	sprintf(buffer,"%s]>\n",buffer);*/
	return err;
}

int WParse::export_xsd(char* buffer) 
{
	char tmpstr[MAX_XSD_LENGTH];
	char last_mode[MAX_TAG_LENGTH]; 
	char last_tag[MAX_TAG_LENGTH];
	char curr_tag[MAX_TAG_LENGTH];
	last_mode[0]='\0';
	last_tag[0]='\0';
	buffer[0] = '\0';
	if (sp_size) {
		for (unsigned int t=0; t<sp_size; t++) {
			sp[t]->write_tag(curr_tag);
			sp[t]->write_set(tmpstr);
			// is it a different tag than the previous?
			if ((strcasecmp(last_tag,curr_tag)!=0)
				&&(last_tag[0]!='\0')
				&&(strcasecmp(last_tag,last_mode)!=0)
				&&(last_mode[0]!=0)) {
					sprintf(buffer,"%s\t\t</%s>\n", buffer, last_mode);
					last_mode[0]='\0';
			} 
			// is it a tag that needs closing immediately?
			if ((strcasecmp(curr_tag,"channel")==0)||
			(strcasecmp(curr_tag,"inputcolumn")==0) ||
			(strcasecmp(curr_tag,"plot")==0) ||
			(strcasecmp(curr_tag,"poll")==0)) {
				sprintf(buffer,"%s\t%s</%s>\n", buffer, tmpstr,curr_tag);
			}
			else {
				sprintf(buffer,"%s\t%s", buffer, tmpstr);
				strcpy(last_mode, curr_tag);
			} 
			strcpy(last_tag, curr_tag);
		}
		if (last_mode[0]!='\0') sprintf(buffer,"%s\t\t</%s>\n", buffer, last_mode);
	}
	return err;
}

int WParse::setup_window() 
{
	// set and create window
	if (winset) {
		wn = new X11Plot(*winset);
		wn->prepare_colours();
		kp = new KVPlot(wn);
		cp = new ClustPlot(wn);
	} else {
		err = ERR_NOWINDOW;
		return err;
	}

	return 0;
}

// plot the kvector (with the peak if required) using the parsed settings:
int WParse::kvplot(KVector *kvect) 
{
	Pk pk; // temporary peak struct
	//for (int skip=0; skip<winset->skip; skip++) 
	if (num_icols==0) // first init: create!
	{
		num_icols = kvect->pvect_size;
		// plotting kvectors:
		if (kvect_hist==NULL)
		{
			kvect_hist = new KVector[num_icols];
			for (vei_t i=0; i<(vei_t)num_icols; i++) {
				plotpset = plotset;
				bool qt = false;
				while ( (plotpset != NULL) && (qt==false) ) {
					if ( (plotpset->type==PLOT_TIME_TYPE)
					&& (plotpset->src==i) ) {
						kvect_hist[i].createVector(plotpset->res);
						qt = true;
					}
					plotpset = plotpset->next;
				}
				if (!qt) kvect_hist[i].createVector(1);
			}
		}
		// plotting kpeaks:
		if (kpeak_hist==NULL)
		{
			kpeak_hist = new Peak[num_icols];
			for (vei_t i=0; i<(vei_t)num_icols; i++) {
				plotpset = plotset;
				bool qt = false;
				while ( (plotpset != NULL) && (qt==false) ) {
					if ( (plotpset->type==PLOT_PEAK_TYPE)
					&& (plotpset->src==i) ) {
						kpeak_hist[i].createPeak(plotpset->res);
						qt = true;
					}
					plotpset = plotpset->next;
				}
				if (!qt) kpeak_hist[i].createPeak(1);
			}
		}
	}
	if (num_icols!=kvect->pvect_size)
		return -1; // error!
	for (int i=0; i<num_icols; i++) {
		kvect_hist[i].add_comp(kvect->pvect[i]);
	}
        for (int i=0; i<(int)num_icols; i++) {
		kvect_hist[i].get_peak(&pk.s, &pk.a, &pk.l, &pk.p);
		kpeak_hist[i].add_peak(&pk);
	}

	plotpset = plotset;
	while(plotpset != NULL) {
		switch (plotpset->type) {
			case PLOT_HIST_TYPE: // if histogram
				kp->histogram(plotpset->id+1, num_plots+1,
					kvect_hist[plotpset->src],
					plotpset->color, 
					plotpset->res, plotpset->title);
				break;
			case PLOT_TIME_TYPE: // if timeseries
				kp->timeseries(plotpset->id+1, num_plots+1,
					kvect_hist[plotpset->src],
					plotpset->color,
					plotpset->title, plotpset->scale);
				break;
			case PLOT_PEAK_TYPE: // if timeseries
				kp->peakplot(plotpset->id+1, num_plots+1,
					kpeak_hist[plotpset->src],
					plotpset->title);
				break;
			case PLOT_TEXT_TYPE: // if textplot
				kp->textplot(plotpset->id+1, num_plots+1, 
					kvect_hist[plotpset->src]);
				break;
			case PLOT_IMPL_TYPE: // if impulse
				kp->impulse(plotpset->id+1, num_plots+1,
					kvect_hist[plotpset->src], plotpset->color, 
					plotpset->title);
				break;
			case PLOT_SPIK_TYPE: // if spiketrain
				kp->spiketrain(plotpset->id+1, num_plots+1,
					kvect_hist[plotpset->src], plotpset->color, 
					plotpset->title);
				break;
		}
		plotpset = plotpset->next;
	} 
	wn->swap_buffers();
        return 0;
}

// plot the current kvector in a topographic map, using the parsed settings:
int WParse::ktplot(KVector *kvect, int x, int y, int max_x, int max_y) 
{
	plotpset = plotset;
	while(plotpset != NULL) {
		switch (plotpset->type) {
			case PLOT_T_BR_TYPE: // if bar plot
				//printf("x%i, y%i, mx%i, my%i\n", x, y, max_x, max_y);
				//cp->barplot( x, y, max_x, max_y, 
				//    kvect[plotpset->src], plotpset->color);
				break;
			case PLOT_T_LN_TYPE: // if line plot
				//cp->lineplot(x, y, max_x, max_y, 
				//    kvect[plotpset->src], plotpset->color);
				break;
		}
		plotpset = plotpset->next;
	} 
	if (x==max_x && y==max_y) wn->swap_buffers();
        return 0;
}

int WParse::check_events() 
{
	wn->delay(10000/winset->framerate);
	switch (wn->eventloop()) { // press 'q' for quit
          case 24: return 0; // q
          case 82: 
	  	break;
          case 86: 
	  	break;
          case 33: 
	  	break;
        }
	return 1;
}

void WParse::export_err(char* buffer) 
{
	if (err==0)
		buffer[0] = '\0'; // empty string
	else if (err<NUM_KPERRS) 
		sprintf(buffer, "%s Line %i.\n\r", kperr_strings[err], errline);
	else 
		sprintf(buffer, "Device Error %i\n\r",err);
	err = 0;
}

void WParse::init(FILE* fp) 
{
	parse(fp);
	setup_window();
}

