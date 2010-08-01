//----------------------------------------------------------------------------
//
//  This file is part of seq42.
//
//  seq42 is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  seq42 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with seq42; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//-----------------------------------------------------------------------------

#ifndef SEQ42_PERFORM
#define SEQ42_PERFORM

class perform;

#include "globals.h"
#include "event.h"
#include "midibus.h"
#include "midifile.h"
#include "sequence.h"
#ifndef __WIN32__
#   include <unistd.h>
#endif
#include <pthread.h>


/* if we have jack, include the jack headers */
#ifdef JACK_SUPPORT
#include <jack/jack.h>
#include <jack/transport.h>
#endif


class perform
{
 private:
    /* vector of sequences */
    sequence *m_seqs[c_max_sequence];

    bool m_seqs_active[ c_max_sequence ];

    bool m_was_active_edit[ c_max_sequence ];
    bool m_was_active_perf[ c_max_sequence ];
    bool m_was_active_names[ c_max_sequence ];
    
    /* our midibus */
    mastermidibus m_master_bus;

    /* pthread info */
    pthread_t m_out_thread;
    pthread_t m_in_thread;
    bool m_out_thread_launched;
    bool m_in_thread_launched;

    bool m_running;
    bool m_inputing;
    bool m_outputing;
    bool m_looping;

    bool m_playback_mode;

    int thread_trigger_width_ms; 

    long m_left_tick;
    long m_right_tick;
    long m_starting_tick;
    
    long m_tick;
    bool m_usemidiclock;
    bool m_midiclockrunning; // stopped or started
    int  m_midiclocktick;
    int  m_midiclockpos;
    
    void set_running( bool a_running );

    void set_playback_mode( bool a_playback_mode );

    condition_var m_condition_var;


#ifdef JACK_SUPPORT
    
    jack_client_t *m_jack_client;
    jack_nframes_t m_jack_frame_current,
                   m_jack_frame_last;
    jack_position_t m_jack_pos;
    jack_transport_state_t m_jack_transport_state;
    jack_transport_state_t m_jack_transport_state_last;
    double m_jack_tick;
    
#endif
    
    bool m_jack_running;
    bool m_jack_master;

    void inner_start( bool a_state );
    void inner_stop();

 public:
    bool is_running();

    unsigned int m_key_bpm_up;
    unsigned int m_key_bpm_dn;

    unsigned int m_key_start;
    unsigned int m_key_stop;

    unsigned int m_key_loop;

    perform();
    ~perform();

    void init( void );

    void clear_all( void );
    
    void launch_input_thread( void );
    void launch_output_thread( void );
    void init_jack( void );
    void deinit_jack( void );
    
    void add_sequence( sequence *a_seq, int a_perf );
    void delete_sequence( int a_num );
    bool is_sequence_in_edit( int a_num );
    
    void clear_sequence_triggers( int a_seq  );

    long get_tick( ) { return m_tick; };

    void set_left_tick( long a_tick );
    long get_left_tick( void );

    void set_starting_tick( long a_tick );
    long get_starting_tick( void );

    void set_right_tick( long a_tick );
    long get_right_tick( void );

    void move_triggers( bool a_direction );
    void copy_triggers(  );
    
    void push_trigger_undo( void );
    void pop_trigger_undo( void );
    void pop_trigger_redo( void );

    void print();

    void start( bool a_state );
    void stop();

    void start_jack();
    void stop_jack();
    void position_jack( bool a_state );

    void off_sequences( void );
    void all_notes_off( void );

    void set_active(int a_sequence, bool a_active);
    void set_was_active( int a_sequence );
    bool is_active(int a_sequence);
    bool is_dirty_edit (int a_sequence);
    bool is_dirty_perf (int a_sequence);
    bool is_dirty_names (int a_sequence);
        
    void new_sequence( int a_sequence );

    /* plays all notes to Curent tick */
    void play( long a_tick );
    void set_orig_ticks( long a_tick  );

    sequence * get_sequence( int a_sequence );

    void reset_sequences( void );

    void set_bpm(int a_bpm);
    int  get_bpm( );

    void set_looping( bool a_looping ){ m_looping = a_looping; };
 
    void mute_all_tracks( void );

    mastermidibus* get_master_midi_bus( );
    
    void output_func();
    void input_func();
    
    long get_max_trigger( void );

    friend class midifile;
    friend class optionsfile;
    friend class options;

#ifdef JACK_SUPPORT

    friend int jack_sync_callback(jack_transport_state_t state, 
                              jack_position_t *pos, void *arg);
    friend void jack_shutdown(void *arg);
    friend void jack_timebase_callback(jack_transport_state_t state, jack_nframes_t nframes, 
                                       jack_position_t *pos, int new_pos, void *arg);
#endif
};

/* located in perform.C */
extern void *output_thread_func(void *a_p);
extern void *input_thread_func(void *a_p);



#ifdef JACK_SUPPORT

int jack_sync_callback(jack_transport_state_t state, 
					   jack_position_t *pos, void *arg);
void print_jack_pos( jack_position_t* jack_pos );
void jack_shutdown(void *arg);
void jack_timebase_callback(jack_transport_state_t state, jack_nframes_t nframes, 
                            jack_position_t *pos, int new_pos, void *arg);
#endif


#endif