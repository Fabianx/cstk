
/* *******************************************************************************************
	Init functions
   *******************************************************************************************/

function init()
{
	// should be replaced with user pref:
	//var file = Components.classes["@mozilla.org/file/local;1"].
	//			createInstance(Components.interfaces.nsILocalFile);
	//file.initWithPath(document.getElementById('xmlfile').value);
	// hmmmmmmmmmm...
	doLoad();
}

function init_par()
{
	// hmmmmmmmmmmm...
	document.getElementById('toolfile').value =
	    "/home/kristof/Documents/Sources/cstk/sourcecode/tools/rtplot/rtplot";
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
// 		rdf_mainfile = window.parent.document.getElementById( 'rdffile' ).value;
// 		ds = refreshRDF(rdf_mainfile);
// 		rebuildFromRDF('',tree_id,ds);
	} catch (e) { alert(e); }
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
	} catch (e) { alert("could not save to file!\n"+
			"(check the file's path and name in the toolbar)"); }
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
		//defaultdir.initWithPath(document.getElementById('xmlfile').value);
		try { fp.displayDirectory = defaultdir.parent; } catch(ex) {;}
		fp.appendFilters(nsIFilePicker.filterXML | nsIFilePicker.filterAll);
		// shows url: fp.fileURL.spec = document.getElementById('xmlfile').value;
		ret = fp.show();
		if ( ret != nsIFilePicker.returnCancel) {
			document.getElementById('xmlfile').value = fp.file.path;
			saveFile(fp.file);
		}
	} catch (e) { alert("could not save to file!\n"+
			"(check the file's path and name)"); }
}

function doLoad()
{
	var data;
	// message box for choosing local or on-line file
	var promptService = Components.classes["@mozilla.org/embedcomp/prompt-service;1"]
			.getService(Components.interfaces.nsIPromptService);
	var flags=promptService.BUTTON_TITLE_IS_STRING * promptService.BUTTON_POS_0 +
		promptService.BUTTON_TITLE_IS_STRING * promptService.BUTTON_POS_1+
		promptService.BUTTON_TITLE_IS_STRING * promptService.BUTTON_POS_2;
	var ret = promptService.confirmEx(window,"Load XML File",
		"Load a local file, or one from the on-line repository?",
		flags, "Local", "On-line", "Cancel", null, {});
	if (ret == 0) // local file:
	{
	 try {
		const nsIFilePicker = Components.interfaces.nsIFilePicker;
		var fp = Components.classes["@mozilla.org/filepicker;1"].createInstance(
									nsIFilePicker);
		fp.init(window, "Select a File", nsIFilePicker.modeOpen);
		var defaultdir = Components.classes["@mozilla.org/file/local;1"].
				createInstance(Components.interfaces.nsILocalFile);
		try { fp.displayDirectory = defaultdir.parent; } catch(ex) {;}
		fp.appendFilters(nsIFilePicker.filterXML | nsIFilePicker.filterAll);
		if (fp.show() != nsIFilePicker.returnCancel)
			if(fp.file.exists()) 
			{
				document.getElementById('xmlfile').value = fp.file.path;
				var fstream = Components.classes["@mozilla.org/network/file-input-stream;1"]
					.createInstance(Components.interfaces.nsIFileInputStream);
				var sstream = Components.classes["@mozilla.org/scriptableinputstream;1"]
					.createInstance(Components.interfaces.nsIScriptableInputStream);
				fstream.init(fp.file, 1, 0, false);
				sstream.init(fstream); 
				data = sstream.read(fp.file.fileSize);
				sstream.close();
				fstream.close();
			}
	 } catch (e) { alert(e); }
	}
	else if(ret == 1) // on-line file:
	{
	 try {
		var array = ["xbow ADXL202EB","DIY smart-it v.2.05", "Mica mote v.1.02", 
			"simulation demo", "XBridge listener", "Logfile 40 acc"];
		var fp = {};
		if (promptService.select( window, "select file" , "select file:" , 6 , array, fp))
		{
			if (fp.value==0)	fp = 'http://cstk.sf.net/set/xbow.xml'; 
			else if (fp.value==1)   fp = 'http://cstk.sf.net/set/smartit_demo.xml';
			document.getElementById('xmlfile').value = fp;
			req = new XMLHttpRequest();
			req.open('GET', fp, false); 
			req.send(null);
			if(req.status == 200)
				data = req.responseText;
		}
	 } catch(e) { alert(e); }
	}
	else return;
	document.getElementById('sourcecode').value=data;
	doParseXML(data);           // parse the xml file 
}

