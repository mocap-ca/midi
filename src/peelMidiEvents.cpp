#include "PeelMidiEvents.h"

void peelMidiEvents::mf_system_mode( const MIDITimedMessage &msg )  {};
void peelMidiEvents::mf_poly_after( const MIDITimedMessage &msg ) {};

void peelMidiEvents::mf_program( const MIDITimedMessage &msg ) {};
void peelMidiEvents::mf_chan_after( const MIDITimedMessage &msg ) {};
void peelMidiEvents::mf_sysex( MIDIClockTime time, const MIDISystemExclusive &ex ) {};
    
void peelMidiEvents::mf_arbitrary( MIDIClockTime time, int len, unsigned char *data ) {};
void peelMidiEvents::mf_metamisc( MIDIClockTime time, int, int, unsigned char * 	) {};
void peelMidiEvents::mf_seqnum( MIDIClockTime time, int ) {};
void peelMidiEvents::mf_smpte( MIDIClockTime time, int, int, int, int, int ) {};
void peelMidiEvents::mf_timesig( MIDIClockTime time, int, int, int, int ) {};

void peelMidiEvents::mf_keysig(MIDIClockTime time, int, int ) {};
void peelMidiEvents::mf_sqspecific( MIDIClockTime time, int, unsigned char * ) {};
void peelMidiEvents::mf_text( MIDIClockTime time, int, int, unsigned char * ) {};
void peelMidiEvents::mf_eot( MIDIClockTime time ) {};

void peelMidiEvents::mf_error( char *err ) { MGlobal::displayError(MString("Midi Error: ") + MString(err)); }
      
void peelMidiEvents::mf_starttrack( int trk ) { /*MGlobal::displayInfo(MString("Starting Track:") + trk);*/ }
void peelMidiEvents::mf_endtrack( int trk ) { /*MGlobal::displayInfo(MString("Ending Track") + trk); */ }
void peelMidiEvents::mf_header( int, int, int ) { }

void peelMidiEvents::mf_bender( const MIDITimedMessage &msg )
{
	peelBend b;
	b.value = msg.GetBenderValue();
	b.channel = msg.GetChannel();
	b.time = msg.GetTime();
	bends.push_back(b);
};


void peelMidiEvents::mf_control( const MIDITimedMessage &msg )
{
	peelControl c;
	c.controller = msg.GetController();
	c.channel = msg.GetChannel();
	c.time  = msg.GetTime();
	c.value = msg.GetControllerValue();
	controllers.push_back(c);
};


void peelMidiEvents::mf_note_on(const MIDITimedMessage &msg )  
{
	char buf[64];	
	//MGlobal::displayInfo(MString("Note On: ") + MString(msg.MsgToText(buf))); 
	peelNote n;
	n.note = msg.GetNote();
	n.channel = msg.GetChannel();
	n.velocity = msg.GetVelocity();
	n.timeOn = msg.GetTime();
	n.timeOff = 0;
	startNotes.push_back(n);

}
void peelMidiEvents::mf_note_off( const  MIDITimedMessage &msg )
{
	char buf[64];	
	//MGlobal::displayInfo(MString("Note Off: ") + MString(msg.MsgToText(buf))); 
	peelNote n;

	for(peelNoteList::iterator i = startNotes.begin(); i != startNotes.end(); i++)
	{
		n = *i;
		if(n.note == msg.GetNote() && n.channel == msg.GetChannel())
		{
			n.timeOff = msg.GetTime();
			notes.push_back(n);
			startNotes.erase(i);
			return;
		}
	}
}

void peelMidiEvents::mf_tempo( MIDIClockTime time, unsigned long _tempo )
{
	peelTempo t;
	t.tempo = _tempo;
	t.time = time;
	tempo.push_back(t);
};