
var tool_mainfile = "/home/kristof/Documents/Sources/cstk/sourcecode/tools/rtplot/rtplot";

/* *******************************************************************************************
	Init functions
   *******************************************************************************************/

function init()
{
	// should be replaced with user pref:
	document.getElementById('xmlfile').value = 
		"/home/kristof/Documents/Sources/cstk/cstk_settings/xbow.xml";
	document.getElementById('rdffile').value = 
	//	"file:///usr/lib/mozilla-1.6/chrome/cstkedit/content/cstkedit/cstkxml.rdf";
		"chrome://cstkedit/content/cstkxml.rdf";
	var file = Components.classes["@mozilla.org/file/local;1"].
				createInstance(Components.interfaces.nsILocalFile);
	file.initWithPath(document.getElementById('xmlfile').value);
	loadFile(file);
}

function init_in()
{
}

function init_out()
{
}

function init_win()
{
}

function init_par()
{
	document.getElementById('toolfile').value = tool_mainfile;
}

/* *******************************************************************************************
	RDF reading / writing functions
   *******************************************************************************************/

function rebuildFromRDF(frame_id, item_id, datasource)
{
        try {
		var tree;
		if (frame_id=='')
			tree = document.getElementById(item_id);
		else 
			tree = frames[frame_id].document.getElementById(item_id);
		tree.database.AddDataSource(datasource);
		tree.builder.rebuild();
	} catch(e) { alert(e); }
}

function refreshRDF(filename)
{
	try{
		const RDF = Components.classes["@mozilla.org/rdf/rdf-service;1"].getService(
						Components.interfaces.nsIRDFService);
		var datasource = RDF.GetDataSource(filename);
		datasource.QueryInterface(Components.interfaces.nsIRDFRemoteDataSource
					).Refresh(false);
		return datasource;
	} catch (e) { alert(e); }
}

/* *******************************************************************************************
	Tree/List functions
   *******************************************************************************************/

function tremove(tree_id)
{
	try {
		var tree_id = tree_id?tree_id:"tree";
		var tree = document.getElementById(tree_id);
		item = tree.view.getItemAtIndex(tree.view.selection.currentIndex); 
		item.parentNode.removeChild(item);
	} catch(e) { alert(e); }
}

function tremove_all(tree_id)
{
	try {
		var tree_id = tree_id?tree_id:"tree";
		var tree = document.getElementById(tree_id);
		//while (tree.hasChildNodes()) 	tree.removeChild(tree.lastChild); 
		while (tree.view.rowCount) {	
			item = tree.view.getItemAtIndex(0); 
			item.parentNode.removeChild(item);
		}
	} catch(e) { alert(e); }
}

function tappend(tree_id)
{
	try {
		var tree_id = tree_id?tree_id:"tree";
		var tree = document.getElementById(tree_id);
		var item = document.createElement('treeitem');
		it = tree.view.getItemAtIndex(0);
		it.parentNode.appendChild(item); 
	} catch(e) { alert(e); }
}

function tedit(tree_id)
{
  tree_id = tree_id?tree_id:"tree";
  var tree = document.getElementById(tree_id);
}

function trefresh(tree_id)
{
	try{	// rdfs are always local:
		rdf_mainfile = window.parent.document.getElementById( 'rdffile' ).value;
		ds = refreshRDF(rdf_mainfile);
		rebuildFromRDF('',tree_id,ds);
	} catch (e) { alert(e); }
}

function lremove(list_id)
{
  list_id = list_id?list_id:"list";
  var listbox=document.getElementById(list_id);
  listbox.removeItemAt(2+document.getElementById(list_id).selectedIndex);
}

function lremove_all(list_id)
{
  list_id = list_id?list_id:"list";
  var listbox=document.getElementById(list_id);
  while (listbox.getRowCount()>0) {
  	listbox.removeItemAt(1);
  }
}

function lappend(list_id, num_cols)
{
  list_id = list_id?list_id:"list";
  var listbox=document.getElementById(list_id);
  var new_id = listbox.getRowCount();
  var i;
  item=listbox.appendItem(new_id);
  for (i = 0; i < num_cols-1; i++)
  	item.appendChild(document.createElement('listcell')).setAttribute('label', ' ');
  listbox.selectItem(item);
  listbox.ensureElementIsVisible(item);
}

function ledit(list_id, num_cols)
{
  list_id = list_id?list_id:"list";
  var listbox=document.getElementById(list_id);
  var i;
  item=document.getElementById(list_id).selectedIndex;
  for (i = 0; i < num_cols-1; i++)
  	item.appendChild(document.createElement('listcell')).setAttribute('label', ' ');
}

/* *******************************************************************************************
	Main UI buttons
   *******************************************************************************************/

function closeWindow()
{
	window.close();
}

function doSave()
{
	try {
		var file = Components.classes["@mozilla.org/file/local;1"].
				createInstance(Components.interfaces.nsILocalFile);
		file.initWithPath(document.getElementById('xmlfile').value);
		saveFile(file);
	} catch (e) { alert(e); }
}

