#! /usr/bin/env python
#***************************************************************************
#                           rtplotgui.py - v.1.00
#                          -----------------------
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

import pygtk   # for visual effects
pygtk.require("2.0")
import gtk, gobject
import os      # executing the rtplot tool
import sys     # arguments
import string  # idem  
import xsd     # xml settings document stuff
import window  # window settings
import xml.parsers.expat # xml parser
import listcol # columns functions for list views

inputstrings = ["Logged Data", "UDP", "Serial Port", "Internal", 
                "Simulation", "Other Process"]
outputstrings = ["None", "Logged Data", "UDP", "Serial Port",  
                 "Internal", "Other Process"]

toolspath = "./cstk_devonly/tools/"
xsdpath = "./cstk_settings/"
xsdfile = ""

section = 0

##############################################################################
# setup functions for widgets: 
##############################################################################
def d_spin(adj, climb, dig, istep, ipage, rmin, rmax, val):
	wd = gtk.SpinButton(adj, climb, dig)
	wd.set_range(rmin,rmax)
	wd.set_increments(istep,ipage)
	wd.set_value(val)
	return wd

def d_entr(val):
	wd = gtk.Entry()
	wd.set_text(val)
	return wd
	
def d_togg(txt, val):
	wd = gtk.CheckButton(txt)
	wd.set_active(val)
	return wd
	
def d_comb(txt, vals):
	wd = gtk.Combo()
	wd.set_popdown_strings(vals)
	wd.entry.set_text(txt)
	return wd

##############################################################################
# read the XML Setting Document 
##############################################################################
def chck(listname, option):
	if not option in listname:	return 0
	else:				return listname[option]

def start_element(name, atts):
	global section
	if name in ('input', 'output', 'window'):
		section = xsd.sect[name]
	elif name in ('file', 'udp', 'rs232', 'int', 'pipe', 'sim', 'none'):
		if (section==1):
			input_tabs.set_current_page(xsd.inp.index(name))
		elif (section==2):
			output_tabs.set_current_page(xsd.outp.index(name))
	if name=='window':
		window.xpos = int(chck(atts,'xpos'))
		window.ypos = int(chck(atts,'ypos'))
		window.width =  int(chck(atts,'width'))
		window.height = int(chck(atts,'height'))
		window.border = int(chck(atts,'border'))
		window.framespersec = int(atts['framespersec'])
		window.skip = int(chck(atts,'skip'))
	elif (name=='plot') & (section==3):
		iter = model.append(None)
		model.set(iter,
			listcol.PLOTCOLUMN_EDIT,	gtk.TRUE,
			listcol.PLOTCOLUMN_ID,		int(chck(atts,'id')),
			listcol.PLOTCOLUMN_SRC,		int(chck(atts,'src')),
			listcol.PLOTCOLUMN_TYPE,	xsd.ptype[chck(atts,'type')],
			listcol.PLOTCOLUMN_TITLE,	chck(atts,'title'),
			listcol.PLOTCOLUMN_RES,		int(chck(atts,'res')),
			listcol.PLOTCOLUMN_SCALE,	xsd.scale[chck(atts,'scale')],
			listcol.PLOTCOLUMN_COLOR,	int(chck(atts,'color')) )
	elif (name=='column') & (section==1):
		iter = input_model.append(None)
		input_model.set(iter,
			listcol.INPUTCOLUMN_EDIT,	gtk.TRUE,
			listcol.INPUTCOLUMN_ID,		int(chck(atts,'id')),
			listcol.INPUTCOLUMN_CH,		int(chck(atts,'channel')),
			listcol.INPUTCOLUMN_NAME,	chck(atts,'name'),
			listcol.INPUTCOLUMN_BITS,	int(chck(atts,'bits')),
			listcol.INPUTCOLUMN_SIGN,	xsd.sign[chck(atts,'sign')],
			listcol.INPUTCOLUMN_FORM,	xsd.form[chck(atts,'format')])
	elif (name=='channel') & (section==1):
		iter = input_ch_model.append(None)
		input_ch_model.set(iter, 
			listcol.INPUTCHANNEL_EDIT,	gtk.TRUE, 
			listcol.INPUTCHANNEL_ID,  	int(chck(atts,'id')),
			listcol.INPUTCHANNEL_SRC,	int(chck(atts,'src')),
			listcol.INPUTCHANNEL_NAME,	chck(atts,'name'),
			listcol.INPUTCHANNEL_BITS,	int(chck(atts,'bits')),
			listcol.INPUTCHANNEL_SIGN,	xsd.sign[chck(atts,'sign')],
			listcol.INPUTCHANNEL_TYPE,	xsd.vtype[chck(atts,'type')],
			listcol.INPUTCHANNEL_FORM,	xsd.form[chck(atts,'format')])
	elif (name=='channel') & (section==2):
		iter = output_model.append(None)
		output_model.set(iter, 
			listcol.OUTPUTCHANNEL_EDIT, 	gtk.TRUE, 
			listcol.OUTPUTCHANNEL_ID,  	int(chck(atts,'id')),
			listcol.OUTPUTCHANNEL_SRC,	int(chck(atts,'src')),
			listcol.OUTPUTCHANNEL_NAME,	chck(atts,'name'),
			listcol.OUTPUTCHANNEL_BITS,	int(chck(atts,'bits')),
			listcol.OUTPUTCHANNEL_SIGN,	xsd.sign[chck(atts,'sign')],
			listcol.OUTPUTCHANNEL_TYPE,	xsd.vtype[chck(atts,'type')],
			listcol.OUTPUTCHANNEL_FORM,	xsd.form[chck(atts,'format')])
	elif name=='file':
		if (section==1):
			input_filename.set_text(atts['filename'])
		elif (section==2):
			output_filename.set_text(atts['filename'])
	elif name=='udp':
		if (section==1):
			input_tabs.set_current_page(1)
		elif (section==2):
			output_tabs.set_current_page(2)
	elif name=='rs232':
		if (section==1):
			input_portname.set_text(chck(atts,'port'))
			input_baudrate.set_value(int(chck(atts,'baudrate')))
			input_bufsize.set_value(int(chck(atts,'buffsize')))
			input_databits.set_value(int(chck(atts,'databits')))
			input_stopbits.set_value(int(chck(atts,'stopbits')))
			input_parity.set_active(xsd.par[chck(atts,'parity')])
			input_sermode.entry.set_text(chck(atts,'mode'))
		elif (section==2):
			output_portname.set_text(chck(atts,'port'))
			output_baudrate.set_value(int(chck(atts,'baudrate')))
			output_bufsize.set_value(int(chck(atts,'buffsize')))
			output_databits.set_value(int(chck(atts,'databits')))
			output_stopbits.set_value(int(chck(atts,'stopbits')))
			output_parity.set_active(xsd.par[chck(atts,'parity')])
			output_sermode.entry.set_text(chck(atts,'mode'))
	elif name=='int':
		if (section==1):
			input_tabs.set_current_page(3)
		elif (section==2):
			output_tabs.set_current_page(4)
	elif name=='pipe':
		if (section==1):
			input_tabs.set_current_page(5)
		elif (section==2):
			output_tabs.set_current_page(5)
	elif (name=='sim')&(section==1):
		input_tabs.set_current_page(4)
	elif (name=='poll')&(section==1):
		input_pollstr.set_text(chck(atts,'command'))

