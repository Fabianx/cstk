
/* *******************************************************************************************
	Init functions
   *******************************************************************************************/

function init()
{
	// should be replaced with user pref:
	document.getElementById('xmlfile').value = 
		"/home/kristof/Documents/Sources/cstk/cstk_settings/smartit_demo.xml";
	document.getElementById('rdffile').value = 
	//	"file:///usr/lib/mozilla-1.6/chrome/cstkedit/content/cstkedit/cstkxml.rdf";
		"chrome://cstkedit/content/cstkxml.rdf";
	var file = Components.classes["@mozilla.org/file/local;1"].
				createInstance(Components.interfaces.nsILocalFile);
	file.initWithPath(document.getElementById('xmlfile').value);
	loadFile(file);
}

function init_par()
{
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
		var fstream = Components.classes["@mozilla.org/network/file-input-stream;1"]
			.createInstance(Components.interfaces.nsIFileInputStream);
		var sstream = Components.classes["@mozilla.org/scriptableinputstream;1"]
			.createInstance(Components.interfaces.nsIScriptableInputStream);
		fstream.init(file, 1, 0, false);
		sstream.init(fstream); 
		var data = sstream.read(file.fileSize);
		sstream.close();
		fstream.close();
		if (data=="")
			alert("File is empty!");
		else {
			updateSource(file, data);   // update source code in editor
			doParseXML(data);           // parse the xml file 
		}
	} catch(e) { alert(e); }
}

function doParseXML(data)
{
	var domParser=Components.classes["@mozilla.org/xmlextras/domparser;1"]
			.createInstance(Components.interfaces.nsIDOMParser); 
	var xmldoc = domParser.parseFromString(data, "text/xml");

	var imode = window.frames['inputt'].document.getElementById('device');
	imode.removeItemAt(6); // last
	imode.appendItem(xmldoc.getElementsByTagName("input").item(0).attributes.item(0).nodeValue,-1);
	imode.selectedIndex=6; // last

	if ( xmldoc.getElementsByTagName("rs232").length > 0 )
	if ( xmldoc.getElementsByTagName("rs232").item(0).parentNode.nodeName =="input")
	{
		parseAtts(xmldoc.getElementsByTagName("rs232"),'inputt','iparams',
					"http://cstk.sf.net/iparams#","rs232", true);
		parseAtts(xmldoc.getElementsByTagName("poll"),'inputt','iparams',
					"http://cstk.sf.net/iparams#","poll", false);
	}

	//parseAtts(xmldoc.getElementsByTagName("logfile"),'outputt','oparams',
	//		"http://cstk.sf.net/oparams#","logfile");

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

function unassert_all(ds)
{
	dsResources = (ds.GetAllResources());
	var thisResource = null;
	var arcCursor = null;
	var thisArc = null;
	while(dsResources.hasMoreElements()) {
		thisResource = 
		dsResources.getNext().QueryInterface(Components.interfaces.nsIRDFResource);
		arcCursor = ds.ArcLabelsOut(thisResource);
		while(arcCursor.hasMoreElements()) {
			thisArc = arcCursor.getNext().QueryInterface(Components.interfaces.nsIRDFResource);
			arcTargets = ds.GetTargets( thisResource, thisArc, true );
			while(arcTargets.hasMoreElements()) {
				tmparc = arcTargets.getNext();
				if (tmparc instanceof Components.interfaces.nsIRDFLiteral){
				 thisTarget = tmparc.QueryInterface(Components.interfaces.nsIRDFLiteral);
				 ds.Unassert(thisResource,thisArc,thisTarget,true);
				}
			}
		}
	}
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
	tree.database.AddDataSource(datasource);

	var ds = tree.database.GetDataSources();
	ds = (ds.getNext(),ds.getNext());
	ds = ds.QueryInterface(Components.interfaces.nsIRDFDataSource);

	var sub, pred, obj;

	if (unassert_bool) unassert_all(ds);
	
	obj  = rdf.GetResource( schema+"node-"+rootname );
	ds.Assert(root, child, obj, true);
	alert("asserted"+rootname);
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
	tree.database.AddDataSource(datasource);

	var ds = tree.database.GetDataSources();
	ds = (ds.getNext(),ds.getNext());
	ds = ds.QueryInterface(Components.interfaces.nsIRDFDataSource);

	var sub, pred, obj;

	// first unassert all: (there should be an easier way!)
	unassert_all(ds);
	

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

