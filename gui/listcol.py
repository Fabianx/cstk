#***************************************************************************
#                            listcol.py - v.1.00
#                           ---------------------
#    begin                : Jul 19 2004
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
#     Functions for setting up the columns for all lists
#

import gtk

####### Input Chs ########
INPUTCHANNEL_ID    = 1
INPUTCHANNEL_SRC   = 2
INPUTCHANNEL_NAME  = 3
INPUTCHANNEL_BITS  = 4
INPUTCHANNEL_SIGN  = 5
INPUTCHANNEL_TYPE  = 6
INPUTCHANNEL_FORM  = 7
INPUTCHANNEL_EDIT  = 0
##########################

####### Input Cols #######
INPUTCOLUMN_ID    = 1
INPUTCOLUMN_CH    = 2
INPUTCOLUMN_NAME  = 3
INPUTCOLUMN_BITS  = 4
INPUTCOLUMN_SIGN  = 5
INPUTCOLUMN_FORM  = 6
INPUTCOLUMN_EDIT  = 0
##########################

####### Output Chs #######
OUTPUTCHANNEL_ID    = 1
OUTPUTCHANNEL_SRC   = 2
OUTPUTCHANNEL_NAME  = 3
OUTPUTCHANNEL_BITS  = 4
OUTPUTCHANNEL_SIGN  = 5
OUTPUTCHANNEL_TYPE  = 6
OUTPUTCHANNEL_FORM  = 7
OUTPUTCHANNEL_EDIT  = 0
##########################

######## Plot Cols #######
PLOTCOLUMN_ID    = 1
PLOTCOLUMN_SRC   = 2
PLOTCOLUMN_TYPE  = 3
PLOTCOLUMN_TITLE = 4
PLOTCOLUMN_RES   = 5
PLOTCOLUMN_SCALE = 6
PLOTCOLUMN_COLOR = 7
PLOTCOLUMN_EDIT  = 0
##########################

###### column types: #####
NUM = 1
TXT = 2
TOG = 3
##########################


##############################################################################
# generic edit functions 
##############################################################################
	
def edit_num(rend, path, new_text, model, col):
	itr = model.get_iter_from_string(path)
	model.set_value(itr, col, int(new_text)) 
	
def edit_txt(rend, path, new_text, model, col):
	itr = model.get_iter_from_string(path)
	model.set_value(itr, col, new_text) 
	
def edit_tog(cell, path, model, col):
	itr = model.get_iter_from_string(path)
	model.set(itr, col, not model.get_value(itr, col)) 
	
def update_row_number(treesel, modl, vw):
	#global glob_sel 
	global glob_rows
	#glob_sel = vw.get_selection()
	glob_rows = []
	treesel.selected_foreach(lambda modl, path, iter: glob_rows.append(path))
	
def list_key_filter(widget, event, modl, vw):
	global selection 
	global glob_rows
	if glob_rows:
		itr = modl.get_iter(glob_rows[0])
	if   event.keyval == gtk.gdk.keyval_from_name('Delete'):
		if itr:
			path = modl.get_path(itr)
			modl.remove(itr)
			#selection.select_path(path)
			#if not glob_sel.path_is_selected(path):
		#		row = path[0]-1
		#		if row >= 0:
		#			glob_sel.select_path((row,))	
	elif event.keyval == gtk.gdk.keyval_from_name('Insert'):
		if itr:
			itr = modl.append(None)
			modl.set_value(itr,0,True)
			#path = modl.get_path(itr)
			#glob_sel.select_iter(itr)
			#glob_sel.select_path(path)
			#glob_sel = vw.get_selection()
	