def end_element(name):
	global section
	if name in ('input','output','window'):
		section = 0
def char_data(data):
	global section
def read_xsd(model):
	output_tabs.set_current_page(xsd.outp.index('none'))
	f = open('%s%s' % (xsdpath, xsdfile), 'r')
	main_window.set_title("CSTK RTPlot v.1.00 %s" % xsdfile)
	fs = f.read()
	# we want XML parsing, damnit!:
	p = xml.parsers.expat.ParserCreate()
	p.StartElementHandler = start_element
	p.EndElementHandler = end_element
	p.CharacterDataHandler = char_data
	p.Parse(fs)


##############################################################################
# write the XML Setting Document 
##############################################################################
def write_xsd(model):
	f=open('%s%s' % (xsdpath, xsdfile), 'w')
	f.write(xsd.head)
	f.write(xsd.dtd)
	f.write("\n<RTPlotSettings>\n")
	conv_pars = ""
	if   input_tabs.get_current_page()==0:
		conv_type = "file"
		conv_pars = ' filename="%s"' % input_filename.get_text()
	elif input_tabs.get_current_page()==1:
		conv_type = "udp"
	elif input_tabs.get_current_page()==2:
		conv_type = "rs232"
		conv_pars = ' port="%s"' % 	input_portname.get_text()
		conv_pars += ' baudrate="%i"' % input_baudrate.get_value()
		conv_pars += ' buffsize="%i"' % input_bufsize.get_value()
		conv_pars += ' databits="%i"' % input_databits.get_value()
		conv_pars += ' stopbits="%i"' % input_stopbits.get_value()
		if input_parity.get_active():
			conv_tgle = "yes"
		else:	conv_tgle = "no"
		conv_pars += ' parity="%s"' % conv_tgle
		conv_pars += ' mode="%s"' % input_sermode.entry.get_text()
	elif input_tabs.get_current_page()==3:
		conv_type = "int"
	elif input_tabs.get_current_page()==4:
		conv_type = "sim"
	elif input_tabs.get_current_page()==5:
		conv_type = "pipe"
	else:	conv_type = "none" 
	f.write(' <input id="%smode">\n' % conv_type)
	f.write('     <%s%s>\n' % (conv_type,conv_pars))
	if (conv_type == "rs232") & ( input_pollstr.get_text() != ""):
		f.write('           <poll mode="ascii" command=')
		f.write('"%s"></poll>\n' % input_pollstr.get_text())
	f.write('           <packet>\n')
	itr = input_ch_model.get_iter_first()
	while itr:
		f.write('                 ')
		f.write('<channel id="%i"' % 
		  input_ch_model.get_value(itr,listcol.INPUTCHANNEL_ID))
		#f.write(' src="%i"' % input_ch_model.get_value(itr,1))
		f.write(' name="%s"' % 
		  input_ch_model.get_value(itr,listcol.INPUTCHANNEL_NAME))
		f.write(' bits="%i"' % 
		  input_ch_model.get_value(itr,listcol.INPUTCHANNEL_BITS) )
		conv_sign = "unsigned"
		if input_ch_model.get_value(itr,listcol.INPUTCHANNEL_SIGN):
			conv_sign = "signed"
		f.write(' sign="%s"' % conv_sign )
		conv_form = "integer"
		f.write(' format="%s">' % conv_form )
		f.write('</channel>\n')
		itr = input_ch_model.iter_next(itr)
	f.write('           </packet>\n')
	f.write('     </%s>\n' % conv_type)
	itr = input_model.get_iter_first()
	while itr:
		f.write('     ')
		f.write('<column id="%i"' % 
		  input_model.get_value(itr,listcol.INPUTCOLUMN_ID))
		f.write(' channel="%i"' % 
		  input_model.get_value(itr,listcol.INPUTCOLUMN_CH))
		f.write(' name="%s"' % 
		  input_model.get_value(itr,listcol.INPUTCOLUMN_NAME))
		f.write(' bits="%i"' % 
		  input_model.get_value(itr,listcol.INPUTCOLUMN_BITS) )
		conv_sign = "unsigned"
		if input_model.get_value(itr,listcol.INPUTCOLUMN_SIGN):
			conv_sign = "signed"
		f.write(' sign="%s"' % conv_sign )
		conv_form = "integer"
		f.write(' format="%s">' % conv_form )
		f.write('</column>\n')
		itr = input_model.iter_next(itr)
	f.write(' </input>\n')
	conv_pars = ""
	if   output_tabs.get_current_page()==1:
		conv_type = "file"
		conv_pars = ' filename="%s"' % output_filename.get_text()
	elif output_tabs.get_current_page()==2:
		conv_type = "udp"
	elif output_tabs.get_current_page()==3:
		conv_type = "rs232"
		conv_pars = ' port="%s"' % 	output_portname.get_text()
		conv_pars += ' baudrate="%i"' % output_baudrate.get_value()
		conv_pars += ' buffsize="%i"' % output_bufsize.get_value()
		conv_pars += ' databits="%i"' % output_databits.get_value()
		conv_pars += ' stopbits="%i"' % output_stopbits.get_value()
		if output_parity.get_active():
			conv_tgle = "yes"
		else:	conv_tgle = "no"
		conv_pars += ' parity="%s"' % conv_tgle
		conv_pars += ' mode="%s"' % output_sermode.entry.get_text()
	elif output_tabs.get_current_page()==4:
		conv_type = "int"
	elif output_tabs.get_current_page()==5:
		conv_type = "pipe"
	else:	conv_type = "none" 
	if conv_type != "none":
		f.write(' <output id="%smode">\n' % conv_type)
		f.write('     <%s%s>\n' % (conv_type, conv_pars))
		f.write('           <packet>\n')
	itr = output_model.get_iter_first()
	while itr:
		f.write('                 ')
		f.write('<channel id="%i"' % 
		  output_model.get_value(itr,listcol.OUTPUTCHANNEL_ID))
		f.write(' src="%i"' % 
		  output_model.get_value(itr,listcol.OUTPUTCHANNEL_SRC))
		f.write(' name="%s"' % 
		  output_model.get_value(itr,listcol.OUTPUTCHANNEL_NAME))
		f.write(' bits="%i"' % 
		  output_model.get_value(itr,listcol.OUTPUTCHANNEL_BITS) )
		conv_sign = "unsigned"
		if output_model.get_value(itr,listcol.OUTPUTCHANNEL_SIGN):
			conv_sign = "signed"
		f.write(' sign="%s"' % conv_sign )
		conv_ctype = "val"
		f.write(' type="%s"' % conv_ctype )
		conv_form = "integer"
		f.write(' format="%s">' % conv_form )
		f.write('</channel>\n')
		itr = output_model.iter_next(itr)
	if conv_type != "none":
		f.write('           </packet>\n')
		f.write('     </%s>\n' % conv_type)
		f.write(' </output>\n')
	# window parameters, plot section
	f.write(' <window xpos="%i" ypos="%i"' % (window.xpos, window.ypos))
	f.write(' width="%i" height="%i"' % (window.width, window.height))
	f.write(' border="%i"' % window.border)
	f.write(' framespersec="%i" skip="%i">\n' % 
					(window.framespersec, window.skip))
	itr = model.get_iter_first()
	while itr:
		ps1 = '     <plot id="%i"' % model.get_value(itr,listcol.PLOTCOLUMN_ID)
		if   model.get_value(itr,listcol.PLOTCOLUMN_TYPE)==1:
			type = "histogram"
		elif model.get_value(itr,listcol.PLOTCOLUMN_TYPE)==2:
			type = "timeseries"
		elif model.get_value(itr,listcol.PLOTCOLUMN_TYPE)==3:
			type = "peakplot"
		elif model.get_value(itr,listcol.PLOTCOLUMN_TYPE)==6:
			type = "textplot"
		ps2 = ' type="%s"' % type
		ps3 = ' src="%i"' % model.get_value(itr,listcol.PLOTCOLUMN_SRC)
		ps4 = ' res="%i"' % model.get_value(itr,listcol.PLOTCOLUMN_RES)
		if  model.get_value(itr,listcol.PLOTCOLUMN_SCALE): 
			scale = "yes"
		else:
			scale = "no"
		ps5 = ' scale="%s"' % scale 
		ps6 = ' title="%s"' % model.get_value(itr,listcol.PLOTCOLUMN_TITLE) 
		ps7 = ' color="%i"></plot>\n' % model.get_value(itr,listcol.PLOTCOLUMN_COLOR)
		f.write('%s%s%s%s%s%s%s' % (ps1,ps2,ps3,ps4,ps5,ps6,ps7))
		itr = model.iter_next(itr)
	f.write(" </window>\n</RTPlotSettings>")
	f.close()