function doParseXML(data)
{
	var domParser=Components.classes["@mozilla.org/xmlextras/domparser;1"]
			.createInstance(Components.interfaces.nsIDOMParser); 
	var xmldoc = domParser.parseFromString(data, "text/xml");

	var imode = window.frames['inputt'].document.getElementById('device');
	imode.removeItemAt(6); // last
	var input_id = xmldoc.getElementsByTagName("input");
	if (input_id.length > 0)
		imode.appendItem(input_id.item(0).attributes.item(0).nodeValue,-1);
	imode.selectedIndex=6; // last

	if ( xmldoc.getElementsByTagName("rs232").length > 0 )
	if ( xmldoc.getElementsByTagName("rs232").item(0).parentNode.nodeName =="input")
	{
		parseAtts(xmldoc.getElementsByTagName("rs232"),'inputt','iparams',
					"http://cstk.sf.net/iparams#","rs232", true);
		parseAtts(xmldoc.getElementsByTagName("poll"),'inputt','iparams',
					"http://cstk.sf.net/iparams#","poll", false);
	}
	if ( xmldoc.getElementsByTagName("sim").length > 0 )
	if ( xmldoc.getElementsByTagName("sim").item(0).parentNode.nodeName =="input")
		parseAtts(xmldoc.getElementsByTagName("sim"),'inputt','iparams',
					"http://cstk.sf.net/iparams#","sim", true);
	if ( xmldoc.getElementsByTagName("logfile").length > 0 )
	if ( xmldoc.getElementsByTagName("logfile").item(0).parentNode.nodeName =="input")
		parseAtts(xmldoc.getElementsByTagName("logfile"),'inputt','iparams',
					"http://cstk.sf.net/iparams#","logfile", true);
	if ( xmldoc.getElementsByTagName("udp").length > 0 )
	if ( xmldoc.getElementsByTagName("udp").item(0).parentNode.nodeName =="input")
		parseAtts(xmldoc.getElementsByTagName("udp"),'inputt','iparams',
					"http://cstk.sf.net/iparams#","udp", true);

	parseAtts(xmldoc.getElementsByTagName("window"),'windowt','wparams',
				"http://cstk.sf.net/wparams#", "window", true);

	ch_cols = ["id", "name", "bits", "sign", "format"];
	ch_xmlcols = ["id", "name", "bits", "sign", "format"];
	parseCols(xmldoc.getElementsByTagName("channel"),'inputt','ichannels',
			"http://cstk.sf.net/ichannels#", ch_cols, ch_xmlcols);
	
	icol_cols = ["id", "ch", "name", "bits", "sign", "format"];
	icol_xmlcols = ["id", "channel", "name", "bits", "sign", "format"];
	parseCols(xmldoc.getElementsByTagName("inputcolumn"),'inputt','icolumns',
			"http://cstk.sf.net/icolumns#", icol_cols, icol_xmlcols);
	
	plot_cols = ["id", "src", "type", "title", "res", "scaled", "color"];
	plot_xmlcols = ["id", "src", "type", "title", "res", "scale", "color"];
	parseCols(xmldoc.getElementsByTagName("plot"),'windowt','plots',
			"http://cstk.sf.net/plots#", plot_cols, plot_xmlcols);
}