##############################################################################
# create and add the view columns to the respective sections: 
##############################################################################
def prep_col(ctype, title, col_id, edit, view):
	if ctype in (TXT,NUM):
		rend = gtk.CellRendererText()
		column = gtk.TreeViewColumn(title, rend, text=col_id, 
	                            editable=edit)
	elif ctype==TOG:
		rend = gtk.CellRendererToggle()
		column = gtk.TreeViewColumn(title, rend, active=col_id, 
	                            activatable=edit)
	column.set_clickable(True)
	column.set_resizable(True)
	view.append_column(column)
	model = view.get_model()
	if ctype==NUM:
		rend.connect("edited", edit_num, model, col_id)
	elif ctype==TXT:
		rend.connect("edited", edit_txt, model, col_id)
	elif ctype==TOG:
		rend.connect("toggled", edit_tog, model, col_id)

def add_inputchannels(view):
	prep_col(NUM, "ID",     INPUTCHANNEL_ID,   INPUTCHANNEL_EDIT, view)
	prep_col(NUM, "Source", INPUTCHANNEL_SRC,  INPUTCHANNEL_EDIT, view)
	prep_col(TXT, "Name",   INPUTCHANNEL_NAME, INPUTCHANNEL_EDIT, view)
	prep_col(NUM, "Bits",   INPUTCHANNEL_BITS, INPUTCHANNEL_EDIT, view)
	prep_col(TOG, "Sign",   INPUTCHANNEL_SIGN, INPUTCHANNEL_EDIT, view)
	prep_col(NUM, "Type",   INPUTCHANNEL_TYPE, INPUTCHANNEL_EDIT, view)
	prep_col(NUM, "Format", INPUTCHANNEL_FORM, INPUTCHANNEL_EDIT, view)

def add_inputcolumns(view):
	prep_col(NUM, "ID",      INPUTCOLUMN_ID,   INPUTCOLUMN_EDIT, view)
	prep_col(NUM, "Channel", INPUTCOLUMN_CH,   INPUTCOLUMN_EDIT, view)
	prep_col(TXT, "Name",    INPUTCOLUMN_NAME, INPUTCOLUMN_EDIT, view)
	prep_col(NUM, "Bits",    INPUTCOLUMN_BITS, INPUTCOLUMN_EDIT, view)
	prep_col(TOG, "Sign",    INPUTCOLUMN_SIGN, INPUTCOLUMN_EDIT, view)
	prep_col(NUM, "Format",  INPUTCOLUMN_FORM, INPUTCOLUMN_EDIT, view)

def add_outputcolumns(view):
	prep_col(NUM, "ID",     OUTPUTCHANNEL_ID,   OUTPUTCHANNEL_EDIT, view)
	prep_col(NUM, "Source", OUTPUTCHANNEL_SRC,  OUTPUTCHANNEL_EDIT, view)
	prep_col(TXT, "Name",   OUTPUTCHANNEL_NAME, OUTPUTCHANNEL_EDIT, view)
	prep_col(NUM, "Bits",   OUTPUTCHANNEL_BITS, OUTPUTCHANNEL_EDIT, view)
	prep_col(TOG, "Sign",   OUTPUTCHANNEL_SIGN, OUTPUTCHANNEL_EDIT, view)
	prep_col(NUM, "Type",   OUTPUTCHANNEL_TYPE, OUTPUTCHANNEL_EDIT, view)
	prep_col(NUM, "Format", OUTPUTCHANNEL_FORM, OUTPUTCHANNEL_EDIT, view)
	 
def add_plotcolumns(view):
	prep_col(NUM, "ID",         PLOTCOLUMN_ID,    PLOTCOLUMN_EDIT, view)
	prep_col(NUM, "Source",     PLOTCOLUMN_SRC,   PLOTCOLUMN_EDIT, view)
	prep_col(NUM, "Type",       PLOTCOLUMN_TYPE,  PLOTCOLUMN_EDIT, view)
	prep_col(TXT, "Title",      PLOTCOLUMN_TITLE, PLOTCOLUMN_EDIT, view)
	prep_col(NUM, "Resolution", PLOTCOLUMN_RES,   PLOTCOLUMN_EDIT, view)
	prep_col(TOG, "Scale",      PLOTCOLUMN_SCALE, PLOTCOLUMN_EDIT, view)
	prep_col(NUM, "Color",      PLOTCOLUMN_COLOR, PLOTCOLUMN_EDIT, view)

		