##############################################################################
# display a load file box 
##############################################################################
def load_file(action, widget):
	filew = gtk.FileSelection("Please select a file...")
	filew.complete("*.xml")
	filew.show()
	res = filew.run()
	if   res == gtk.RESPONSE_OK:
		global xsdpath
		global xsdfile 
		xsdfile = filew.selection_entry.get_text()
		xsdpath = filew.get_filename().rstrip(xsdfile)
		input_model.clear()
		input_ch_model.clear()
		output_model.clear()
		model.clear()
		read_xsd(model) # also updates title
	filew.destroy()
	
def load_iofile(action, entry):
	filew = gtk.FileSelection("Please select a file...")
	filew.show()
	res = filew.run()
	if   res == gtk.RESPONSE_OK:
		filename = filew.get_filename()
	filew.destroy()
	entry.set_text(filename)

##############################################################################
# display a save file box 
##############################################################################
def save_file(action, widget):
	filew = gtk.FileSelection("Please select a file...")
	filew.complete("*.xml")
	filew.show()
	res = filew.run()
	if   res == gtk.RESPONSE_OK:
		global xsdpath
		global xsdfile 
		xsdfile = filew.selection_entry.get_text()
		xsdpath = filew.get_filename().rstrip(xsdfile)
		write_xsd(model) # also updates title
	filew.destroy()

