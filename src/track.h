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


#ifndef SEQ42_TRACK
#define SEQ42_TRACK

class track;

#include "trigger.h"
#include "sequence.h"
#include "mutex.h"

class track
{

  private:

    /* holds the sequences */
    vector < sequence > m_vector_sequence;
    static sequence m_sequence_clipboard;

    /* holds the triggers */
    list < trigger > m_list_trigger;
    trigger m_trigger_clipboard;

    stack < list < trigger > >m_list_trigger_undo;
    stack < list < trigger > >m_list_trigger_redo;

    /* markers */
    list < trigger >::iterator m_iterator_play_trigger;
    list < trigger >::iterator m_iterator_draw_trigger;

    /* track name */
    string m_name;

    /* contains the proper midi channel */
    char m_midi_channel;
    char m_bus;

    /* song playback mode mute */
    bool m_song_mute;

    /* outputs to sequence to this Bus on midichannel */
    mastermidibus *m_masterbus;

    bool m_trigger_copied;

    void split_trigger( trigger &trig, long a_split_tick);

    bool m_dirty_perf;
    bool m_dirty_names;

    mutex m_mutex;
    void lock ();
    void unlock ();


  public:

      track ();
     ~track ();

    void push_trigger_undo (void);
    void pop_trigger_undo (void);
    void pop_trigger_redo (void);

    void add_sequence( sequence *a_seq );
    void delete_sequence( int a_num );

    sequence *get_sequence( int a_seq );
    int get_sequence_index( sequence *a_sequence );

    // How many sequences does this track have?
    int get_number_of_sequences(void);



    //
    //  Gets and Sets
    //
    void set_name (string a_name);
    void set_name (char *a_name);
    /* returns string of name */
    const char *get_name (void);

    void set_song_mute (bool a_mute);
    bool get_song_mute (void);

    /* midi channel */
    unsigned char get_midi_channel ();
    void set_midi_channel (unsigned char a_ch);
    /* sets the midibus to dump to */ 
    void set_midi_bus (char a_mb); 
    char get_midi_bus (void); 
 
    void set_master_midi_bus (mastermidibus * a_mmb);

    void set_dirty();
    /* signals that a redraw is needed from recording */
    /* resets flag on call */
    bool is_dirty_perf();
    bool is_dirty_names();

    /* dumps contents to stdout */
    void print ();
    void print_triggers();
    
    //
    // Selection and Manipulation
    //
    void add_trigger (long a_tick, long a_length, long a_offset = 0);
    void split_trigger( long a_tick );
    void grow_trigger (long a_tick_from, long a_tick_to, long a_length);
    void del_trigger (long a_tick );
    bool get_trigger_state (long a_tick);
    bool get_trigger_sequence (long a_tick);
    bool select_trigger(long a_tick);
    bool unselect_triggers (void);
    bool intersectTriggers( long position, long& start, long& end );
    void del_selected_trigger( void );
    void cut_selected_trigger( void );
    void copy_selected_trigger( void );
    void paste_trigger( void );
    void move_selected_triggers_to(long a_tick, int a_which=2);
    long get_selected_trigger_start_tick( void );
    long get_selected_trigger_end_tick( void );

    long get_max_trigger (void);

    void move_triggers (long a_start_tick, long a_distance, bool a_direction);
    void copy_triggers (long a_start_tick, long a_distance);
    void clear_triggers (void);


    void reset_draw_trigger_marker (void);

    // FIXME: Change API to just return a pointer to the next trigger (or a NULL pointer if no more triggers)?
    bool get_next_trigger (long *a_tick_on, long *a_tick_off, bool * a_selected, long *a_tick_offset, sequence **a_seq);

    /* Return true if at least one of this track's sequences is being edited. */
    bool get_editing( void );

    void reset_sequences(bool a_playback_mode);
    void set_playing_off(void);

    /* send a note off for all active notes */
    void off_playing_notes (void);

    void play( long a_tick, bool a_playback_mode );
    void set_orig_tick (long a_tick);

};


#endif