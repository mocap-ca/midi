#include "midiMono.h"

using namespace std;

// static data
MTypeId midiMonoSelect::id( 0x82858 );

MObject midiMonoSelect::aChannel;
MObject midiMonoSelect::aNoteMultiply;
MObject midiMonoSelect::aValueMultiply;
MObject midiMonoSelect::aDebug;

MObject midiMonoSelect::aNote;
MObject midiMonoSelect::aValue;




midiMonoSelect::midiMonoSelect()
{
}

midiMonoSelect::~midiMonoSelect()
{
}

void * midiMonoSelect::creator()
{
    return new midiMonoSelect();
}



MStatus midiMonoSelect::initialize()
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

	aNoteMultiply = nAttr.create("noteMultiply", "nm", MFnNumericData::kFloat, 1.0f/128.0f, &stat);
	nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
	addAttribute(aNoteMultiply);

	aValueMultiply = nAttr.create("valueMultiply", "vm", MFnNumericData::kFloat, 1.0f/128.0f, &stat);
	nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
	addAttribute(aValueMultiply);


	aDebug = nAttr.create("debug", "d", MFnNumericData::kBoolean, 0, &stat);
	nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);
	addAttribute(aDebug);

	// Outputs

	aNote = nAttr.create("note", "n",  MFnNumericData::kFloat, 0, &stat);
	nAttr.setKeyable(false);
    nAttr.setStorable(false);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
	addAttribute(aNote);

	aValue = nAttr.create("value", "val", MFnNumericData::kFloat, 0, &stat);
	nAttr.setKeyable(false);
    nAttr.setStorable(false);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
	addAttribute(aValue);


	attributeAffects(aChannel, aValue);
	attributeAffects(aChannel, aNote);
	attributeAffects(aValueMultiply, aValue);
	attributeAffects(aNoteMultiply, aNote);

    return MS::kSuccess;
}



MStatus midiMonoSelect::compute(const MPlug& plug, MDataBlock& block) 
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

	float noteMultiply = block.inputValue(aNoteMultiply).asFloat();
	float valueMultiply = block.inputValue(aValueMultiply).asFloat();

	bool set = false;
	for(int i=0; i< intArray.length(); i++)
	{
		int n = intArray.length();
		if(intArray[i]>0)
		{
			MDataHandle hNote = block.outputValue(aNote);
			CHECK_MSTATUS(stat);
			hNote.set((float)i * noteMultiply);
			hNote.setClean();

			MDataHandle hValue = block.outputValue(aValue);
			CHECK_MSTATUS(stat);
			hValue.set((float)intArray[i] * valueMultiply);
			hValue.setClean();

			set = true;
			if(block.inputValue(aDebug).asBool())
			{
				cout << "Midi Mono Select Debug Data:";
				cout << "  note " << i << ":" << intArray[i] << endl;
			}

			break;
		}
	}

	if(!set)
	{
		MDataHandle hNote = block.outputValue(aNote);
		CHECK_MSTATUS(stat);
		hNote.set(0.0f);
		hNote.setClean();

		MDataHandle hValue = block.outputValue(aValue);
		CHECK_MSTATUS(stat);
		hValue.set(0.0f);
		hValue.setClean();
	}

	return MS::kSuccess;


	//cout << "elements: " << intArray.length() << endl;
	if(note < intArray.length())
	{

		
		return MS::kSuccess;
	}
	return MS::kFailure;
}