##############################################################################
# display the about box 
##############################################################################
def about_box(action, widget):
	info = "RTPLOT GUI v.1.00\nStill Under Construction"
	about = gtk.MessageDialog(parent=main_window, 
				  buttons=gtk.BUTTONS_CLOSE,
				  flags=gtk.DIALOG_DESTROY_WITH_PARENT,
				  type=gtk.MESSAGE_INFO,
				  message_format=info);
	#about.action_area.get_children()[0].connect("clicked",about.destroy)
	about.show()
	res = about.run()
	about.destroy()

##############################################################################
# display the windows options dialog box 
##############################################################################
def window_box(action, widget):
	lbl = "RTPLOT Window parameters:"
	# these should be treated as globals (=quick&dirty programming!)	
	windlg = gtk.MessageDialog(parent=main_window, 
				  buttons=gtk.BUTTONS_OK_CANCEL,
				  flags=gtk.DIALOG_DESTROY_WITH_PARENT,
				  type=gtk.MESSAGE_INFO,
				  message_format=lbl);
	wt = gtk.Table(6,3,0)
	wd_x = d_spin(None, 1, 0, 1, 100,   1, 65536, window.xpos)
	wd_y = d_spin(None, 1, 0, 1, 100,   1, 65536, window.ypos)
	wd_w = d_spin(None, 1, 0, 1, 100, 100, 65536, window.width)
	wd_h = d_spin(None, 1, 0, 1, 100, 100, 65536, window.height)
	wd_b = d_spin(None, 1, 0, 1,   5,   0, 65536, window.border)
	wd_f = d_spin(None, 1, 0, 1,  10,   1, 65536, window.framespersec)
	wd_s = d_spin(None, 1, 0, 1,  10,   0, 65536, window.skip)
	wt.attach(gtk.Label("X pos:"),0,2,0,1,xpadding=4,ypadding=4)
	wt.attach(wd_x,2,3,0,1,xpadding=4,ypadding=4)
	wt.attach(gtk.Label("Y pos:"),3,5,0,1,xpadding=4,ypadding=4)
	wt.attach(wd_y,5,6,0,1,xpadding=4,ypadding=4)
	wt.attach(gtk.Label("Width:"),0,2,1,2,xpadding=4,ypadding=4)
	wt.attach(wd_w,2,3,1,2,xpadding=4,ypadding=4)
	wt.attach(gtk.Label("Height:"),3,5,1,2,xpadding=4,ypadding=4)
	wt.attach(wd_h,5,6,1,2,xpadding=4,ypadding=4)
	wt.attach(gtk.Label("Border:"),0,1,2,3,xpadding=4,ypadding=4)
	wt.attach(wd_b,1,2,2,3,xpadding=4,ypadding=4)
	wt.attach(gtk.Label("FPS:"),2,3,2,3,xpadding=4,ypadding=4)
	wt.attach(wd_f,3,4,2,3,xpadding=4,ypadding=4)
	wt.attach(gtk.Label("Skip:"),4,5,2,3,xpadding=4,ypadding=4)
	wt.attach(wd_s,5,6,2,3,xpadding=4,ypadding=4)
	windlg.vbox.add(wt)
	windlg.show_all()
	res = windlg.run()
	if   res == gtk.RESPONSE_OK:
		window.xpos         = wd_x.get_value()
		window.ypos         = wd_y.get_value()
		window.width        = wd_w.get_value()
		window.height       = wd_h.get_value()
		window.border       = wd_b.get_value()
		window.framespersec = wd_f.get_value()
		window.skip         = wd_s.get_value()
	windlg.destroy()