function doSaveAs()
{
	try {
		const nsIFilePicker = Components.interfaces.nsIFilePicker;
		var fp = Components.classes["@mozilla.org/filepicker;1"].createInstance(
									nsIFilePicker);
		fp.init(window, "Select a File", nsIFilePicker.modeSave);
		var defaultdir = Components.classes["@mozilla.org/file/local;1"].
				createInstance(Components.interfaces.nsILocalFile);
		defaultdir.initWithPath(document.getElementById('xmlfile').value);
		try { fp.displayDirectory = defaultdir.parent; } catch(ex) {;}
		fp.appendFilters(nsIFilePicker.filterXML | nsIFilePicker.filterAll);
		// shows url: fp.fileURL.spec = document.getElementById('xmlfile').value;
		ret = fp.show();
		if ( ret != nsIFilePicker.returnCancel) {
			document.getElementById('xmlfile').value = fp.file.path;
			saveFile(fp.file);
		} 
	} catch (e) { alert(e); }
}

function doLoad()
{
	try {
		const nsIFilePicker = Components.interfaces.nsIFilePicker;
		var fp = Components.classes["@mozilla.org/filepicker;1"].createInstance(
									nsIFilePicker);
		fp.init(window, "Select a File", nsIFilePicker.modeOpen);
		var defaultdir = Components.classes["@mozilla.org/file/local;1"].
				createInstance(Components.interfaces.nsILocalFile);
		defaultdir.initWithPath(document.getElementById('xmlfile').value);
		try { fp.displayDirectory = defaultdir.parent; } catch(ex) {;}
		fp.appendFilters(nsIFilePicker.filterXML | nsIFilePicker.filterAll);
		if (fp.show() != nsIFilePicker.returnCancel) 
			loadFile(fp.file);
	} catch (e) { alert(e); }
}

function updateSource(file, data)
{
	//editor: document.getElementById('sourcecode').setAttribute("src",'file://'+file.path);
	document.getElementById('sourcecode').value=data;
}

function loadFile(file)
{
	if(file.exists()) 
	try {
		document.getElementById('xmlfile').value = file.path;
		var data = "";
		var fstream = Components.classes["@mozilla.org/network/file-input-stream;1"]
			.createInstance(Components.interfaces.nsIFileInputStream);
		var sstream = Components.classes["@mozilla.org/scriptableinputstream;1"]
			.createInstance(Components.interfaces.nsIScriptableInputStream);
		fstream.init(file, 1, 0, false);
		sstream.init(fstream); 
		data += sstream.read(-1);
		sstream.close();
		fstream.close();
		if (data=="")
			alert("File is empty!");
		else {
			updateSource(file, data);   // update source code in editor
			doParseXML(data);           // parse the xml file 
		}
		doRefresh(); // refresh trees and lists in the UI 
	} catch(e) { alert(e); }
}

function doParseXML(data)
{
	// parse parse parse
}

function saveFile(file)
{
	if(file.exists()) 
	try {
		var foStream = Components.classes["@mozilla.org/network/file-output-stream;1"]
			.createInstance(Components.interfaces.nsIFileOutputStream);
		// use 0x02 | 0x10 to open file for appending.
		foStream.init(file, 0x02 | 0x08 | 0x20, 0664, 0); // write, create, truncate
		data = document.getElementById('sourcecode').value ;
		foStream.write(data, data.length);
		foStream.close();
        } catch (e) { alert(e); } 
}

function doRefresh()
{
	try {
		rdf_mainfile = document.getElementById('rdffile').value;
		ds = refreshRDF(rdf_mainfile);
		rebuildFromRDF('inputt' ,'iparams',  ds); 
		rebuildFromRDF('inputt' ,'ichannels',ds); 
		rebuildFromRDF('inputt' ,'icolumns', ds);
		rebuildFromRDF('outputt','oparams',  ds); 
		rebuildFromRDF('outputt','ochannels',ds); 
		rebuildFromRDF('outputt','ocolumns', ds); 
		rebuildFromRDF('windowt','wparams',  ds); 
		rebuildFromRDF('windowt','plots',    ds);
		rebuildFromRDF('paramst','params',   ds);
	} catch (e) { alert(e); }
}

function doRun()
{
	try {
		var str_LocalProgram = window.frames['paramst'].document.getElementById('toolfile').value;
		const FileFactory = new Components.Constructor(
					"@mozilla.org/file/local;1","nsILocalFile","initWithPath");
		var obj_Program = new FileFactory(str_LocalProgram);
		var obj_Process = Components.classes["@mozilla.org/process/util;1"
				].createInstance(Components.interfaces.nsIProcess);
		obj_Process.init(obj_Program);
		var args = [document.getElementById('xmlfile').value];
		obj_Process.run(false, args, args.length, {});
	} catch (e) { alert(e); }
}

function getToolFile()
{
	try {
		tool_mainfile = document.getElementById('toolfile').value;
	} catch (e) { alert(e); }
}

