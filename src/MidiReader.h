#define _BOOL


#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
#include <string.h>

using namespace std;

#include <maya/MPxFileTranslator.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>

#include "jdkmidi/world.h"
#include "jdkmidi/track.h"
#include "jdkmidi/multitrack.h"
#include "jdkmidi/filereadmultitrack.h"
#include "jdkmidi/fileread.h"
#include "jdkmidi/fileshow.h"
#include "jdkmidi/sequencer.h"

#include "PeelMidiEvents.h"

using namespace jdkmidi;


class peelMidiImporter : public MPxFileTranslator  
{ 
public: 
						peelMidiImporter(); 
						~peelMidiImporter(); 
	MStatus				reader (const MFileObject& file, const MString& optionsString, MPxFileTranslator::FileAccessMode mode); 
	bool				haveWriteMethod () const;
	bool				haveReadMethod () const;
	bool				canBeOpened () const;
	MString				defaultExtension () { return MString("mid"); }
	static void*        creator();
	MFileKind           identifyFile (const MFileObject &file, const char *buf, short size) const;
	FileAccessMode      fileAccessMode ()  const;
	bool				haveNamespaceSupport() const;
	bool				haveReferenceMethod() const;
	MString				filter() const;
}; 