##############################################################################
# display the input channels list box 
##############################################################################
def ichannel_box(action, widget):
	lbl = "RTPLOT Input Source:"
	# these should be treated as globals (=quick&dirty programming!)
	icdlg = gtk.MessageDialog(parent=main_window, 
				  buttons=gtk.BUTTONS_CLOSE,
				  flags=gtk.DIALOG_DESTROY_WITH_PARENT,
				  type=gtk.MESSAGE_INFO,
				  message_format=lbl);
	icdlg.set_size_request(500,300)
	icdlg.vbox.add(input_ch_list_window)
	icdlg.show_all()
	res = icdlg.run()
	#if   res == gtk.RESPONSE_OK:
	#	window.skip         = wd_s.get_value()
	icdlg.destroy()

##############################################################################
# start rtplot 
##############################################################################
def startrtplot(widget, model):
	write_xsd(model)
	os.system('%srtplot %s%s &' % (toolspath, xsdpath, xsdfile))

##############################################################################
# MAIN WINDOW 
##############################################################################
# create the main window:
main_window = gtk.Window(gtk.WINDOW_TOPLEVEL)
main_window.set_name("CONTEXT")
main_window.set_size_request(640,600)
main_window.set_border_width(3)
main_window.connect("destroy", gtk.mainquit)
main_window.set_title("CSTK RTPlot v.1.00")

# create menu bar:
accel_group = gtk.AccelGroup()
menu_items = (
	("/_File",	None,	None,	0,	"<Branch>"),
	("/File/_Load",	"<control>L",	load_file,	0,	None),
	("/File/_Save",	"<control>S",	save_file,	0,	None),
	("/File/_Quit",	"<control>Q",	gtk.mainquit,	0,	None),
	("/_Options",	None,	None,	0,	"<Branch>"),
	("/Options/_Window...",	"<control>W",	window_box,	0,	None),
	("/Options/_Input Source...",	"<control>I",	ichannel_box,	0,	None),
	("/_Help",	None,	None,	0,	"<Branch>"),
	("/Help/_About",	"<control>A",	about_box,	0,	None)
)
menu_factory = gtk.ItemFactory(gtk.MenuBar, "<main>", accel_group)
menu_factory.create_items(menu_items)
main_window.add_accel_group(accel_group)

##############################################################################
# create frame for the input parameters:
input_frame = gtk.Frame("Input Parameters")
input_frame.set_border_width(4)
input_tabs = gtk.Notebook()
input_tabs.set_tab_pos(gtk.POS_LEFT)
input_tabs.set_border_width(3)
input_page = (gtk.VBox(), gtk.VBox(), gtk.VBox(), 
              gtk.VBox(), gtk.VBox(), gtk.VBox() )
for i in range(6):
	input_tabs.append_page( input_page[i], gtk.Label(inputstrings[i]))

table = ( gtk.Table(2, 5, 1), gtk.Table(1, 5, 1), gtk.Table(3, 7, 1),
          gtk.Table(1, 5, 1), gtk.Table(1, 5, 1), gtk.Table(1, 5, 1) )


 #### Logged Datafile: #####################################################
input_filename   = d_entr("datalog.txt")
input_filemode   = d_comb("annotated ascii", ["annotated ascii","NMEA", "Excel"])
input_filebrowse = gtk.Button("Browse...")
input_filebrowse.set_border_width(4)
input_filebrowse.connect("clicked", load_iofile, input_filename)
table[0].attach(gtk.Label(" Filename: "),0,1,0,1)
table[0].attach(input_filename, 1,4,0,1)
table[0].attach(input_filebrowse, 4,5,0,1)
table[0].attach(gtk.Label(" File Mode: "),0,1,1,2)
table[0].attach(input_filemode, 1,5,1,2)

 #### UDP: #################################################################
