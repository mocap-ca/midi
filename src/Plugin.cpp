#include "MidiNode.h"
#include "MidiReader.h"
#include "MidiNoteSelect.h"
#include "MidiMono.h"
#include <maya/MFnPlugin.h>


MStatus initializePlugin(MObject obj) 
{ 
	MStatus status; 
	MFnPlugin plugin(obj, "Peel", "1.0", "Any"); 
	status =  plugin.registerFileTranslator("peelMidi", "", peelMidiImporter::creator, "", "", true); 
	if (status != MS::kSuccess) 
	{ 
		status.perror("Register File Translator"); 
		return status; 
	} 

    status = plugin.registerNode("peelMidi", midiNode::id, &midiNode::creator, &midiNode::initialize, MPxNode::kDependNode);
	if (status != MS::kSuccess) 
	{ 
		status.perror("Register Midi Node"); 
		return status; 
	}

    status = plugin.registerNode("peelMidiSelect", midiNoteSelect::id, &midiNoteSelect::creator, &midiNoteSelect::initialize, MPxNode::kDependNode);
	if (status != MS::kSuccess) 
	{ 
		status.perror("Register Midi Note Select"); 
		return status; 
	} 

    status = plugin.registerNode("peelMonoSelect", midiMonoSelect::id, &midiMonoSelect::creator, &midiMonoSelect::initialize, MPxNode::kDependNode);
	if (status != MS::kSuccess) 
	{ 
		status.perror("Register Midi Mono Select"); 
		return status; 
	} 


	return status; 
} 


MStatus uninitializePlugin(MObject obj)  
{ 
	MStatus   status; 
	MFnPlugin plugin( obj ); 
	status =  plugin.deregisterFileTranslator("peelMidi"); 
	if (status != MS::kSuccess) 
	{ 
		status.perror("Deregister File Translator"); 
		return status; 
	} 

    status =  plugin.deregisterNode( midiNode::id );
	if (status != MS::kSuccess) 
	{ 
		status.perror("Deregister Midi Node"); 
		return status; 
	} 

	status =  plugin.deregisterNode( midiNoteSelect::id );
	if (status != MS::kSuccess) 
	{ 
		status.perror("Deregister Midi Note Select"); 
		return status; 
	} 

	status =  plugin.deregisterNode( midiMonoSelect::id );
	if (status != MS::kSuccess) 
	{ 
		status.perror("Deregister Midi Mono Select"); 
		return status; 
	} 

	cout << "UnLoaded\n";
	return status; 
} 

