#ifndef ___PEEL_MIDI_EVENTS_H__
#define ___PEEL_MIDI_EVENTS_H__

#include "jdkmidi/fileread.h"
#define _BOOL
#include <fstream>
#include <sstream>
#include <iostream>
#include <ostream>
#include <string.h>
#include <list>
#include <vector>

#include <maya/MGlobal.h>

using namespace jdkmidi;

struct peelNote
{
	unsigned char note;
	unsigned char velocity;
	unsigned char channel;
	MIDIClockTime timeOn;
	MIDIClockTime timeOff;
};

struct peelBend
{
	unsigned char channel;
	short value;
	MIDIClockTime time;
};

struct peelControl
{
	unsigned char controller;
	unsigned char value;
	unsigned char channel;
	MIDIClockTime time;
};

struct peelTempo
{
	unsigned long tempo;
	MIDIClockTime time;
};

typedef std::list<peelBend>    peelBendList;
typedef std::list<peelControl> peelControlList;
typedef std::list<peelNote>    peelNoteList;
typedef std::vector<peelNote>  peelNoteVector;
typedef std::vector<peelTempo> peelTempoVector;

class peelMidiEvents : public MIDIFileEvents
{
       void    mf_system_mode( const MIDITimedMessage &msg );
       void    mf_note_on( const MIDITimedMessage &msg );
       void    mf_note_off( const  MIDITimedMessage &msg );
       void    mf_poly_after( const MIDITimedMessage &msg );
       void    mf_bender( const MIDITimedMessage &msg );
       void    mf_program( const MIDITimedMessage &msg );
       void    mf_chan_after( const MIDITimedMessage &msg );
       void    mf_control( const MIDITimedMessage &msg );
       void    mf_sysex( MIDIClockTime time, const MIDISystemExclusive &ex );
      
       void    mf_arbitrary( MIDIClockTime time, int len, unsigned char *data );
       void    mf_metamisc( MIDIClockTime time, int, int, unsigned char * 	);
       void    mf_seqnum( MIDIClockTime time, int );
       void    mf_smpte( MIDIClockTime time, int, int, int, int, int );
       void    mf_timesig( MIDIClockTime time, int, int, int, int );
       void    mf_tempo( MIDIClockTime time, unsigned long tempo );
       void    mf_keysig(MIDIClockTime time, int, int );
       void    mf_sqspecific( MIDIClockTime time, int, unsigned char * );
       void    mf_text( MIDIClockTime time, int, int, unsigned char * );
       void    mf_eot( MIDIClockTime time );

       void    mf_error( char * );
      
       void    mf_starttrack( int trk );
       void    mf_endtrack( int trk );
       void    mf_header( int, int, int );

public:
	   peelNoteList startNotes;
	   peelNoteVector notes;
	   peelTempoVector tempo;
	   peelControlList controllers;
	   peelBendList bends;

};

#endif