input_portnr   = d_spin(None, 1, 0, 1, 10, 1, 9999, 1525)
input_hostname = d_entr("localhost")
input_udpmode  = d_comb("ascii", ["ascii","binary"])
table[1].attach(gtk.Label(" Port: "),0,1,0,1)
table[1].attach(input_portnr,1,2,0,1)
table[1].attach(gtk.Label(" Hostname: "),2,3,0,1)
table[1].attach(input_hostname, 3,5,0,1)
table[1].attach(gtk.Label(" Mode: "),2,3,1,2)
table[1].attach(input_udpmode,3,5,1,2)

 #### Serial Port: #########################################################
input_portname = d_entr('/dev/ttyS0')
input_baudrate = d_spin(None, 1, 0, 1200, 1200, 1200, 115200, 2400)
input_bufsize  = d_spin(None, 1, 0,    1,   10,   16,   1024,  512)
input_databits = d_spin(None, 1, 0,    1,    2,    1,     16,    8)
input_stopbits = d_spin(None, 1, 0,    1,    2,    1,     16,    1)
input_pollstr  = d_entr("")
input_parity   = d_togg("Parity", False)
input_sermode  = d_comb("binary", ["ascii","binary"])
table[2].attach(gtk.Label(" Portname: "),0,1,0,1)
table[2].attach(input_portname,1,2,0,1)
table[2].attach(gtk.Label(" Buffersize: "),2,3,0,1)
table[2].attach(input_bufsize,3,5,0,1)
table[2].attach(gtk.Label(" Poll: "),5,6,0,1)
table[2].attach(input_pollstr,6,7,0,1)
table[2].attach(gtk.Label(" Baudrate: "),0,1,1,2)
table[2].attach(input_baudrate, 1,2,1,2)
table[2].attach(gtk.Label(" Databits: "),2,3,1,2)
table[2].attach(input_databits,3,4,1,2)
table[2].attach(gtk.Label(" Stopbits: "),4,5,1,2)
table[2].attach(input_stopbits,5,6,1,2)
table[2].attach(input_parity,6,7,1,2)
table[2].attach(gtk.Label(" Mode: "),0,1,2,3)
table[2].attach(input_sermode,1,7,2,3)

 #### Internal: ############################################################
input_intproc = d_entr("/dev/dsp")
input_intmode = d_comb("ascii", ["ascii","binary"])
table[3].attach(gtk.Label(" Process: "),0,1,0,1)
table[3].attach(input_intproc,1,2,0,1)
table[3].attach(gtk.Label(" Mode: "),2,3,0,1)
table[3].attach(input_intmode, 3,4,0,1)
  
 #### simulation: ##########################################################
input_simnum = d_spin(None, 1, 0, 1, 10, 1, 100, 3)
table[4].attach( gtk.Label(" Number of Input Channels: "), 0, 2, 0, 1)
table[4].attach( input_simnum, 2, 3, 0, 1)

 #### Other Process: #######################################################
input_pipeproc = d_entr("procname")
input_pipemode = d_comb("ascii", ["ascii","binary"])
table[5].attach(gtk.Label(" Process: "),0,1,0,1)
table[5].attach(input_pipeproc,1,2,0,1)
table[5].attach(gtk.Label(" Mode: "),2,3,0,1)
table[5].attach(input_pipemode, 3,4,0,1)
 
inputlist_window = (gtk.ScrolledWindow(), gtk.ScrolledWindow(), 
                    gtk.ScrolledWindow(), gtk.ScrolledWindow(), 
		    gtk.ScrolledWindow(), gtk.ScrolledWindow())
input_view = (gtk.TreeView(), gtk.TreeView(), gtk.TreeView(), 
              gtk.TreeView(), gtk.TreeView(), gtk.TreeView())
input_model = gtk.TreeStore(gobject.TYPE_BOOLEAN,
			    gobject.TYPE_UINT, gobject.TYPE_UINT, 
			    gobject.TYPE_STRING, gobject.TYPE_UINT, 
			    gobject.TYPE_BOOLEAN, gobject.TYPE_UINT) 

for i in range(6):
	inputlist_window[i].set_border_width(9) 
	inputlist_window[i].set_policy(gtk.POLICY_AUTOMATIC, 
				       gtk.POLICY_AUTOMATIC) 
	inputlist_window[i].set_shadow_type(gtk.SHADOW_ETCHED_IN) 
	input_view[i].set_model(input_model)
	input_view[i].set_rules_hint(True)
	input_view[i].set_search_column(True)
	input_view[i].set_reorderable(True)
	listcol.add_inputcolumns(input_view[i])
	inputlist_window[i].add(input_view[i])
	table[i].set_border_width(4)
	input_page[i].pack_start(table[i],0)
	input_page[i].pack_start(inputlist_window[i])
	selection = input_view[i].get_selection()
	selection.connect("changed", listcol.update_row_number, input_model, 
	                   input_view[i])
	input_view[i].connect("key-press-event", listcol.list_key_filter, 
			   input_model, input_view[i])

