#ifndef ___MIDI_NOTE_SELECT_H__
#define ___MIDI_NOTE_SELECT_H__

#define _BOOL

#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
#include <string.h>

#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MDataBlock.h>
#include <maya/MPlug.h>
#include <maya/MPxNode.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnIntArrayData.h>


class midiNoteSelect : public MPxNode
{
public:
			midiNoteSelect();
    virtual ~midiNoteSelect();

    virtual MStatus compute( const MPlug&, MDataBlock& );
    static  void *  creator();
    static  MStatus initialize();
    static  MTypeId id;

private:
	// Input attributes
	static MObject aChannel;
	static MObject aNote;
	static MObject aMultiply;
	static MObject aDebug;

	// Output attributes
	static MObject aValue;


};


#endif