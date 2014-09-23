#define _BOOL

#include <maya/MPxFileTranslator.h>
#include <maya/MFStream.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>
#include <maya/MFnAnimCurve.h>

#include <string>

#include "MidiFileIn.h"


class midiImport : public MPxFileTranslator {
public:
	midiImport ();
	virtual ~midiImport (); 

	MStatus				doIt (const MArgList &argList);
	static void *		creator ();
	virtual MStatus		reader(	const MFileObject& file, const MString& optionsString, FileAccessMode mode);
	virtual bool		haveReadMethod() const;
	virtual bool		haveWriteMethod() const;
	virtual bool		canBeOpened() const;
	virtual MString 	defaultExtension() const;
	virtual MFileKind	identifyFile(	const MFileObject& fileName, const char* buffer, short size) const;

};

midiImport::midiImport()
: MPxFileTranslator()
{
}

midiImport::~midiImport()
{
}

void *midiImport::creator()
{
	return new midiImport();
}


bool midiImport::haveReadMethod() const
{
	return false;
}

bool midiImport::haveWriteMethod() const
{
	return false;
}

bool midiImport::canBeOpened() const
{
	return false;
}

MString midiImport::defaultExtension() const
{
	return MString("mid");
}

MPxFileTranslator::MFileKind midiImport::identifyFile(const MFileObject& fileName, const char* buffer, short size) const
{
	const char *name = fileName.name().asChar();
	int   nameLength = strlen(name);

	if ((nameLength > 5) && !strcasecmp(name+nameLength-4, ".mid")) {
		return kIsMyFileType;
	}

	return	kNotMyFileType;
}

MStatus animImport::reader(	const MFileObject& file, const MString& options, FileAccessMode mode) 
{
	std::vector<unsigned char> e;
	unsigned long tick;

	double time;
	double mult;

	std::string sfile = std::string(file.name().asChar());
	MidiFileIn mf(sfile);
	MString attr;
	MPlug   plug;

	MSelectionList sList;
	MGlobal::getActiveSelectionList(sList);

	if (sList.isEmpty()) {
		MGlobal::displayError("Nothing is selected for midi file import.");
		return (MS::kFailure);
	}

	Mobject oDepNode;
	sList.getDependNode(0, oDepNode);

	MFnDependencyNode depNode(oDepNode);




	tick = f.getNextEvent(&e, 1);

	time = (double)tick;

	while(e.size())
	{
		if(e[0]==0x90)
		{
			unsigned char note = e[1];
			unsigned char vel = e[2];

			attr = "chan_" + chan;

			if(depNode.hasAttribute(attr))
			{
				plug = findPlug(attr, false, &stat);
			}
			else
			{
				MFnNumericAttribute newAttr;
				stat = depNode.addAttribute(newAttr.create(attr, attr, MFnNumericData::kFloat),  MFnDependencyNode::kLocalDynamicAttr);
				plug = findPlug(attr, false, &stat);
			}
			MFnAnimCurve curve(plug, &stat);

			curve.addKey(MTime(time), (double)vel, MFnAnimCurve::kTangentStep, MFnAnimCurve::kTangentStep, 0, &stat);
				
			//printf("%04i: note: %02x   vel:%02x\n", tick, note, vel);
		}
		else
		{

			/*
			printf("%i:", tick);

			for(size_t i=0; i<e.size();i++)
			{
				printf("%x", e[i]);
			}
			printf("\n");
			*/
		}

		tick = f.getNextEvent(&e, 1);
		time += (double)tick;
	}
		
	

}


MStatus initializePlugin(MObject obj)
{
	MStatus stat = MS::kFailure;

	MFnPlugin impPlugIn(obj, "Alias", "3.0", "midiImport", "none",
											midiImport::creator,
											(char *)midiImportOptionScript,
											(char *)midiImportDefaultOptions, 
											true);

	if (stat != MS::kSuccess) {
		return stat;
	}

	return stat;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus stat = MS::kFailure;

	MFnPlugin impPlugIn(obj);
	stat = impPlugIn.deregisterFileTranslator("midiImport");

	if (stat != MS::kSuccess) {
		return stat;
	}

	return stat;
}