# build-up for the input channel window: 
input_ch_view = gtk.TreeView()
input_ch_model = gtk.TreeStore(gobject.TYPE_BOOLEAN, gobject.TYPE_UINT, 
			       gobject.TYPE_UINT, gobject.TYPE_STRING, 
			       gobject.TYPE_UINT, gobject.TYPE_BOOLEAN, 
			       gobject.TYPE_UINT, gobject.TYPE_UINT) 
input_ch_view.set_model(input_ch_model)
input_ch_view.set_rules_hint(True)
input_ch_view.set_search_column(True)
input_ch_view.set_reorderable(True)
listcol.add_inputchannels(input_ch_view)
selection = input_ch_view.get_selection()
selection.connect("changed", listcol.update_row_number, input_ch_model, 
                   input_ch_view)
input_view[i].connect("key-press-event", listcol.list_key_filter, 
		   input_ch_model, input_ch_view)
input_ch_list_window = gtk.ScrolledWindow()
input_ch_list_window.add(input_ch_view)

input_frame.add(input_tabs)

##############################################################################
# create frame for the output parameters:
output_frame = gtk.Frame("Output Parameters")
output_frame.set_border_width(4)
output_tabs = gtk.Notebook()
output_tabs.set_tab_pos(gtk.POS_LEFT)
output_tabs.set_border_width(3)
output_page = (gtk.VBox(), gtk.VBox(), gtk.VBox(), 
              gtk.VBox(), gtk.VBox(), gtk.VBox() )
for i in range(6):
	output_tabs.append_page( output_page[i], gtk.Label(outputstrings[i]))


table = ( gtk.Table(1, 1, 1), gtk.Table(2, 5, 1), gtk.Table(2, 5, 1),
          gtk.Table(3, 5, 1), gtk.Table(1, 4, 1), gtk.Table(1, 4, 1) )

 #### No Output: ###########################################################
table[0].add(gtk.Label("No output"))

 #### Logged Datafile: #####################################################
output_filename   = d_entr("datalog.txt")
output_filemode   = d_comb("annotated ascii", ["annotated ascii","NMEA", "Excel"])
output_filebrowse = gtk.Button("Browse...")
output_filebrowse.set_border_width(4)
output_filebrowse.connect("clicked", load_iofile, output_filename)
table[1].attach(gtk.Label(" Filename: "),0,1,0,1)
table[1].attach(output_filename, 1,4,0,1)
table[1].attach(output_filebrowse, 4,5,0,1)
table[1].attach(gtk.Label(" File Mode: "),0,1,1,2)
table[1].attach(output_filemode, 1,5,1,2)

 #### UDP: #################################################################
output_portnr   = d_spin(None, 1, 0, 1, 10, 1, 9999, 1525)
output_hostname = d_entr("localhost")
output_udpmode  = d_comb("ascii", ["ascii","binary"])
table[2].attach(gtk.Label(" Port: "),0,1,0,1)
table[2].attach(output_portnr,1,2,0,1)
table[2].attach(gtk.Label(" Hostname: "),2,3,0,1)
table[2].attach(output_hostname, 3,5,0,1)
table[2].attach(gtk.Label(" Mode: "),2,3,1,2)
table[2].attach(output_udpmode,3,5,1,2)

 #### Serial Port: #########################################################
output_portname = d_entr('/dev/ttyS0')
output_baudrate = d_spin(None, 1, 0, 1200, 1200, 1200, 115200, 2400)
output_bufsize  = d_spin(None, 1, 0,    1,   10,   16,   1024,  512)
output_databits = d_spin(None, 1, 0,    1,    2,    1,     16,    8)
output_stopbits = d_spin(None, 1, 0,    1,    2,    1,     16,    1)
output_parity   = d_togg("Parity", False)
output_sermode  = d_comb("binary", ["ascii","binary"])
table[3].attach(gtk.Label(" Portname: "),0,1,0,1)
table[3].attach(output_portname,1,2,0,1)
table[3].attach(gtk.Label(" Buffersize: "),2,3,0,1)
table[3].attach(output_bufsize,3,5,0,1)
table[3].attach(gtk.Label(" Baudrate: "),0,1,1,2)
table[3].attach(output_baudrate, 1,2,1,2)
table[3].attach(gtk.Label(" Databits: "),2,3,1,2)
table[3].attach(output_databits,3,4,1,2)
table[3].attach(gtk.Label(" Stopbits: "),4,5,1,2)
table[3].attach(output_stopbits,5,6,1,2)
table[3].attach(output_parity,6,7,1,2)
table[3].attach(gtk.Label(" Mode: "),0,1,2,3)
table[3].attach(output_sermode,1,7,2,3)

 #### Internal: ############################################################