function parseAtts(tag, tab_id, tree_id, schema, rootname, unassert_bool)
{
	var rdf  = Components.classes["@mozilla.org/rdf/rdf-service;1"].getService(
						Components.interfaces.nsIRDFService);
	var root = rdf.GetResource(schema + "list");
	var child= rdf.GetResource(schema + "child");

	var tree = window.frames[tab_id].document.getElementById(tree_id);

	var datasource = Components.classes["@mozilla.org/rdf/datasource;1?name=in-memory-datasource"].
				createInstance(Components.interfaces.nsIRDFInMemoryDataSource);
	var ds;

	if (unassert_bool) {
		// remove previous datasources:
		ds = tree.database.GetDataSources();
		while (ds.hasMoreElements())
			tree.database.RemoveDataSource(ds.getNext());
	}

	tree.database.AddDataSource(datasource);
	ds = tree.database.GetDataSources();
	ds = ds.getNext();
	ds = ds.QueryInterface(Components.interfaces.nsIRDFDataSource);

	var sub, pred, obj;

	obj  = rdf.GetResource( schema+"node-"+rootname );
	ds.Assert(root, child, obj, true);
	sub  = obj;
	ds.Assert(sub, rdf.GetResource(schema+"att"), rdf.GetLiteral(rootname), true);
 
	if (tag.length>0)
	{
		for (var i=0; i<tag.item(0).attributes.length; i++) 
		{
			sub  = rdf.GetResource(schema+"node-"+rootname);
			pred = child;
			obj  = rdf.GetResource( schema+"node-"+rootname+"child"+i );
			ds.Assert(sub, pred, obj, true);		
			sub  = obj;
			pred = rdf.GetResource(schema+"att");
			obj  = rdf.GetLiteral(tag.item(0).attributes.item(i).nodeName);
			ds.Assert(sub, pred, obj, true);
			pred = rdf.GetResource(schema+"val");
			obj  = rdf.GetLiteral(tag.item(0).attributes.item(i).nodeValue);
			ds.Assert(sub, pred, obj, true);
		}
	}

	tree.builder.rebuild();
}

function parseCols(tag, tab_id, tree_id, schema, cols, xmlcols)
{
	var rdf  = Components.classes["@mozilla.org/rdf/rdf-service;1"].getService(
						Components.interfaces.nsIRDFService);
	var root = rdf.GetResource(schema + "list");
	var child= rdf.GetResource(schema + "child");

	var tree = window.frames[tab_id].document.getElementById(tree_id);
	
	var datasource = Components.classes["@mozilla.org/rdf/datasource;1?name=in-memory-datasource"].
				createInstance(Components.interfaces.nsIRDFInMemoryDataSource);
	var ds;

	// remove previous datasources:
	ds = tree.database.GetDataSources();
	while (ds.hasMoreElements())
		tree.database.RemoveDataSource(ds.getNext());

	tree.database.AddDataSource(datasource);
	ds = tree.database.GetDataSources();
	ds = ds.getNext();
	ds = ds.QueryInterface(Components.interfaces.nsIRDFDataSource);

	var sub, pred, obj;

	for (var j=0; j<tag.length; j++)
	{
		obj = rdf.GetResource( schema+"node-"+j );
		ds.Assert(root, child, obj, true);
		sub = obj;
		for (var i=0; i<tag.item(j).attributes.length; i++) 
		{
			var nodename = tag.item(j).attributes.item(i).nodeName;
			for (var z=0; z<cols.length; z++)
				if (nodename == xmlcols[z])
					pred = rdf.GetResource(schema+cols[z]);
			obj = rdf.GetLiteral(tag.item(j).attributes.item(i).nodeValue);
			ds.Assert(sub, pred, obj, true);
		}
	}

	tree.builder.rebuild();
}

function saveFile(file)
{
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

function doRun()
{
	// TODO: figure out whether the file is local, create temp if necessary
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

