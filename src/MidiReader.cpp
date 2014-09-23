#include "MidiReader.h"

#define CHECKIT CHECK_MSTATUS_AND_RETURN_IT

using namespace std;

bool		peelMidiImporter::haveWriteMethod() const { return false; }
bool		peelMidiImporter::haveReadMethod() const { return true; }
bool		peelMidiImporter::canBeOpened() const { return false; } 
bool        peelMidiImporter::haveNamespaceSupport() const { return false; }
bool        peelMidiImporter::haveReferenceMethod() const { return false; }
MString     peelMidiImporter::filter() const { return MString("*.mid"); }

peelMidiImporter::peelMidiImporter()
{
}

peelMidiImporter::~peelMidiImporter()
{
}

void* peelMidiImporter::creator() { 
    return new peelMidiImporter(); 
} 

MStatus peelMidiImporter::reader ( const MFileObject& file, const MString& options, MPxFileTranslator::FileAccessMode mode) 
{ 
	MIDIFileReadStreamFile rs( file.fullName().asChar() );
	peelMidiEvents events;
    MIDIFileRead reader( &rs, &events );

	reader.Parse();

	bool noteList[255];

	MString msg;


	MString node;
	CHECKIT(MGlobal::executeCommand("createNode peelMidi", node));
	CHECKIT(MGlobal::executeCommand(MString("connectAttr -f time1.outTime ") + node + ".timeIn"));
	CHECKIT(MGlobal::executeCommand(MString("setAttr -type \"string\" ") + node + ".file \"" + file.fullName() + "\""));

	//MGlobal::executeCommand("createNode peelMidi");
	for(char chan=0; chan<16; chan++)
	{
		msg="";
		memset(noteList, 0, 255);

		for(peelNoteVector::iterator i = events.notes.begin(); i != events.notes.end(); i++)
		{
			peelNote n = *i;
			if(n.channel==chan)
				noteList[n.note] = true;
		}


		msg = (MString)"Channel " + chan + ":";
		bool first = true;
		for(int i=0;i<255;i++)
		{
			if(noteList[i]==1)
			{
				MString select;
				CHECKIT(MGlobal::executeCommand("createNode peelMidiSelect", select));
				CHECKIT(MGlobal::executeCommand(MString("connectAttr -f ") + node + ".c" + (chan+1) + " " + select + ".c"));
				CHECKIT(MGlobal::executeCommand(MString("setAttr ") + select + ".note " + i))
				if(first)
					first = false;
				else
					msg += ",";

				msg += i;
			}
		}
		MGlobal::displayInfo(msg);
	}
	
	/*
	for(peelNoteVector::iterator i = events.notes.begin(); i != events.notes.end(); i++)
	{
		peelNote n = *i;

		MString s = MString("createNote(") + n.timeOn + "," + n.timeOff + "," + n.note + "," + n.channel + "," + n.velocity + ")";
		MGlobal::executeCommand(s, false, false);
	}
	*/
	MGlobal::displayInfo((MString)"Number of tracks: " + reader.GetNumberTracks());
	

	
	return MS::kSuccess; 
} 



peelMidiImporter::MFileKind peelMidiImporter::identifyFile (const MFileObject &file, const char *buf, short size) const 
{ 
	const char *name = file.name().asChar(); 
	int   nameLength = (int)strlen(name); 
	if ((nameLength > 4) && !_stricmp(name+nameLength-4, ".mid")) 
	{ 
		return (kIsMyFileType); 
	} 
	return (kNotMyFileType); 
} 

	
			