output_intproc = d_entr("/dev/dsp")
output_intmode = d_comb("ascii", ["ascii","binary"])
table[4].attach(gtk.Label(" Process: "),0,1,0,1)
table[4].attach(output_intproc,1,2,0,1)
table[4].attach(gtk.Label(" Mode: "),2,3,0,1)
table[4].attach(output_intmode, 3,4,0,1)
  
 #### Other Process: #######################################################
output_pipeproc = d_entr("procname")
output_pipemode = d_comb("ascii", ["ascii","binary"])
table[5].attach(gtk.Label(" Process: "),0,1,0,1)
table[5].attach(output_pipeproc,1,2,0,1)
table[5].attach(gtk.Label(" Mode: "),2,3,0,1)
table[5].attach(output_pipemode, 3,4,0,1)

outputlist_window = (gtk.ScrolledWindow(), gtk.ScrolledWindow(), 
                     gtk.ScrolledWindow(), gtk.ScrolledWindow(), 
		     gtk.ScrolledWindow())
output_view = (gtk.TreeView(), gtk.TreeView(), gtk.TreeView(), 
               gtk.TreeView(), gtk.TreeView())
output_model = gtk.TreeStore(gobject.TYPE_BOOLEAN,
			gobject.TYPE_UINT, gobject.TYPE_UINT, 
                        gobject.TYPE_STRING, gobject.TYPE_UINT, 
			gobject.TYPE_BOOLEAN, gobject.TYPE_UINT,
			gobject.TYPE_UINT) 
				
for i in range(5):
	outputlist_window[i].set_border_width(9) 
	outputlist_window[i].set_policy(gtk.POLICY_AUTOMATIC, 
					gtk.POLICY_AUTOMATIC) 
	outputlist_window[i].set_shadow_type(gtk.SHADOW_ETCHED_IN) 
	output_view[i].set_model(output_model)
	output_view[i].set_rules_hint(True)
	output_view[i].set_search_column(True)
	output_view[i].set_reorderable(True)
	listcol.add_outputcolumns(output_view[i])
	outputlist_window[i].add(output_view[i])
	selection = output_view[i].get_selection()
	selection.set_mode(gtk.SELECTION_BROWSE)
	selection.connect("changed", listcol.update_row_number, output_model, 
	                        output_view[i])
	output_view[i].connect("key-press-event", listcol.list_key_filter, 
				output_model, output_view)
	
for i in range(6):
	table[i].set_border_width(4)
	output_page[i].pack_start(table[i],0)
	if i>0:
		output_page[i].pack_start(outputlist_window[i-1])

output_tabs.set_current_page(xsd.outp.index('none'))
output_frame.add(output_tabs)

##############################################################################
# create frame for the plot parameters:
plot_frame = gtk.Frame("Plot Parameters")
plot_frame.set_border_width(4)
plotlist_window = gtk.ScrolledWindow()
plotlist_window.set_border_width(3) 
plotlist_window.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC) 
plotlist_window.set_shadow_type(gtk.SHADOW_ETCHED_IN) 
plot_frame.add(plotlist_window)

plot_view = gtk.TreeView()
model = gtk.TreeStore(gobject.TYPE_BOOLEAN, gobject.TYPE_UINT, 
	gobject.TYPE_UINT, gobject.TYPE_UINT, 
	gobject.TYPE_STRING, gobject.TYPE_UINT, 
	gobject.TYPE_BOOLEAN, gobject.TYPE_UINT) 
plot_view.set_model(model)
plot_view.set_rules_hint(True)
plot_view.set_search_column(True)
plot_view.set_reorderable(True)
listcol.add_plotcolumns(plot_view)
plotlist_window.add(plot_view)

selection = plot_view.get_selection()
selection.set_mode(gtk.SELECTION_BROWSE)
selection.connect("changed", listcol.update_row_number, model, plot_view)
plot_view.connect("key-press-event", listcol.list_key_filter, model, plot_view)

# create start button:
start_button = gtk.Button("Start...")
start_button.connect("clicked",startrtplot, model)

# create a box for our main sections:
vbox = gtk.VBox(homogeneous=0, spacing=3)
main_window.add(vbox)
vbox.pack_start(menu_factory.get_widget("<main>"), 0, 1, 0)
v2box = gtk.VPaned()
v2box.add1(input_frame)
v2box.add2(output_frame)
v3box = gtk.VPaned()
v3box.add1(v2box)
v3box.add2(plot_frame)
vbox.pack_start(v3box, expand=1, fill=1, padding=0)
vbox.pack_start(start_button, 0, 0, 0)

main_window.show_all()

if len(sys.argv)>2:
	xsdpath = sys.argv[1]
	xsdfile = sys.argv[2]
	read_xsd(model)
elif len(sys.argv)>1:
	xsdpath = ""
	xsdfile = sys.argv[1]
	read_xsd(model)
else:	load_file(0,0)
	
gtk.mainloop()
