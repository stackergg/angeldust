#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

struct player_info_t {
	int64_t __pad0;
	union {
		int64_t xuid;
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char name [ 128 ];
	int userid;
	char guid [ 33 ];
	unsigned int friendsid;
	char friendsname [ 128 ];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles [ 4 ];
	unsigned char filesdownloaded;
};

class INetChannelInfo
{
public:

	enum {
		GENERIC = 0,	// must be first and is default group
		LOCALPLAYER,	// bytes for local player entity update
		OTHERPLAYERS,	// bytes for other players update
		ENTITIES,		// all other entity bytes
		SOUNDS,			// game sounds
		EVENTS,			// event messages
		USERMESSAGES,	// user messages
		ENTMESSAGES,	// entity messages
		VOICE,			// voice data
		STRINGTABLE,	// a stringtable update
		MOVE,			// client move cmds
		STRINGCMD,		// string command
		SIGNON,			// various signondata
		TOTAL,			// must be last and is not a real group
	};

	virtual const char  *GetName( void ) const = 0;	// get channel name
	virtual const char  *GetAddress( void ) const = 0; // get channel IP address as string
	virtual float		GetTime( void ) const = 0;	// current net time
	virtual float		GetTimeConnected( void ) const = 0;	// get connection time in seconds
	virtual int			GetBufferSize( void ) const = 0;	// netchannel packet history size
	virtual int			GetDataRate( void ) const = 0; // send data rate in byte/sec

	virtual bool		IsLoopback( void ) const = 0;	// true if loopback channel
	virtual bool		IsTimingOut( void ) const = 0;	// true if timing out
	virtual bool		IsPlayback( void ) const = 0;	// true if demo playback

	virtual float		GetLatency( int flow ) const = 0;	 // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency( int flow ) const = 0; // average packet latency in seconds
	virtual float		GetAvgLoss( int flow ) const = 0;	 // avg packet loss[0..1]
	virtual float		GetAvgChoke( int flow ) const = 0;	 // avg packet choke[0..1]
	virtual float		GetAvgData( int flow ) const = 0;	 // data flow in bytes/sec
	virtual float		GetAvgPackets( int flow ) const = 0; // avg packets/sec
	virtual int			GetTotalData( int flow ) const = 0;	 // total flow in/out in bytes
	virtual int			GetSequenceNr( int flow ) const = 0;	// last send seq number
	virtual bool		IsValidPacket( int flow, int frame_number ) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual float		GetPacketTime( int flow, int frame_number ) const = 0; // time when packet was send
	virtual int			GetPacketBytes( int flow, int frame_number, int group ) const = 0; // group size of this packet
	virtual bool		GetStreamProgress( int flow, int *received, int *total ) const = 0;  // TCP progress if transmitting
	virtual float		GetTimeSinceLastReceived( void ) const = 0;	// get time since last recieved packet in seconds
	virtual	float		GetCommandInterpolationAmount( int flow, int frame_number ) const = 0;
	virtual void		GetPacketResponseLatency( int flow, int frame_number, int *pnLatencyMsecs, int *pnChoke ) const = 0;
	virtual void		GetRemoteFramerate( float *pflFrameTime, float *pflFrameTimeStdDeviation ) const = 0;

	virtual float		GetTimeoutSeconds( ) const = 0;
};

class iv_engine_client {
public:
	int get_local_player( ) {
		using original_fn = int( __thiscall* )( iv_engine_client* );
		return ( *( original_fn** ) this ) [ 12 ]( this );
	}
	int get_player_for_user_id( int user_id ) {
		using original_fn = int( __thiscall* )( iv_engine_client*, int );
		return ( *( original_fn** ) this ) [ 9 ]( this, user_id );
	}
	void get_player_info( int index, player_info_t* info ) {
		using original_fn = void( __thiscall* )( iv_engine_client*, int, player_info_t* );
		return ( *( original_fn** ) this ) [ 8 ]( this, index, info );
	}
	void get_screen_size( int& width, int& height ) {
		using original_fn = void( __thiscall* )( iv_engine_client*, int&, int& );
		return ( *( original_fn** ) this ) [ 5 ]( this, width, height );
	}
	void execute_cmd( const char* cmd ) {
		using original_fn = void( __thiscall* )( iv_engine_client*, const char* );
		return ( *( original_fn** ) this ) [ 108 ]( this, cmd ); // this always seems to crash whilst debugging, just feel free to continue.
	}
	void set_view_angles( vec3_t& angles ) {
		using original_fn = void( __thiscall* )( iv_engine_client*, vec3_t& );
		return ( *( original_fn** ) this ) [ 19 ]( this, angles );
	}
	vec3_t get_view_angles( ) {
		vec3_t temp;
		using original_fn = void( __thiscall* )( iv_engine_client*, vec3_t& );
		( *( original_fn** ) this ) [ 18 ]( this, temp );
		return temp;
	}
	bool in_game() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[26](this);
	}
	bool connected() {
		using original_fn = bool(__thiscall*)(iv_engine_client*);
		return (*(original_fn**)this)[27](this);
	}

	virtual INetChannelInfo	      *GetNetChannelInfo( void ) = 0;

};