#include "MidiNoteSelect.h"

using namespace std;

// static data
MTypeId midiNoteSelect::id( 0x82857 );

MObject midiNoteSelect::aChannel;
MObject midiNoteSelect::aNote;
MObject midiNoteSelect::aMultiply;
MObject midiNoteSelect::aValue;
MObject midiNoteSelect::aDebug;




midiNoteSelect::midiNoteSelect()
{
}

midiNoteSelect::~midiNoteSelect()
{
}

void * midiNoteSelect::creator()
{
    return new midiNoteSelect();
}



MStatus midiNoteSelect::initialize()
{
	MStatus stat;
	MFnTypedAttribute tAttr;
	MFnNumericAttribute nAttr;


	aChannel = tAttr.create("Channel", "c", MFnData::kIntArray, MObject::kNullObj, &stat);
    tAttr.setKeyable(true);
    tAttr.setStorable(true);
    tAttr.setReadable(true);
    tAttr.setWritable(true);
	addAttribute(aChannel);

	aNote = nAttr.create("note", "n",  MFnNumericData::kInt, 0, &stat);
	nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
	addAttribute(aNote);

	aValue = nAttr.create("value", "val", MFnNumericData::kFloat, 0, &stat);
	nAttr.setKeyable(false);
    nAttr.setStorable(false);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
	addAttribute(aValue);

	aMultiply = nAttr.create("multiply", "m", MFnNumericData::kFloat, 1.0f/128.0f, &stat);
	nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
	addAttribute(aMultiply);

	aDebug = nAttr.create("debug", "d", MFnNumericData::kBoolean, 0, &stat);
	nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
	addAttribute(aDebug);

	attributeAffects(aChannel, aValue);
	attributeAffects(aNote, aValue);
	attributeAffects(aMultiply, aValue);

    return MS::kSuccess;
}



MStatus midiNoteSelect::compute(const MPlug& plug, MDataBlock& block) 
{
	if(plug != aValue)
		return MS::kUnknownParameter;

	MStatus stat;

	MDataHandle hNote = block.inputValue(aNote);
	int note = hNote.asInt();

	MDataHandle data = block.inputValue(aChannel, &stat);
	CHECK_MSTATUS(stat);
	MObject o = data.data();

	MFnIntArrayData intArrayFn(o, &stat);
	CHECK_MSTATUS(stat);
	MIntArray intArray = intArrayFn.array(&stat);
	CHECK_MSTATUS(stat);

	float multiply = block.inputValue(aMultiply).asFloat();

	if(block.inputValue(aDebug).asBool())
	{
		for(int i=0; i< intArray.length(); i++)
		{
			int n = intArray.length();
			if(intArray[i]>0)
			{
				if(i==note)
					cout << "*";
				else
					cout << " ";

				cout << "Midi Note Select Debug Data:";
				cout << "  note " << i << ":" << intArray[i] << endl;
			}
		}
	}

	//cout << "elements: " << intArray.length() << endl;
	if(intArray.length() > note)
	{
		

		MDataHandle hValue = block.outputValue(aValue, &stat);
		CHECK_MSTATUS(stat);
		hValue.set((float)intArray[note] * multiply);
		hValue.setClean();
		
		return MS::kSuccess;
	}
	return MS::kFailure;
}

