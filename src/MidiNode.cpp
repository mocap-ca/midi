#include "MidiNode.h"






// static data
MTypeId midiNode::id( 0x81856 );

// Attributes
MObject midiNode::aFile;
MObject midiNode::aTime;
MObject midiNode::aChan[16];

#define MAKE_INPUT(attr)	\
    CHECK_MSTATUS(attr.setKeyable(true) );		\
    CHECK_MSTATUS( attr.setStorable(true) );	\
    CHECK_MSTATUS( attr.setReadable(true) );	\
    CHECK_MSTATUS( attr.setWritable(true) );

#define MAKE_OUTPUT(attr)			\
    CHECK_MSTATUS( attr.setKeyable(false) );	\
    CHECK_MSTATUS( attr.setStorable(false) );	\
    CHECK_MSTATUS( attr.setReadable(true) );	\
    CHECK_MSTATUS( attr.setWritable(false) );
	





midiNode::midiNode()
{
}

midiNode::~midiNode()
{
}

void * midiNode::creator()
{
    return new midiNode();
}

MObject midiNode::createChannel(int chan)
{
	MStatus stat;
	MObject attr = MObject::kNullObj;
	MFnTypedAttribute tAttr;

	MString name("Channel");
	MString breif("c");

	name+=(chan+1);
	breif+=(chan+1);

	attr = tAttr.create(name, breif, MFnData::kIntArray, MObject::kNullObj, &stat);
	MAKE_OUTPUT(tAttr);
	CHECK_MSTATUS(stat);

	return attr;

}

MStatus midiNode::initialize()
{
	MStatus stat;
	MFnUnitAttribute uAttr;
	MFnTypedAttribute tAttr;


    // Input attributes

	aFile = tAttr.create("file", "file", MFnData::kString,  MObject::kNullObj, &stat);
	CHECK_MSTATUS( stat );
	MAKE_INPUT(tAttr);
	CHECK_MSTATUS( addAttribute(aFile) );


	aTime = uAttr.create("timeIn", "t", MTime(), &stat);
	CHECK_MSTATUS( stat );
	MAKE_INPUT(uAttr);
	CHECK_MSTATUS( addAttribute(aTime) );

	MStatus s = MS::kSuccess;
	for(int i=0; i<16 && s==MS::kSuccess;i++)
	{
		aChan[i] = createChannel(i);
	    CHECK_MSTATUS( addAttribute(aChan[i]) );
		CHECK_MSTATUS( attributeAffects(aFile, aChan[i]) );
		CHECK_MSTATUS( attributeAffects(aTime, aChan[i]) );

	}

    return MS::kSuccess;
}



MStatus midiNode::compute(const MPlug& plug, MDataBlock& block) 
{
	for(int i=0; i<16;i++)
	{
		if(plug == aChan[i])
			return processChannel(i,block);
	}

	return MS::kUnknownParameter;
}


MStatus midiNode::processChannel(int chan, MDataBlock& block)
{
	MStatus stat;

	MObject plug = aChan[chan];

	MString file = block.inputValue(aFile).asString();
	MTime   time = block.inputValue(aTime).asTime();

	FILE *fp = fopen( file.asChar(), "rb" );
	if(!fp)
		MGlobal::displayInfo("Could not open file");
	else
		fclose(fp);

	MIDIFileReadStreamFile  rs( file.asChar() );
	MIDIMultiTrack          tracks(64);
    MIDIFileReadMultiTrack  track_loader( &tracks );
	peelMidiEvents events;
    MIDIFileRead reader( &rs, &events );
	reader.Parse();

	reader.GetDivision();
	int division = reader.GetDivision();

	float tempo = 120.0f;  // bpm

	if(events.tempo.size()>0)
	{
		tempo = 60000000 / events.tempo.at(0).tempo;
	}

	
	// Note events could go on and off between frames, so we
	// need a start and end sample range.  tms1 is current frame
	// frame, tms2 is next frame

	float tms1 = time.as(MTime::kMinutes) * tempo * division;  

	MTime currentUnits(1, MTime::uiUnit());
	float inc = currentUnits.as(MTime::kMinutes);
	float tms2 = tms1 +  inc * tempo * division;

	MDataHandle handle = block.outputValue(plug, &stat);
	CHECK_MSTATUS(stat);

	MIntArray noteArray(300,0);

	for(peelNoteVector::iterator i = events.notes.begin(); i != events.notes.end(); i++)
	{
		peelNote n = *i;
		if(n.channel == chan 
			&& !((n.timeOn < tms1 && n.timeOff < tms2) || (n.timeOn > tms1 && n.timeOff > tms2)) 
			&& n.note < 128)
		{
			//MString s = MString("createNote(") + n.timeOn + "," + n.timeOff + "," + n.note + "," + n.channel + ")";
			CHECK_MSTATUS(noteArray.set(n.velocity, n.note));
		}
	}

	for(peelControlList::iterator i = events.controllers.begin(); i!= events.controllers.end(); i++)
	{
		peelControl c = *i;
		if(c.channel == chan && c.time >= tms1 && c.time <= tms2)
		{
			//MString s = MString("createController(") + c.time + "," + c.value + "," + c.value + ")";
			CHECK_MSTATUS(noteArray.set(c.value, c.controller+127));
		}
	}

	for(peelBendList::iterator i = events.bends.begin(); i!= events.bends.end(); i++)
	{
		peelBend b = *i;
		if(b.channel == chan && b.time >= tms1 && b.time <= tms2)
		{
			CHECK_MSTATUS(noteArray.set(b.value,240));
		}
	}


	MFnIntArrayData noteArrayFn;
	MObject noteArrayObj = noteArrayFn.create(noteArray, &stat);
	CHECK_MSTATUS(stat);

	handle.set(noteArrayObj);
	handle.setClean();

	//MIDI!


	block.setClean(plug);

    return MS::kSuccess;
}



