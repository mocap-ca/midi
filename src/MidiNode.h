// Based on brickShader.cpp 
// Alastair Macleod
#define _BOOL

#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
#include <string.h>
#include <list>
#include <vector>

#include <maya/MIOStream.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MTime.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MFnIntArrayData.h>

#include <math.h>

#include "PeelMidiEvents.h"

#include "jdkmidi/world.h"
#include "jdkmidi/track.h"
#include "jdkmidi/multitrack.h"
#include "jdkmidi/filereadmultitrack.h"
#include "jdkmidi/fileread.h"
#include "jdkmidi/fileshow.h"
#include "jdkmidi/sequencer.h"

using namespace jdkmidi;

class midiNode : public MPxNode
{
public:
			midiNode();
    virtual ~midiNode();

    virtual MStatus compute( const MPlug&, MDataBlock& );
    static  void *  creator();
    static  MStatus initialize();
    static  MTypeId id;

	static  MObject createChannel(int channel);
	static  MStatus processChannel(int i, MDataBlock& block);

private:
	// Input attributes
	static MObject aFile;
	static MObject aTime;

	// Output attributes
	static MObject aChan[16];


};
